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
#include <sqlite3.h>
#include "hk_sqlite3database.h"
#include "hk_sqlite3connection.h"
#include "hk_sqlite3table.h"
#include "hk_sqlite3resultquery.h"
#include "hk_sqlite3actionquery.h"
#include "hk_sqlite3view.h"

hk_sqlite3database::hk_sqlite3database(hk_sqlite3connection* c):hk_database(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3database::hk_sqlite3database");
#endif
   p_sqliteconnection=c;
   p_sqlitehandler=NULL;
}


hk_sqlite3database::~hk_sqlite3database(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3database::~hk_sqlite3database");
#endif
      if (p_sqlitehandler)
      {
        sqlite3_close(p_sqlitehandler);
	p_sqlitehandler=0;
      }

}


hk_sqlite3connection* hk_sqlite3database::connection(void)
{
    return p_sqliteconnection;
}


void hk_sqlite3database::driver_specific_tablelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3database::driver_specific_tablelist");
#endif
    p_tablelist.erase(p_tablelist.begin(),p_tablelist.end());

hk_datasource* ds=driver_specific_new_resultquery(NULL);
     ds->set_sql("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name");
     ds->enable();
     hk_column* col=ds->column_by_name("name");
     if (!col)
       {
         //show_warningmessage("Bug: hk_sqlite3database::driver_specific_tablelist could not find systemcolumn");
         delete ds;
	 return;
       }


    unsigned int z=0;
    while (  (z <ds->max_rows() ) )
    {
    p_tablelist.insert(p_tablelist.end(),col->asstring());
    ds->goto_next();
    ++z;
    }
   delete ds;

}


void hk_sqlite3database::driver_specific_viewlist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3database::driver_specific_viewlist");
#endif
    p_viewlist.erase(p_viewlist.begin(),p_viewlist.end());

hk_datasource* ds=driver_specific_new_resultquery(NULL);
     ds->set_sql("SELECT name FROM sqlite_master WHERE type='view' ORDER BY name");
     ds->enable();
     hk_column* col=ds->column_by_name("name");
     if (!col)
       {
         //show_warningmessage("Bug: hk_sqlite3database::driver_specific_viewlist could not find systemcolumn");
         delete ds;
	 return;
       }


    unsigned int z=0;
    while (  (z <ds->max_rows() ) )
    {
    p_viewlist.insert(p_viewlist.end(),col->asstring());
    ds->goto_next();
    ++z;
    }
   delete ds;

}


hk_datasource* hk_sqlite3database::driver_specific_new_table(hk_presentation* p)
{
    return new hk_sqlite3table(this,p);
}


hk_datasource* hk_sqlite3database::driver_specific_new_resultquery(hk_presentation* p)
{
    return new hk_sqlite3resultquery(this,p);
}


hk_actionquery* hk_sqlite3database::driver_specific_new_actionquery(void)
{
    return new hk_sqlite3actionquery(this);
}


bool hk_sqlite3database::driver_specific_select_db(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3database::driver_specific_select_db");
#endif

   vector<hk_string>* l=p_connection->dblist();
   bool result= find(l->begin(),l->end(),name())!=l->end();
   if (result||p_url.directory().size()>0)
   {
      hk_string n= p_url.directory().size()>0? p_url.url(): p_sqliteconnection->databasepath()+"/"+name()+".hk_sqlite3";
      if (p_sqlitehandler)
      {
        sqlite3_close(p_sqlitehandler);
	p_sqlitehandler=0;
      }
      if (sqlite3_open(n.c_str(),&p_sqlitehandler))
       {
         p_sqliteconnection->servermessage(sqlite3_errmsg(p_sqlitehandler));
	 show_warningmessage(hk_translate("Driver error!\n")+hk_translate("Servermessage: ")+connection()->last_servermessage());

	 return false;
       }
   }
 return true;
}



bool    hk_sqlite3database::driver_specific_rename_table(const hk_string& oldname,const hk_string& newname)
{
  hk_actionquery* a=driver_specific_new_actionquery();
  if (! a) return false;
  hk_string s="ALTER TABLE \""+oldname+"\" RENAME TO \""+newname+"\"";
  a->set_sql(s.c_str(),s.size());
  bool result=a->execute();
  if (!result)
   {
    p_sqliteconnection->servermessage(sqlite3_errmsg(p_sqlitehandler));
   }
  delete a;
return result;
}

hk_datasource* hk_sqlite3database::driver_specific_new_view(hk_presentation* p)
{
    hk_sqlite3view *x;
    x = new hk_sqlite3view(this , p);
    return x ;

}




