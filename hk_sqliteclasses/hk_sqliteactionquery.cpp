// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_sqliteclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_sqliteactionquery.h"
#include "hk_sqlitedatabase.h"
#include "hk_sqliteconnection.h"
#include <sqlite.h>

hk_sqliteactionquery::hk_sqliteactionquery(hk_sqlitedatabase* db) :hk_actionquery(db)
{
    sqlitedb=db;
   // p_identifierdelimiter="`";
}


bool hk_sqliteactionquery::driver_specific_execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqliteactionquery::driver_specific_execute");
#endif

        if  (!sqlitedb||!sqlitedb->dbhandler()) 
	 {
	  cerr <<"error p_sqlitedatabase==NULL||p_sqlitedatabase->dbhandler()"<<endl;
	  cerr <<"db="<<sqlitedb<<" handler="<<sqlitedb->dbhandler()<<endl;
	  return false;
	 } 
        sqlite_vm* vm=0;
	char* errormsg=0;
	int result= sqlite_compile(sqlitedb->dbhandler() ,p_sql,0,&vm,&errormsg );
	//cerr <<"SQL="<<p_sql<<endl;
	if (result!=SQLITE_OK)
	{
          sqlitedb->sqliteconnection()->servermessage(errormsg);
	  cerr <<errormsg<<" ";
	  sqlite_freemem(errormsg);
	  errormsg=0;
	  cerr<< "compile problem"<<endl;
	  return false;
	}
            
	//cerr <<"sqlite_compile result="<<result<<endl;
	int colnums=0;
	const char** data=0;
	const char** colnames=0;
	if (vm)
	{
	result=sqlite_step(vm,&colnums,&data,&colnames);
	//cerr <<"sqlite_step result="<<result<<endl;
	}
	sqlite_finalize(vm,&errormsg);
	vm=0;
	if (result==SQLITE_ERROR) 
        {
	  sqlitedb->sqliteconnection()->servermessage(errormsg);
	  sqlite_freemem(errormsg);
	  errormsg=0;
         return false;
	}  
	
	    
   
   return true;	    

/*    if (!sqlitedb->connection()->dbhandler()) return false;


    int i=sqlite_real_query(sqlitedb->connection()->dbhandler(),p_sql,p_length);


#ifdef HK_DEBUG
    hkdebug("hk_sqliteactionquery::driver_specific_execute after query");
#endif
    if (i==0)  return true;
    else
    {
        hk_string error;
        switch (i)
        {
            case CR_COMMANDS_OUT_OF_SYNC : error="CR_COMMANDS_OUT_OF_SYNC";break;
            case CR_SERVER_GONE_ERROR : error="CR_SERVER_GONE_ERROR";break;
            case CR_SERVER_LOST : error="CR_SERVER_LOST";break;
            case CR_UNKNOWN_ERROR : error="CR_UNKNOWN_ERROR";break;
            default : error="misc. Mysql error message!";
        }
        cerr <<"MYSQL Error: "<<error<<endl;
        sqlitedb->connection()->servermessage();
        return false;
    }
*/
return false;
}


bool hk_sqliteactionquery::driver_specific_sql(const char* )
{
    return true;
}
