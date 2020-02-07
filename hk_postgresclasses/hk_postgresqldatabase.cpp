// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Edited by Anirban Biswas <utpal@cal2.vsnl.net.in>
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_postgresqldatabase.h"
#include "hk_postgresqlconnection.h"
#include "hk_postgresqltable.h"
#include "hk_postgresqlresultquery.h"
#include "hk_postgresqlactionquery.h"
#include "hk_postgresqlview.h"


hk_postgresqldatabase::hk_postgresqldatabase(hk_postgresqlconnection* c):hk_database(c)
{
    p_postgresqlconnection=c;
    p_systemdateformat="Y-M-D";
    hk_datasource* d=driver_specific_new_resultquery(NULL);
    if (d)
    {
    d->set_sql("show DateStyle",true);
    d->enable();
    hk_column* col=d->column_by_name("DateStyle");
    if (col)
    {
      hk_string result=string2upper(col->asstring());
      bool DMY=(result.find("DMY")!=hk_string::npos);
      bool MDY=(result.find("MDY")!=hk_string::npos);
      
      if (MDY) p_systemdateformat="M-D-Y";
      else 
      if (DMY) p_systemdateformat="D-M-Y";
      
      if (result.find("SQL")!=hk_string::npos)
          {
            p_systemdateformat=(DMY?"D/M/Y":"M/D/Y");
          }
      else 
      if (result.find("GERMAN")!=hk_string::npos)
      {
        p_systemdateformat="D.M.Y";
      }
    }
    delete d;
    }
}


hk_postgresqlconnection* hk_postgresqldatabase::connection(void)
{
    return p_postgresqlconnection;
}


void hk_postgresqldatabase::driver_specific_tablelist(void)
{

    p_tablelist.erase(p_tablelist.begin(),p_tablelist.end());
    if (p_postgresqlconnection!=NULL)
    {

        if ( p_postgresqlconnection->connect())
        {
//pg_table where postgresql stores name of all the relations of a DB.
            PGresult* Res = PQexec(p_postgresqlconnection->dbhandler() , "select relname as name from pg_class,pg_namespace where relkind='r' and nspname!~'information_schema' and nspname!~'pg_catalog' and pg_class.relnamespace = pg_namespace.oid ORDER BY relname" );
            if (PQresultStatus(Res) == PGRES_TUPLES_OK)
            {
                for (long i=0; i<PQntuples(Res);i++)
                {
                    p_tablelist.insert(p_tablelist.end(), PQgetvalue(Res ,i ,  0));
                }
            }
            PQclear(Res);
        }
    }
    //sort(p_tablelist.begin(),p_tablelist.end());

}


void hk_postgresqldatabase::driver_specific_viewlist(void)
{
    p_viewlist.erase(p_viewlist.begin(),p_viewlist.end());
    if (p_postgresqlconnection!=NULL)
    {

        if ( p_postgresqlconnection->connect())
        {
//pg_table where postgresql stores name of all the relations of a DB.
            PGresult* Res = PQexec(p_postgresqlconnection->dbhandler() , "select relname as name from pg_class,pg_namespace where relkind='v' and nspname!~'information_schema' and nspname!~'pg_catalog' and pg_class.relnamespace = pg_namespace.oid ORDER BY relname" );
            if (PQresultStatus(Res) == PGRES_TUPLES_OK)
            {
                for (long i=0; i<PQntuples(Res);i++)
                {
                    p_viewlist.insert(p_viewlist.end(), PQgetvalue(Res ,i ,  0));
                }
            }
            PQclear(Res);
        }
    }
    //sort(p_viewlist.begin(),p_viewlist.end());

}


hk_datasource* hk_postgresqldatabase::driver_specific_new_table(hk_presentation* p)
{
    return new hk_postgresqltable(this,p);
}


hk_datasource* hk_postgresqldatabase::driver_specific_new_resultquery(hk_presentation* p)
{
    hk_postgresqlresultquery *x;
    x = new hk_postgresqlresultquery(this , p);
    return x ;

}


hk_datasource* hk_postgresqldatabase::driver_specific_new_view(hk_presentation* p)
{
    hk_postgresqlview *x;
    x = new hk_postgresqlview(this , p);
    return x ;

}


hk_actionquery * hk_postgresqldatabase::driver_specific_new_actionquery(void)
{
    hk_postgresqlactionquery *a;
    a = new hk_postgresqlactionquery(this);
    return a;
}


bool hk_postgresqldatabase::driver_specific_select_db(void)
{

//    PGconn *newconn;
    if (p_postgresqlconnection!=NULL)
    {
        p_postgresqlconnection->set_postgresdatabase(name());
        p_postgresqlconnection->driver_specific_disconnect();
        return p_postgresqlconnection->driver_specific_connect();
    }
    return false;

}


bool    hk_postgresqldatabase::driver_specific_rename_table(const hk_string& oldname,const hk_string& newname)
{
   hk_postgresqlactionquery* query=new hk_postgresqlactionquery(this);
  hk_string sql="ALTER TABLE "+query->identifierdelimiter()+oldname+query->identifierdelimiter()+" RENAME TO "+query->identifierdelimiter()+newname+query->identifierdelimiter();
   query->set_sql(sql.c_str(),sql.size());
   bool res=query->execute();
   delete query;
   return res;

}




hk_string hk_postgresqldatabase::systemdateformat(void)
{
return p_systemdateformat;
}
