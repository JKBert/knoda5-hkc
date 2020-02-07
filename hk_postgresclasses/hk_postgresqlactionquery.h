// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_POSTGRESQLACTIONQUERY
#define HK_POSTGRESQLACTIONQUERY
#include <hk_actionquery.h>
class hk_postgresqldatabase;
class hk_postgresqlactionquery:public hk_actionquery
{
    friend class hk_postgresqldatabase;
    friend class hk_postgresqldatasource;

    public:

    protected:
        hk_postgresqlactionquery(hk_postgresqldatabase* db);
        bool    driver_specific_execute(void);
        bool    driver_specific_sql(const char* s);
    private:
        hk_postgresqldatabase* postgresqldb;

} ;
#endif
