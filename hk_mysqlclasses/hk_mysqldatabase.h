// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mysqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_MYSQLDATABASE
#define HK_MYSQLDATABASE
#include "hk_database.h"
#include "hk_mysqldatasource.h"

class hk_mysqlconnection;
class hk_presentation;

class hk_mysqldatabase:public hk_database
{
    friend class hk_mysqlconnection;

    public:
        hk_mysqlconnection* connection(void);
        ~hk_mysqldatabase();

    protected:
        hk_mysqldatabase(hk_mysqlconnection*c);
        void                driver_specific_tablelist(void);
        hk_datasource*      driver_specific_new_table(hk_presentation* p);
        hk_datasource*      driver_specific_new_resultquery(hk_presentation* p);
        hk_actionquery*     driver_specific_new_actionquery(void);
	virtual bool    driver_specific_rename_table(const hk_string& oldname,const hk_string& newname);
        bool            driver_specific_select_db(void);
        void driver_specific_viewlist(void);
        virtual hk_datasource* driver_specific_new_view(hk_presentation* p);

    private:
        hk_mysqlconnection* p_mysqlconnection;
        list<hk_mysqldatasource*> p_dsourcelist;
} ;
#endif
