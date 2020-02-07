// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_dbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_dbasecolumn.h"
#include "hk_dbasedatasource.h"


hk_dbasecolumn::hk_dbasecolumn(hk_dbasedatasource* ds, const hk_string& tTRUE,const hk_string& tFALSE)
:hk_storagecolumn(ds,tTRUE,tFALSE)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_dbasecolumn::constructor");
#endif
    p_driverspecific_dateformat="YMD";
    p_sqlnulldelimiter=defaulttextdelimiter;
    
}


hk_dbasecolumn::~hk_dbasecolumn(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbasecolumn::destructor");
#endif

}


bool hk_dbasecolumn::driver_specific_asstring(const hk_string& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbasecolumn::driver_specific_asstring(char*)");
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
        hk_string v=replace_all("'",s,"''");
	a=v.size();
	p_driver_specific_data= new char[a+1];
//Testing if works with postgresql
        strncpy(p_driver_specific_data,v.c_str(),a);
        p_driver_specific_data_size = a;          //cout <<"nichtbinï¿œ"<<endl;

    p_original_new_data=new char[a+1];
    strncpy(p_original_new_data,s.c_str(),a);
    p_original_new_data_size=a;

    return true;


}

