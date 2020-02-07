// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_storageclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_STORAGECOLUMN
#define HK_STORAGECOLUMN
#include "hk_column.h"
using namespace std;
class hk_storagedatasource;

class hk_storagecolumn:public hk_column

{
    public:
        void     set_fieldnumber(unsigned int f);

    protected:
        hk_storagecolumn(hk_storagedatasource* ds, const hk_string& TRUESTRING,const hk_string& FALSESTRING);
        virtual ~hk_storagecolumn(void);
        hk_string   driver_specific_asstring_at(unsigned long a);
        void    driver_specific_asbool(bool b);
        bool    driver_specific_asbool_at(unsigned long a);
        virtual bool        driver_specific_is_nullvalue_at(unsigned long);
        char*   p_asstring;

        virtual const struct_raw_data* driver_specific_asbinary_at(unsigned long a);

    private:

        hk_storagedatasource*   p_storagedatasource;

} ;
#endif
