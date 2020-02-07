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
#include "hk_xbasedatasource.h"
#include "hk_xbasedatabase.h"
#include "hk_xbaseactionquery.h"
#include "hk_xbaseconnection.h"
#include "hk_xbasecolumn.h"
#include <exception>
#include <new>


hk_xbasedatasource::hk_xbasedatasource(hk_xbasedatabase* d,hk_presentation* p):hk_storagedatasource(d,p)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_xbasedatasource::constructor");
#endif
    p_enabled=false;
    p_xbasedatabase=d;
    p_sqlquery=NULL;
    p_currow=0;
    p_true="Y";
    p_false="N";
    p_actionquery=new hk_xbaseactionquery(d);
}


hk_xbasedatasource::~hk_xbasedatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasedatasource::destructor");
#endif
    if (accessmode()!=standard &&is_enabled()) driver_specific_batch_disable();
}





bool hk_xbasedatasource::driver_specific_enable(void)
{
     if (!datasource_open())
      {
       cerr<<"hk_xbasedatasource::driver_specific_enable !datasource_open() !!!"<<endl;
       return false;
      } 
      bool p_cancel=false;
     int max=progressinterval();
;

      
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

hk_column* hk_xbasedatasource::driver_specific_new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasedatasource::driver_specific_new_column");
#endif
    hk_column* col=new hk_xbasecolumn(this,p_true,p_false);

    return col;
}











list<hk_column*>* hk_xbasedatasource::driver_specific_columns(void)
{
if (p_columns==NULL&&type()==ds_table&&p_name.size()>0&&p_xbasedatabase->sqlhandle())
 {
  p_columns=new list<hk_column*>;
   xbXBase xc;
   xbDbf dbf( &xc );
   hk_url p_url=p_xbasedatabase->name();
   hk_string n= p_url.directory().size()>0? p_url.directory()+"/"+name()+
   			(p_url.extension().size()>0?"."+p_url.extension():"")
			                  : p_xbasedatabase->database_path()+"/"+name()+".dbf";
   if( dbf.OpenDatabase( n.c_str() ))
     {
       return p_columns;
     }
   for( int j = 0; j < dbf.FieldCount(); j++ ) 
   {
    int z=0;
    hk_xbasecolumn *col= new hk_xbasecolumn(this,p_true,p_false);
    col->set_fieldnumber(z++);
    col->set_name(dbf.GetFieldName(j));
    hk_column::enum_columntype t;
    int size=dbf.GetFieldLen(j);
      switch (dbf.GetFieldType( j ))
      {	 
	 case 'N': 	t=(size>6?hk_column::integercolumn:hk_column::smallintegercolumn);
	 					break;
	 case 'F':      t=(size>8?hk_column::floatingcolumn:hk_column::smallfloatingcolumn);
	 					break;
	 case 'D': 		t=hk_column::datecolumn;
	 					break;
	 case 'L':		t=hk_column::boolcolumn;
	 					break;
	 case 'C':		t=hk_column::textcolumn;
	 					break;
	 case 'M':		t=hk_column::memocolumn;
	 					//size=10;
	 					break;
	default:		t=hk_column::othercolumn;
       }
    col->set_columntype(t);
    col->set_size(size);
    p_columns->insert(p_columns->end(),col);
       
   }
  dbf.CloseDatabase();

 }
return p_columns;
}



bool hk_xbasedatasource::driver_specific_create_columns(void)
{
  clear_columnlist();
  p_columns=new list<hk_column*>;
   if (!p_sqlquery) return false;
   int z = 0;//counting the fieldnumbers
   int nf=p_sqlquery->getNumFields();
   for (int f=0;f<nf;++f)
   {
    hk_xbasecolumn *col= new hk_xbasecolumn(this,p_true,p_false);
    col->set_fieldnumber(z++);
    col->set_name(p_sqlquery->isSelect()->getFieldName(f));
    int size=p_sqlquery->isSelect()->getFieldLength(f);
    int r=p_sqlquery->isSelect()->getFieldType(f);
    hk_column::enum_columntype coltype=hk_column::othercolumn;
    switch(r)
    {
	case XB_CHAR_FLD: coltype=hk_column::textcolumn;break;
        case XB_LOGICAL_FLD:  coltype=hk_column::boolcolumn;break;
	case XB_NUMERIC_FLD:  coltype=hk_column::integercolumn;break;
	case XB_DATE_FLD:  coltype=hk_column::datecolumn;break;
	case XB_MEMO_FLD:  coltype=hk_column::memocolumn;break;
	case XB_FLOAT_FLD:  coltype=hk_column::floatingcolumn;break;
    }
    col->set_columntype(coltype);
    col->set_size(size);
    p_columns->insert(p_columns->end(),col);
    }
return true;
}


bool hk_xbasedatasource::driver_specific_insert_data(void)
{  
    struct_raw_data* datarow=new struct_raw_data[p_columns->size()];
    list<hk_column*>::iterator col_it;
    col_it=p_columns->begin();
    unsigned int spalte=0;
    while (spalte<p_columns->size())
    {
        const struct_raw_data* changed_data=(*col_it)->changed_data();
            datarow[spalte].length=changed_data->length;;
            char* data=NULL;
            if (changed_data->data)
            {
                data=new char[datarow[spalte].length];
                for (unsigned int tk=0;tk<datarow[spalte].length;tk++) data[tk]=changed_data->data[tk];
            }
            datarow[spalte].data=data;
        spalte++;
        col_it++;
    }
    insert_data(datarow);
    return true;

}




bool hk_xbasedatasource::driver_specific_batch_enable(void)
{
if (!datasource_open()) return false;
if (accessmode()==batchwrite)   return true; 
p_counter=0;
if (datasource_fetch_next_row())
  set_maxrows(1);
else set_maxrows(0);  
return true;
}


bool hk_xbasedatasource::driver_specific_batch_disable(void)
{
return datasource_close();

}


bool hk_xbasedatasource::driver_specific_batch_goto_next(void)
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


bool hk_xbasedatasource::datasource_open(void)
{
   if (p_print_sqlstatements)   print_sql();
   if (p_enabled) return true;
   if (p_sqlquery) delete p_sqlquery;
   if  (!p_xbasedatabase->connection()->is_connected()) return false;
   p_currow=0;
   bool parsed;
   if (!p_xbasedatabase->sqlhandle()) return false;
   p_sqlquery=p_xbasedatabase->sqlhandle()->openQuery(p_sql.c_str(),parsed);	
   if (!p_sqlquery)
   {
     p_xbasedatabase->connection()->servermessage(p_xbasedatabase->sqlhandle()->lastError());
     return false;
   }
   if (!p_sqlquery->isSelect())
   {
     delete p_sqlquery;
     p_sqlquery=NULL;
     show_warningmessage(hk_translate("Query is no valid SELECT query!"));
     return false;
   }
   if (!p_sqlquery->execute(0,0))
   {
     show_warningmessage(hk_translate("Error! Query could not be executed!"));
     delete p_sqlquery;
     p_sqlquery=NULL;
     return false;
   }
      
   if (accessmode()==batchwrite)
        {
            clear_columnlist();
            driver_specific_create_columns();
            return true;
        }
  
  driver_specific_create_columns();   
 return true;
}





bool hk_xbasedatasource::datasource_fetch_next_row(void)
{
 if (p_currow>=p_sqlquery->getNumRows())
 {
  return false;
 } 
 
 int nf=p_sqlquery->getNumFields();
 struct_raw_data* datarow=new struct_raw_data[nf];
   for (int f=0;f<nf;++f)
   {
   
	XBSQLValue v = p_sqlquery->isSelect()->getField (p_currow, f) ;
               //create new data row
	      const char* coltext=v.getText();
	      hk_string n;
	      if (coltext) n=smallstringconversion(coltext,p_database->databasecharset(),"");
	      cerr <<"n:"<<n<<endl;
              datarow[f].length=(coltext?strlen(n.c_str())+1:0); 
              char* dt=NULL;
              if (coltext)
	       {
	       dt=new char[datarow[f].length];
               strcpy(dt,n.c_str());
	       }
		
              datarow[f].data=dt;

   }
//store data
insert_data(datarow);
++p_currow;
 return true;
}


bool hk_xbasedatasource::datasource_close(void)
{
 delete p_sqlquery;
 p_sqlquery=NULL;
 return true;
}




