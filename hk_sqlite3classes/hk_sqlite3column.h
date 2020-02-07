// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_sqlite3classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_SQLITE3COLUMN
#define HK_SQLITE3COLUMN
#include "hk_storagecolumn.h"
class hk_sqlite3datasource;

class hk_sqlite3column:public hk_storagecolumn

{
    friend class hk_sqlite3datasource;
    friend class hk_sqlite3table;

    public:

    protected:
        hk_sqlite3column(hk_sqlite3datasource* ds, const hk_string& TRUESTRING,const hk_string& FALSESTRING);
        virtual ~hk_sqlite3column(void);
        bool    driver_specific_asstring(const hk_string& s);
        const char* driver_specific_transformed_asstring_at(unsigned long position);
        virtual bool    driver_specific_asbinary(const char* b);

    private:

        hk_sqlite3datasource* p_sqlitedatasource;
        hk_string p_resulttablename;
        hk_string p_asstringbuffer;

} ;
#endif
