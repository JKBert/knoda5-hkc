// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_paradoxclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_paradoxdatabase.h"
#include "hk_paradoxconnection.h"
#include "hk_paradoxtable.h"
#include "hk_paradoxresultquery.h"
#include "hk_paradoxactionquery.h"
#include "hk_url.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include <dlfcn.h>

hk_paradoxdatabase::hk_paradoxdatabase(hk_paradoxconnection* c):hk_database(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxdatabase::hk_paradoxdatabase");
#endif
  p_paradoxconnection=c;
}


hk_paradoxdatabase::~hk_paradoxdatabase(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxdatabase::~hk_paradoxdatabase");
#endif

}











void hk_paradoxconnection::servermessage(const hk_string& error)
{
       set_last_servermessage(error);

}


void hk_paradoxdatabase::driver_specific_tablelist(void)
{
    DIR *dp;
    struct dirent *entry;

p_tablelist.erase(p_tablelist.begin(),p_tablelist.end());
      hk_string dir= p_url.directory().size()>0? p_url.url(): p_paradoxconnection->databasepath()+"/"+name();

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
  	        hk_string::size_type p =n.find(".db");
		if (p==hk_string::npos)
		   p =n.find(".DB");
		if (p!=hk_string::npos)
		{
		  hk_url u=n;
		  p_tablelist.insert(p_tablelist.end(),u.filename());
		}
	    }
        }
        closedir(dp);

    }
   sort(p_tablelist.begin(),p_tablelist.end());
}


bool hk_paradoxdatabase::driver_specific_select_db(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxdatabase::driver_specific_select_db");
#endif

 return true;

}


hk_datasource* hk_paradoxdatabase::driver_specific_new_table(hk_presentation* p)
{
    return new hk_paradoxtable(this,p);
}


hk_datasource* hk_paradoxdatabase::driver_specific_new_resultquery(hk_presentation* p)
{
    return new hk_paradoxresultquery(this,p);
}

hk_actionquery* hk_paradoxdatabase::driver_specific_new_actionquery(void)
{
    return new hk_paradoxactionquery(this);
}


bool    hk_paradoxdatabase::delete_table(const hk_string& table,enum_interaction x)
{
    hk_string warning=hk_translate("Delete table \"%TBLNAME%\"?");
    warning=replace_all("%TBLNAME%",warning,table);
    if (x==interactive)
       if(!show_yesnodialog(warning,true)) return false;

    hk_url url=table;
    hk_string filename=url.directory().size()>0?table:database_path()+"/"+table+".db";
    hk_string blobfile=url.directory().size()>0?table:database_path()+"/"+table+".mb";
    int r=unlink(filename.c_str());
    bool result=(r==0);
    unlink(blobfile.c_str());
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



