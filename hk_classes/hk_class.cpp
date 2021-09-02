// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// copyright (c) 2020 Patrik Hanak <hanakp@users.sourceforge.net>
// This file is part of the hk_classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.56 $
#include "hk_class.h"
#include "hk_report.h"
#include "hk_dsdatavisible.h"
#include "hk_drivermanager.h"
#include "hk_font.h"
#include "hk_presentation.h"
#include "hk_interpreter.h"
#include "hk_url.h"
#include <cassert>

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define VERSION UNKNOWN
#endif

#include <termios.h>
#include <sys/stat.h>
#include <locale.h>




hk_string hk_class::p_begintag_begin="<";
hk_string hk_class::p_begintag_end=">";
hk_string hk_class::p_endtag_begin="</";
hk_string hk_class::p_endtag_end=">";
hk_string hk_class::p_begintag="";
hk_string hk_class::p_endtag="";
hk_string hk_class::p_defaulttimeformat="h:m:s";
hk_string hk_class::p_defaultdateformat="D.M.Y";
hk_string hk_class::p_defaultdatetimeformat="D.M.Y h:m:s";
hk_string hk_class::p_locale="";
hk_string hk_class::p_defaultdriver="";
hk_string hk_class::p_emptytag="";
hk_string hk_class::p_emptytag_begin="<";
hk_string hk_class::p_emptytag_end="/>";
hk_class::enum_measuresystem hk_class::p_defaultmeasuresystem=hk_class::cm;

messagewindowtype* hk_class::p_warning=NULL;
yesno_dialogtype* hk_class::p_yesno=NULL;
translatefunctiontype* hk_class::p_translate=NULL;
file_dialogtype*  hk_class::p_filedialog=NULL;
stringvalue_dialogtype*  hk_class::p_directorydialog=NULL;
stringvalue_dialogtype* hk_class::p_stringvalue=NULL;

bool    hk_class::p_showpedantic=true;
bool    hk_class::p_generaldebug=false;
bool    hk_class::p_runtime_only=false;
bool    hk_class::p_default_automatic_data_update=false;


unsigned int hk_class::p_taglevel=0;

const char* line="========================================\n";

hk_class::hk_class()
{
#ifdef HK_DEBUG
    p_debug=false;
#endif
}


hk_class::~hk_class()
{
#ifdef HK_DEBUG
    hkdebug("hk_class::destructor");
#endif
}


#ifdef HK_DEBUG
void hk_class::hkclassname(const hk_string& n)
{
#ifdef HK_DEBUG
    hkdebug("hk_class::classname");
#endif

    p_classname=n;
}


hk_string hk_class::hkclassname(void)const
{
    return p_classname;

}


void hk_class::wanna_debug(bool d)
{
    p_debug=d;
}


bool hk_class::wanna_debug(void)const
{
    return p_debug;
}


void hk_class::hkdebug(const hk_string& d)const
{
    if (p_debug||p_generaldebug)
    {
        cerr <<"HKDebug: ";
        if (p_classname.size()>0) cerr<<p_classname;
        else cerr<<"Unknown classname";
        cerr<<" "<<d<<endl;
    }

}


void hk_class::hkdebug(const hk_string& d, const hk_string& e)const
{
    if (p_debug||p_generaldebug)
    {
        cerr <<"HKDebug: ";
        if (p_classname.size()>0) cerr<<p_classname;
        else cerr<<"Unknown classname";
        cerr<<" "<<d<<e<<endl;
    }

}


void hk_class::hkdebug(const hk_string& d,int i)const
{
    if (p_debug||p_generaldebug)
    {
        cerr <<"HKDebug: ";
        if (p_classname.size()>0) cerr<<p_classname;
        else cerr<<"Unknown classname";
        cerr<<" "<<d<<i<<endl;
    }

}


void hk_class::hkdebug(const hk_string& d,double i)const
{
    if (p_debug||p_generaldebug)
    {
        cerr <<"HKDebug: ";
        if (p_classname.size()>0) cerr<<p_classname;
        else cerr<<"Unknown classname";
        cerr<<" "<<d<<i<<endl;
    }

}
#endif

void hk_class::show_warningmessage(const hk_string& m)
{
    //cerr <<"m: l�ge: "<<m.size()<<" wert: "<<m<<endl;
    if (m.size()==0 ||m=="\n"||m==" "||m=="\t") return;
    if (p_warning!=NULL)p_warning(m);
    else cerr <<m<<endl;
}


void hk_class::set_warningmessage(messagewindowtype* m)
{
    hk_class::p_warning=m;
}


void hk_class::set_translatefunction(translatefunctiontype* t)
{
    hk_class::p_translate=t;
}


bool hk_class::show_yesnodialog(const hk_string& m, bool default_value)
{
    if (!p_showpedantic)return default_value;
    if (p_yesno!=NULL) return p_yesno(m,default_value);
    cerr <<"\n"<<line<<hk_translate("Question:")<<endl<<line;
    cerr <<m<<endl;
    cerr <<hk_translate("n=No            all other keys = yes")<<endl<<line;

    hk_string i;
    cin >>i;
    if (i=="n"||i=="N") return false;
    else return true;

}


void hk_class::set_yesnodialog(yesno_dialogtype* d)
{

    p_yesno=d;
}


void hk_class::set_showpedantic(bool s)
{
    p_showpedantic=s;
}


#ifdef HK_DEBUG
void hk_class::set_generaldebug(bool d)
{

    p_generaldebug=d;
}
#endif

hk_string hk_class::hk_translate(const hk_string& t)
{
    if (p_translate==NULL) return t;
    else return p_translate(t);

}


hk_string  hk_class::show_stringvaluedialog(const hk_string& t)
{

    if (p_stringvalue!=NULL) return p_stringvalue(t);

    cerr <<endl<< t<<endl;
    hk_string i;
    getline(cin,i);
    return i;
}


void hk_class::set_stringvaluedialog(stringvalue_dialogtype* t)
{

    p_stringvalue=t;
}


void  hk_class::savedata(ostream& )
{
}


// void  hk_class::loaddata(const hk_string& )
// {
// 
// }


void  hk_class::loaddata(xmlNodePtr)
{

}







//NEW
xmlNodePtr hk_class::get_tagvalue(xmlNodePtr where,const hk_string &tag, hk_string &value, int position,enum_tagtype tagtype)
{
// cerr <<endl<<"SUCHE get_tagvalue:"<<tag<<" "<<position<<endl;
 if (!where)
 {
//   cerr <<"WHERE=NULL"<<endl;
  return NULL;
 }
    int counter=0;
    xmlNodePtr cur=where;
    if (position<1)
    {
     assert(position>0);
     position=1;
    }
    set_tag(tag);
    xmlNodePtr result=NULL;
//     cerr <<" tg=";
    while(cur && !result)
    {
	hk_string tg=(const char*)cur->name;
// 	cerr <<" #"<<tg<<"# ";
	if (tg==tag)
	{
		++counter;
		if (counter==position) result=cur;
	
	}
	xmlNodePtr child=cur->xmlChildrenNode;
	if (child &&!result)
	{
// 	  cerr<<"CHILD BEGIN"<<endl;
	  result=get_tagvalue(child,tag,value,position,tagtype);
// 	  cerr<<"CHILD END"<<endl;
	}
	cur=cur->next;
    }
//     cerr <<endl;
//     bool res=true;
    if (!result)
    {
//     cerr <<"NICHT GEFUNDEN!"<<endl;
    return NULL;
    }
    xmlChar* v= xmlNodeGetContent(result->xmlChildrenNode);
    if (v) value=u2l((const char*)v);
    else value="";
    if (tagtype==normaltag)
    {
     value=replace_all("&lt;",value,"<");
     value=replace_all("&amp;",value,"&");
    }
//  cerr <<"GEFUNDEN get_tagvalue:"<<tag<<"='"<<value<<"'"<<endl;
    return result;

}


xmlNodePtr hk_class::get_tagvalue(xmlNodePtr where, const hk_string &tag, unsigned long &value, int position)
{
    hk_string v;
    xmlNodePtr r=get_tagvalue(where,tag,v,position);
    if (r) value=atoi(v.c_str());
    return r;
}


xmlNodePtr hk_class::get_tagvalue(xmlNodePtr where, const hk_string &tag, unsigned int &value, int position)
{
    hk_string v;
    xmlNodePtr r=get_tagvalue(where,tag,v,position);
    if (r) value=atoi(v.c_str());
    return r;
}


xmlNodePtr hk_class::get_tagvalue(xmlNodePtr where, const hk_string &tag, long &value, int position)
{
    hk_string v;
    xmlNodePtr r=get_tagvalue(where,tag,v,position);
    if (r) value=atoi(v.c_str());
    return r;
}


xmlNodePtr hk_class::get_tagvalue(xmlNodePtr where, const hk_string &tag, int &value, int position)
{
    hk_string v;
    xmlNodePtr r=get_tagvalue(where,tag,v,position);
    if (r) value=atoi(v.c_str());
    return r;
}


xmlNodePtr hk_class::get_tagvalue(xmlNodePtr where, const hk_string &tag, bool &value, int position)
{
    hk_string v;
    xmlNodePtr r=get_tagvalue(where,tag,v,position);
    if (r) value=(v=="YES");

    return r;

}


xmlNodePtr hk_class::get_tagvalue(xmlNodePtr &where, const hk_string &tag)
{
    hk_string v;
    xmlNodePtr r=get_tagvalue(where,tag,v,1);

    return r;

}

// END NEW
























void hk_class::set_tagvalue(ostream& stream,const hk_string& tag,const hk_string& value)
{
#ifdef HK_DEBUG
//  cout <<"hk_class::set_tagvalue "<<tag<<endl;
//  p_generaldebug=true;
#endif

    set_tag(tag);
    set_levelspace(stream);
    if (value.size()==0)
    {//emptytag
	stream <<l2u(p_emptytag)<<endl;
	return;
    }
    hk_string v=replace_all("&",value,"&amp;");
    v=replace_all("<",v,"&lt;");
    stream << l2u(p_begintag)<<l2u(v)<<l2u(p_endtag)<<endl;
}


void hk_class::set_tagvalue(ostream& stream,const hk_string& tag,unsigned long value)
{
    set_tag(tag);
    set_levelspace(stream);
    stream << l2u(p_begintag)<<value<<l2u(p_endtag)<<endl;
}


void hk_class::set_tagvalue(ostream& stream,const hk_string& tag,unsigned int value)
{
    set_tag(tag);
    set_levelspace(stream);
    stream << l2u(p_begintag)<<value<<l2u(p_endtag)<<endl;
}


void hk_class::set_tagvalue(ostream& stream,const hk_string& tag,long value)
{
    set_tag(tag);
    set_levelspace(stream);
    stream << l2u(p_begintag)<<value<<l2u(p_endtag)<<endl;
}


void hk_class::set_tagvalue(ostream& stream,const hk_string& tag,int value)
{
    set_tag(tag);
    set_levelspace(stream);
    stream << l2u(p_begintag)<<value<<l2u(p_endtag)<<endl;
}


void hk_class::set_tagvalue(ostream& stream,const hk_string& tag,bool value)
{
    set_tag(tag);
    set_levelspace(stream);
    stream << l2u(p_begintag)<<l2u((value ?"YES":"NO"))<<l2u(p_endtag)<<endl;
}


void hk_class::set_tag(const hk_string& tag)
{
    p_begintag=p_begintag_begin+tag+p_begintag_end;
    p_endtag=p_endtag_begin+tag+p_endtag_end;
    p_emptytag=p_emptytag_begin+tag+p_emptytag_end;
}


void hk_class::start_mastertag(ostream& stream,const hk_string& tag)
{
    if (tag.size()>0)
    {
        set_levelspace(stream);
        stream <<p_begintag_begin<<tag<<p_begintag_end<<endl;
        p_taglevel++;
    }

}


void hk_class::end_mastertag(ostream& stream,const hk_string & tag)
{
    if (tag.size()>0)
    {
        if (p_taglevel>0)p_taglevel--;
        set_levelspace(stream);
        stream <<p_endtag_begin<<tag<<p_endtag_end<<endl;

    }
}


hk_string hk_class::defaultdateformat(void)
{
    return   p_defaultdateformat;
}


hk_string hk_class::defaulttimeformat(void)
{
    return p_defaulttimeformat;
}


hk_string hk_class::defaultdatetimeformat(void)
{
    return p_defaultdatetimeformat;
}


void hk_class::set_defaultdatetimeformat(const hk_string& tformat,const hk_string& dformat,const hk_string&dtformat)
{
    p_defaulttimeformat=tformat;
    p_defaultdateformat=dformat;
    p_defaultdatetimeformat=dtformat;

}


hk_string align2text(hk_visible::alignmenttype t)
{
    switch (t)
    {
        case hk_visible::alignright: return "RIGHT";
        case hk_visible::aligncenter: return "CENTER";
        default: return "LEFT";

    }

}


void hk_class::save_preferences(void)
{
    char* h= getenv("HOME");
    hk_string  p_classespath=(h==NULL?"/tmp":h);

    p_classespath.append("/").append(HKCCONFIGDIR);
    mkdir (p_classespath.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
    hk_string p_filename =p_classespath+"/preferences";
    ofstream* s= new ofstream(p_filename.c_str());
    if (s==NULL)return;
    *s<<"<?xml version=\"1.0\" ?>\n"<<endl;
    start_mastertag(*s,"PREFERENCES");
    set_tagvalue(*s,"HK_VERSION",(hk_string)VERSION);
    start_mastertag(*s,"GENERAL");
    set_tagvalue(*s,"SHOWPEDANTIC",p_showpedantic);
    set_tagvalue(*s,"DRIVERPATH",hk_drivermanager::path());
    set_tagvalue(*s,"DEFAULTFONT",hk_font::defaultfontname());
    set_tagvalue(*s,"DEFAULTFONTSIZE",hk_font::defaultfontsize());
    set_tagvalue(*s,"DEFAULTTEXTALIGNMENT",align2text(hk_visible::defaulttextalignment()));
    set_tagvalue(*s,"DEFAULTNUMBERALIGNMENT",align2text(hk_visible::defaultnumberalignment()));
    set_tagvalue(*s,"MAXIMIZEDWINDOWS",hk_visible::open_maximized_windows());
    set_tagvalue(*s,"DEFAULTPRECISION",(long)hk_dsdatavisible::defaultprecision());
    set_tagvalue(*s,"DEFAULTTHOUSANDSSEPARATOR",hk_dsdatavisible::defaultuse_numberseparator());
    set_tagvalue(*s,"DEFAULTDRIVER",p_defaultdriver);
    set_tagvalue(*s,"DEFAULTSIZETYPE",(hk_string)(hk_presentation::defaultsizetype()==hk_presentation::relative?"RELATIVE":"ABSOLUTE"));
    set_tagvalue(*s,"MEASURESYSTEM",(hk_string)(p_defaultmeasuresystem==hk_class::cm?"CM":"INCH"));
    set_tagvalue(*s,"SNAP2GRIDX",hk_presentation::snap2gridx());
    set_tagvalue(*s,"SNAP2GRIDY",hk_presentation::snap2gridy());
    set_tagvalue(*s,"AUTOMATIC_DATA_UPDATE",p_default_automatic_data_update);
    end_mastertag(*s,"GENERAL");

    start_mastertag(*s,"HK_REGIONAL");
    set_tagvalue(*s,"DEFAULTTIMEFORMAT",p_defaulttimeformat);
    set_tagvalue(*s,"DEFAULTDATETIMEFORMAT",p_defaultdatetimeformat);
    set_tagvalue(*s,"DEFAULTDATEFORMAT",p_defaultdateformat);
    set_tagvalue(*s,"LOCALE",p_locale);
    end_mastertag(*s,"HK_REGIONAL");
    start_mastertag(*s,"REPORT");
    set_tagvalue(*s,"PRINTERCOMMAND",hk_report::printcommand());
    set_tagvalue(*s,"EMBEDFONTS",hk_report::embedfonts());
    end_mastertag(*s,"REPORT");
    end_mastertag(*s,"PREFERENCES");
    s->close();

}


bool hk_class::runtime_only(void)
{
    return p_runtime_only;
}


void hk_class::set_levelspace(ostream& stream)
{
    //JKB if (stream==NULL) return;
    if (!stream) return;

    unsigned int l=p_taglevel;

    while (l>0)
    {
        stream <<"  ";
        l--;
    }

}


void hk_class::set_locale(const hk_string& locale)
{
    if (!setlocale(LC_ALL,locale.c_str()))
    {
        show_warningmessage(replace_all("%1",hk_translate("Warning! Your local charset '%1' could not be set!"),locale));
    }
    else
{
            setlocale(LC_NUMERIC,"C");
            setlocale(LC_MONETARY,"C");
      	    p_locale=locale;
}
}


hk_string hk_class::locale(void)
{
    return p_locale;
}


void hk_class::set_defaultdriver(const hk_string& d)
{
    p_defaultdriver=d;
}


hk_string hk_class::defaultdriver(void)
{
    return p_defaultdriver;

}


void hk_class::set_measuresystem(enum_measuresystem m)
{
p_defaultmeasuresystem=m;
}


hk_class::enum_measuresystem hk_class::measuresystem(void)
{

return p_defaultmeasuresystem;
}

hk_interpreter* hk_class::new_interpreter(const hk_string& interpreter,hk_presentation* p)
{
hk_string i=string2lower(interpreter);
 
if (i=="python")return new hk_pythoninterpreter(p);
return new hk_no_interpreter(p);

}


void hk_class::set_filedialog(file_dialogtype* f)
{
  p_filedialog=f;
}

hk_string hk_class::show_filedialog(const hk_string& url,enum_operationmode m)
{
   if (p_filedialog) return p_filedialog(url,m);
   hk_url u=url;
   hk_string sep="=============================================";
   int k;
   do
   {
   cout<<(m==file_open?hk_translate("Load file"):hk_translate("Save file"))<<endl;
   cout<<sep<<endl;
   cout<<"1 "<<replace_all("%1",hk_translate("Change directory (%1)"),u.directory())<<endl;
   cout<<"2 "<<replace_all("%1",hk_translate("Change file (%1)"),u.filename())<<endl;
   cout<<"3 "<<hk_translate("End input")<<endl;
   hk_string r;
   char c;
       do
       {
       cin.get(c);
       r=c;
       k=atoi(r.c_str());
       }
       while (k<1 ||k>3);
    if (k==1)
      {//set directory
       cout<<hk_translate("Enter directory:")<<endl;
       cin>>r;
       r=trim(r);
       hk_url tmpurl=r;
       if (!tmpurl.directory().size()>0 ||tmpurl.filename().size()>0)
       	{
        show_warningmessage(hk_translate("No valid directory name"));
	}
	else
	u=r+u.filename()+(u.extension().size()>0?"."+u.extension():"");
      }
    else
    if (k==2)
      {
       cout<<hk_translate("Enter filename:")<<endl;
       cin>>r;
       r=trim(r);
       hk_url tmpurl=r;
       if (!tmpurl.is_valid()) show_warningmessage(hk_translate("No valid file name"));
       else
       u=u.directory()+r;
      }
  }
  while(k!=3);

  return u.url();
}





void hk_class::set_directorydialog(stringvalue_dialogtype* f)
{
  p_directorydialog=f;
}

hk_string hk_class::show_directorydialog(const hk_string& url)
{
   if (p_directorydialog) return p_directorydialog(url);
   hk_url u=url;
   hk_string sep="=============================================";
   int k;
   do
   {
   cout<<hk_translate("Select directory:")<<endl;
   cout<<sep<<endl;
   cout<<"1 "<<replace_all("%1",hk_translate("Change directory (%1)"),u.directory())<<endl;
   cout<<"2 "<<hk_translate("End input")<<endl;
   hk_string r;
   char c;
       do
       {
       cin.get(c);
       r=c;
       k=atoi(r.c_str());
       }
       while (k<1 ||k>3);
    if (k==1)
      {//set directory
       cout<<hk_translate("Enter directory:")<<endl;
       cin>>r;
       r=trim(r);
       hk_url tmpurl=r;
       if (!tmpurl.directory().size()>0 ||tmpurl.filename().size()>0)
       	{
        show_warningmessage(hk_translate("No valid directory name"));
	}
	else
	u=r+u.filename()+(u.extension().size()>0?"."+u.extension():"");
      }
  }
  while(k!=2);

  return u.url();
}


void hk_class::set_default_automatic_data_update(bool a)
{
p_default_automatic_data_update=a;
}

bool hk_class::default_automatic_data_update(void)
{
return p_default_automatic_data_update;
}






void show_warningmessage(const hk_string& m)
{
  hk_class::show_warningmessage(m);
}

bool show_yesnodialog(const hk_string& m, bool default_value)
{
  return hk_class::show_yesnodialog(m,default_value);
}


hk_string show_filedialog(const hk_string& f,enum_operationmode m)
{
  return hk_class::show_filedialog(f,m);
}


hk_string show_directorydialog(const hk_string& t)
{
  return hk_class::show_directorydialog(t);
}


hk_string show_stringvaluedialog(const hk_string& t)
{
  return hk_class::show_stringvaluedialog(t);
}


