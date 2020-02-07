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
#ifndef HK_FIREBIRDACTIONQUERY
#define HK_FIREBIRDACTIONQUERY
#include "hk_actionquery.h"
#include <ibase.h>

class hk_firebirddatabase;
class hk_firebirdactionquery:public hk_actionquery
{
    friend class hk_firebirddatabase;
    friend class hk_firebirddatasource;

    public:

    protected:
        hk_firebirdactionquery(hk_firebirddatabase* db);
        bool    driver_specific_execute(void);
        bool    driver_specific_sql(const char* s);
    private:
        bool is_create_database_query();
        bool execute_create_database_query();
        hk_firebirddatabase* p_firebirddatabase;
	void set_error();
	void free_handles(void);

        XSQLDA* p_sqlhandle;
        isc_tr_handle p_transaction;
        isc_stmt_handle p_stmt;
        ISC_STATUS_ARRAY p_status;
};
#endif
