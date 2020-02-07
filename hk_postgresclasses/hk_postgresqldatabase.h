// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Edited by Anirban Biswas <utpal@cal2.vsnl.net.in>
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_POSTGRESQLDATABASE
#define HK_POSTGRESQLDATABASE
#include "hk_database.h"
#include "hk_postgresqldatasource.h"
#include "hk_postgresqlresultquery.h"

class hk_postgresqlconnection;
class hk_presentation;

class hk_postgresqldatabase:public hk_database
{
    friend class hk_postgresqlconnection;

    public:
        hk_postgresqlconnection* connection(void);
        hk_string systemdateformat(void); 

    protected:
        hk_postgresqldatabase(hk_postgresqlconnection*c);
        void                driver_specific_tablelist(void);
        void                driver_specific_viewlist(void);
        hk_datasource*      driver_specific_new_table(hk_presentation* p);
        hk_datasource*      driver_specific_new_resultquery(hk_presentation* p);
        hk_actionquery  *driver_specific_new_actionquery(void);
	virtual bool    driver_specific_rename_table(const hk_string& oldname,const hk_string& newname);
        bool    driver_specific_select_db(void);
        virtual hk_datasource* driver_specific_new_view(hk_presentation* p);

    private:
        hk_postgresqlconnection * p_postgresqlconnection;
        list<hk_postgresqldatasource*> p_dsourcelist;
        hk_string p_systemdateformat;
} ;
#endif
