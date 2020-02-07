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
#ifndef HK_SQLITEDATABASE
#define HK_SQLITEDATABASE
#include "hk_database.h"
#include "hk_sqlitedatasource.h"
#include "sqlite/sqlite.h"

class hk_sqliteconnection;
class hk_presentation;

class hk_sqlitedatabase:public hk_database
{
    friend class hk_sqliteconnection;

    public:
        hk_sqliteconnection* connection(void);
        ~hk_sqlitedatabase();

        sqlite* dbhandler() {return p_sqlitehandler;}
	hk_sqliteconnection* sqliteconnection(void) {return p_sqliteconnection;}
    protected:
        hk_sqlitedatabase(hk_sqliteconnection*c);
        void                driver_specific_tablelist(void);
        hk_datasource*      driver_specific_new_table(hk_presentation* p);
        hk_datasource*      driver_specific_new_resultquery(hk_presentation* p);
        hk_actionquery*     driver_specific_new_actionquery(void);
        void                driver_specific_viewlist(void);
	virtual bool    driver_specific_rename_table(const hk_string& oldname,const hk_string& newname);
        bool            driver_specific_select_db(void);
        virtual hk_datasource* driver_specific_new_view(hk_presentation* p);
    private:
        hk_sqliteconnection* p_sqliteconnection;
        list<hk_sqlitedatasource*> p_dsourcelist;
	sqlite* p_sqlitehandler;
} ;
#endif
