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
#include "hk_sqlite3column.h"
#include "hk_sqlite3datasource.h"
#include "hk_sqlite3database.h"
#include "hk_sqlite3connection.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sqlite3.h>

hk_sqlite3column::hk_sqlite3column(hk_sqlite3datasource* ds, const hk_string& tTRUE,const hk_string& tFALSE)
:hk_storagecolumn(ds,tTRUE,tFALSE)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_sqlite3column::constructor");
#endif
    p_sqlitedatasource=ds;
    p_driverspecific_timestampformat="YMDhms";
}


hk_sqlite3column::~hk_sqlite3column(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3column::destructor");
#endif

}


const char* hk_sqlite3column::driver_specific_transformed_asstring_at(unsigned long position)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3column::driver_specific_transformed_asstring_at(unsigned long)");
#endif
     p_asstringbuffer=replace_all("'",smallstringconversion(driver_specific_asstring_at(position),"",p_datasource->database()->databasecharset()),"''");
    return p_asstringbuffer.c_str();
}


bool hk_sqlite3column::driver_specific_asstring(const hk_string& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3column::driver_specific_asstring(char*)");
#endif
    unsigned long a = s.size();
    if (p_driver_specific_data!=NULL)
    {
        delete[] p_driver_specific_data;
        p_driver_specific_data=NULL;
    }
    if (p_original_new_data!=NULL)
    {
        delete[] p_original_new_data;
        p_original_new_data=NULL;
    }
        hk_string v=replace_all("'",smallstringconversion(s,"",p_datasource->database()->databasecharset()),"''");
	a=v.size();
	p_driver_specific_data= new char[a+1];
//Testing if works with postgresql
        strncpy(p_driver_specific_data,v.c_str(),a);
        p_driver_specific_data_size = a;          //cout <<"nichtbin�"<<endl;

    a=s.size();
    p_original_new_data=new char[a+1];
    strncpy(p_original_new_data,s.c_str(),a);
    p_original_new_data_size=a;

    return true;


}


bool hk_sqlite3column::driver_specific_asbinary(const char* b)
{
    if (p_driver_specific_data!=NULL)
    {
        delete[] p_driver_specific_data;
        p_driver_specific_data=NULL;
    }
    p_driver_specific_data= new char[p_original_new_data_size];
    p_driver_specific_data_size=p_original_new_data_size;
    //strncpy(p_driver_specific_data,b);
    memcpy(p_driver_specific_data,b,p_driver_specific_data_size);
    return true;
}
