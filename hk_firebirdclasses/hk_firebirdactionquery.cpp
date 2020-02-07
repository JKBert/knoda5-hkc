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
#include "hk_firebirdactionquery.h"
#include "hk_firebirddatabase.h"
#include "hk_firebirdconnection.h"

hk_firebirdactionquery::hk_firebirdactionquery(hk_firebirddatabase* db) :hk_actionquery(db)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_firebirdactionquery::hk_firebirdactionquery");
#endif
    p_firebirddatabase=db;
   p_identifierdelimiter="\"";
    p_sqlhandle=NULL;
    p_transaction=NULL;
    p_stmt=NULL;
}


bool hk_firebirdactionquery::driver_specific_execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdactionquery::driver_specific_execute");
#endif
   if (!p_firebirddatabase->firebirdconnection()->handle())
   {
     cerr <<"no firebirdconnectionhandle"<<endl;
     return false;
   }
   if (is_create_database_query())
   {
     return execute_create_database_query();
   }
   if (p_sqlhandle) free(p_sqlhandle);
   const int fnr=20;
   p_sqlhandle=(XSQLDA*)malloc(XSQLDA_LENGTH(fnr));
   p_sqlhandle->sqln=fnr;
   p_sqlhandle->version=1;
   isc_db_handle h=p_firebirddatabase->firebirdconnection()->handle();
   if (isc_dsql_allocate_statement(p_status,&h,&p_stmt))
   {
   set_error();
   free_handles();
   return false;
   }


   if (!p_transaction)
   {
     if (isc_start_transaction(p_status,&p_transaction,1,&h,0,NULL))
     {
   	set_error();
   	free_handles();
   	return false;
     }
   }

   if (isc_dsql_prepare(p_status,&p_transaction,&p_stmt,0,const_cast<char*>(p_sql),SQL_DIALECT_V6,p_sqlhandle))
   {
   	set_error();
   	free_handles();
   	return false;
   }

  bool result=true;
  if (isc_dsql_execute(p_status,&p_transaction,&p_stmt,SQL_DIALECT_V6,NULL))
  {
#ifdef HK_DEBUG
    hkdebug("hk_firebirdactionquery::driver_specific_execute NOT successful");
#endif
  set_error();
  result=false;
  }
  else
  {
    if (p_transaction)
    {
      if (isc_commit_transaction(p_status,&p_transaction))
        {
#ifdef HK_DEBUG
    hkdebug("hk_firebirdactionquery::driver_specific_execute commit transaction NOT SUCCESSFUL");
#endif
          set_error();
          result=false;
        }
    }

  }

#ifdef HK_DEBUG
    hkdebug("hk_firebirdactionquery::driver_specific_execute successful=",result);
#endif


  free_handles();
return result;
}


bool hk_firebirdactionquery::driver_specific_sql(const char* )
{
    return true;
}

void hk_firebirdactionquery::free_handles(void)
{
   if (p_stmt)
   if( isc_dsql_free_statement(p_status,&p_stmt,DSQL_close))
   {
   	//set_error();
   }
   if (p_sqlhandle) free(p_sqlhandle);
   p_sqlhandle=NULL;
   p_transaction=NULL;
   p_stmt=NULL;

}


void hk_firebirdactionquery::set_error(void)
{
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
  p_firebirddatabase->firebirdconnection()->servermessage(errors);
}

bool hk_firebirdactionquery::is_create_database_query()
{
   if (!p_sql)
   {
    return false;
   }
    hk_string::size_type offset=0;

    enum
    {
	S_START,S_IN_TAG
    } state=S_START;

int row=1;
int col=1;
hk_string tag;
int tagnr=0;
while (offset<p_length)
{
	hk_string x(1,p_sql[offset]);
	switch (state)
	{
	case S_START:
			tag="";
			if (isspace(x[0]))
				{
					if (x=="\n")
					{
						++row;col=0;
					}
				  break;
				}
			else
			{
			tag=x;
       		        state=S_IN_TAG;
			}
			break;
	case S_IN_TAG:
			if (isspace(x[0]))
			 {
			    state=S_START;
			    hk_string uptag=string2upper(tag);
			    if (tagnr==1 && uptag=="DATABASE") return true;
			    if (tagnr==0 && uptag=="CREATE")
			    {
			      tagnr=1;
			    }
			    else
			    {
			     return false;
			    }
			 }
			tag+=x;



	}
	++offset;++col;
}
  return false;
}

bool hk_firebirdactionquery::execute_create_database_query()
{
  isc_db_handle db_handle=NULL;
  isc_tr_handle tr_dummy_handle=NULL;

  if (isc_dsql_execute_immediate(p_status,&db_handle,&tr_dummy_handle,p_length,const_cast<char*>(p_sql),SQL_DIALECT_V6,NULL))
  {
  set_error();
  return false;
  }
 isc_detach_database(p_status, &db_handle);

  return true;
}

