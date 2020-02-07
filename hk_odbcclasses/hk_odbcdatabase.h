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
#ifndef HK_ODBCDATABASE
#define HK_ODBCDATABASE
#include "hk_database.h"
#include "hk_odbcdatasource.h"

class hk_odbcconnection;
class hk_presentation;
class hk_odbcdatabaseprivate;

class hk_odbcdatabase:public hk_database
{
    friend class hk_odbcconnection;

    public:
        hk_odbcconnection* connection(void);
        ~hk_odbcdatabase();
hk_string  columntypeparameter(hk_column::enum_columntype) ;
    protected:
        hk_odbcdatabase(hk_odbcconnection*c);
        void                driver_specific_tablelist(void);
        void                driver_specific_viewlist(void);
        hk_datasource*      driver_specific_new_table(hk_presentation* p);
        hk_datasource*      driver_specific_new_resultquery(hk_presentation* p);
        hk_actionquery*     driver_specific_new_actionquery(void);
        virtual hk_datasource* driver_specific_new_view(hk_presentation* p);

        bool            driver_specific_select_db(void);
        virtual void            driver_specific_set_name(void);
	void set_colvalues(void);
	void parse_parameters(void);

    private:
        hk_odbcconnection* p_odbcconnection;
        list<hk_odbcdatasource*> p_dsourcelist;
	hk_odbcdatabaseprivate* p_private;
} ;
#endif
