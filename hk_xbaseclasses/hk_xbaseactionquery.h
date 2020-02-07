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
#ifndef HK_XBASEACTIONQUERY
#define HK_XBASEACTIONQUERY
#include <hk_actionquery.h>
class hk_xbasedatabase;
class hk_xbaseactionquery:public hk_actionquery
{
    friend class hk_xbasedatabase;
    friend class hk_xbasedatasource;

    public:

    protected:
        hk_xbaseactionquery(hk_xbasedatabase* db);
        bool    driver_specific_execute(void);
        bool    driver_specific_sql(const char* s);
    private:
        hk_xbasedatabase* p_xbasedatabase;

};
#endif
