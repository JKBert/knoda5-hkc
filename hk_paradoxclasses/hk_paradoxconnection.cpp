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
#include "hk_paradoxconnection.h"
#include "hk_paradoxdatabase.h"
#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include "hk_drivermanager.h"
#include "hk_actionquery.h"
#include "hk_url.h"




int hk_paradoxconnection::p_reference=0;

hk_paradoxconnection::hk_paradoxconnection(hk_drivermanager* c):hk_connection(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxconnection::hk_paradoxconnection");
#endif
    if (p_reference==0)
     {
      PX_boot();
     }
    ++p_reference;
}


hk_paradoxconnection::~hk_paradoxconnection()
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxconnection::~hk_paradoxconnection");
#endif
  --p_reference;
  if (p_reference==0)
    {
      PX_shutdown();
    }
}



bool hk_paradoxconnection::server_supports(support_enum t) const
{
    switch (t)
    {
            case SUPPORTS_AUTOINCCOLUMN:;
            case SUPPORTS_BOOLCOLUMN:;
            case SUPPORTS_DATECOLUMN:;
            case SUPPORTS_TIMECOLUMN:;
//            case SUPPORTS_DATETIMECOLUMN:;
            case SUPPORTS_BINARYCOLUMN:;
            case SUPPORTS_MEMOCOLUMN:;
            case SUPPORTS_TIMESTAMPCOLUMN:;

	    case SUPPORTS_TEXTCOLUMN:;
            case SUPPORTS_INTEGERCOLUMN:;
            case SUPPORTS_SMALLINTEGERCOLUMN:;
            case SUPPORTS_FLOATINGCOLUMN:;
//            case SUPPORTS_SMALLFLOATINGCOLUMN:;

	    case SUPPORTS_PROPRIETARYCOLUMN:;

//            case SUPPORTS_SQL:;
//            case SUPPORTS_TRANSACTIONS:;
//            case SUPPORTS_REFERENTIALINTEGRITY:;
            case SUPPORTS_NEW_DATABASE:;
            case SUPPORTS_DELETE_DATABASE:;
//            case SUPPORTS_ALTER_TABLE:;
           case SUPPORTS_DELETE_TABLE:;
//            case SUPPORTS_CREATE_INDEX:;
//            case SUPPORTS_DELETE_INDEX:;
//            case SUPPORTS_CHANGE_PASSWORD:;
//            case SUPPORTS_RENAME_DATABASE:;
//            case SUPPORTS_RENAME_TABLE:;
//	    case SUPPORTS_VIEWS:;
//	    case SUPPORTS_NEW_VIEW:;
//	    case SUPPORTS_ALTER_VIEW:;
//	    case SUPPORTS_DELETE_VIEW:;
//	    case SUPPORTS_ADD_COLUMN:;
//	    case SUPPORTS_DELETE_COLUMN:;
//	    case SUPPORTS_CHANGE_COLUMNTYPE:;
//	    case SUPPORTS_CHANGE_COLUMNNAME:;

//	    case SUPPORTS_SQL_GROUP_BY:;
//	    case SUPPORTS_SQL_ORDER_BY:;
//	    case SUPPORTS_SQL_HAVING:;
//	    case SUPPORTS_SQL_WHERE:;
//	    case SUPPORTS_SQL_ALIAS:;

	    case SUPPORTS_LOCAL_FILEFORMAT:;
	    case SUPPORTS_NONALPHANUM_FIELDNAMES:;
	    case SUPPORTS_NONASCII_FIELDNAMES:;
	    case SUPPORTS_SPACE_FIELDNAMES:;

            			return true;
            case SUPPORTS_NEW_TABLE:
#if PXLIB_MAJOR_VERSION>0 || (PXLIB_MAJOR_VERSION ==0 && PXLIB_MINOR_VERSION > 5)
return true;
#else
return false;
#endif
;
        default : return false;
    }

}


hk_string hk_paradoxconnection::drivername(void) const
{

    return "paradox";
}



bool hk_paradoxconnection::server_needs(need_enum t) const
{
    switch (t)
    {
//         case NEEDS_NOTHING:
	case NEEDS_NULL_TERMINATED_SQL:
	case NEEDS_DIRECTORY_AS_DATABASE:
            return true;

        default: return false;
    }

}



bool  hk_paradoxconnection::driver_specific_connect()
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxconnection::driver_specific_connect");
#endif
p_connected=true;
return true;
}




bool hk_paradoxconnection::driver_specific_disconnect()

{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxconnection::driver_specific_disconnect");
#endif
return true;

}



hk_database* hk_paradoxconnection::driver_specific_new_database(void)
{
    hk_paradoxdatabase* db;
    db = new hk_paradoxdatabase(this);
    return db;
}



vector<hk_string>* hk_paradoxconnection::driver_specific_dblist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxconnection::driver_specific_dblist");
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
            if (S_ISDIR(statbuf.st_mode))
	    {
                hk_string n=entry->d_name;
  	        hk_string::size_type p =n.find(".db");
		if (p==hk_string::npos)
		   p =n.find(".DB");
		if (p==hk_string::npos &&n!="output" &&n!="."&&n!="..")
		p_databaselist.insert(p_databaselist.end(),n);
	    }
        }
        closedir(dp);

    }
    sort(p_databaselist.begin(),p_databaselist.end());
    return &p_databaselist;
}



bool    hk_paradoxconnection::create_database(const hk_string& dbase)
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










bool    hk_paradoxconnection::delete_database(const hk_string& dbase,enum_interaction c)
{
    hk_string warning=hk_translate("Delete the database \"%DBNAME%\"?");
    warning=replace_all("%DBNAME%",warning,dbase);
    if(c==interactive && !show_yesnodialog(warning,true)) return false;

    hk_url url=dbase;
    hk_string filename=url.directory().size()>0?dbase:databasepath()+"/"+dbase;
    cerr <<"delete database:#"<<filename<<"#"<<endl;
    delete_databasedirectory(filename);
    return true;

}



















//***********************************************************

hk_connection* create_connection(hk_drivermanager* cl)
{
    return new hk_paradoxconnection(cl);
}


hk_string hk_classesversion(void)
{
return (hk_string)HK_VERSION;
}
