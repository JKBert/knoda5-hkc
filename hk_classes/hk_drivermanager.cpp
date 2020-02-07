// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.71 $

#include "hk_drivermanager.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
//#include <iostream.h>
#include "hk_connection.h"
#include "hk_report.h"
#include "hk_font.h"
#include "hk_dsdatavisible.h"
#include "hk_presentation.h"
#include <locale.h>
#include <algorithm>
#include <config.h>


//#include <Python.h>



/*void __attribute__((constructor)) hk_classes_init(void)
{
   cerr<<" HK_CLASSES INIT"<<endl<<endl;
#ifdef HAVE_PYTHON
   if (!Py_IsInitialized())Py_Initialize();
#endif
}


void __attribute__((destructor)) hk_classes_finish(void)
{
   cerr<<" HK_CLASSES FINISH"<<endl<<endl;
#ifdef HAVE_PYTHON
   //Py_Finalize();
#endif
}
*/




class hk_drivermanagerprivate
{
  public:
  list<void*>p_dlhandlelist;
};

requestdialogtype* hk_drivermanager::p_driverselectdialog=NULL;
hk_string hk_drivermanager::p_hk_classespath=(hk_string)HKCLASSES+"/drivers";
vector<hk_string> hk_drivermanager::p_driverlist;

typedef hk_connection*  connectiontype(hk_drivermanager* );
typedef hk_string  versiontype(void);

hk_drivermanager::hk_drivermanager(void)
{
   init(false);

}


hk_drivermanager::hk_drivermanager(bool runtime):hk_class()
{
   init(runtime);

}

void hk_drivermanager::init(bool runtime)
{
    p_private = new hk_drivermanagerprivate;
    hk_report::setup_reportbasics();
    p_hk_classespath=HKCLASSES;
    p_hk_classespath=p_hk_classespath+"/drivers";
    scan_directory();
    char* h= getenv("HOME");
    hk_string classespath=(h==NULL?"/tmp":h);
    classespath +="/.hk_classes";
    mkdir (classespath.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
    load_preferences();
    p_runtime_only=runtime;
//     lt_dlinit();

}


hk_drivermanager::~hk_drivermanager(void)
{
    list<hk_connection*>::iterator it= p_connections.begin();
    while (it!=p_connections.end())
    {
        hk_connection* con=(*it);
        it++;
        con->disconnect();
        delete con;
    }

   list <void*>::iterator hit=p_private->p_dlhandlelist.begin();
   while(hit!=p_private->p_dlhandlelist.end())
   {
      void* handle=(*hit);
      ++hit;
      dlclose(handle);
   }
//    lt_dlexit();
delete p_private;
}


hk_connection* hk_drivermanager::new_connection(const hk_string& drivername,enum_interaction in)
{
#ifdef HK_DEBUG
    hkdebug("hk_drivermanager::new_connection");
#endif
    hk_connection*     n=NULL;
    connectiontype *connection=NULL;
    versiontype *version=NULL;
    hk_string d;
    hk_string driver;
    hk_string error;
    if (drivername.size()==0)
    {
        if (interactive) driver=show_driverselectdialog();
    }
    else driver=string2lower(drivername);

    if (driver.size()==0)return NULL;

    d=p_hk_classespath+"/libhk_";
    d=d+driver;

    hk_string localdriver=d+"driver.so";

#ifdef HK_DEBUG
    hkdebug("Driver Path: ",d);
#endif
//      cerr <<"try open localdriver:"<<localdriver<<endl;
    void* handle=dlopen(localdriver.c_str(),RTLD_LAZY |RTLD_GLOBAL);
    if (!handle)
       {
	   const char* e=dlerror();
	   if (e) error=e;
        localdriver=d+"driver.dylib";//Mac OSX
        handle=dlopen(localdriver.c_str(),RTLD_LAZY |RTLD_GLOBAL);
       }


    if (!handle)
       {
	   const char* e=dlerror();
	   if (e) error=error+"\n"+e;
        localdriver=d+"driver.sl";
        handle=dlopen(localdriver.c_str(),RTLD_LAZY |RTLD_GLOBAL);
       }

    if (handle)
    {

#ifdef HK_DEBUG
        hkdebug("hk_drivermanager::new_connection handle=true");
#endif
        connection=(connectiontype*)dlsym(handle,"create_connection");
        version=(versiontype*)dlsym(handle,"hk_classesversion");

        if ((dlerror())!=NULL||connection==NULL||version==NULL)
        {
            if (in==interactive)
	    {
	       hk_string err=hk_translate("\nOld driver with no version information!");
                show_warningmessage(hk_translate("Internal error: Database driver not compatible")+((connection&&!version)?err:""));
            }
            return NULL;

        }
        else
        {
#ifdef HK_DEBUG
            hkdebug("hk_drivermanager::new_connection (*connection)(this)");
#endif
      hk_string v=(*version)();
         if (v!=HK_VERSION)
	    {
		show_warningmessage(replace_all("%2",replace_all("%1",hk_translate("Wrong driver version: %1\nYou need driver version %2"),v),HK_VERSION));
		return NULL;
	    }
            n= (*connection)(this);
        }

    }
    else
    { cerr <<"nohandle!!!"<<endl;
        if (interactive)
	{
	   const char* e=dlerror();
	   if (e) error=error+"\n"+e;
	   hk_string w=hk_translate("Database driver '%DRIVER%' could not be loaded.\nReason: ")+(error.empty()?hk_translate("Unknown reason"):error);
            show_warningmessage(replace_all("%DRIVER%",w,driver));
	 }
        return NULL;
    }

    char* h= getenv("HOME");
    hk_string classespath=(h==NULL?"/tmp":h);
    classespath +="/.hk_classes/";
    classespath +=driver;
    mkdir (classespath.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
    if (n!=NULL)
    {
	p_private->p_dlhandlelist.push_back(handle);
	n->set_classespath(classespath);
        p_connections.insert(p_connections.end(),n);
    }
#ifdef HK_DEBUG
    hkdebug("hk_drivermanager::new_connection ENDE");
#endif

    return n;

}


void hk_drivermanager::set_path(const hk_string& path)
{
    p_hk_classespath=path;
    scan_directory();

}


vector<hk_string>* hk_drivermanager::driverlist(void)
{
    scan_directory();
    return &p_driverlist;

}


hk_string hk_drivermanager::path(void)
{
    return p_hk_classespath;
}


void hk_drivermanager::scan_directory(void)
{

    DIR *dp;
    struct dirent *entry;

    hk_string datei;
    p_driverlist.erase(p_driverlist.begin(),p_driverlist.end());

    if ((dp=opendir(p_hk_classespath.c_str()))!=NULL)
    {

        while ((entry = readdir(dp))!=NULL)
        {
            datei=entry->d_name;
	    hk_string driver_extension="driver.la";
            hk_string::size_type p =datei.find(driver_extension);
            if (p!=hk_string::npos &&(datei.size()-driver_extension.size())==p)
            {
                datei.replace(p,datei.size()-p,"");
                p =datei.find("libhk_");
                if (p==0)
                {
                    datei.replace(0,6,"");
                    p_driverlist.insert(p_driverlist.end(),datei);
                }
            }
        }
        closedir(dp);
    }
    sort(p_driverlist.begin(),p_driverlist.end());
}


void hk_drivermanager::set_driverselectdialog(requestdialogtype* dialog)
{

    hk_drivermanager::p_driverselectdialog=dialog;
}


hk_string hk_drivermanager::show_driverselectdialog(void)
{
    if (p_driverselectdialog!=NULL)
    {
        return p_driverselectdialog();
//   return 0;
    }
    else
    {
        hk_string result;
        cout<<endl<<        "==========================================="<<endl;
        cout <<endl<<hk_translate("The following database drivers were found:")<<endl;
        cout<<          "==========================================="<<endl;
        vector<hk_string>::iterator it=p_driverlist.begin();
        int i=1;
        while (it!=p_driverlist.end())
        {
            cout <<"("<<i++<<") "<<(*it)<<endl;
            it++;
        }
        cout <<endl<<"("<<i<<") "<<hk_translate("Select directory")<<endl;
        cout<<          "==========================================="<<endl;
        cout <<endl<<hk_translate("Please select: ");
        char c;
        hk_string r;
        int k;
        do
        {
            cin.get(c);
            r=c;
            k=atoi(r.c_str());
        }
        while(k<1 || k>i);
//      int y=1;
        it=p_driverlist.begin();
        if (k==i)
        {
            hk_string v;
            cout<<endl<<          "==========================================="<<endl;
            cout <<hk_translate("Enter new Driver Path")<<endl;
            cout <<hk_translate("Actual Path is: ")<<p_hk_classespath<<endl;
            cout<<     "==========================================="<<endl;
            cout <<hk_translate("New Path: ");
            cin>>v;
            set_path(v.c_str());
            result=show_driverselectdialog();
        }
        else
        {
            result=p_driverlist[k-1];
        }
        return  result.c_str();
    }

}


void    hk_drivermanager::connection_remove(hk_connection* c)
{
    if (c!=NULL)
      {
       p_connections.remove(c);

      }

}


hk_visible::alignmenttype text2align(const hk_string& t)
{
    if (t=="RIGHT") return hk_visible::alignright;
    if (t=="CENTER") return  hk_visible::aligncenter;
    return hk_visible::alignleft;
}


void hk_drivermanager::load_preferences(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_drivermanager::load_configuration");
#endif
    hk_string f="/etc/hk_classes.conf";
    xmlDocPtr d;
    xmlNodePtr node;
    if (file_exists(f))
    {
    d=xmlParseFile(f.c_str());
    node=xmlDocGetRootElement(d);           
    bool loaduserpreferences=true;
    if (d)
    {
	set_preferences(node);
	get_tagvalue(node,"LOADUSERPREFERENCES",loaduserpreferences);
    } //else cerr<<"no central configuration file"<<endl;
    if (!loaduserpreferences)
    {
      //cerr <<"don't load userpreferences"<<endl;
      return;
    }
    }
    //load user preferences
    char* h= getenv("HOME");
    hk_string    p_classespath=(h==NULL?"/tmp":h);
    p_classespath +="/.hk_classes";
    hk_string n=p_classespath+"/preferences";
    

    d=xmlParseFile(n.c_str());
    node=xmlDocGetRootElement(d);           
    if (node)
    {
	node=node->xmlChildrenNode;
	set_preferences(node);
    }
//     else cerr <<"no local configuration file"<<endl;
}


void hk_drivermanager::set_preferences(xmlNodePtr p_new_data)
{

        hk_string b;
	bool r;
	xmlNodePtr child=get_tagvalue(p_new_data,"GENERAL");
	if (child) child=child->xmlChildrenNode;
	else
	{
	 cerr <<"GENERALchild=NULL!"<<endl;
	}
        if (get_tagvalue(child,"RUNTIME",r)) p_runtime_only=true;
        if (get_tagvalue(child,"AUTOMATIC_DATA_UPDATE",r)) set_default_automatic_data_update(r);
        if (get_tagvalue(child,"DEFAULTFONT",b))
        {
            long s=12;
            get_tagvalue(child,"DEFAULTFONTSIZE",s);

            hk_font::set_defaultfont(b,s);
        }
        if (get_tagvalue(child,"DEFAULTDRIVER",b)) hk_class::set_defaultdriver(b);
        if (get_tagvalue(child,"DEFAULTTEXTALIGNMENT",b)) hk_visible::set_defaulttextalignment(text2align(b));
        if (get_tagvalue(child,"DEFAULTNUMBERALIGNMENT",b)) hk_visible::set_defaultnumberalignment(text2align(b));
        if (get_tagvalue(child,"DEFAULTSIZETYPE",b))hk_presentation::set_defaultsizetype(b=="RELATIVE"?hk_presentation::relative:hk_presentation::absolute);
        if (get_tagvalue(child,"MEASURESYSTEM",b)) hk_class::set_measuresystem((b=="CM"?hk_class::cm:hk_class::inch));

        if (get_tagvalue(child,"DRIVERPATH",b))
        {
//the following is a hack for LFS
            #ifdef __LP64__
            if (b=="/usr/lib/hk_classes/drivers")
              b="/usr/lib64/hk_classes/drivers";
            #endif
            if (b.find("/usr/local/hk_classes")!=hk_string::npos)
            {
                b=HKCLASSES;b+="/drivers";

                set_path(b);
                show_warningmessage("Due to a new directory hierarchy, your preferences have been changed");
                save_preferences();
            }
            else    set_path(b);
        }

        bool bb;
        if (get_tagvalue(child,"SHOWPEDANTIC",bb)) set_showpedantic(bb);
        if (get_tagvalue(child,"MAXIMIZEDWINDOWS",bb)) hk_visible::set_open_maximized_windows(bb);
        long i;
        if (get_tagvalue(child,"DEFAULTPRECISION",i) && get_tagvalue(child,"DEFAULTTHOUSANDSSEPARATOR",bb))
            hk_dsdatavisible::set_defaultnumberformat(bb,i);
         if (get_tagvalue(child,"SNAP2GRIDX",i)) hk_presentation::set_snap2gridx(i);
         if (get_tagvalue(child,"SNAP2GRIDY",i)) hk_presentation::set_snap2gridy(i);


	 child=get_tagvalue(p_new_data,"HK_REGIONAL");
	if (child) child=child->xmlChildrenNode;
	b="";
        if (get_tagvalue(child,"LOCALE",b))set_locale(b);
        if (get_tagvalue(child,"DEFAULTTIMEFORMAT",b)) p_defaulttimeformat=b;
        if (get_tagvalue(child,"DEFAULTDATETIMEFORMAT",b)) p_defaultdatetimeformat=b;
        if (get_tagvalue(child,"DEFAULTDATEFORMAT",b)) p_defaultdateformat=b;

	 child=get_tagvalue(p_new_data,"REPORT");
	if (child) child=child->xmlChildrenNode;
        if (get_tagvalue(child,"PRINTERCOMMAND",b))    hk_report::set_printcommand(b);
        if (get_tagvalue(child,"EMBEDFONTS",r)) hk_report::set_embedfonts(r);

}




hk_connection* hk_drivermanager::find_existing_connection(const hk_string& drivername,const hk_string& host,unsigned int tcp_port,const hk_string& user)
{

    list<hk_connection*>::iterator it=p_connections.begin();
    while (it!=p_connections.end())
    {
        if (
            (*it)->drivername()==drivername
            &&
            (*it)->host()==host
            &&
            (*it)->tcp_port()==tcp_port
            &&
            (*it)->user()==user
            ) return (*it);

        it++;
    }

    return NULL;
}

hk_database* hk_drivermanager::open_connectionfile(const hk_url& url)
{
    xmlDocPtr d=xmlParseFile(url.url().c_str());
    if (!d)
    {
      show_warningmessage(replace_all("%1",hk_translate("Error parsing connectionfile(%1)!"),url.url()));
      return NULL;
    }
    xmlNodePtr node=xmlDocGetRootElement(d);           
    if (!node)
    {
      show_warningmessage(replace_all("%1",hk_translate("No such connectionfile(%1)!"),url.url()));
      return NULL;
    }


 hk_database* db=NULL;
 hk_string connectionname;
 hk_string dbname;
 hk_string username;
 hk_string passwordname;
 hk_string hostname;
 hk_string tcpportname;
 bool emulatebool=false;
 get_tagvalue(node,"CONNECTION",connectionname);
 get_tagvalue(node,"DATABASE",dbname);
 get_tagvalue(node,"USER",username);
 get_tagvalue(node,"PASSWORD",passwordname);
 get_tagvalue(node,"HOST",hostname);
 get_tagvalue(node,"TCPPORT",tcpportname);
 get_tagvalue(node,"BOOLEANEMULATION",emulatebool);
 if (connectionname.size()==0)
 {
  cerr <<"no connectionname"<<endl;
  return NULL;
 }
 hk_connection* c=new_connection(connectionname);
 if (!c)
 {
 cerr <<"no connection"<<endl;
 return NULL;
 }
  c->set_host(hostname);
  c->set_tcp_port(atoi(tcpportname.c_str()));

 c->set_user(username);
 c->set_password(passwordname);
 c->set_booleanemulation(emulatebool);
 if (!c->connect())
 {
   //show_warningmessage(hk_translate("Connection could not be established"));
   delete c;
   return NULL;
 }
 db=c->new_database(dbname);

 return db;

}
