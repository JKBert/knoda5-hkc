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
#include <sqlite.h>
#include "hk_sqlitedatabase.h"
#include "hk_sqliteconnection.h"
#include "hk_sqlitetable.h"
#include "hk_sqliteresultquery.h"
#include "hk_sqliteactionquery.h"
#include "hk_sqliteview.h"

hk_sqlitedatabase::hk_sqlitedatabase(hk_sqliteconnection* c):hk_database(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitedatabase::hk_sqlitedatabase");
#endif
   p_sqliteconnection=c;
   p_sqlitehandler=NULL;
}


hk_sqlitedatabase::~hk_sqlitedatabase(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitedatabase::~hk_sqlitedatabase");
#endif
      if (p_sqlitehandler)
      {
        sqlite_close(p_sqlitehandler);
	p_sqlitehandler=0;
      }

}


hk_sqliteconnection* hk_sqlitedatabase::connection(void)
{
    return p_sqliteconnection;
}


void hk_sqlitedatabase::driver_specific_tablelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitedatabase::driver_specific_tablelist");
#endif
    p_tablelist.erase(p_tablelist.begin(),p_tablelist.end());

hk_datasource* ds=driver_specific_new_resultquery(NULL);
     ds->set_sql("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name");
     ds->enable();
     hk_column* col=ds->column_by_name("name");
     if (!col)
       {
        // show_warningmessage("Bug: hk_sqlitedatabase::driver_specific_tablelist could not find systemcolumn");
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


void hk_sqlitedatabase::driver_specific_viewlist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitedatabase::driver_specific_viewlist");
#endif
    p_viewlist.erase(p_viewlist.begin(),p_viewlist.end());

hk_datasource* ds=driver_specific_new_resultquery(NULL);
     ds->set_sql("SELECT name FROM sqlite_master WHERE type='view' ORDER BY name");
     ds->enable();
     hk_column* col=ds->column_by_name("name");
     if (!col)
       {
         //show_warningmessage("Bug: hk_sqlitedatabase::driver_specific_viewlist could not find systemcolumn");
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


hk_datasource* hk_sqlitedatabase::driver_specific_new_table(hk_presentation* p)
{
    return new hk_sqlitetable(this,p);
}


hk_datasource* hk_sqlitedatabase::driver_specific_new_resultquery(hk_presentation* p)
{
    return new hk_sqliteresultquery(this,p);
}


hk_actionquery* hk_sqlitedatabase::driver_specific_new_actionquery(void)
{
    return new hk_sqliteactionquery(this);
}


bool hk_sqlitedatabase::driver_specific_select_db(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitedatabase::driver_specific_select_db");
#endif
   
   vector<hk_string>* l=p_connection->dblist();
   bool result= find(l->begin(),l->end(),name())!=l->end(); 
   if (result||p_url.directory().size()>0) 
   {
      char* errormsg=0;
      hk_string n=p_url.directory().size()>0?p_url.url():p_sqliteconnection->databasepath()+"/"+name()+".hk_sqlite";
      if (p_sqlitehandler)
      {
        sqlite_close(p_sqlitehandler);
	p_sqlitehandler=0;
      }
      p_sqlitehandler=sqlite_open(n.c_str(),0,&errormsg);
      if (!p_sqlitehandler && errormsg)
       {
         p_sqliteconnection->servermessage(errormsg);
	 sqlite_freemem(errormsg);
	 errormsg=0;
	 show_warningmessage(hk_translate("Driver error!\n")+hk_translate("Servermessage: ")+connection()->last_servermessage());
	 return false;
       }
   }   
 return true;
}



bool    hk_sqlitedatabase::driver_specific_rename_table(const hk_string& /*oldname*/,const hk_string& /*newname*/)
{
/*   hk_sqliteactionquery* query=new hk_sqliteactionquery(this);
  hk_string sql="ALTER TABLE "+query->identifierdelimiter()+oldname+query->identifierdelimiter()+" RENAME TO "+query->identifierdelimiter()+newname+query->identifierdelimiter();
   query->set_sql(sql.c_str(),sql.size());
   bool res=query->execute();
   delete query;
   return res;
*/
return false;
}

hk_datasource* hk_sqlitedatabase::driver_specific_new_view(hk_presentation* p)
{
    hk_sqliteview *x;
    x = new hk_sqliteview(this , p);
    return x ;

}




