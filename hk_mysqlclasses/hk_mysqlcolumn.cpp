// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mysqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_mysqlcolumn.h"
#include "hk_mysqldatasource.h"
#include "hk_mysqlconnection.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <mysql.h>
#include <errmsg.h>

hk_mysqlcolumn::hk_mysqlcolumn(hk_mysqldatasource* ds, const hk_string& tTRUE,const hk_string& tFALSE)
:hk_storagecolumn(ds,tTRUE,tFALSE)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlcolumn::constructor");
//wanna_debug(true);
#endif
    p_mysqldatasource=ds;
    p_driverspecific_timestampformat="YMDhms";

}


hk_mysqlcolumn::~hk_mysqlcolumn(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlcolumn::destructor");
#endif

}


const char* hk_mysqlcolumn::driver_specific_transformed_asstring_at(unsigned long position)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlcolumn::driver_specific_transformed_asstring_at(unsigned long)");
#endif
    if (p_mysqldatasource==NULL) return NULL;
    if ((p_mysqldatasource->columndata(position,p_fieldnr)==NULL)
        ||(p_mysqldatasource->max_rows()==0)
        ||(position>=p_mysqldatasource->max_rows())) return "";

    struct_raw_data*  data=p_mysqldatasource->columndata(position,p_fieldnr);
    if (p_asstring!=NULL)
    {
        delete[] p_asstring;
        p_asstring=NULL;
    }
    if (!p_mysqldatasource->dbhandler())
    {
        return "";
    }

    if (!(data->data))
    {
        cerr <<"hk_mysqlcolumn return NULL"<<endl;
        return "NULL";
    }
    p_asstring= new char[2*data->length+1];
#ifdef HAVE_MYSQL_REAL_ESCAPE_STRING
    if (p_mysqldatasource->dbhandler()) mysql_real_escape_string(p_mysqldatasource->dbhandler(),p_asstring,data->data,data->length);
#else
    if (p_mysqldatasource->dbhandler()) mysql_escape_string(p_asstring,data->data,data->length);
#endif
    return p_asstring;

}


bool hk_mysqlcolumn::driver_specific_asstring(const hk_string& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlcolumn::driver_specific_asstring(char*)");
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
    p_driver_specific_data= new char[2*a+1];
    if (p_mysqldatasource->mysqlconnection()==NULL)
    {
        cerr <<"!!!!!!!!!!!!!!!! mysqlconnection==NULL"<<endl;
        delete [] p_driver_specific_data;
        p_driver_specific_data=NULL;
        return false;
    }
    if (!p_mysqldatasource->dbhandler())
    {
        delete [] p_driver_specific_data;
        p_driver_specific_data=NULL;
        return false;
    }
#ifdef HAVE_MYSQL_REAL_ESCAPE_STRING
    p_driver_specific_data_size=mysql_real_escape_string(p_mysqldatasource->dbhandler(),p_driver_specific_data,s.c_str(),a);
#else
    p_driver_specific_data_size=mysql_escape_string(p_driver_specific_data,s.c_str(),a);
#endif
    p_original_new_data=new char[a+1];
    strcpy(p_original_new_data,s.c_str());
    p_original_new_data_size=a;
    return true;
}


bool hk_mysqlcolumn::driver_specific_asbinary(const char* b)
{
    if (p_driver_specific_data!=NULL)
    {
        delete[] p_driver_specific_data;
        p_driver_specific_data=NULL;
    }
    p_driver_specific_data= new char[2*p_original_new_data_size+1];
    if (!p_mysqldatasource->dbhandler())
    {
        delete [] p_driver_specific_data;
        p_driver_specific_data=NULL;
        return false;
    }
#ifdef HAVE_MYSQL_REAL_ESCAPE_STRING
    p_driver_specific_data_size=mysql_real_escape_string(p_mysqldatasource->dbhandler(),p_driver_specific_data,b,p_original_new_data_size);
#else
    p_driver_specific_data_size=mysql_escape_string(p_driver_specific_data,b,p_original_new_data_size);
#endif
    return true;
}
