// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_sqlite3classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include <hk_sqlite3connection.h>
#include <hk_sqlite3database.h>
#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include "hk_drivermanager.h"
#include "hk_actionquery.h"
#include "hk_url.h"

#ifndef SQLITE_VERSION_NUMBER
#define SQLITE_VERSION_NUMBER 0
#endif

hk_sqlite3connection::hk_sqlite3connection(hk_drivermanager* c):hk_connection(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3connection::hk_sqlite3connection");
#endif
    /*p_SQL_Connection=NULL;
    set_tcp_port(default_tcp_port());*/
    
    const char* sqltokens[]=
{
"ABORT","AFTER","ATTACH","AUTOINCREMENT","BEFORE","CONFLICT","DATABASE","DETACH",
"EACH","EXCLUSIVE","EXPLAIN","FAIL","GLOB","IGNORE","INDEX","INSTEAD","ISNULL",
"LIMIT","NOTNULL","OFFSET","PRAGMA","RAISE","REINDEX","RENAME","REPLACE","ROW",
"STATEMENT","TEMP","TRIGGER","VACUUM"
};
   for (unsigned int i=0;i<sizeof(sqltokens)/sizeof(const char*);++i)
    {
    p_sqlstatements.push_back(sqltokens[i]);
    }



}


hk_sqlite3connection::~hk_sqlite3connection()
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3connection::~hk_sqlite3connection");
#endif
}


bool  hk_sqlite3connection::driver_specific_connect()
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3connection::driver_specific_connect");
#endif
p_connected=true;
return true;
}




bool hk_sqlite3connection::driver_specific_disconnect()

{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3connection::driver_specific_disconnect");
#endif
return true;

}


vector<hk_string>* hk_sqlite3connection::driver_specific_dblist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3connection::driver_specific_dblist");
#endif
    DIR *dp;
    struct dirent *entry;
    hk_string datei;
    hk_string fileend=".hk_sqlite3";

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


hk_database* hk_sqlite3connection::driver_specific_new_database(void)
{
    hk_sqlite3database* db;
    db = new hk_sqlite3database(this);
    return db;
}



bool hk_sqlite3connection::server_supports(support_enum t) const
{
    switch (t)
    {
	  case SUPPORTS_ADD_COLUMN:;
	  case SUPPORTS_ALTER_TABLE:if (SQLITE_VERSION_NUMBER<3002000) return false;
          case SUPPORTS_NEW_DATABASE:;
          case SUPPORTS_DELETE_DATABASE:;
          case SUPPORTS_NEW_TABLE:;
          case SUPPORTS_DELETE_TABLE:;
	  //case SUPPORTS_REFERENTIALINTEGRITY:;
	  case SUPPORTS_RENAME_TABLE:;
	  case SUPPORTS_TRANSACTIONS:;
	  case SUPPORTS_SQL:;
          case  SUPPORTS_AUTOINCCOLUMN:;
          case SUPPORTS_BOOLCOLUMN:;
          case SUPPORTS_DATECOLUMN:;
          case SUPPORTS_TIMECOLUMN:;
          case SUPPORTS_DATETIMECOLUMN:;
          case SUPPORTS_BINARYCOLUMN:;
          case SUPPORTS_MEMOCOLUMN:;
          case SUPPORTS_TIMESTAMPCOLUMN:;
	  case SUPPORTS_TEXTCOLUMN:;
          case SUPPORTS_INTEGERCOLUMN:;
          case SUPPORTS_SMALLINTEGERCOLUMN:;
          case SUPPORTS_FLOATINGCOLUMN:;
          case SUPPORTS_SMALLFLOATINGCOLUMN:;
	  case SUPPORTS_LOCAL_FILEFORMAT:
	  case SUPPORTS_VIEWS:
	  case SUPPORTS_NEW_VIEW:
	  case SUPPORTS_ALTER_VIEW:
	  case SUPPORTS_DELETE_VIEW:
	  case SUPPORTS_CREATE_INDEX:
	  case SUPPORTS_DELETE_INDEX:
	  case SUPPORTS_SQL_GROUP_BY:
	  case SUPPORTS_SQL_ORDER_BY:
	  case SUPPORTS_SQL_HAVING:
	  case SUPPORTS_SQL_WHERE:
	  case SUPPORTS_SQL_ALIAS:
	  case SUPPORTS_NONALPHANUM_FIELDNAMES:;
	  case SUPPORTS_NONASCII_FIELDNAMES:;
	  case SUPPORTS_SPACE_FIELDNAMES:;
	  case SUPPORTS_SQL_JOINS:;
	  case SUPPORTS_SQL_UNION:;
            return true;
        default : return false;
    }

}


hk_string hk_sqlite3connection::drivername(void) const
{

    return "sqlite3";
}



bool hk_sqlite3connection::server_needs(need_enum t) const
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


bool hk_sqlite3connection::driver_specific_new_password(const hk_string& /*newpasswd*/)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3connection::driver_specific_new_password");
#endif
    return true;

}


void hk_sqlite3connection::servermessage(const hk_string& error)
{
       set_last_servermessage(error);

}


unsigned int    hk_sqlite3connection::default_tcp_port(void) const
{
    return 3306;
}


bool    hk_sqlite3connection::create_database(const hk_string& dbase)
{
    if (database()==NULL) new_database();
    if (database()==NULL||!is_connected()) return false;
    driver_specific_dblist();
    if (find(p_databaselist.begin(),p_databaselist.end(),dbase)!=p_databaselist.end())
      return false;
      hk_url url=dbase;
      hk_string n=(url.directory().size()>0?dbase:databasepath()+"/"+dbase+".hk_sqlite3");

      ifstream ifs(n.c_str(),ios::in);
      if (ifs)
        {
         //servermessage(hk_translate("Database already exists"));
	  return false;
	}


      sqlite3* p_sqlitehandler;
      if (sqlite3_open(n.c_str(),&p_sqlitehandler))
      {
         servermessage(sqlite3_errmsg(p_sqlitehandler));
	 return false;

      }
    sqlite3_close(p_sqlitehandler);
    hk_database* db=driver_specific_new_database();
    if (db && database_exists(dbase))
    {db->set_name(dbase);
    db->create_centralstoragetable();
    delete db;
    return true;
    }
    else return false;
}

bool    hk_sqlite3connection::delete_database(const hk_string& dbase,enum_interaction c)
{
    hk_string warning=hk_translate("Delete the database \"%DBNAME%\"?");
    warning=replace_all("%DBNAME%",warning,dbase);
    if(c==interactive && !show_yesnodialog(warning,true)) return false;

    hk_url url=dbase;
    hk_string filename=url.directory().size()>0?dbase:databasepath()+"/"+dbase+".hk_sqlite3";
    int r=unlink(filename.c_str());
    return r==0;

}



//***********************************************************

hk_connection* create_connection(hk_drivermanager* cl)
{
    return new hk_sqlite3connection(cl);
}


hk_string hk_classesversion(void)
{
return (hk_string)HK_VERSION;
}
