// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Edited by Anirban Biswas <utpal@cal2.vsnl.net.in>
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_postgresqlcolumn.h"
#include "hk_postgresqldatasource.h"
#include "hk_postgresqldatabase.h"
hk_postgresqlcolumn::hk_postgresqlcolumn(hk_postgresqldatasource* ds, const hk_string& tTRUE,const hk_string& tFALSE)
:hk_storagecolumn(ds,tTRUE,tFALSE)
{
#ifdef HK_DEBUG
    hkdebug("postgresqlcolumn::constructor");
//wanna_debug(true);
#endif
    p_postgresqldatasource=ds;
    p_driver_specific_delimit_bool=true;
    set_columntype(textcolumn);
    hk_string dt="Y-M-D";
    p_driverspecific_dateformat=dt;
    p_driverspecific_timeformat="h:m:s";
    p_driverspecific_datetimeformat=dt+" h:m:s";
    p_driverspecific_timestampformat=dt+" h:m:s";
//     cerr <<"column dateformat:"<<dt<<endl;
 

}


hk_postgresqlcolumn::~hk_postgresqlcolumn(void)
{
#ifdef HK_DEBUG
    hkdebug("postgresqlcolumn::destructor");
#endif

}


unsigned int hk_postgresqlcolumn::fieldnr()
{
    return p_fieldnr;
}


void hk_postgresqlcolumn::fieldnr(unsigned int f)
{
    p_fieldnr=f;
}


bool hk_postgresqlcolumn::driver_specific_asstring(const hk_string& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqlcolumn::driver_specific_asstring(char*)");
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
    if (columntype()==binarycolumn)
    {
        size_t psize=0;
        p_driver_specific_data=(char*) escapeBytea((unsigned char*)const_cast<char*>(s.c_str()),s.size(),&psize);
//cout <<"bin�column"<<endl;
        p_driver_specific_data_size=(psize>0?psize-1:0);
    }
    else
    {                                             // not a binary column
        hk_string v=replace_all("\\",s,"\\\\");
	v=replace_all("\'",v,"\\'");//cerr <<"ERSETZE BACKSLASH='"<<v<<"'"<<endl;
	a=v.size();
	p_driver_specific_data= new char[a+1];
//Testing if works with postgresql
        strncpy(p_driver_specific_data,v.c_str(),a);
        p_driver_specific_data_size = a;          //cout <<"nichtbin�"<<endl;
    }

    p_original_new_data=new char[a+1];
    strcpy(p_original_new_data,s.c_str());
    p_original_new_data_size=s.size();

    return true;
}


bool hk_postgresqlcolumn::driver_specific_asbinary(const char* b)
{
    if (p_driver_specific_data!=NULL)
    {
        delete[] p_driver_specific_data;
        p_driver_specific_data=NULL;
    }

    size_t psize=0;
    p_driver_specific_data=(char*) escapeBytea((unsigned char*)const_cast<char*>(b),p_original_new_data_size,&psize);
    p_driver_specific_data_size=(psize>0?psize-1:0);

/*	  p_driver_specific_data= new char[2*p_original_new_data_size+1];
    strncpy(p_driver_specific_data,b,p_original_new_data_size);
    p_driver_specific_data_size = p_original_new_data_size;
*/
    return true;
}


const char* hk_postgresqlcolumn::driver_specific_transformed_asstring_at(unsigned long position)
{

    p_asstringbuffer=replace_all("'",driver_specific_asstring_at(position),"\\'");
    p_asstringbuffer=replace_all("\\",driver_specific_asstring_at(position),"\\\\");
    return p_asstringbuffer.c_str();

}
