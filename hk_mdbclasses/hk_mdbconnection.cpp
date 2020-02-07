// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mdbclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_mdbconnection.h"
#include "hk_mdbdatabase.h"
#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include "hk_drivermanager.h"
#include "hk_actionquery.h"
#include "hk_url.h"

int hk_mdbconnection::p_reference=0;


hk_mdbconnection::hk_mdbconnection(hk_drivermanager* c):hk_connection(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_mdbconnection::hk_mdbconnection");
#endif
    /*p_SQL_Connection=NULL;
    set_tcp_port(default_tcp_port());*/
    if (p_reference==0)
     {
      mdb_init();
     }
    ++p_reference;
}


hk_mdbconnection::~hk_mdbconnection()
{
#ifdef HK_DEBUG
    hkdebug("hk_mdbconnection::~hk_mdbconnection");
#endif

  --p_reference;
  if (p_reference==0)
    {
     mdb_exit();
    }
}



bool hk_mdbconnection::server_supports(support_enum t) const
{
    switch (t)
    {
	  //case SUPPORTS_SQL:;
          //case  SUPPORTS_AUTOINCCOLUMN:;
          case SUPPORTS_BOOLCOLUMN:;
          //case SUPPORTS_DATECOLUMN:;
          //case SUPPORTS_TIMECOLUMN:;
          case SUPPORTS_DATETIMECOLUMN:;
          //case SUPPORTS_BINARYCOLUMN:;
	  case SUPPORTS_TEXTCOLUMN:;
          case SUPPORTS_INTEGERCOLUMN:;
          case SUPPORTS_SMALLINTEGERCOLUMN:;
          case SUPPORTS_FLOATINGCOLUMN:;
          case SUPPORTS_SMALLFLOATINGCOLUMN:;
          case SUPPORTS_MEMOCOLUMN:;
	  case SUPPORTS_LOCAL_FILEFORMAT:
	  //case SUPPORTS_SQL_WHERE:;
	  case SUPPORTS_NONALPHANUM_FIELDNAMES:;
	  case SUPPORTS_NONASCII_FIELDNAMES:;
	  case SUPPORTS_SPACE_FIELDNAMES:;
	  //case SUPPORTS_NEW_TABLE:;
            return true;
        default : return false;
    }

}


hk_string hk_mdbconnection::drivername(void) const
{

    return "mdb";
}



bool hk_mdbconnection::server_needs(need_enum t) const
{
    switch (t)
    {
//         case NEEDS_NOTHING:
	case NEEDS_NULL_TERMINATED_SQL:
	case NEEDS_MANUAL_CHARSET:
            return true;

        default: return false;
    }

}



bool  hk_mdbconnection::driver_specific_connect()
{
#ifdef HK_DEBUG
    hkdebug("hk_mdbconnection::driver_specific_connect");
#endif
p_connected=true;
return true;
}




bool hk_mdbconnection::driver_specific_disconnect()

{
#ifdef HK_DEBUG
    hkdebug("hk_mdbconnection::driver_specific_disconnect");
#endif
return true;

}



hk_database* hk_mdbconnection::driver_specific_new_database(void)
{
    hk_mdbdatabase* db;
    db = new hk_mdbdatabase(this);
    return db;
}



vector<hk_string>* hk_mdbconnection::driver_specific_dblist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mdbconnection::driver_specific_dblist");
#endif
    DIR *dp;
    struct dirent *entry;
    hk_string datei;
    hk_string fileend=".mdb";

    p_databaselist.erase(p_databaselist.begin(),p_databaselist.end());
    if ((dp=opendir(databasepath().c_str()))!=NULL)
    {

        while ((entry = readdir(dp))!=NULL)
        {
	    datei=entry->d_name;
    	   hk_string fname=databasepath()+"/"+datei;
           struct stat statbuf;
	   stat(fname.c_str(),&statbuf);
            if (S_ISREG(statbuf.st_mode))
	    {
	    hk_string::size_type p =datei.find(fileend);
            if (p!=hk_string::npos)
            {
                datei.replace(p,datei.size()-p,"");
                p_databaselist.insert(p_databaselist.end(),datei);
            }
	    }
        }
        closedir(dp);

    }
    sort(p_databaselist.begin(),p_databaselist.end());
    return &p_databaselist;
}





















//***********************************************************

hk_connection* create_connection(hk_drivermanager* cl)
{
    return new hk_mdbconnection(cl);
}


hk_string hk_classesversion(void)
{
return (hk_string)HK_VERSION;
}
