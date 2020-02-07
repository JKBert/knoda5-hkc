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
#ifndef HK_SQLITE3DATASOURCE
#define HK_SQLITE3DATASOURCE
#include <sqlite3.h>
#include "hk_storagedatasource.h"
#include "hk_sqlite3column.h"
#include <hk_column.h>
#include <list>
#include <vector>
#include <algorithm>

class hk_sqlite3database;
class hk_sqlite3connection;
class hk_presentation;

class hk_sqlite3datasource:public hk_storagedatasource
{
    friend class hk_sqlite3database;

    public:
        hk_sqlite3connection* sqliteconnection(void);


    protected:
        hk_sqlite3datasource(hk_sqlite3database* d,hk_presentation* p);
        virtual ~hk_sqlite3datasource();

        bool driver_specific_enable(void);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_create_columns(void);
        virtual bool driver_specific_insert_data(void);
        hk_sqlite3database* p_sqlitedatabase;
        virtual hk_column* driver_specific_new_column(void);
        virtual bool driver_specific_batch_enable(void);
        virtual bool driver_specific_batch_disable(void);
        virtual bool driver_specific_batch_goto_next(void);
        virtual bool    insert_row(enum_interaction c=interactive);
        virtual bool    update_row(enum_interaction c=interactive);
        virtual bool datasource_open(void);
	virtual bool datasource_fetch_next_row(void);
	virtual bool datasource_close(void);

	void parse_createstatement(void);
class coltest
 {
   public:
   coltest()
    {
      p_notnull=false;
      p_primary=false;
      p_autoinc=false;
    }
   hk_string p_name;
   bool p_notnull;
   bool p_primary;
   bool p_autoinc;
 };

    private:
        sqlite3_stmt *vm;
	int colnums;
        list<coltest*> p_coltest;

}


;
#endif
