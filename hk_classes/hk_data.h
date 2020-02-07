// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_DATA
#define HK_DATA
#include "hk_class.h"
using namespace std;
const hk_string defaulttextdelimiter="'";
const hk_string defaultidentifierdelimiter="\"";
/**
 *@short Base class for all kind of queries
 *@version $Revision: 1.21 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *@internal
 *for internal use only.
 *Base class for queries of all type
 */

class hk_data: public hk_class
{
    friend class hk_database;
    public:
/**
 *name of the Datasource. If the datasource is of type "Table","Query", this name will be used to create automatically
 *a SQL-statement : "SELECT * FROM <name>"
 */
        virtual hk_string   name(void) const;
        virtual void    set_name(const hk_string& n);
/**
 *Datasources can be of different types (i.e. table). You never should get in contact with ds_unkown (which would
 *be understood as a bug)
 *
 */
        enum enum_datasourcetypes {ds_table,ds_query,ds_actionquery,ds_view,ds_unknown};
        virtual enum_datasourcetypes type(void) const;
	hk_string identifierdelimiter(void) const {return p_identifierdelimiter;}
	hk_string textdelimiter(void) const {return p_sqltextdelimiter;}
	static bool print_sqlstatements(void) {return p_print_sqlstatements;}
	static void set_print_sqlstatements(bool p);

    protected:
        hk_data();
        virtual ~hk_data();
        virtual void before_source_vanishes(void);
        virtual void before_connection_disconnects(void);
        virtual void filelist_changes(listtype t);
        virtual bool driver_specific_name(const hk_string& n);
        hk_string   p_name;
        hk_string p_sqltextdelimiter;
	hk_string  p_identifierdelimiter;
	static bool    p_print_sqlstatements;//will be used to decide whether sql statements will be printed to cerr

    private:
//p_already_handled is used from hk_database in bulk operations to find out if this object has been already handled.
        bool    p_already_handled;

};
#endif
