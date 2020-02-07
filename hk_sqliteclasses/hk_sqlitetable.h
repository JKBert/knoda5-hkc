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
//$Revision: 1.4 $
#ifndef HK_SQLITETABLE
#define HK_SQLITETABLE
#include "hk_datasource.h"
#include "hk_sqlitedatasource.h"

class hk_sqlitedatabase;
class hk_presentation;
class hk_sqlitetable :  public hk_sqlitedatasource
{
    friend class hk_sqlitedatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_sqlitetable(hk_sqlitedatabase* db,hk_presentation* p);
        bool driver_specific_enable(void);
        bool driver_specific_name(const hk_string& n);
        virtual bool driver_specific_create_table_now(void);
        virtual bool driver_specific_alter_table_now(void);
        virtual list<indexclass>* driver_specific_indices(void);
        virtual bool    driver_specific_drop_index(const hk_string& i);
        virtual bool    driver_specific_create_index(const hk_string& name,bool unique,list<hk_string>& fields);

    private:
        hk_string  internal_new_fields_arguments(bool);
        hk_string  internal_delete_fields_arguments(void);
        hk_string  internal_alter_fields_arguments(void);
        hk_string  field2string(hk_column::enum_columntype,const hk_string& m="");
        hk_string getprimarystring(bool alter);
        hk_string primarystring;
        bool is_alteredfield(const hk_string f);
        bool is_deletedfield(const hk_string f);
        list<indexclass> p_indices;
        list<indexclass>::iterator findindex(const hk_string& i);
	void parse_indices(list<hk_string>& tokens, const hk_string & indices);
};
#endif
