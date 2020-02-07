// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_paradoxclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.10 $
#ifndef HK_PARADOXTABLE
#define HK_PARADOXTABLE
#include "hk_paradoxdatasource.h"
#include <stdio.h>
#include "pxlib/pxversion.h"
class hk_paradoxdatabase;
class hk_presentation;
class hk_paradoxtable :  public hk_paradoxdatasource
{
    friend class hk_paradoxdatabase;
    public:
        enum_datasourcetypes type() const;
         virtual bool    insert_row(enum_interaction c=interactive);
         virtual bool    update_row(enum_interaction c=interactive);
         virtual bool    delete_row(enum_interaction c=interactive);
        virtual ~hk_paradoxtable();
virtual        bool delete_rows(unsigned long from,unsigned long to,bool check_depending_datasources=true,enum_interaction c=interactive);

    protected:
        hk_paradoxtable(hk_paradoxdatabase* db,hk_presentation* p);
        bool driver_specific_enable(void);
        bool driver_specific_disable(void);
        virtual bool driver_specific_create_table_now(void);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_create_columns(void);
        virtual bool datasource_open(void);
	virtual bool datasource_fetch_next_row(void);
	virtual bool datasource_close(void);
        virtual bool driver_specific_insert_data(void);
	int p_currow;
	char* data;
	pxval_t** dataptr;
	hk_string p_codepage;
	FILE* file;

};
#endif
