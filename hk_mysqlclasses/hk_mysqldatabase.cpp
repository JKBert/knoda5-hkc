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
#include <mysql.h>
#include "hk_mysqldatabase.h"
#include "hk_mysqlconnection.h"
#include "hk_mysqltable.h"
#include "hk_mysqlresultquery.h"
#include "hk_mysqlactionquery.h"
#include "hk_mysqlview.h"
hk_mysqldatabase::hk_mysqldatabase(hk_mysqlconnection* c):hk_database(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqldatabase::hk_mysqldatabase");
#endif
    p_mysqlconnection=c;
}


hk_mysqldatabase::~hk_mysqldatabase(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqldatabase::~hk_mysqldatabase");
#endif

}


hk_mysqlconnection* hk_mysqldatabase::connection(void)
{
    return p_mysqlconnection;
}


void hk_mysqldatabase::driver_specific_tablelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqldatabase::driver_specific_tablelist");
#endif
    MYSQL_RES* Res;
    MYSQL_ROW row;
    char *wild=NULL;
    p_tablelist.erase(p_tablelist.begin(),p_tablelist.end());
    if (p_mysqlconnection!=NULL)
    {
        if (p_mysqlconnection->connect())
        {
            if (p_mysqlconnection->server_supports(hk_connection::SUPPORTS_VIEWS))
            {

        	hk_string sql="select TABLE_NAME  from information_schema.tables WHERE TABLE_SCHEMA='"+name()+"' and TABLE_TYPE='BASE TABLE'";
        	hk_datasource* ds=new_resultquery();
        	if (ds)
        	{
        	  ds->set_sql(sql);
        	  ds->enable();
        	  hk_column* col=ds->column_by_name("TABLE_NAME");
        	  if (col)
        	  {
        	       unsigned long m=ds->max_rows();
        	       for (unsigned long i=0;i<m;i++)
    			{
        		 p_tablelist.insert(p_tablelist.end(),trim(col->asstring()));
        		 ds->goto_next();
        		}

          	  }
                  else show_warningmessage("Error hk_mysqldatabase::driver_specific_tablelist, Systemcolumn could not be loaded");
        	delete ds;

                }
            }
            else
            {

            Res=mysql_list_tables(p_mysqlconnection->dbhandler(),wild);
            if (Res==NULL) return ;
            while ((row=mysql_fetch_row(Res)))
            {
                for (unsigned int o=0; o<mysql_num_fields(Res);o++)
                {
                    p_tablelist.insert(p_tablelist.end(),row[o]);
                }
            }
            mysql_free_result(Res);
            }
        }
    }
    sort(p_tablelist.begin(),p_tablelist.end());

}


hk_datasource* hk_mysqldatabase::driver_specific_new_table(hk_presentation* p)
{
    return new hk_mysqltable(this,p);
}


hk_datasource* hk_mysqldatabase::driver_specific_new_resultquery(hk_presentation* p)
{
    return new hk_mysqlresultquery(this,p);
}


hk_actionquery* hk_mysqldatabase::driver_specific_new_actionquery(void)
{
    return new hk_mysqlactionquery(this);
}


bool hk_mysqldatabase::driver_specific_select_db(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqldatabase::driver_specific_select_db");
#endif
    if (p_mysqlconnection!=NULL)
    {
        if (mysql_select_db(p_mysqlconnection->dbhandler(),name().c_str())==0)return true;
    }
    return false;
}



bool    hk_mysqldatabase::driver_specific_rename_table(const hk_string& oldname,const hk_string& newname)
{
   hk_mysqlactionquery* query=new hk_mysqlactionquery(this);
  hk_string sql="ALTER TABLE "+query->identifierdelimiter()+oldname+query->identifierdelimiter()+" RENAME TO "+query->identifierdelimiter()+newname+query->identifierdelimiter();
   query->set_sql(sql.c_str(),sql.size());
   bool res=query->execute();
   delete query;
   return res;

}


void hk_mysqldatabase::driver_specific_viewlist(void)
{
    p_viewlist.erase(p_viewlist.begin(),p_viewlist.end());
    if (p_mysqlconnection!=NULL)
    {

        hk_string sql="select TABLE_NAME  from information_schema.views WHERE TABLE_SCHEMA='"+name()+"'";
        hk_datasource* ds=new_resultquery();
        if (ds)
        {
          ds->set_sql(sql);
          ds->enable();
          hk_column* col=ds->column_by_name("TABLE_NAME");
          if (col)
          {
               unsigned long m=ds->max_rows();
               for (unsigned long i=0;i<m;i++)
    		{
        		p_viewlist.insert(p_viewlist.end(),trim(col->asstring()));
        		ds->goto_next();
        	}

          }
          else show_warningmessage("Error hk_mysqldatabase::driver_specific_viewlist, Systemcolumn could not be loaded");
        delete ds;
        }

    //sort(p_viewlist.begin(),p_viewlist.end());
    }

}

hk_datasource* hk_mysqldatabase::driver_specific_new_view(hk_presentation* p)
{
    hk_mysqlview *x;
    x = new hk_mysqlview(this , p);
    return x ;

}


