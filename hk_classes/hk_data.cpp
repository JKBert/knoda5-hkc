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
//$Revision: 1.18 $
#include "hk_data.h"
bool hk_data::p_print_sqlstatements=false;

hk_data::hk_data():hk_class()
{
    p_already_handled=false;
        p_sqltextdelimiter=defaulttextdelimiter;
	p_identifierdelimiter=defaultidentifierdelimiter;

}


hk_data::~hk_data()
{
}


void hk_data::before_source_vanishes()
{

}


void hk_data::before_connection_disconnects(void)
{

}


hk_string hk_data::name(void) const
{
    return p_name;
}


void hk_data::set_name(const hk_string& n)
{
#ifdef HK_DEBUG
    hkdebug("hk_data::set_name");
#endif
    hk_string newname=trim(n);
    if (  driver_specific_name(newname))
    {
        p_name=newname;
//      if (registerchange) after_name_changes();
    }
}


hk_data::enum_datasourcetypes hk_data::type(void) const
{
    return ds_unknown;
}


void hk_data::filelist_changes(listtype )
{
//cout <<"hk_data::filelist_changes"<<endl;
}


bool hk_data::driver_specific_name(const hk_string& )
{
    return true;
}


void hk_data::set_print_sqlstatements(bool p)
 {
  p_print_sqlstatements=p;
 }

