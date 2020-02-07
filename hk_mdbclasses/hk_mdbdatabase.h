// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mdbclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_MDBDATABASE
#define HK_MDBDATABASE
#include "hk_database.h"
#ifdef __cplusplus
  extern "C" {
#endif
#include "mdbtools/mdbtools.h"
#ifdef __cplusplus
  }
#endif
class hk_mdbconnection;
class hk_presentation;

class hk_mdbdatabase:public hk_database
{
    friend class hk_mdbconnection;

    public:
        hk_mdbconnection* connection(void){return p_mdbconnection;}
        ~hk_mdbdatabase();
        MdbHandle* dbhandle(void){return p_mdbhandle;}
    protected:
        hk_mdbdatabase(hk_mdbconnection*c);
        void                driver_specific_tablelist(void);
        bool            driver_specific_select_db(void);
        hk_datasource*      driver_specific_new_table(hk_presentation* p);
        hk_datasource*      driver_specific_new_resultquery(hk_presentation* p);

    private:
    hk_mdbconnection* p_mdbconnection;
    MdbHandle* p_mdbhandle;

} ;
#endif
