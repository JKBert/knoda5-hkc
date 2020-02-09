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
//$Revision: 1.17 $
#include "hk_actionquery.h"
#include "hk_database.h"

#include <time.h>
#include <string.h>

class hk_actionqueryprivate
{
public:
hk_actionqueryprivate()
			{
			 p_administerdata=false;
			}
bool p_administerdata;
};

hk_actionquery::hk_actionquery(hk_database* db):hk_data()
{
#ifdef HK_DEBUG
    hkdebug("hk_actionquery::constructor");
#endif
   p_private=new hk_actionqueryprivate;
    p_sql=NULL;
    p_length=0;
    p_database=NULL;
    p_database=db;
}


hk_actionquery::~hk_actionquery(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_actionquery::destructor");
#endif
    p_database->ds_remove(this);
    if (p_private->p_administerdata)
    {
    p_length=0;
    delete[] p_sql;
     p_sql=NULL;

    }
delete p_private;
}


hk_data::enum_datasourcetypes hk_actionquery::type(void)
{
    return ds_actionquery;
}

void hk_actionquery::set_sql(const hk_string& s,bool convertdelimiter)
{
hk_string sq;
if (convertdelimiter) sq=sqlconvertdelimiter(s);
else sq=s;
char* buffer=new char[sq.size()+1];//+1 to store the string including the delimiting \0
memcpy(buffer,sq.c_str(),sq.size()+1);
set_sql(buffer,sq.size());
p_private->p_administerdata=true;
}



void hk_actionquery::set_sql(const char* s,unsigned long l)
{
#ifdef HK_DEBUG
    hkdebug("hk_actionquery::set_sql");
#endif

    if (p_private->p_administerdata)
    {
    p_length=0;
    delete[] p_sql;
     p_sql=NULL;

    }
    if (driver_specific_sql(s))
    {
    p_length=l;
     p_sql=s;
    }
 p_private->p_administerdata=false;
}

const char* hk_actionquery::sql(void)
{
return p_sql;
}

bool hk_actionquery::execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_actionquery::execute");
#endif
   if (p_print_sqlstatements) print_sql();
    return driver_specific_execute();
}


void   hk_actionquery::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_actionquery::before_source_vanishes");
#endif
//  p_database->ds_remove(this);
//  p_database=NULL;
}


bool    hk_actionquery::driver_specific_sql(const char*)
{
    return false;
}

unsigned long hk_actionquery::length(void)
{
 return p_length;
}

void hk_actionquery::print_sql(void)
{
  if (p_sql)
  {
   for (int i=0;i<10;++i)cerr<<"*";
   time_t timeval;
   time(&timeval);
   cerr<<" "<<ctime(&timeval);
   cerr <<"hk_actionquery SQL: '";
   for (unsigned int v=0;v<p_length;v++){cerr <<p_sql[v];} cerr <<"'"<<endl;
  }
}



hk_string hk_actionquery::sqlconvertdelimiter(const hk_string& s)
     {
     hk_string result=s;
      for(unsigned int i=0;i<result.size();++i)
    		{
		if (result[i]==defaulttextdelimiter[0]&&p_sqltextdelimiter.size()>0)
		    result[i]=p_sqltextdelimiter[0];
		else
		if (result[i]==defaultidentifierdelimiter[0]&&p_identifierdelimiter.size()>0) result[i]=p_identifierdelimiter[0];
		}
    return result;
     }

