// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_dbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.1 $
#ifndef HK_DBASETABLE
#define HK_DBASETABLE
#include "hk_dbasedatasource.h"

class hk_dbasedatabase;
class hk_presentation;
class hk_dbasetable :  public hk_dbasedatasource
{
    friend class hk_dbasedatabase;
    public:
        enum_datasourcetypes type() const;
         virtual bool    insert_row(enum_interaction c=interactive);
         virtual bool    update_row(enum_interaction c=interactive);
         virtual bool    delete_row(enum_interaction c=interactive);
	 virtual        bool delete_rows(unsigned long from,unsigned long to,
	 				 bool check_depending_datasources=true,
	 				 enum_interaction c=interactive);

    protected:
        hk_dbasetable(hk_dbasedatabase* db,hk_presentation* p);
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
	void set_colvalues(bool is_update);
};
#endif
