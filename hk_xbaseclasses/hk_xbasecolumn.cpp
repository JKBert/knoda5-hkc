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
#include "hk_xbasecolumn.h"
#include "hk_xbasedatasource.h"
#include "hk_xbasedatabase.h"

hk_xbasecolumn::hk_xbasecolumn(hk_xbasedatasource* ds, const hk_string& tTRUE,const hk_string& tFALSE)
:hk_storagecolumn(ds,tTRUE,tFALSE)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_xbasecolumn::constructor");
#endif
    p_driverspecific_dateformat="YMD";
    p_sqlnulldelimiter=defaulttextdelimiter;
   
}


hk_xbasecolumn::~hk_xbasecolumn(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasecolumn::destructor");
#endif

}




const char* hk_xbasecolumn::driver_specific_transformed_asstring_at(unsigned long position)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasecolumn::driver_specific_transformed_asstring_at(unsigned long)");
#endif
     p_asstringbuffer=replace_all("'",driver_specific_asstring_at(position),"\\'");
    return p_asstringbuffer.c_str();
}



bool hk_xbasecolumn::driver_specific_asstring(const hk_string& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasecolumn::driver_specific_asstring(char*)");
#endif
// cerr <<"hk_xbasecolumn::driver_specific_asstring: '"<<s<<"'"<<endl;
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
        hk_string v=replace_all("'",smallstringconversion(s,"",p_datasource->database()->databasecharset()),"\\'");
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


