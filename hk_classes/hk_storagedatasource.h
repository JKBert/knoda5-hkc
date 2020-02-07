// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_storageclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_STORAGEDATASOURCE
#define HK_STORAGEDATASOURCE
#include "hk_datasource.h"
#include "hk_column.h"
#include <list>
#include <vector>
#include <algorithm>
using namespace std;

class hk_presentation;

class hk_storagedatasource:public hk_datasource
{
    public:
//	vector <struct_raw_data*>* data();
        struct_raw_data* columndata(unsigned long row, unsigned int fieldnr);
        enum_datasourcetypes type();
        virtual void    dump_data(void);

    protected:
/**
 * deletes all internally stored data
 */

        void delete_data(void);
/**
 *adds a new row at the end of the datalist.
@param datarow an array of struct_raw_data (define
as  struct_raw_data* datarow=new struct_raw_data[num_fields]) where num_fields is the number of fields existing
datarow[0] represents the first column (column with fieldnr 0) etc

*/
        bool insert_data(struct_raw_data* datarow);
/**
 *has to be called from driver_specific_enable in case of a memory allocation error. This function shows a error message
 */
        void memory_error(void);
        hk_storagedatasource(hk_database* d,hk_presentation* p);

        virtual ~hk_storagedatasource();

        bool driver_specific_enable(void);
        bool driver_specific_disable(void);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_goto_row(unsigned long r);
        virtual unsigned long driver_specific_max_rows(void);
        virtual bool driver_specific_create_columns(void);
        virtual bool driver_specific_update_data(void);
        virtual bool driver_specific_insert_data(void);
        virtual bool driver_specific_delete_data_at(unsigned long);
        virtual bool driver_specific_batch_goto_previous(void);
        virtual bool driver_specific_batch_goto_next(void);

        bool p_data_changed;
        long p_debug_counter;
        struct_raw_data p_d;
        void set_maxrows(unsigned long);
        bool is_batchprevious(void){return use_batch_previous;}

    private:
        vector<struct_raw_data*> p_data;
        unsigned long p_rows;
        bool use_batch_previous;
}


;
#endif
