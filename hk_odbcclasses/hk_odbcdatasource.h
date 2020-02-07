// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_odbcclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_ODBCDATASOURCE
#define HK_ODBCDATASOURCE
#include <sql.h>
#include "hk_storagedatasource.h"
#include "hk_odbccolumn.h"
#include "hk_column.h"
#include <list>
#include <vector>
#include <algorithm>

class hk_odbcdatabase;
class hk_odbcconnection;
class hk_presentation;

class hk_odbcdatasource:public hk_storagedatasource
{
    friend class hk_odbcdatabase;

    public:

    protected:
        hk_odbcdatasource(hk_odbcdatabase* d,hk_presentation* p);
        virtual ~hk_odbcdatasource();
        bool driver_specific_enable(void);
        virtual bool driver_specific_create_columns(void);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_insert_data(void);
        hk_odbcdatabase* p_odbcdatabase;
        virtual hk_column* driver_specific_new_column(void);

        virtual bool driver_specific_batch_enable(void);
        virtual bool driver_specific_batch_disable(void);
        virtual bool driver_specific_batch_goto_next(void);

    private:
        void add_data(SQLSMALLINT numfields);
        void clear_result();
        void set_servermessage(void);
        unsigned long *p_length;
        SQLHSTMT p_result;
}


;
#endif

