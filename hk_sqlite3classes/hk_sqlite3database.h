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
#ifndef HK_SQLITE3DATABASE
#define HK_SQLITE3DATABASE
#include "hk_database.h"
#include "hk_sqlite3datasource.h"
#include <sqlite3.h>

class hk_sqlite3connection;
class hk_presentation;

class hk_sqlite3database:public hk_database
{
    friend class hk_sqlite3connection;

    public:
        hk_sqlite3connection* connection(void);
        ~hk_sqlite3database();

        sqlite3* dbhandler() {return p_sqlitehandler;}
	hk_sqlite3connection* sqliteconnection(void) {return p_sqliteconnection;}
    protected:
        hk_sqlite3database(hk_sqlite3connection*c);
        void                driver_specific_tablelist(void);
        hk_datasource*      driver_specific_new_table(hk_presentation* p);
        hk_datasource*      driver_specific_new_resultquery(hk_presentation* p);
        hk_actionquery*     driver_specific_new_actionquery(void);
        void                driver_specific_viewlist(void);
	virtual bool    driver_specific_rename_table(const hk_string& oldname,const hk_string& newname);
        bool            driver_specific_select_db(void);
        virtual hk_datasource* driver_specific_new_view(hk_presentation* p);
    private:
        hk_sqlite3connection* p_sqliteconnection;
        list<hk_sqlite3datasource*> p_dsourcelist;
	sqlite3* p_sqlitehandler;
} ;
#endif
