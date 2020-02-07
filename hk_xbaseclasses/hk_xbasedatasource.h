// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_xbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_XBASEDATASOURCE
#define HK_XBASEDATASOURCE
#include "hk_storagedatasource.h"
#include "hk_xbasecolumn.h"
#include <list>
#include <vector>
#include <algorithm>
#include <xbsql.h>

class hk_xbasedatabase;
class hk_xbaseconnection;
class hk_presentation;

class hk_xbasedatasource:public hk_storagedatasource
{
    friend class hk_xbasedatabase;

    public:

    protected:
        hk_xbasedatasource(hk_xbasedatabase* d,hk_presentation* p);
        virtual ~hk_xbasedatasource();
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
        hk_xbasedatabase* p_xbasedatabase;
        XBSQLQuery* p_sqlquery;
	int p_currow;
}


;
#endif
