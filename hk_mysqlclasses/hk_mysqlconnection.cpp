// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mysqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_mysqlconnection.h"
#include "hk_mysqldatabase.h"
#include <dlfcn.h>
#include "hk_drivermanager.h"
#include "hk_actionquery.h"
#include <mysqld_error.h>


int hk_mysqlconnection::p_reference=0;


hk_mysqlconnection::hk_mysqlconnection(hk_drivermanager* c):hk_connection(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlconnection::hk_mysqlconnection");
#endif
    p_SQL_Connection=NULL;
    set_tcp_port(default_tcp_port());
#if  MYSQL_VERSION_ID >=40110
    if (p_reference==0)
     {
//      mysql_library_init(0,0,0);
     }
#endif
    ++p_reference;
    
    
const char* sqltokens[]=
{
"ACCESSIBLE","ANALYZE","ASENSITIVE","BEFORE","BIGINT","BINARY","BLOB",
"CALL","CHANGE","CONDITION","CONTRIBUTORS","DATABASE","DATABASES",
"DAY_HOUR","DAY_MICROSECOND","DAY_MINUTE","DAY_SECOND","DELAYED",
"DETERMINISTIC","DISTINCTROW","DIV","DUAL","EACH","ELSEIF","ENCLOSED",
"ESCAPED","EXIT","EXPLAIN","FLOAT4","FLOAT8","FORCE","FULLTEXT",
"HIGH_PRIORITY","HOUR_MICROSECOND","HOUR_MINUTE","HOUR_SECOND",
"IGNORE","INDEX","INFILE","INOUT","INT1","INT2","INT3","INT4","INT8",
"ITERATE","KEYS","KILL","LEAVE","LIMIT","LINEAR","LINES","LOAD","LOCALTIME",
"LOCALTIMESTAMP","LOCK","LONG","LONGBLOB","LONGTEXT","LOOP","LOW_PRIORITY",
"MEDIUMBLOB","MEDIUMINT","MEDIUMTEXT","MIDDLEINT","MINUTE_MICROSECOND",
"MINUTE_SECOND","MOD","MODIFIES","NO_WRITE_TO_BINLOG","OPTIMIZE",
"OPTIONALLY","OUT","OUTFILE","PURGE","RANGE","READS","READ_ONLY",
"READ_WRITE","REGEXP","RELEASE","RENAME","REPEAT","REPLACE","REQUIRE",
"RETURN","RLIKE","SCHEMAS","SECOND_MICROSECOND",
"SENSITIVE","SEPARATOR","SHOW","SPATIAL","SPECIFIC","SQLEXCEPTION","SQLWARNING",
"SQL_BIG_RESULT","SQL_CALC_FOUND_ROWS","SQL_SMALL_RESULT","SSL",
"STARTING","STRAIGHT_JOIN","TERMINATED","TINYBLOB","TINYINT",
"TINYTEXT","TRIGGER","UNDO","UNLOCK","UNSIGNED","UPGRADE",
"USE","UTC_DATE","UTC_TIME","UTC_TIMESTAMP","VARBINARY","VARCHARACTER",
"WHILE","X509","XOR","YEAR_MONTH","ZEROFILLL"
};
   for (unsigned int i=0;i<sizeof(sqltokens)/sizeof(const char*);++i)
    {
    p_sqlstatements.push_back(sqltokens[i]);
    }


}



hk_mysqlconnection::~hk_mysqlconnection()
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlconnection::~hk_mysqlconnection");
#endif
//       driver_specific_disconnect();
    if (p_SQL_Connection) mysql_close(p_SQL_Connection);
    p_SQL_Connection=NULL;
  --p_reference;
 #if  MYSQL_VERSION_ID >=40110
 if (p_reference==0)
    {
//      mysql_library_end();
    }
#endif

}


bool  hk_mysqlconnection::driver_specific_connect()
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlconnection::driver_specific_connect");
#endif

    char* db=0;
    if (!p_connected)
    {
        p_SQL_Connection=mysql_init(p_SQL_Connection);
        mysql_options(p_SQL_Connection,MYSQL_READ_DEFAULT_GROUP,"hk_classes");
        p_connected=mysql_real_connect(p_SQL_Connection,
         host().size()>0?host().c_str():NULL
        ,user().c_str()
        ,password().c_str()
        ,db,tcp_port()
        ,NULL
        ,0);
        if (!p_connected)
        {servermessage();
            mysql_close(p_SQL_Connection);
            p_SQL_Connection=NULL;
        }
    }
    if (!p_connected) servermessage();
    return p_connected;

}


MYSQL* hk_mysqlconnection::dbhandler()
{
    return p_SQL_Connection;
}


bool hk_mysqlconnection::driver_specific_disconnect()

{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlconnection::driver_specific_disconnect");
#endif
    if (p_connected)
    {
        mysql_close(p_SQL_Connection);
        p_connected=false;
        p_SQL_Connection=NULL;

    }
    return p_connected;

}


vector<hk_string>* hk_mysqlconnection::driver_specific_dblist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlconnection::driver_specific_dblist");
#endif
    MYSQL_RES* Res;
    MYSQL_ROW row;
    char *wild=NULL;
    unsigned int i;
    p_databaselist.erase(p_databaselist.begin(),p_databaselist.end());

    if (p_connected)
    {
        Res=mysql_list_dbs(p_SQL_Connection,wild);
        if (Res==NULL)return &p_databaselist;
        while ((row=mysql_fetch_row(Res)))
        {
            for (i=0 ; i < mysql_num_fields(Res);i++)
            {
                p_databaselist.insert(p_databaselist.end(),(row[i]));
            }
        }
        mysql_free_result(Res);
    }
    ;
    return &p_databaselist;
}


hk_database* hk_mysqlconnection::driver_specific_new_database(void)
{
    hk_mysqldatabase* db;
    db = new hk_mysqldatabase(this);
    return db;
}



bool hk_mysqlconnection::server_supports(support_enum t) const
{
 bool supports_views=false;
 bool supports_union=false;
 if ( database() )
 {
   switch (t)
   {
   case SUPPORTS_VIEWS:
   case SUPPORTS_NEW_VIEW:
   case SUPPORTS_ALTER_VIEW:
   case SUPPORTS_SQL_UNION:
   {
   hk_datasource* r=database()->new_resultquery();
   if (r)
   {
   hk_string s="select version() as v";
   r->set_sql(s);
   if (r->enable())
   {
     hk_column* c=r->column_by_name("v");
     if (c)
      {
        hk_string version=c->asstring();
        supports_views=(version>"5.0.0");
        supports_union=(version>"4.0.0");
      }
   }

   delete r;
   }
   }
   default:;
   }
 }


    switch (t)
    {
	case SUPPORTS_VIEWS:
	case SUPPORTS_NEW_VIEW:
	case SUPPORTS_ALTER_VIEW:
	case SUPPORTS_DELETE_VIEW: return supports_views;
	case SUPPORTS_SQL_UNION: return supports_union;
        case SUPPORTS_REFERENTIALINTEGRITY:
        case SUPPORTS_TRANSACTIONS:
	case SUPPORTS_RENAME_DATABASE:
	case SUPPORTS_LOCAL_FILEFORMAT:
	case NEEDS_DIRECTORY_AS_DATABASE:
//	case SUPPORTS_PROPRIETARYCOLUMN: // commented out because of the 'Set' type
            return false;
        case SUPPORTS_BOOLCOLUMN : return booleanemulation();
        default : return true;
    }

}


hk_string hk_mysqlconnection::drivername(void) const
{

    return "mysql";
}



bool hk_mysqlconnection::server_needs(need_enum t) const
{
    switch (t)
    {
        case NEEDS_LOGIN:
        case NEEDS_HOST:
        case NEEDS_USERNAME:
        case NEEDS_PASSWORD:
        case NEEDS_PORT:
	case NEEDS_BOOLEANEMULATION:
	case NEEDS_DATABASENAME:

            return true;

        default: return false;
    }

}


bool hk_mysqlconnection::driver_specific_new_password(const hk_string& newpasswd)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlconnection::driver_specific_new_password");
#endif
    hk_mysqldatabase* db=new hk_mysqldatabase(this);
    hk_actionquery* p_q= db->driver_specific_new_actionquery();
    if (p_q==NULL)
       {
	return false;
       }
    hk_string pwdsql="SET PASSWORD = PASSWORD('";
    pwdsql+=newpasswd+"')";
    p_q->set_sql(pwdsql.c_str(),pwdsql.size());
    bool result= p_q->execute();
    delete p_q;
    delete db;
    return result;

}


void hk_mysqlconnection::servermessage(void)
{
    if (p_SQL_Connection)
    {
        set_last_servermessage(mysql_error(p_SQL_Connection));
        cerr<< "Mysql error message "<<mysql_errno(p_SQL_Connection)<<" : "<<last_servermessage()<<endl;
    }
}


unsigned int    hk_mysqlconnection::default_tcp_port(void) const
{
    return 3306;
}


//***********************************************************

hk_connection* create_connection(hk_drivermanager* cl)
{
    return new hk_mysqlconnection(cl);
}


hk_string hk_classesversion(void)
{
return (hk_string)HK_VERSION;
}
