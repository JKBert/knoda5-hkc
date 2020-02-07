// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_paradoxclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_PARADOXDATABASE
#define HK_PARADOXDATABASE
#include "hk_database.h"
extern "C" {
#include "pxlib/paradox.h"
}
class hk_paradoxconnection;
class hk_presentation;

class hk_paradoxdatabase:public hk_database
{
    friend class hk_paradoxconnection;

    public:
        hk_paradoxconnection* connection(void){return p_paradoxconnection;}
        ~hk_paradoxdatabase();
        hk_paradoxconnection* paradoxconnection(void) const {return p_paradoxconnection;}
virtual bool    delete_table(const hk_string& table,enum_interaction x=interactive);
    protected:
        hk_paradoxdatabase(hk_paradoxconnection*c);
        void                driver_specific_tablelist(void);
        bool            driver_specific_select_db(void);
        hk_datasource*      driver_specific_new_table(hk_presentation* p);
        hk_datasource*      driver_specific_new_resultquery(hk_presentation* p);
        hk_actionquery*     driver_specific_new_actionquery(void);

    private:
    hk_paradoxconnection* p_paradoxconnection;


} ;
#endif
