// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_dbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_dbasedatabase.h"
#include "hk_dbaseconnection.h"
#include "hk_dbasetable.h"
#include "hk_dbaseresultquery.h"
#include "hk_dbaseactionquery.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>



hk_dbasedatabase::hk_dbasedatabase(hk_dbaseconnection* c):hk_database(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbasedatabase::hk_dbasedatabase");
#endif
  p_dbaseconnection=c;
//   p_dbasehandle=new xbDbf(&x);
  
}


hk_dbasedatabase::~hk_dbasedatabase(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbasedatabase::~hk_dbasedatabase");
#endif
//   delete p_dbasehandle;
}


void hk_dbaseconnection::servermessage(const hk_string& error)
{
       set_last_servermessage(error);

}


void hk_dbasedatabase::driver_specific_tablelist(void)
{
    DIR *dp;
    struct dirent *entry;

p_tablelist.erase(p_tablelist.begin(),p_tablelist.end());
      hk_string dir= p_url.directory().size()>0? p_url.url(): p_dbaseconnection->databasepath()+"/"+name();
    if ((dp=opendir(dir.c_str()))!=NULL)
    {

        while ((entry = readdir(dp))!=NULL)
        {
    	   hk_string fname=dir+"/"+entry->d_name;
           struct stat statbuf;
	   stat(fname.c_str(),&statbuf);

	   stat(fname.c_str(),&statbuf);
            if (S_ISREG(statbuf.st_mode))
	    {
                hk_string n=entry->d_name;
  	        hk_string::size_type p =n.find(".dbf");
		if (p==hk_string::npos)
		   p =n.find(".DBF");
		if (p!=hk_string::npos)
		{
		  hk_url u=n;
		  p_tablelist.insert(p_tablelist.end(),u.filename());
		}
	    }
        }
        closedir(dp);

    }
    else cerr <<"opendir fails"<<endl;
   sort(p_tablelist.begin(),p_tablelist.end());
}


bool hk_dbasedatabase::driver_specific_select_db(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbasedatabase::driver_specific_select_db");
#endif
//       xbShort rc;
//       hk_string n= p_url.directory().size()>0? p_url.directory(): p_dbaseconnection->databasepath()+"/"+name();
//       if ((rc=p_dbasehandle->OpenDatabase(n.c_str()))!=XB_NO_ERROR)
//       {
//          x.DisplayError( rc );
//          return false;
//       }
 return true;
}


hk_datasource* hk_dbasedatabase::driver_specific_new_table(hk_presentation* p)
{
    return new hk_dbasetable(this,p);
}


hk_datasource* hk_dbasedatabase::driver_specific_new_resultquery(hk_presentation* p)
{
    return new hk_dbaseresultquery(this,p);
}

hk_actionquery* hk_dbasedatabase::driver_specific_new_actionquery(void)
{
    return new hk_dbaseactionquery(this);
}



bool    hk_dbasedatabase::delete_table(const hk_string& table,enum_interaction x)
{
    hk_string warning=hk_translate("Delete table \"%TBLNAME%\"?");
    warning=replace_all("%TBLNAME%",warning,table);
    if (x==interactive)
       if(!show_yesnodialog(warning,true)) return false;

    hk_url url=table;
    hk_string filename=url.directory().size()>0?table:database_path()+"/"+table+".dbf";
    int r=unlink(filename.c_str());
    bool result=(r==0);
    if (result)
    {
     tablelist(false);
     inform_datasources_filelist_changes(lt_table);
    }
    else
    {
        hk_string reason=hk_translate("Table could not be deleted!")+"\n"+hk_translate("Servermessage: ")+connection()->last_servermessage();
        show_warningmessage(reason);
    }

    return r==0;

}

