// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mdbclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_mdbdatabase.h"
#include "hk_mdbconnection.h"
#include "hk_mdbtable.h"
#include "hk_mdbresultquery.h"




hk_mdbdatabase::hk_mdbdatabase(hk_mdbconnection* c):hk_database(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_mdbdatabase::hk_mdbdatabase");
#endif
  p_mdbconnection=c;
  p_mdbhandle=0;
 set_databasecharset("UTF8");
 mdb_set_date_fmt("%d.%m.%Y %H:%M:%S");
}


hk_mdbdatabase::~hk_mdbdatabase(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mdbdatabase::~hk_mdbdatabase");
#endif
      if (p_mdbhandle)
      {
        mdb_close(p_mdbhandle);
	p_mdbhandle=0;
      }

}











void hk_mdbconnection::servermessage(const hk_string& error)
{
       set_last_servermessage(error);

}


void hk_mdbdatabase::driver_specific_tablelist(void)
{
  if (!p_mdbhandle) return;
    p_tablelist.erase(p_tablelist.begin(),p_tablelist.end());

 	/* loop over each entry in the catalog */
 	for (unsigned int i=0; i < p_mdbhandle->num_catalog; i++)
	{
		MdbCatalogEntry* entry = (MdbCatalogEntry*)g_ptr_array_index(p_mdbhandle->catalog, i);

     		if (entry->object_type == MDB_TABLE &&!mdb_is_system_table(entry))
                 p_tablelist.push_back(entry->object_name);
	}
}


bool hk_mdbdatabase::driver_specific_select_db(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mdbdatabase::driver_specific_select_db");
#endif
      hk_string n= p_url.directory().size()>0? p_url.url(): p_mdbconnection->databasepath()+"/"+name()+".mdb";
      if (p_mdbhandle)
      {
        mdb_close(p_mdbhandle);

	p_mdbhandle=0;
      }
      p_mdbhandle=mdb_open(n.c_str(), MDB_NOFLAGS);
      if (!p_mdbhandle)
       {
         p_mdbconnection->servermessage("mdb error in select db");
	 show_warningmessage(hk_translate("Driver error!\n")+hk_translate("Servermessage: ")+connection()->last_servermessage());

	 return false;
       }

 	if (!mdb_read_catalog (p_mdbhandle, MDB_ANY))
	{
		show_warningmessage(hk_translate("File does not appear to be an Access database"));
		mdb_close(p_mdbhandle);
		p_mdbhandle=0;
		return false;
	}



 return true;
}


hk_datasource* hk_mdbdatabase::driver_specific_new_table(hk_presentation* p)
{
    return new hk_mdbtable(this,p);
}


hk_datasource* hk_mdbdatabase::driver_specific_new_resultquery(hk_presentation* p)
{
    return new hk_mdbresultquery(this,p);
}



