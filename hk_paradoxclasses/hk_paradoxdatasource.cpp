// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_paradoxclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_paradoxdatasource.h"
#include "hk_paradoxdatabase.h"
#include "hk_paradoxconnection.h"
#include "hk_paradoxcolumn.h"
#include "hk_string.h"
#include <exception>
#include <new>

static hk_string errormsg;

void errorhandler(pxdoc_t *, int , const char *str, void *)
{
  cerr << "PXLib:"<<str<<endl;
  errormsg=str;
}

hk_string lasterror()
{
  return errormsg;
}

hk_paradoxdatasource::hk_paradoxdatasource(hk_paradoxdatabase* d,hk_presentation* p):hk_storagedatasource(d,p)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_paradoxdatasource::constructor");
#endif
    p_enabled=false;
    p_paradoxdatabase=d;
    p_paradoxhandle=NULL;//PX_new2(errorhandler,NULL,NULL,NULL);
    //p_paradoxhandle=PX_new();

    p_paradoxheader=NULL;
    p_true="1";
    p_false="0";
}


hk_paradoxdatasource::~hk_paradoxdatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxdatasource::destructor");
#endif
    if (accessmode()!=standard &&is_enabled()) driver_specific_batch_disable();
if ( p_paradoxhandle )
{
        PX_close(p_paradoxhandle);
       	PX_delete(p_paradoxhandle);
}

}





bool hk_paradoxdatasource::driver_specific_enable(void)
{
errormsg="";
     if (!datasource_open())
      {
       return false;
      }


        try
       {
        while (datasource_fetch_next_row())
	 {
	 }
       }
       catch (std::bad_alloc errormessage)
       {
         return true;
       }

      //dump_data();
      datasource_close();

return true;
}

hk_column* hk_paradoxdatasource::driver_specific_new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxdatasource::driver_specific_new_column");
#endif
    hk_column* col=new hk_paradoxcolumn(this,p_true,p_false);

    return col;
}











list<hk_column*>* hk_paradoxdatasource::driver_specific_columns(void)
{

return p_columns;
}



bool hk_paradoxdatasource::driver_specific_create_columns(void)
{
  clear_columnlist();
  p_columns=new list<hk_column*>;

return true;
}




bool hk_paradoxdatasource::driver_specific_batch_enable(void)
{
if (!datasource_open()) return false;
if (accessmode()==batchwrite)   return true;
p_counter=0;
if (datasource_fetch_next_row())
  set_maxrows(1);
else set_maxrows(0);
return true;
}


bool hk_paradoxdatasource::driver_specific_batch_disable(void)
{
return datasource_close();

}


bool hk_paradoxdatasource::driver_specific_batch_goto_next(void)
{
try
{
if (datasource_fetch_next_row())
  {
    set_maxrows(max_rows()+1);
    ++p_counter;
    return true;
  }
 else
    return false;
}
catch (std::bad_alloc errormessage)
 {
  datasource_close();
  return false;
 }
}


bool hk_paradoxdatasource::datasource_open(void)
{
 return true;
}

bool hk_paradoxdatasource::datasource_fetch_next_row(void)
{

 return true;
}


bool hk_paradoxdatasource::datasource_close(void)
{
 return true;
}




