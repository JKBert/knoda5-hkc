// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_firebirdclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_firebirdconnection.h"
#include "hk_firebirddatabase.h"
#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#ifdef HAVE_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif

#include "hk_drivermanager.h"
#include "hk_actionquery.h"
#include "hk_url.h"

#define FBIRD_DPB_BUFFER_LEN 4096


hk_firebirdconnection::hk_firebirdconnection(hk_drivermanager* c):hk_connection(c)
{
#ifdef HK_DEBUG
// wanna_debug(true);
    hkdebug("hk_firebirdconnection::hk_firebirdconnection");
#endif
    /*p_SQL_Connection=NULL;
    set_tcp_port(default_tcp_port());*/
    set_tcp_port(default_tcp_port());
    set_host("localhost");
    set_user("sysdba");
    p_connectionsuccessfullyestablished=false;
    p_firebirdhandle=NULL;

const char* sqltokens[]=
{
"ACTIVE","ADMIN","AFTER","AS","ASCENDING","AT","AUTO","AUTODDL","BASED","BASENAME","BASE_NAME","BEFORE",
"BLOB","BLOBEDIT","BUFFER","BY","CACHE","CHECK_POINT_LEN","CHECK_POINT_LENGTH","COMMITTED","COMPILETIME",
"COMPUTED","CONDITIONAL","CONTAINING","CSTRING","DATABASE","DB_KEY","DEBUG","DESCENDING","DISPLAY","ECHO",
"EDIT","ENTRY_POINT","EVENT","EXIT","EXTERN","FILE","FILTER","FREE_IT","FUNCTION","GDSCODE","GENERATOR",
"GEN_ID","GROUP_COMMIT_","GROUP_COMMIT_WAIT","HELP","IN","INACTIVE","INDEX","INIT","INPUT_TYPE",
"INTERBASE","IS","ISQL","KEYWORDS","KEYWORDS","LC_MESSAGES","LC_TYPE","LENGTH","LEV","LOGFILE",
"LOG_BUFFER_SIZE","LOG_BUF_SIZE","LONG","MANUAL","MAXIMUM","MAXIMUM_SEGMENT","MAX_SEGMENT","MERGE MESSAGE",
"MINIMUM","MODULE_NAME","NO","NOAUTO","NUM_LOG_BUFFERS","NUM_LOG_BUFS","ON","OR","OUTPUT_TYPE","OVERFLOW",
"PAGE","PAGELENGTH","PAGES","PAGE_SIZE","PARAMETER","PASSWORD","PLAN","POST_EVENT","PROTECTED","QUIT",
"RAW_PARTITIONS","RDB$DB_KEY","RECORD_VERSION","RELEASE","RESERV","RESERVING","RETAIN","RETURN","RETURNING_VALUES",
"RETURNS","ROLE","RUNTIME","SEGMENT","SHADOW","SHARED","SHELL","SHOW","SINGULAR","SNAPSHOT","SORT","SQLWARNING",
"STABILITY","STARTING","STARTS","STATEMENT","STATIC","STATISTICS","SUB_TYPE","SUSPEND","TERMINATOR","TO",
"TRIGGER","TYPE","UNCOMMITTED","VARIABLE","VERSION","WAIT","WAIT_TIME","WEEKDAY","WHILE","YEARDAY"
};
   for (unsigned int i=0;i<sizeof(sqltokens)/sizeof(const char*);++i)
    {
    p_sqlstatements.push_back(sqltokens[i]);
    }



}


hk_firebirdconnection::~hk_firebirdconnection()
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdconnection::~hk_firebirdconnection");
#endif

}

void hk_firebirdconnection::servermessage(const hk_string& error)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdconnection::servermessage",error);
#endif
       set_last_servermessage(error);

}


bool hk_firebirdconnection::server_supports(support_enum t) const
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
            case SUPPORTS_SMALLFLOATINGCOLUMN:;

	    case SUPPORTS_PROPRIETARYCOLUMN:;

            case SUPPORTS_SQL:;
//            case SUPPORTS_TRANSACTIONS:;
            case SUPPORTS_REFERENTIALINTEGRITY:;
            case SUPPORTS_NEW_DATABASE:;
            case SUPPORTS_DELETE_DATABASE:;
            case SUPPORTS_NEW_TABLE:;
            case SUPPORTS_ALTER_TABLE:;
            case SUPPORTS_DELETE_TABLE:;
            case SUPPORTS_CREATE_INDEX:;
            case SUPPORTS_DELETE_INDEX:;
//            case SUPPORTS_CHANGE_PASSWORD:;
//            case SUPPORTS_RENAME_DATABASE:;
            case SUPPORTS_RENAME_TABLE:;
	    case SUPPORTS_VIEWS:;
	    case SUPPORTS_NEW_VIEW:;
	    case SUPPORTS_ALTER_VIEW:;
	    case SUPPORTS_DELETE_VIEW:;
	    case SUPPORTS_ADD_COLUMN:;
	    case SUPPORTS_DELETE_COLUMN:;
	    case SUPPORTS_CHANGE_COLUMNTYPE:;
	    case SUPPORTS_CHANGE_COLUMNNAME:;
//	    case SUPPORTS_ALTER_PRIMARY_KEY:;
//	    case SUPPORTS_ALTER_NOT_NULL:;

	    case SUPPORTS_SQL_GROUP_BY:;
	    case SUPPORTS_SQL_ORDER_BY:;
	    case SUPPORTS_SQL_HAVING:;
	    case SUPPORTS_SQL_WHERE:;
	    case SUPPORTS_SQL_ALIAS:;

	    case SUPPORTS_LOCAL_FILEFORMAT:;
	    case SUPPORTS_NONALPHANUM_FIELDNAMES:;
	    case SUPPORTS_NONASCII_FIELDNAMES:;
	    case SUPPORTS_SPACE_FIELDNAMES:;
	    case SUPPORTS_SQL_JOINS:;
            return true;
        default : return false;
    }

}


hk_string hk_firebirdconnection::drivername(void) const
{

    return "firebird";
}



bool hk_firebirdconnection::server_needs(need_enum t) const
{
    switch (t)
    {
        case NEEDS_LOGIN:
	case NEEDS_HOST:
        case NEEDS_USERNAME:
        case NEEDS_PASSWORD:
//        case NEEDS_PORT:
	case NEEDS_NULL_TERMINATED_SQL:
	case NEEDS_DATABASENAME:
	case NEEDS_BOOLEANEMULATION:
             return true;

        default: return false;
    }

}



bool  hk_firebirdconnection::driver_specific_connect()
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdconnection::driver_specific_connect");
#endif


    char dpb_buffer[FBIRD_DPB_BUFFER_LEN], *dpb;
    char charset[] = "NONE";
    short dpb_length;
    dpb = dpb_buffer;
    *dpb++ = isc_dpb_version1;

    int len = strlen(user().c_str());
    if(len > 0)
    {
		*dpb++ = isc_dpb_user_name;
		*dpb++ = len;
		strcpy (dpb, user().c_str());
		dpb += len;
	}

	len = strlen(password().c_str());
	if(len > 0)
	{
		*dpb++ = isc_dpb_password;
		*dpb++ = len;
		strcpy (dpb, password().c_str());
		dpb += len;
	}

	len = strlen(charset);
	{
		*dpb++ = isc_dpb_lc_ctype;
		*dpb++ = len;
		strcpy (dpb, charset);
		dpb += len;
	}

    dpb_length = dpb - dpb_buffer;
    hk_string connectstring;
    if (host().size()>0)
      {
        connectstring=host()+"/"+ulongint2string(tcp_port())+":";

      }
    connectstring+=defaultdatabase();
    if (isc_attach_database(p_status, (short) strlen(connectstring.c_str()),const_cast<char*> (connectstring.c_str()), &p_firebirdhandle,
       dpb_length, dpb_buffer))
    {
      set_error();
        return false;

    }
p_connected=true;
return true;
}




bool hk_firebirdconnection::driver_specific_disconnect()

{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdconnection::driver_specific_disconnect");
#endif


    if (p_connected)
    {
    isc_detach_database(p_status, &p_firebirdhandle);
        p_firebirdhandle=NULL;
        p_connected=false;

    }
    return p_connected;


}



hk_database* hk_firebirdconnection::driver_specific_new_database(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdconnection::driver_specific_new_database");
#endif
    hk_firebirddatabase* db;
    db = new hk_firebirddatabase(this);
    if (db)
      db->set_name(defaultdatabase());
    return db;
}



vector<hk_string>* hk_firebirdconnection::driver_specific_dblist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdconnection::driver_specific_dblist");
#endif
    p_databaselist.erase(p_databaselist.begin(),p_databaselist.end());
    p_databaselist.insert(p_databaselist.end(),defaultdatabase());
    return &p_databaselist;
}






unsigned int    hk_firebirdconnection::default_tcp_port(void) const
{
    return 3050;
}






bool hk_firebirdconnection::create_database(const hk_string& db)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdconnection::create_database");
#endif

    hk_string dbase=db;
    if (!server_supports(SUPPORTS_NEW_DATABASE))
      {
       show_warningmessage(hk_translate("The driver does not support creating new databases!"));
       return false;
      }
    hk_url url=dbase;
    if (url.extension().size()==0) dbase=dbase+".fdb";
    if (url.directory().size()==0)
    {
     hk_url def=defaultdatabase();
     if (def.directory().size()>0)
      dbase=def.directory()+"/"+dbase;
    }

    if (database()==NULL) new_database();
    if (database()==NULL||!is_connected())
    {
      show_warningmessage(hk_translate("Not connected to the server"));
      return false;
    }
    hk_actionquery* q=database()->new_actionquery();
    if (!q)
    	{
               show_warningmessage(hk_translate("Bug: Actionquery could not be created"));
		return false;
	}
    hk_string sql="CREATE DATABASE '"+dbase;
    sql+="' USER '"+user()+"' PASSWORD '"+password()+"'";

    bool result=false;
    if (q!=NULL)
    {
        q->set_sql(sql.c_str(),sql.size());
        result=q->execute();
        delete  q;
    }
/*    if (!result)
    {
        hk_string reason=hk_translate("Database could not be created!")+"\n"+hk_translate("Servermessage: ")+last_servermessage();
        show_warningmessage(reason);
    }*/
    return result;
}



bool    hk_firebirdconnection::driver_specific_delete_database(const hk_string& dbase)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdconnection::delete_database");
#endif
    bool result=false;
    if (database()==NULL) return false;
    if (dbase!=database()->name())
    {

     isc_db_handle db1=NULL;
       if (isc_attach_database(p_status, dbase.size(), const_cast<char*>(dbase.c_str()), &db1, 0, NULL))
       {
      set_error();
        return false;
       }
          isc_drop_database(p_status,&db1);
    if (p_status[0] == 1 && p_status[1])
    {
      set_error();
      return false;
    }
    else
    {
    isc_detach_database(p_status, &db1);
     return true;

    }
    }

    isc_drop_database(p_status,&p_firebirdhandle);
    if (p_status[0] == 1 && p_status[1])
    {
      result=false;
      set_error();
    }
    else
    {
    disconnect();
     result=true;

    }
  return result;
}


void hk_firebirdconnection::set_error(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdconnection::set_error");
#endif
#ifdef HAVE_FIREBIRD2
  const ISC_STATUS* pvector=p_status;
#else
  long* pvector=p_status;
#endif
  char msg[512];
  hk_string errors;
  while (
#ifdef HAVE_FIREBIRD2
  fb_interpret(msg,sizeof(msg),&pvector)
#else
  isc_interprete(msg,&pvector)
#endif 
)
  {
    errors+=msg;
    errors+="\n";
  }
  servermessage(errors);
}








//***********************************************************

hk_connection* create_connection(hk_drivermanager* cl)
{
    return new hk_firebirdconnection(cl);
}


hk_string hk_classesversion(void)
{
return (hk_string)HK_VERSION;
}
