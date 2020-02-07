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
#ifndef HK_SQLITEDATASOURCE
#define HK_SQLITEDATASOURCE
#include "sqlite/sqlite.h"
#include "hk_storagedatasource.h"
#include "hk_sqlitecolumn.h"
#include "hk_column.h"
#include <list>
#include <vector>
#include <algorithm>

class hk_sqlitedatabase;
class hk_sqliteconnection;
class hk_presentation;

class hk_sqlitedatasource:public hk_storagedatasource
{
    friend class hk_sqlitedatabase;

    public:
        hk_sqliteconnection* sqliteconnection(void);


    protected:
        hk_sqlitedatasource(hk_sqlitedatabase* d,hk_presentation* p);
        virtual ~hk_sqlitedatasource();

        bool driver_specific_enable(void);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_create_columns(void);
        virtual bool driver_specific_insert_data(void);
        hk_sqlitedatabase* p_sqlitedatabase;
        virtual hk_column* driver_specific_new_column(void);
        virtual bool driver_specific_batch_enable(void);
        virtual bool driver_specific_batch_disable(void);
        virtual bool driver_specific_batch_goto_next(void);

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
        sqlite_vm *vm;
	const char** data;
	const char** colnames;
	int colnums;
        list<coltest*> p_coltest;
}


;
#endif
