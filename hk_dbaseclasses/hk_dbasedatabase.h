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
#ifndef HK_DBASEDATABASE
#define HK_DBASEDATABASE
#include "hk_database.h"
#include "xbase/xbase64.h"
class hk_dbaseconnection;
class hk_presentation;

class hk_dbasedatabase:public hk_database
{
    friend class hk_dbaseconnection;

    public:
        hk_dbaseconnection* connection(void){return p_dbaseconnection;}
        ~hk_dbasedatabase();
 	xbXBase* dbhandle(void) {return &p_dbasehandle;}
virtual bool    delete_table(const hk_string& table,enum_interaction x=interactive);
    protected:
        hk_dbasedatabase(hk_dbaseconnection*c);
        void                driver_specific_tablelist(void);
        bool            driver_specific_select_db(void);
        hk_datasource*      driver_specific_new_table(hk_presentation* p);
        hk_datasource*      driver_specific_new_resultquery(hk_presentation* p);
        hk_actionquery*     driver_specific_new_actionquery(void);

    private:
    hk_dbaseconnection* p_dbaseconnection;
      xbXBase p_dbasehandle;

        
     
} ;
#endif
