// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_odbcclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_odbcconnection.h"
#include "hk_odbcdatabase.h"
#include <dlfcn.h>
#include "hk_drivermanager.h"
#include "hk_actionquery.h"
#include <sqltypes.h>
#include <sqlext.h>
#include "config.h"
class hk_odbcconnectionprivate
{
public:
};

hk_odbcconnection::hk_odbcconnection(hk_drivermanager* c):hk_connection(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcconnection::hk_odbcconnection");
#endif
    p_private=new hk_odbcconnectionprivate;
    SQLRETURN result =SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&p_odbcenvironment);
    result = SQLSetEnvAttr(p_odbcenvironment,SQL_ATTR_ODBC_VERSION,(void*) SQL_OV_ODBC3,0);
    result =SQLAllocHandle(SQL_HANDLE_DBC,p_odbcenvironment,&p_odbcconnectionhandle);
    p_unicodenames=false;
    set_tcp_port(default_tcp_port());
}


hk_odbcconnection::~hk_odbcconnection()
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcconnection::~hk_odbcconnection");
#endif
//       driver_specific_disconnect();
    SQLDisconnect(p_odbcconnectionhandle);
    SQLFreeHandle(SQL_HANDLE_DBC,p_odbcconnectionhandle);
    SQLFreeHandle(SQL_HANDLE_ENV,p_odbcenvironment);
    delete p_private;
}


bool  hk_odbcconnection::driver_specific_connect()
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcconnection::driver_specific_connect");
#endif

    if (!database())
    {
        return p_connected=true;

    }
    p_backend=unknown;
    if (!p_connected&& database())
    { 
//		p_connected=odbc_real_connect(p_SQL_Connection,host().c_str(),user().c_str(),password().c_str(),db,tcp_port(),xunix_port,0);
        SQLDisconnect(p_odbcconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_DBC,p_odbcconnectionhandle);
        SQLRETURN res =SQLAllocHandle(SQL_HANDLE_DBC,p_odbcenvironment,&p_odbcconnectionhandle);
        res=SQLConnect(p_odbcconnectionhandle,
            (database()->name().size()>0?
	    (SQLCHAR*)const_cast<char*>(database()->name().c_str()):
	    (SQLCHAR*) const_cast<char*>(defaultdatabase().c_str())
	    )
	    ,SQL_NTS,
            (SQLCHAR*)const_cast<char*>(user().c_str()),SQL_NTS,
            (SQLCHAR*)const_cast<char*>(password().c_str()),SQL_NTS
            );
        p_connected=( res==SQL_SUCCESS||res==SQL_SUCCESS_WITH_INFO);
        if (!p_connected)
        {
//if (show_passworddialog()) driver_specific_connect();
            cerr <<"no connection"<<endl;
	    set_last_servermessage(hk_translate("Connection not possible (wrong user and/or password?)"));
//		    odbc_close(p_SQL_Connection);
//		    p_odbcenvironment=NULL;
        } else
        {
            SQLCHAR dbmsname[50]="";
            SQLSMALLINT size;
            SQLGetInfo(p_odbcconnectionhandle,SQL_DBMS_NAME,dbmsname,sizeof(dbmsname),&size);
            cerr <<"ODBC Backend: "<<dbmsname<<endl;
            hk_string backend =string2upper((const char*)dbmsname);
            if (backend.find("MYSQL")!=hk_string::npos) p_backend=mysql;
            else
            if (backend.find("POSTGRES")!=hk_string::npos) p_backend=postgresql;
            else
            if (backend.find("MICROSOFT")!=hk_string::npos) p_backend=mssql;
            else
            if (backend.find("SAP")!=hk_string::npos
	        ||backend.find("MAXDB")!=hk_string::npos
		) p_backend=sapdb;
        }
    }
    p_unicodenames=(p_backend==sapdb);
    if (!p_connected) servermessage();
    return p_connected;

}


SQLHENV hk_odbcconnection::envhandler()
{
    return p_odbcenvironment;
}


bool hk_odbcconnection::driver_specific_disconnect()

{
#ifdef HK_DEBUG
    hkdebug("hk_odbcconnection::driver_specific_disconnect");
#endif
    if (p_connected)
    {
//SQLFreeHandle(SQL_HANDLE_ENV,p_odbcenvironment);
        p_connected=false;
//p_odbcenvironment=NULL;

    }
    return p_connected;

}


vector<hk_string>* hk_odbcconnection::driver_specific_dblist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcconnection::driver_specific_dblist");
#endif
    p_databaselist.erase(p_databaselist.begin(),p_databaselist.end());

    short int next;
    const int buffergroesse=100;
    short int beschreibungslaenge;
    short int databasenamelaenge;
    SQLCHAR db[buffergroesse];
    SQLCHAR beschreibung[buffergroesse];
    next=SQL_FETCH_FIRST;
    int count =0;
    SQLRETURN nreturn;
    while (
        (nreturn=SQLDataSources(    p_odbcenvironment,next,
        db,buffergroesse,&databasenamelaenge,
        beschreibung,buffergroesse,&beschreibungslaenge
        ))
        ==SQL_SUCCESS ||nreturn==SQL_SUCCESS_WITH_INFO)
    {
        p_databaselist.insert(p_databaselist.end(),(char*)db);
        next=SQL_FETCH_NEXT;
        count++;
    }

    return &p_databaselist;
}


hk_database* hk_odbcconnection::driver_specific_new_database(void)
{
    hk_odbcdatabase* db;
    db = new hk_odbcdatabase(this);
    return db;
}


bool hk_odbcconnection::server_supports(support_enum t) const
{
hk_odbcdatabase* db=dynamic_cast<hk_odbcdatabase*>(database());
if (db)
  {
     switch (t)
     {
        case SUPPORTS_AUTOINCCOLUMN:

			return (p_backend==mysql
				    ||p_backend==postgresql
				    ||p_backend==sapdb);
			break;
        case SUPPORTS_DATECOLUMN:
			return db->columntypeparameter(hk_column::datecolumn).size()>0;
			break;
        case SUPPORTS_TIMECOLUMN:
			return db->columntypeparameter(hk_column::timecolumn).size()>0;
			break;
        case SUPPORTS_TIMESTAMPCOLUMN:
			return db->columntypeparameter(hk_column::timestampcolumn).size()>0;
			break;
        case SUPPORTS_BOOLCOLUMN:
			return db->columntypeparameter(hk_column::boolcolumn).size()>0;
			break;
	case SUPPORTS_VIEWS:
			return (p_backend!=mysql);
			break;
	default:;
     }

  }
    switch (t)
    {
        case SUPPORTS_REFERENTIALINTEGRITY:
        case SUPPORTS_TRANSACTIONS:
        //case SUPPORTS_BOOLCOLUMN:
        case SUPPORTS_DATETIMECOLUMN:
        case SUPPORTS_TIMESTAMPCOLUMN:
        case SUPPORTS_NEW_DATABASE:
        case SUPPORTS_DELETE_DATABASE:
        case SUPPORTS_CHANGE_PASSWORD:
	case SUPPORTS_BINARYCOLUMN:
	case SUPPORTS_MEMOCOLUMN:
       // case SUPPORTS_NEW_TABLE:
       // case SUPPORTS_ALTER_TABLE:
      //  case SUPPORTS_DELETE_TABLE:
	//case SUPPORTS_VIEWS:
	case SUPPORTS_NEW_VIEW:
	case SUPPORTS_ALTER_VIEW:
	case SUPPORTS_DELETE_VIEW:
	case SUPPORTS_RENAME_TABLE:
	case SUPPORTS_RENAME_DATABASE:
	case SUPPORTS_LOCAL_FILEFORMAT:
        case SUPPORTS_PROPRIETARYCOLUMN:
            return false;

        default : return true;
    }

}


bool hk_odbcconnection::server_needs(need_enum t) const
{
    switch (t)
    {
        case NEEDS_LOGIN:
        case NEEDS_USERNAME:
        case NEEDS_PASSWORD:
	case NEEDS_DATABASENAME:
            return true;

        default: return false;
    }

}


hk_string hk_odbcconnection::drivername(void) const
{

    return "odbc";
}


bool hk_odbcconnection::driver_specific_new_password(const hk_string& newpasswd)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcconnection::driver_specific_new_password");
#endif
    hk_odbcdatabase* db=new hk_odbcdatabase(this);
    hk_actionquery* p_q= db->new_actionquery();
    if (p_q==NULL) return false;
    hk_string pwdsql="SET PASSWORD = PASSWORD('";
    pwdsql+=newpasswd+"')";
    p_q->set_sql(pwdsql.c_str(),pwdsql.size());
    bool result= p_q->execute();
    delete p_q;
    delete db;
    return result;

}


void hk_odbcconnection::servermessage(void)
{
    if (p_odbcenvironment)
    {
//      set_last_servermessage(odbc_error(p_SQL_Connection));
//      cerr<< "Mysql error message "<<odbc_errno(p_SQL_Connection)<<" : "<<last_servermessage()<<endl;
    }
}


unsigned int    hk_odbcconnection::default_tcp_port(void) const
{
    return 0;
}


SQLHDBC hk_odbcconnection::connectionhandle(void)
{

    return p_odbcconnectionhandle;
}


//***********************************************************
hk_connection* create_connection(hk_drivermanager* cl)
{
    return new hk_odbcconnection(cl);

}


hk_string hk_classesversion(void)
{
return (hk_string)HK_VERSION;
}
