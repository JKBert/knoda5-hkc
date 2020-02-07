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
#ifndef HK_POSTGRESSQLRESULTQUERY
#define HK_POSTGRESSQLRESULTQUERY
//#include <hk_string>
//#include <postgresql.h>
//#include <errmsg.h>
#include <hk_datasource.h>
#include <hk_postgresqldatasource.h>
class hk_postgresqldatabase;
class hk_presentation;

class hk_postgresqlresultquery:public hk_postgresqldatasource
{
    friend class hk_postgresqldatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_postgresqlresultquery(hk_postgresqldatabase* db,hk_presentation* p);

    private:

}  ;
#endif
