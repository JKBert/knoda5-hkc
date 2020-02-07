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
#include "hk_storagecolumn.h"
#include "hk_storagedatasource.h"

hk_storagecolumn::hk_storagecolumn(hk_storagedatasource* ds, const hk_string& tTRUE,const hk_string& tFALSE) :hk_column(ds,tTRUE,tFALSE)
{
#ifdef HK_DEBUG
    hkdebug("storagecolumn::constructor");
//wanna_debug(true);
#endif
    p_storagedatasource=ds;
    p_asstring=NULL;

}


hk_storagecolumn::~hk_storagecolumn(void)
{
#ifdef HK_DEBUG
    hkdebug("storagecolumn::destructor");
#endif
    if (p_asstring!=NULL)
    {
        delete[] p_asstring;
        p_asstring=NULL;
    }

}


void hk_storagecolumn::set_fieldnumber(unsigned int f)
{
    p_fieldnr=f;
}


hk_string   hk_storagecolumn::driver_specific_asstring_at(unsigned long a)
{
#ifdef HK_DEBUG
    hkdebug("hk_storagecolumn::driver_specific_asstring_at",(int)a);
#endif
    if (p_storagedatasource==NULL) return "";
    if ((p_storagedatasource->columndata(a,p_fieldnr)==NULL)||(p_storagedatasource->max_rows()==0)
        ||(a>=p_storagedatasource->max_rows())) return "";

    struct_raw_data*  data=p_storagedatasource->columndata(a,p_fieldnr);
    if (p_asstring!=NULL)
    {
        delete[] p_asstring;
        p_asstring=NULL;
    }
    if (!data) return "";
    if (!data->data) return "";                   //"NULL VALUE";//if field value is NULL return

    p_asstring= new char[data->length+1];

    unsigned int tk;
    for ( tk=0; tk <data->length;tk++)
    {
        p_asstring[tk]= data->data[tk];
    }
    p_asstring[tk]=0;
    return p_asstring;

}


void hk_storagecolumn::driver_specific_asbool(bool b)
{
    if (b) set_asstring("TRUE",true,true);
    else   set_asstring("FALSE",true,true);
}


bool hk_storagecolumn::driver_specific_asbool_at(unsigned long a)
{

    if (is_numerictype(this))
    {
        return (format_number(asstring_at(a,true),false,false,0,locale())==p_true);

    }
    return (asstring_at(a,true)=="TRUE");
}


const struct_raw_data* hk_storagecolumn::driver_specific_asbinary_at(unsigned long a)
{
#ifdef HK_DEBUG
    hkdebug("driver_specific_asbinary_at");
#endif
    if (p_storagedatasource==NULL) return NULL;
    if ((p_storagedatasource->max_rows()==0)||(a>=p_storagedatasource->max_rows())) return NULL;
    return p_storagedatasource->columndata(a,p_fieldnr);

}


bool        hk_storagecolumn::driver_specific_is_nullvalue_at(unsigned long a)
{
    struct_raw_data*  data=p_storagedatasource->columndata(a,p_fieldnr);
    if (!data||p_storagedatasource==NULL||(p_storagedatasource->max_rows()==0)
        ||(a>=p_storagedatasource->max_rows())) return false;
    return !(data->data);                         //returns true if data->data is NULL else false

}
