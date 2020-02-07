// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_xbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_xbaseactionquery.h"
#include "hk_xbasedatabase.h"
#include "hk_xbaseconnection.h"
#include <xbsql.h>

hk_xbaseactionquery::hk_xbaseactionquery(hk_xbasedatabase* db) :hk_actionquery(db)
{
    p_xbasedatabase=db;
   // p_identifierdelimiter="`";
}


bool hk_xbaseactionquery::driver_specific_execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbaseactionquery::driver_specific_execute");
#endif
        if  (!p_xbasedatabase||!p_xbasedatabase->sqlhandle()) 
	 {
	  cerr <<"error p_xbasedatabase==NULL||p_xbasedatabase->sqlhandle()"<<endl;
	  cerr <<"db="<<p_xbasedatabase<<" handler="<<p_xbasedatabase->sqlhandle()<<endl;
	  return false;
	 } 
	 
	 
bool	   parsed ;
XBSQLQuery *q= p_xbasedatabase->sqlhandle()->openQuery (p_sql, parsed) ;

bool result=false;
hk_string error;
if (!q)
   {
     if (!p_xbasedatabase->sqlhandle()->execCommand(p_sql))
     {
     p_xbasedatabase->connection()->servermessage(p_xbasedatabase->sqlhandle()->lastError());
      return false;
     }
     else return true;
   }
if (q->isUpdate())
    result=q->isUpdate()->execute(0,0);
else
if (q->isInsert())
    result=q->isInsert()->execute(0,0);
else    
if (q->isDelete())
    result=q->isDelete()->execute(0,0);
 
if (!result)           
    p_xbasedatabase->connection()->servermessage(p_xbasedatabase->sqlhandle()->lastError());

   delete q;
   return result;	    

}


bool hk_xbaseactionquery::driver_specific_sql(const char* )
{
    return true;
}
