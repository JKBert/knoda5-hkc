// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_firebirdclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.9 $
#ifndef HK_FIREBIRDTABLE
#define HK_FIREBIRDTABLE
#include "hk_firebirddatasource.h"

class hk_firebirddatabase;
class hk_presentation;
class hk_firebirdtable :  public hk_firebirddatasource
{
    friend class hk_firebirddatabase;
    public:
        enum_datasourcetypes type() const;
   virtual       ~hk_firebirdtable();

    protected:
        hk_firebirdtable(hk_firebirddatabase* db,hk_presentation* p);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_create_table_now(void);
        virtual bool driver_specific_alter_table_now(void);
        bool driver_specific_enable(void);
        virtual list<indexclass>* driver_specific_indices(void);
        virtual bool    driver_specific_create_index(const hk_string& name,bool unique,list<hk_string>& fields);
        virtual bool    driver_specific_drop_index(const hk_string& i);
	virtual void driver_specific_referenceslist(void);
   private:
     list<indexclass>::iterator findindex(const hk_string& i);
        hk_string  internal_new_fields_arguments(bool);
        hk_string  internal_delete_fields_arguments(void);
        hk_string  internal_alter_fields_arguments(void);
        hk_string  field2string(hk_column::enum_columntype,const hk_string& m="");
        void create_autoinc_field(const hk_string& field);
       bool parse_for_generatorname(const hk_string &, hk_string& generator,hk_string& column);
        hk_string getprimarystring(bool alter);
     hk_datasource* p_tempdatasource;
     bool p_fieldset;
        hk_string primarystring;
     hk_string p_primary_key_name;
        list<indexclass> p_indices;
};
#endif
