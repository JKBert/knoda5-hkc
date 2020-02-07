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
#include "hk_sqlite3actionquery.h"
#include "hk_sqlite3database.h"
#include "hk_sqlite3connection.h"
#include <sqlite3.h>

hk_sqlite3actionquery::hk_sqlite3actionquery(hk_sqlite3database* db) :hk_actionquery(db)
{
    sqlitedb=db;
   // p_identifierdelimiter="`";
}


bool hk_sqlite3actionquery::driver_specific_execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3actionquery::driver_specific_execute");
#endif

        if  (!sqlitedb||!sqlitedb->dbhandler()) 
	 {
	  cerr <<"error p_sqlitedatabase==NULL||p_sqlitedatabase->dbhandler()"<<endl;
	  cerr <<"db="<<sqlitedb<<" handler="<<sqlitedb->dbhandler()<<endl;
	  return false;
	 } 
        sqlite3_stmt* vm=0;
	if( sqlite3_prepare(sqlitedb->dbhandler() ,p_sql,p_length,&vm,0 ))
	{
          sqlitedb->sqliteconnection()->servermessage(sqlite3_errmsg(sqlitedb->dbhandler()));
	  cerr <<sqlite3_errmsg(sqlitedb->dbhandler())<<" ";
	  cerr<< "compile problem"<<endl;
	  return false;
	}
            
	//cerr <<"sqlite_compile result="<<result<<endl;
	int result=0;
	if (vm)
	{
	
	result=sqlite3_step(vm);
	//cerr <<"sqlite_step result="<<result<<endl;
	}
	sqlite3_finalize(vm);
	vm=0;
	if (result==SQLITE_ERROR) 
        {
	  sqlitedb->sqliteconnection()->servermessage(sqlite3_errmsg(sqlitedb->dbhandler()));
         return false;
	}  
	
	    
   
   return true;	    

}


bool hk_sqlite3actionquery::driver_specific_sql(const char* )
{
    return true;
}
