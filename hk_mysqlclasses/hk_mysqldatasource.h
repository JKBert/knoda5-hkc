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
#ifndef HK_MYSQLDATASOURCE
#define HK_MYSQLDATASOURCE
#include <mysql.h>
#include "hk_storagedatasource.h"
#include "hk_mysqlcolumn.h"
#include "hk_column.h"
#include <list>
#include <vector>
#include <algorithm>

class hk_mysqldatabase;
class hk_mysqlconnection;
class hk_presentation;

class hk_mysqldatasource:public hk_storagedatasource
{
    friend class hk_mysqldatabase;

    public:
        hk_mysqlconnection* mysqlconnection(void);

        MYSQL* dbhandler(void);
        virtual void    set_name(const hk_string& n,bool registerchange=true);

    protected:
        hk_mysqldatasource(hk_mysqldatabase* d,hk_presentation* p);
        virtual ~hk_mysqldatasource();

        bool driver_specific_enable(void);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_create_columns(void);
        virtual bool driver_specific_insert_data(void);
        hk_mysqldatabase* p_mysqldatabase;
        virtual hk_column* driver_specific_new_column(void);
        virtual bool driver_specific_batch_enable(void);
        virtual bool driver_specific_batch_disable(void);
        virtual bool driver_specific_batch_goto_next(void);

    private:
        void add_data(unsigned int numfields);
        void set_handle(void);
        MYSQL_RES* p_result;
        MYSQL_ROW p_row;
        unsigned long *p_length;
        MYSQL* p_SQL_Connection;
        void set_uniquenames(list<hk_column*>::iterator);
}


;
#endif
