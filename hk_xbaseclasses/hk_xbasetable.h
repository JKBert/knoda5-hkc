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
//$Revision: 1.1 $
#ifndef HK_XBASETABLE
#define HK_XBASETABLE
#include "hk_xbasedatasource.h"

class hk_xbasedatabase;
class hk_presentation;
class hk_xbasetable :  public hk_xbasedatasource
{
    friend class hk_xbasedatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_xbasetable(hk_xbasedatabase* db,hk_presentation* p);
        virtual bool driver_specific_create_table_now(void);
   
   
   private:     
	hk_string  internal_new_fields_arguments(bool);
//        hk_string  internal_delete_fields_arguments(void);
//        hk_string  internal_alter_fields_arguments(void);
        hk_string  field2string(hk_column::enum_columntype,const hk_string& m="");
        hk_string getprimarystring(bool alter);
        hk_string primarystring;


};
#endif
