// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_firebirdclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_FIREBIRDDATASOURCE
#define HK_FIREBIRDDATASOURCE
#include "hk_storagedatasource.h"
#include "hk_firebirdcolumn.h"
#include <list>
#include <vector>
#include <algorithm>
#include <ibase.h>

class hk_firebirddatabase;
class hk_firebirdconnection;
class hk_presentation;

class hk_firebirddatasource:public hk_storagedatasource
{
    friend class hk_firebirddatabase;

    public:

    protected:
        hk_firebirddatasource(hk_firebirddatabase* d,hk_presentation* p);
        virtual ~hk_firebirddatasource();
        bool driver_specific_enable(void);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_create_columns(void);
        virtual bool driver_specific_insert_data(void);
        virtual hk_column* driver_specific_new_column(void);
        virtual bool driver_specific_batch_enable(void);
        virtual bool driver_specific_batch_disable(void);
        virtual bool driver_specific_batch_goto_next(void);
        virtual bool datasource_open(void);
	virtual bool datasource_fetch_next_row(void);
	virtual bool datasource_close(void);
        virtual bool    insert_row(enum_interaction c=interactive);
        virtual bool    update_row(enum_interaction c=interactive);
	void set_error();
	void free_handles(void);
        hk_firebirddatabase* p_firebirddatabase;
        XSQLDA* p_sqlhandle;
        isc_tr_handle p_transaction;
        isc_stmt_handle p_stmt;
        ISC_STATUS_ARRAY p_status;
        int p_currow;
         char* p_sqlstatement;
}


;
#endif
