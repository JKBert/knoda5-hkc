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
#include "hk_dbasedatasource.h"
#include "hk_dbasedatabase.h"
#include "hk_dbaseconnection.h"
#include "hk_dbasecolumn.h"
#include <exception>
#include <new>


hk_dbasedatasource::hk_dbasedatasource(hk_dbasedatabase* d,hk_presentation* p):hk_storagedatasource(d,p)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_dbasedatasource::constructor");
#endif
    p_enabled=false;
    p_dbasedatabase=d;
    p_dbfdatasource=new xbDbf(p_dbasedatabase->dbhandle());
    p_true="T";
    p_false="F";
}


hk_dbasedatasource::~hk_dbasedatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_dbasedatasource::destructor");
#endif
    if (accessmode()!=standard &&is_enabled()) driver_specific_batch_disable();
    delete p_dbfdatasource;
}





bool hk_dbasedatasource::driver_specific_enable(void)
{
     if (!datasource_open())
      {
       return false;
      } 
 
     bool p_cancel=false;
     int max=progressinterval();
      
        try
       {
        int i=1;
        while (datasource_fetch_next_row() && !p_cancel)
	 {
	 if (progressdialog() && (i %15000 ==0))
	 {
          p_cancel=progressdialog()(i,max,hk_translate("Executing query ..."));
	 }
	 ++i;
	if (i>max-30000)max+=10000;
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

hk_column* hk_dbasedatasource::driver_specific_new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbasedatasource::driver_specific_new_column");
#endif
    hk_column* col=new hk_dbasecolumn(this,p_true,p_false);

    return col;
}











list<hk_column*>* hk_dbasedatasource::driver_specific_columns(void)
{

return p_columns;
}



bool hk_dbasedatasource::driver_specific_create_columns(void)
{
  clear_columnlist();
  p_columns=new list<hk_column*>;

return true;
}


bool hk_dbasedatasource::driver_specific_insert_data(void)
{
return false;

}




bool hk_dbasedatasource::driver_specific_batch_enable(void)
{
if (!datasource_open()) return false;
if (accessmode()==batchwrite)   return true; 
p_counter=0;
if (datasource_fetch_next_row())
  set_maxrows(1);
else set_maxrows(0);  
return true;
}


bool hk_dbasedatasource::driver_specific_batch_disable(void)
{
return datasource_close();

}


bool hk_dbasedatasource::driver_specific_batch_goto_next(void)
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


bool hk_dbasedatasource::datasource_open(void)
{
 return true;
}

bool hk_dbasedatasource::datasource_fetch_next_row(void)
{

 return true;
}


bool hk_dbasedatasource::datasource_close(void)
{
 return true;
}




