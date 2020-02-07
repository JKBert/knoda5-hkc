// // ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_firebirdclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_firebirddatabase.h"
#include "hk_firebirdconnection.h"
#include "hk_firebirdtable.h"
#include "hk_firebirdresultquery.h"
#include "hk_firebirdview.h"
#include "hk_firebirdactionquery.h"


hk_firebirddatabase::hk_firebirddatabase(hk_firebirdconnection* c):hk_database(c)
{
#ifdef HK_DEBUG
//     wanna_debug(true);
    hkdebug("hk_firebirddatabase::hk_firebirddatabase");
#endif
  p_firebirdconnection=c;
}


hk_firebirddatabase::~hk_firebirddatabase(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatabase::~hk_firebirddatabase");
#endif

}














void hk_firebirddatabase::driver_specific_tablelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatabase::driver_specific_tablelist");
#endif
    p_tablelist.erase(p_tablelist.begin(),p_tablelist.end());
    if (p_firebirdconnection!=NULL)
    {
        hk_string sql="select RDB$RELATION_NAME as \"tables\" from RDB$RELATIONS where RDB$SYSTEM_FLAG=0 AND RDB$VIEW_BLR is NULL";
        hk_datasource* ds=new_resultquery();
        if (ds)
        {
          ds->set_sql(sql);
          ds->enable();
          hk_column* col=ds->column_by_name("tables");
          if (col)
          {
               unsigned long m=ds->max_rows();
               for (unsigned long i=0;i<m;i++)
    		{
        		p_tablelist.insert(p_tablelist.end(),trim(col->asstring()));
        		ds->goto_next();
        	}

          }
          else show_warningmessage("Error hk_firebirddatabase::driver_specific_tablelist, Systemcolumn could not be loaded");
        delete ds;
        }

    //sort(p_tablelist.begin(),p_tablelist.end());
    }

}


void hk_firebirddatabase::driver_specific_viewlist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatabase::driver_specific_viewlist");
#endif
    p_viewlist.erase(p_viewlist.begin(),p_viewlist.end());
    if (p_firebirdconnection!=NULL)
    {

        hk_string sql="select RDB$RELATION_NAME as \"views\" from RDB$RELATIONS where RDB$SYSTEM_FLAG=0 AND RDB$VIEW_BLR is NOT NULL";
        hk_datasource* ds=new_resultquery();
        if (ds)
        {
          ds->set_sql(sql);
          ds->enable();
          hk_column* col=ds->column_by_name("views");
          if (col)
          {
               unsigned long m=ds->max_rows();
               for (unsigned long i=0;i<m;i++)
    		{
        		p_viewlist.insert(p_viewlist.end(),trim(col->asstring()));
        		ds->goto_next();
        	}

          }
          else show_warningmessage("Error hk_firebirddatabase::driver_specific_viewlist, Systemcolumn could not be loaded");
        delete ds;
        }

    //sort(p_viewlist.begin(),p_viewlist.end());
    }

}


bool hk_firebirddatabase::set_name(const hk_string& n)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatabase::set_name");
#endif
   hk_url url=n;
   hk_string newname=n;
   if (url.directory().size()==0 && p_firebirdconnection->defaultdatabase().size()>0)
     {
       hk_url defdburl=p_firebirdconnection->defaultdatabase();
       if (defdburl.directory().size()>0) newname=defdburl.directory()+"/"+newname;
       if (url.extension().size()==0)newname=newname+".fdb";
     }
   return hk_database::set_name(newname);
}



bool hk_firebirddatabase::driver_specific_select_db(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatabase::driver_specific_select_db");
#endif

    if (p_firebirdconnection!=NULL)
    {
        p_firebirdconnection->set_defaultdatabase(name());
        p_firebirdconnection->driver_specific_disconnect();
        return p_firebirdconnection->driver_specific_connect();
    }
    return false;

}

hk_datasource* hk_firebirddatabase::driver_specific_new_table(hk_presentation* p)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatabase::driver_specific_new_table");
#endif
    return new hk_firebirdtable(this,p);
}


hk_datasource* hk_firebirddatabase::driver_specific_new_resultquery(hk_presentation* p)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatabase::driver_specific_resultquery");
#endif
    return new hk_firebirdresultquery(this,p);
}


hk_datasource* hk_firebirddatabase::driver_specific_new_view(hk_presentation* p)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatabase::driver_specific_new_view");
#endif

    hk_firebirdview *x;
    x = new hk_firebirdview(this , p);
    return x ;

}


hk_actionquery * hk_firebirddatabase::driver_specific_new_actionquery(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatabase::driver_specific_new_actionquery");
#endif
    hk_firebirdactionquery *a;
    a = new hk_firebirdactionquery(this);
    return a;
}

