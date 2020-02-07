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
#ifndef HK_XBASEDATABASE
#define HK_XBASEDATABASE
#include "hk_database.h"
#include <xbsql.h>
class hk_xbaseconnection;
class hk_presentation;

class hk_xbasedatabase:public hk_database
{
    friend class hk_xbaseconnection;

    public:
        hk_xbaseconnection* connection(void){return p_xbaseconnection;}
        ~hk_xbasedatabase();
	XBaseSQL* sqlhandle(void) {return p_xbasesqlhandle;}
    protected:
        hk_xbasedatabase(hk_xbaseconnection*c);
        void                driver_specific_tablelist(void);
        bool            driver_specific_select_db(void);
        hk_datasource*      driver_specific_new_table(hk_presentation* p);
        hk_datasource*      driver_specific_new_resultquery(hk_presentation* p);
        hk_actionquery*     driver_specific_new_actionquery(void);

    private:
    hk_xbaseconnection* p_xbaseconnection;
    XBaseSQL*    p_xbasesqlhandle;
     
} ;
#endif
