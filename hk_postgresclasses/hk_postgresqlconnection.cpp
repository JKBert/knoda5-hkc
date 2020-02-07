// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Edited by Anirban Biswas <utpal@cal2.vsnl.net.in>
// This file is part of the hk_postgreclearsqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_postgresqlconnection.h"
#include <hk_postgresqldatabase.h>
#include <dlfcn.h>
#include "hk_drivermanager.h"
#include "hk_actionquery.h"

hk_postgresqlconnection::hk_postgresqlconnection(hk_drivermanager* c):hk_connection(c)
{
    p_SQL_Connection=NULL;
    set_tcp_port(default_tcp_port());
    set_host("");
    set_user("postgres");
    p_connectionsuccessfullyestablished=false;
    const char* sqltokens[]=
{
"A","ABORT","ABS","ACCESS","ADA","ADMIN","AFTER","AGGREGATE","ALIAS","ALSO","ALWAYS","ANALYSE","ANALYZE",
"ARRAY","ASENSITIVE","ASSIGNMENT","ASYMMETRIC","ATOMIC","ATTRIBUTE","ATTRIBUTES","BACKWARD","BEFORE",
"BERNOULLI","BIGINT","BINARY","BITVAR","BLOB","BOOLEAN","BREADTH","CACHE","CALL","CALLED","CARDINALITY",
"CATALOG_NAME","CEIL","CEILING","CHAIN","CHARACTERISTICS","CHARACTERS","CHARACTER_SET_CATALOG",
"CHARACTER_SET_NAME","CHARACTER_SET_SCHEMA","CHECKED","CHECKPOINT","CLASS","CLASS_ORIGIN","CLOB",
"CLUSTER","COBOL","COLLATION_CATALOG","COLLATION_NAME","COLLATION_SCHEMA","COLLECT","COLUMN_NAME",
"COMMAND_FUNCTION","COMMAND_FUNCTION_CODE","COMMENT","COMMITTED","COMPLETION","CONDITION",
"CONDITION_NUMBER","CONNECTION_NAME","CONSTRAINT_CATALOG","CONSTRAINT_NAME","CONSTRAINT_SCHEMA",
"CONSTRUCTOR","CONTAINS","CONVERSION","COPY","CORR","COVAR_POP","COVAR_SAMP","CREATEDB","CREATEROLE",
"CREATEUSER","CSV","CUBE","CUME_DIST","CURRENT_DEFAULT_TRANSFORM_GROUP","CURRENT_PATH",
"CURRENT_ROLE","CURRENT_TRANSFORM_GROUP_FOR_TYPE","CURSOR_NAME","CYCLE","DATA",
"DATABASE","DATETIME_INTERVAL_CODE","DATETIME_INTERVAL_PRECISION","DEFAULTS","DEFINED","DEFINER",
"DEGREE","DELIMITER","DELIMITERS","DENSE_RANK","DEPTH","DEREF","DERIVED","DESTROY","DESTRUCTOR",
"DETERMINISTIC","DICTIONARY","DISABLE","DISPATCH","DYNAMIC","DYNAMIC_FUNCTION","DYNAMIC_FUNCTION_CODE",
"EACH","ELEMENT","ENABLE","ENCODING","ENCRYPTED","EQUALS","EVERY","EXCLUDE","EXCLUDING","EXCLUSIVE",
"EXISTING","EXP","EXPLAIN","FILTER","FINAL","FLOOR","FOLLOWING","FORCE","FORTRAN","FORWARD","FREE",
"FREEZE","FUNCTION","FUSION","GENERAL","GENERATED","GRANTED","GREATEST","GROUPING","HANDLER",
"HEADER","HIERARCHY","HOLD","HOST","IGNORE","ILIKE","IMMUTABLE","IMPLEMENTATION","IMPLICIT",
"INCLUDING","INCREMENT","INDEX","INFIX","INHERIT","INHERITS","INITIALIZE","INOUT","INSTANCE",
"INSTANTIABLE","INSTEAD","INTERSECTION","INVOKER","ISNULL","ITERATE","KEY_MEMBER","KEY_TYPE",
"LANCOMPILER","LARGE","LATERAL","LEAST","LENGTH","LESS","LIMIT","LISTEN","LOAD","LOCALTIME",
"LOCALTIMESTAMP","LOCATION","LOCATOR","LOCK","LOGIN","MAP","MATCHED","MAXVALUE","MEMBER",
"MERGE","MESSAGE_LENGTH","MESSAGE_OCTET_LENGTH","MESSAGE_TEXT","METHOD","MINVALUE","MOD",
"MODE","MODIFIES","MODIFY","MORE","MOVE","MULTISET","MUMPS","NAME","NCLOB","NESTING",
"NEW","NOCREATEDB","NOCREATEROLE","NOCREATEUSER","NOINHERIT","NOLOGIN","NONE","NORMALIZE",
"NORMALIZED","NOSUPERUSER","NOTHING","NOTIFY","NOTNULL","NOWAIT","NULLABLE","NULLS","NUMBER","OBJECT",
"OCTETS","OFF","OFFSET","OIDS","OLD","OPERATION","OPERATOR","OPTIONS","ORDERING","ORDINALITY","OTHERS",
"OUT","OVER","OVERLAY","OVERRIDING","OWNER","PARAMETER","PARAMETERS","PARAMETER_MODE","PARAMETER_NAME",
"PARAMETER_ORDINAL_POSITION","PARAMETER_SPECIFIC_CATALOG","PARAMETER_SPECIFIC_NAME","PARAMETER_SPECIFIC_SCHEMA",
"PARTITION","PASCAL","PASSWORD","PATH","PERCENTILE_CONT","PERCENTILE_DISC","PERCENT_RANK","PLACING",
"PLI","POSTFIX","POWER","PRECEDING","PREFIX","PREORDER","PREPARED","PROCEDURAL","QUOTE","RANGE","RANK",
"READS","RECHECK","RECURSIVE","REF","REFERENCING","REGR_AVGX","REGR_AVGY","REGR_COUNT","REGR_INTERCEPT",
"REGR_R2","REGR_SLOPE","REGR_SXX","REGR_SXY","REGR_SYY","REINDEX","RELEASE","RENAME","REPEATABLE","REPLACE",
"RESET","RESTART","RESULT","RETURN","RETURNED_CARDINALITY","RETURNED_LENGTH","RETURNED_OCTET_LENGTH",
"RETURNED_SQLSTATE","RETURNS","ROLE","ROLLUP","ROUTINE","ROUTINE_CATALOG","ROUTINE_NAME","ROUTINE_SCHEMA","ROW",
"ROW_COUNT","ROW_NUMBER","RULE","SAVEPOINT","SCALE","SCHEMA_NAME","SCOPE","SCOPE_CATALOG","SCOPE_NAME",
"SCOPE_SCHEMA","SEARCH","SECURITY","SELF","SENSITIVE","SEQUENCE","SERIALIZABLE","SERVER_NAME","SETOF",
"SETS","SHARE","SHOW","SIMILAR","SIMPLE","SOURCE","SPECIFIC","SPECIFICTYPE","SPECIFIC_NAME","SQLEXCEPTION",
"SQLWARNING","SQRT","STABLE","START","STATE","STATEMENT","STATIC","STATISTICS","STDDEV_POP","STDDEV_SAMP",
"STDIN","STDOUT","STORAGE","STRICT","STRUCTURE","STYLE","SUBCLASS_ORIGIN","SUBLIST","SUBMULTISET","SUPERUSER",
"SYMMETRIC","SYSID","SYSTEM","SYSTEM_USER","TABLESAMPLE","TABLESPACE","TABLE_NAME","TEMP","TEMPLATE","TERMINATE",
"THAN","TIES","TOAST","TOP_LEVEL_COUNT","TRANSACTIONS_COMMITTED","TRANSACTIONS_ROLLED_BACK","TRANSACTION_ACTIVE",
"TRANSFORM","TRANSFORMS","TREAT","TRIGGER","TRIGGER_CATALOG","TRIGGER_NAME","TRIGGER_SCHEMA","TRUNCATE","TRUSTED",
"TYPE","UESCAPE","UNBOUNDED","UNCOMMITTED","UNDER","UNENCRYPTED","UNLISTEN","UNNAMED","UNNEST","UNTIL",
"USER_DEFINED_TYPE_CATALOG","USER_DEFINED_TYPE_CODE","USER_DEFINED_TYPE_NAME","USER_DEFINED_TYPE_SCHEMA","VACUUM",
"VALID","VALIDATOR","VARIABLE","VAR_POP","VAR_SAMP","VERBOSE","VOLATILE","WIDTH_BUCKET","WINDOW","WITHIN",
"WITHOUT","ZONE"
};
   for (unsigned int i=0;i<sizeof(sqltokens)/sizeof(const char*);++i)
    {
    p_sqlstatements.push_back(sqltokens[i]);
    }

}


hk_postgresqlconnection::~hk_postgresqlconnection()
{
//driver_specific_disconnect();
    if (p_SQL_Connection) PQfinish(p_SQL_Connection);
    p_SQL_Connection=NULL;
}


bool  hk_postgresqlconnection::driver_specific_connect()
{
//  char* db=0;
//  char* xunix_port=0;
    hk_string conn_info;
    if (!p_connected)
    {
        if (user().size()>0)
	{
	 conn_info="user=";
         conn_info+=user();
	}
	if (password().size()>0)
	{
	 conn_info+=" password='"+password()+"'";

	}
        conn_info+=" dbname=";
        hk_string defaultdb=defaultdatabase().size()>0?defaultdatabase():"template1";
        conn_info+=p_postgresdatabase.size()==0?defaultdb:"'"+p_postgresdatabase+"'";
	if (host().size()>0)
	{
	 conn_info+=" host=";
         conn_info+=host();
         conn_info+=" port=";
         conn_info+=longint2string(tcp_port());
	}
        if (p_SQL_Connection!=NULL) PQfinish(p_SQL_Connection);
        p_SQL_Connection = PQconnectdb(conn_info.c_str());
        if(PQstatus(p_SQL_Connection) == CONNECTION_OK)
        {
            p_connected = true;
            p_connectionsuccessfullyestablished=true;
        }
        else
        {
            cerr << "NOT WORKING"<<endl;
            p_connected = false;
            if (p_connectionsuccessfullyestablished&&p_postgresdatabase.size()>0)
            {
//login was possible, but database name is wrong
//so try to connect again with the default databasename
                p_postgresdatabase="";
                driver_specific_connect();
                return false;
            }
        }

    }

    if (!p_connected)
        servermessage();

    return p_connected;

}


PGconn * hk_postgresqlconnection::dbhandler()
{
    return p_SQL_Connection;
}


void hk_postgresqlconnection::getdbhandler(PGconn *newconnection)
{
    p_SQL_Connection = newconnection;

}


bool hk_postgresqlconnection::driver_specific_disconnect()

{
    if (p_connected)
    {
        PQfinish(p_SQL_Connection);
        p_SQL_Connection=NULL;
        p_connected=false;

    }
    return p_connected;

}


vector<hk_string>* hk_postgresqlconnection::driver_specific_dblist(void)
{

    PGresult  * Res;
    p_databaselist.erase(p_databaselist.begin(),p_databaselist.end());

    if (p_connected)
    {

        Res=PQexec(dbhandler() , "SELECT datname FROM pg_database");
        if (  (PQresultStatus(Res)) == PGRES_TUPLES_OK )
        {
            for (int i=0 ; i < PQntuples(Res); i++)
            {
                p_databaselist.insert(p_databaselist.end(), PQgetvalue(Res , i , 0) );
            }
        }
        PQclear(Res);
    } else cerr <<"not connected"<<endl;
    ;
    return &p_databaselist;
}


hk_database* hk_postgresqlconnection::driver_specific_new_database(void)
{
    hk_postgresqldatabase* db;
    db = new hk_postgresqldatabase(this);
    return db;
}


bool hk_postgresqlconnection::server_supports(support_enum t) const
{
    switch (t)
    {
        case SUPPORTS_TRANSACTIONS:
        case SUPPORTS_DATETIMECOLUMN:
	case SUPPORTS_RENAME_DATABASE:
	case SUPPORTS_LOCAL_FILEFORMAT:
            return false;

        default : return true;
    }

}


bool hk_postgresqlconnection::server_needs(need_enum t) const
{
    switch (t)
    {
        case NEEDS_LOGIN:
        case NEEDS_HOST:
        case NEEDS_USERNAME:
        case NEEDS_PASSWORD:
        case NEEDS_PORT:
	case NEEDS_NULL_TERMINATED_SQL:
	case NEEDS_DATABASENAME:
             return true;

        default: return false;
    }

}


bool hk_postgresqlconnection::driver_specific_new_password(const hk_string& newpasswd)
{
    hk_postgresqldatabase* db=new hk_postgresqldatabase(this);
    hk_actionquery* p_q= db->driver_specific_new_actionquery();
    if (p_q==NULL) return false;
    hk_string pwdsql="ALTER USER ";
    pwdsql+=user();
    pwdsql+=" WITH PASSWORD '";
    pwdsql+=newpasswd;
    pwdsql+="'";
    p_q->set_sql(pwdsql.c_str(),pwdsql.size());
    bool result= p_q->execute();
    delete p_q;
    delete db;
    return result;

}


void hk_postgresqlconnection::servermessage(void)
{
    if (!p_SQL_Connection)return;
    set_last_servermessage(PQerrorMessage(p_SQL_Connection));
    cerr<< "Postgresql error message "<<PQerrorMessage(p_SQL_Connection)<<endl;
}


void    hk_postgresqlconnection::set_postgresdatabase(const hk_string& db)
{
    p_postgresdatabase=db;

}


hk_string hk_postgresqlconnection::drivername(void) const
{

    return "postgres";
}


unsigned int    hk_postgresqlconnection::default_tcp_port(void) const
{
    return 5432;
}


bool    hk_postgresqlconnection::driver_specific_delete_database(const hk_string& dbase)
{

if (database() && database()->name()==dbase)
{
   new_database("template1");
}
return hk_connection::driver_specific_delete_database(dbase);

}



//***********************************************************

hk_connection* create_connection(hk_drivermanager* cl)
{
    return new hk_postgresqlconnection(cl);
}


hk_string hk_classesversion(void)
{
return (hk_string)HK_VERSION;
}
