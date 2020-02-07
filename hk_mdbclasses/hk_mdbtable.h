// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mdbclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.2 $
#ifndef HK_MDBTABLE
#define HK_MDBTABLE
#include "hk_mdbdatasource.h"

class hk_mdbdatabase;
class hk_presentation;
class hk_mdbtable :  public hk_mdbdatasource
{
    friend class hk_mdbdatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_mdbtable(hk_mdbdatabase* db,hk_presentation* p);
        bool driver_specific_enable(void);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_create_columns(void);
        virtual bool driver_specific_insert_data(void);
        virtual hk_column* driver_specific_new_column(void);
        virtual bool driver_specific_batch_enable(void);
        virtual bool driver_specific_batch_disable(void);
        virtual bool driver_specific_batch_goto_next(void);
        hk_mdbdatabase* p_mdbdatabase;
        virtual bool datasource_open(void);
	virtual bool datasource_fetch_next_row(void);
	virtual bool datasource_close(void);
	void set_columnspecifica(hk_mdbcolumn*col,MdbColumn* c);
MdbTableDef* p_table;
char **p_bound_values;
int  *p_bound_lens;
void free_handles(void);

};
#endif
