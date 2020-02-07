// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_odbcclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_odbccolumn.h"
#include "hk_odbcdatasource.h"
#include "hk_odbcconnection.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sql.h>
//#include <errmsg.h>

hk_odbccolumn::hk_odbccolumn(hk_odbcdatasource* ds, const hk_string& tTRUE,const hk_string& tFALSE)
:hk_storagecolumn(ds,tTRUE,tFALSE)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbccolumn::constructor");
//wanna_debug(true);
#endif
    p_odbcdatasource=ds;

}


hk_odbccolumn::~hk_odbccolumn(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbccolumn::destructor");
#endif

}


const char* hk_odbccolumn::driver_specific_transformed_asstring_at(unsigned long position)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbccolumn::driver_specific_transformed_asstring_at(unsigned long)");
#endif
    p_asstringbuffer=replace_all(p_datasource->textdelimiter(),driver_specific_asstring_at(position),p_datasource->textdelimiter()+p_datasource->textdelimiter());
    return p_asstringbuffer.c_str();

}


bool hk_odbccolumn::driver_specific_asstring(const hk_string& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbccolumn::driver_specific_asstring(char*)");
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
    p_driver_specific_data= new char[a+1];
/*	  if (p_odbcdatasource->odbcconnection()==NULL)
      {
        cerr <<"!!!!!!!!!!!!!!!! odbcconnection==NULL"<<endl;
         delete [] p_driver_specific_data;
         p_driver_specific_data=NULL;
        return false;
      }
    */
    hk_string v=replace_all(p_datasource->textdelimiter(),s,p_datasource->textdelimiter()+p_datasource->textdelimiter());
    cerr <<"setze s="<<s<<endl;
    cerr <<"nach transform:"<<v<<endl;
    a=v.size();
    p_original_new_data=new char[a+1];            
    strncpy(p_original_new_data,s.c_str(),s.size());
    strncpy(p_driver_specific_data,v.c_str(),a);
    p_driver_specific_data_size=a;
    p_original_new_data_size=a;
    return true;





}


/*
bool hk_odbccolumn::driver_specific_asbinary(const char* b)
{
    if (p_driver_specific_data!=NULL) {
                delete[] p_driver_specific_data;
                p_driver_specific_data=NULL;
                  }
      p_driver_specific_data= new char[2*p_original_new_data_size+1];
if (!p_odbcdatasource->dbhandler())
          {
         delete [] p_driver_specific_data;
p_driver_specific_data=NULL;
return false;
}
#ifdef HAVE_odbc_REAL_ESCAPE_STRING
p_driver_specific_data_size=odbc_real_escape_string(p_odbcdatasource->dbhandler(),p_driver_specific_data,b,p_original_new_data_size);
#else
p_driver_specific_data_size=odbc_escape_string(p_driver_specific_data,b,p_original_new_data_size);
#endif
return true;
}

*/
