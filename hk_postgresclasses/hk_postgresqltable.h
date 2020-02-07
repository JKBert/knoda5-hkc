// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Edited by Anirban Biswas. <utpal@cal2.vsnl.net.in>
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.12 $

#ifndef HK_POSTGRESQLTABLE
#define HK_POSTGRESQLTABLE
#include <hk_datasource.h>
#include <hk_postgresqldatasource.h>

class hk_postgresqldatabase;
class hk_presentation;
class hk_postgresqltable :  public hk_postgresqldatasource
{
    friend class hk_postgresqldatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_postgresqltable(hk_postgresqldatabase* db,hk_presentation* p);
        bool driver_specific_enable(void);
        bool driver_specific_name(const hk_string& n);
        virtual bool driver_specific_create_table_now(void);
        virtual bool driver_specific_alter_table_now(void);
        virtual list<indexclass>* driver_specific_indices(void);
        virtual bool    driver_specific_drop_index(const hk_string& i);
        virtual bool    driver_specific_create_index(const hk_string& name,bool unique,list<hk_string>& fields);
        virtual bool driver_specific_create_columns(void);
        virtual void driver_specific_after_copy_table();
	virtual void driver_specific_referenceslist(void);

    private:
        hk_string  internal_new_fields_arguments(bool);
        bool  internal_alter_fields_arguments(void);
        hk_string  internal_delete_fields_arguments(void);
        hk_string  field2string(hk_column::enum_columntype,const hk_string& m="");
        hk_string getprimarystring(bool alter);
        bool is_unallowed_alter(void);
        void set_indexquery(void);
        hk_string primarystring;
        bool is_alteredfield(const hk_string& f);
        bool is_deletedfield(const hk_string& f);
        list<indexclass> p_indices;
        list<indexclass>::iterator findindex(const hk_string& i);
};
#endif
