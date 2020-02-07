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
#include "hk_dbaseconnection.h"
#include "hk_dbasedatabase.h"
#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include "hk_drivermanager.h"
#include "hk_actionquery.h"
#include "hk_url.h"



hk_dbaseconnection::hk_dbaseconnection(hk_drivermanager* c):hk_connection(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbaseconnection::hk_dbaseconnection");
#endif
    /*p_SQL_Connection=NULL;
    set_tcp_port(default_tcp_port());*/
}


hk_dbaseconnection::~hk_dbaseconnection()
{
#ifdef HK_DEBUG
    hkdebug("hk_dbaseconnection::~hk_dbaseconnection");
#endif

}



bool hk_dbaseconnection::server_supports(support_enum t) const
{
    switch (t)
    {
          case SUPPORTS_NEW_DATABASE:;
          case SUPPORTS_DELETE_DATABASE:;
          case SUPPORTS_NEW_TABLE:;
          case SUPPORTS_DELETE_TABLE:;
// 	  case SUPPORTS_SQL:;
          case SUPPORTS_BOOLCOLUMN:;
          case SUPPORTS_DATECOLUMN:;
          case SUPPORTS_MEMOCOLUMN:;
	  case SUPPORTS_TEXTCOLUMN:;
          case SUPPORTS_INTEGERCOLUMN:;
//           case SUPPORTS_SMALLINTEGERCOLUMN:;
          case SUPPORTS_FLOATINGCOLUMN:;
//           case SUPPORTS_SMALLFLOATINGCOLUMN:;
	  case SUPPORTS_LOCAL_FILEFORMAT:
// 	  case SUPPORTS_SQL_GROUP_BY:
// 	  case SUPPORTS_SQL_ORDER_BY:
// 	  case SUPPORTS_SQL_HAVING:
// 	  case SUPPORTS_SQL_WHERE:
// 	  case SUPPORTS_SQL_ALIAS:
// 	  case SUPPORTS_NONALPHANUM_FIELDNAMES:;
// 	  case SUPPORTS_NONASCII_FIELDNAMES:;
// 	  case SUPPORTS_SPACE_FIELDNAMES:;
          return true;
        default : return false;
    }

}


hk_string hk_dbaseconnection::drivername(void) const
{

    return "dbase";
}



bool hk_dbaseconnection::server_needs(need_enum t) const
{
    switch (t)
    {
//         case NEEDS_NOTHING:
	case NEEDS_NULL_TERMINATED_SQL:
	case NEEDS_MANUAL_CHARSET:
	case NEEDS_DIRECTORY_AS_DATABASE:
            return true;

        default: return false;
    }

}



bool  hk_dbaseconnection::driver_specific_connect()
{
#ifdef HK_DEBUG
    hkdebug("hk_dbaseconnection::driver_specific_connect");
#endif
p_connected=true;
return true;
}




bool hk_dbaseconnection::driver_specific_disconnect()

{
#ifdef HK_DEBUG
    hkdebug("hk_dbaseconnection::driver_specific_disconnect");
#endif
return true;

}



hk_database* hk_dbaseconnection::driver_specific_new_database(void)
{
    hk_dbasedatabase* db;
    db = new hk_dbasedatabase(this);
    return db;
}



vector<hk_string>* hk_dbaseconnection::driver_specific_dblist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbaseconnection::driver_specific_dblist");
#endif
    DIR *dp;
    struct dirent *entry;

    p_databaselist.erase(p_databaselist.begin(),p_databaselist.end());
    if ((dp=opendir(databasepath().c_str()))!=NULL)
    {

        while ((entry = readdir(dp))!=NULL)
        {
           struct stat statbuf;
	   stat(entry->d_name,&statbuf);
            if (statbuf.st_mode & S_IFDIR)
	    {
                hk_string n=entry->d_name;
  	        hk_string::size_type p =n.find(".dbf");
		if (p==hk_string::npos)
		   p =n.find(".DBF");
		if (p==hk_string::npos &&n!="output" &&n!="."&&n!="..")
		p_databaselist.insert(p_databaselist.end(),n);
	    }
        }
        closedir(dp);

    }
    sort(p_databaselist.begin(),p_databaselist.end());
    return &p_databaselist;
}




bool    hk_dbaseconnection::create_database(const hk_string& dbase)
{

    /*hk_database* db=driver_specific_new_database();
    if (db && database_exists(dbase))
    {db->set_name(dbase);
    db->create_centralstoragetable();
    delete db;
    return true;
    }
    else return false;
   */
   hk_url url=dbase;
   hk_string n=(url.directory().size()>0?dbase:databasepath()+"/"+dbase);
   mkdir (n.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
   return true;
}










bool    hk_dbaseconnection::delete_database(const hk_string& dbase,enum_interaction c)
{
    hk_string warning=hk_translate("Delete the database \"%DBNAME%\"?");
    warning=replace_all("%DBNAME%",warning,dbase);
    if(c==interactive && !show_yesnodialog(warning,true)) return false;

    hk_url url=dbase;
    hk_string filename=url.directory().size()>0?dbase:databasepath()+"/"+dbase;
    delete_databasedirectory(filename);
    return true;

}



// unsigned int hk_dbaseconnection::maxfieldnamesize(void) const
// {
// return 10;
// }















//***********************************************************

hk_connection* create_connection(hk_drivermanager* cl)
{
    return new hk_dbaseconnection(cl);
}


hk_string hk_classesversion(void)
{
return (hk_string)HK_VERSION;
}
