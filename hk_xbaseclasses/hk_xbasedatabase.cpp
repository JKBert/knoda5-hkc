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
#include "hk_xbasedatabase.h"
#include "hk_xbaseconnection.h"
#include "hk_xbasetable.h"
#include "hk_xbaseresultquery.h"
#include "hk_xbaseactionquery.h"



hk_xbasedatabase::hk_xbasedatabase(hk_xbaseconnection* c):hk_database(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasedatabase::hk_xbasedatabase");
#endif
  p_xbaseconnection=c;
  p_xbasesqlhandle=NULL;
  
}


hk_xbasedatabase::~hk_xbasedatabase(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasedatabase::~hk_xbasedatabase");
#endif

}


void hk_xbaseconnection::servermessage(const hk_string& error)
{
       set_last_servermessage(error);

}


void hk_xbasedatabase::driver_specific_tablelist(void)
{
p_tablelist.erase(p_tablelist.begin(),p_tablelist.end());

if (!p_xbasesqlhandle) return;	
XBSQLTableSet *ts = p_xbasesqlhandle->getTableSet() ;
for (int i = 0 ; i < ts->getNumTables() ; i += 1)
 {
   p_tablelist.push_back(ts->getTableName (i)) ;
 } 
delete	 ts ;

}


bool hk_xbasedatabase::driver_specific_select_db(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasedatabase::driver_specific_select_db");
#endif
      if (p_xbasesqlhandle)
       {  
       	delete p_xbasesqlhandle;
	p_xbasesqlhandle=NULL;

       }
      hk_string n= p_url.directory().size()>0? p_url.directory(): p_xbaseconnection->databasepath()+"/"+name();
      p_xbasesqlhandle=new XBaseSQL(n.c_str());
      p_xbasesqlhandle->setClosePack(true);
      p_xbasesqlhandle->setCaseSensitive(true);
      p_xbasesqlhandle->setUseWildcard(true);
      p_xbasesqlhandle->setGoSlow(false);

 return true;
}


hk_datasource* hk_xbasedatabase::driver_specific_new_table(hk_presentation* p)
{
    return new hk_xbasetable(this,p);
}


hk_datasource* hk_xbasedatabase::driver_specific_new_resultquery(hk_presentation* p)
{
    return new hk_xbaseresultquery(this,p);
}

hk_actionquery* hk_xbasedatabase::driver_specific_new_actionquery(void)
{
    return new hk_xbaseactionquery(this);
}



