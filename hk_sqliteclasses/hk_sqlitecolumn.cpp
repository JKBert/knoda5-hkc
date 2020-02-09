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
#include "hk_sqlitecolumn.h"
#include "hk_sqlitedatasource.h"
#include "hk_sqliteconnection.h"
#include "hk_sqlitedatabase.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sqlite.h>
#include <string.h>

hk_sqlitecolumn::hk_sqlitecolumn(hk_sqlitedatasource* ds, const hk_string& tTRUE,const hk_string& tFALSE)
:hk_storagecolumn(ds,tTRUE,tFALSE)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_sqlitecolumn::constructor");
#endif
    p_sqlitedatasource=ds;
    p_driverspecific_timestampformat="YMDhms";
}


hk_sqlitecolumn::~hk_sqlitecolumn(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitecolumn::destructor");
#endif

}


const char* hk_sqlitecolumn::driver_specific_transformed_asstring_at(unsigned long position)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitecolumn::driver_specific_transformed_asstring_at(unsigned long)");
#endif
         p_asstringbuffer=replace_all("'",smallstringconversion(driver_specific_asstring_at(position),"",p_datasource->database()->databasecharset()),"''");

    return p_asstringbuffer.c_str();
}


bool hk_sqlitecolumn::driver_specific_asstring(const hk_string& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitecolumn::driver_specific_asstring(char*)");
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

    p_original_new_data=new char[s.size()+1];
    strncpy(p_original_new_data,s.c_str(),s.size());
    p_original_new_data_size=s.size();

    return true;


}


bool hk_sqlitecolumn::driver_specific_asbinary(const char* b)
{
    if (p_driver_specific_data!=NULL)
    {
        delete[] p_driver_specific_data;
        p_driver_specific_data=NULL;
    }
    p_driver_specific_data= new char[p_original_new_data_size+1];
    p_driver_specific_data_size=p_original_new_data_size+1;
    strcpy(p_driver_specific_data,b);
    return true;
}
