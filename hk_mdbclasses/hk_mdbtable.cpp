// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mdbclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.8 $
#include "hk_mdbtable.h"
#include "hk_mdbdatabase.h"
#include "hk_actionquery.h"
#include "hk_mdbdatabase.h"
#include "hk_mdbconnection.h"
#include "hk_mdbcolumn.h"
#include <exception>
#include <new>


hk_mdbtable::hk_mdbtable(hk_mdbdatabase* d,hk_presentation* p):hk_mdbdatasource(d,p)

{
    p_readonly=true;
    p_enabled=false;
    p_mdbdatabase=d;
    p_table=NULL;
    p_bound_values=NULL;
    p_bound_lens=NULL;

}


hk_datasource::enum_datasourcetypes hk_mdbtable::type() const
{
    return ds_table;
}


bool hk_mdbtable::driver_specific_enable(void)
{
     bool p_cancel=false;
     int max=progressinterval();
     if (!datasource_open())
      {
       return false;
      }


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
         free_handles();
         return true;
       }

      //dump_data();
      datasource_close();

return true;
}

hk_column* hk_mdbtable::driver_specific_new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mdbtable::driver_specific_new_column");
#endif
    hk_column* col=new hk_mdbcolumn(this,p_true,p_false);

    return col;
}











list<hk_column*>* hk_mdbtable::driver_specific_columns(void)
{
    if (p_columns==NULL&&type()==ds_table&&p_name.size()>0&&p_mdbdatabase->dbhandle())
    {
      p_columns=new list<hk_column*>;
      MdbTableDef* table =  mdb_read_table_by_name(p_mdbdatabase->dbhandle(),(gchar*) const_cast<char*>(p_name.c_str()),MDB_TABLE);
      if (!table) return p_columns;
      mdb_read_columns(table);

      for (unsigned int i=0; i<table->num_cols;++i)
      {
          MdbColumn* mc=(MdbColumn*)g_ptr_array_index(table->columns,i);

	  hk_mdbcolumn *col= new hk_mdbcolumn(this,p_true,p_false);
          col->set_fieldnumber(i);
	  col->set_name(mc->name);
	  set_columnspecifica(col,mc);
          p_columns->push_back(col);


      }

    }

return p_columns;
}


void hk_mdbtable::set_columnspecifica(hk_mdbcolumn*col,MdbColumn* c)
{
  if (!c||!col) return;
	  switch(c->col_type)
	  {
	    case MDB_BOOL:
	                col->set_columntype(hk_column::boolcolumn);
			break;
	case MDB_BYTE:
	                col->set_columntype(hk_column::smallintegercolumn);
			break;
	case MDB_INT:
	                col->set_columntype(hk_column::smallintegercolumn);
			break;
	case MDB_LONGINT:
	                col->set_columntype(hk_column::integercolumn);
			break;
	case MDB_MONEY:
	                col->set_columntype(hk_column::floatingcolumn);
			break;
	case MDB_FLOAT:
	                col->set_columntype(hk_column::smallfloatingcolumn);
			break;
	case MDB_DOUBLE:
	                col->set_columntype(hk_column::floatingcolumn);
			break;
	case MDB_SDATETIME:
	                col->set_columntype(hk_column::datetimecolumn);
			break;
	case MDB_TEXT:
	                col->set_columntype(hk_column::textcolumn);
			break;
	case MDB_OLE:
	                col->set_columntype(hk_column::binarycolumn);
			break;
	case MDB_MEMO:
	                col->set_columntype(hk_column::memocolumn);
			break;
	case MDB_REPID:
	                col->set_columntype(hk_column::auto_inccolumn);
			break;
	case MDB_NUMERIC:
	                col->set_columntype(hk_column::floatingcolumn);
			break;
	    default:
	      col->set_columntype(hk_column::textcolumn);
	  }
      col->set_size(c->col_size==0?255:c->col_size);
}

bool hk_mdbtable::driver_specific_create_columns(void)
{
  clear_columnlist();
  p_columns=new list<hk_column*>;
     int z = 0;//counting the fieldnumbers
  if (p_table->num_cols==0) return false;
  for (unsigned int h=0;h<p_table->num_cols;++h)
      {
          MdbColumn* c=(MdbColumn*)g_ptr_array_index(p_table->columns,h);
          hk_string nm=(c->name==0?"":c->name);
          hk_mdbcolumn *col= new hk_mdbcolumn(this,p_true,p_false);
          col->set_fieldnumber(z++);
	  col->set_name(nm);
	  set_columnspecifica(col,c);

      p_columns->insert(p_columns->end(),col);
      }

return true;
}


bool hk_mdbtable::driver_specific_insert_data(void)
{
return false;

}




bool hk_mdbtable::driver_specific_batch_enable(void)
{
if (!datasource_open()) return false;
if (accessmode()==batchwrite)   return true;
p_counter=0;
if (datasource_fetch_next_row())
  set_maxrows(1);
else set_maxrows(0);
return true;
}


bool hk_mdbtable::driver_specific_batch_disable(void)
{
return datasource_close();

}


bool hk_mdbtable::driver_specific_batch_goto_next(void)
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


bool hk_mdbtable::datasource_open(void)
{
   if (p_print_sqlstatements)   print_sql();
   if (p_enabled) return true;
   if (accessmode()==batchwrite)
        {
            clear_columnlist();
            driver_specific_create_columns();
            return true;
        }

      p_table=mdb_read_table_by_name(p_mdbdatabase->dbhandle(), (gchar*)const_cast<char*>(name().c_str()), MDB_TABLE);
     if  (!p_mdbdatabase->connection()->is_connected()) return false;

       if (!p_table)
       {
        show_warningmessage(hk_translate("MDBTools - Table could not be opened!"));
	return false;
       }

      mdb_read_columns(p_table);
      mdb_rewind_table(p_table);
      p_bound_values = (char **) g_malloc(p_table->num_cols * sizeof(char *));
      p_bound_lens = (int *) g_malloc(p_table->num_cols * sizeof(int));
	for (unsigned int j=0;j<p_table->num_cols;j++) {
		p_bound_values[j] = (char *) g_malloc0(MDB_BIND_SIZE);
		mdb_bind_column(p_table, j+1, p_bound_values[j], &p_bound_lens[j]);
	}



      driver_specific_create_columns();
 return true;
}

bool hk_mdbtable::datasource_fetch_next_row(void)
{
 if (!p_table) return false; //datasource was not (successfully) opened
 if (!mdb_fetch_row(p_table)) return false;
 unsigned int colnums=p_table->num_cols;
 struct_raw_data* datarow=new struct_raw_data[colnums];
 for (unsigned int j=0;j<colnums;++j)
 {
            //create new data row
              hk_string result;
	      const char* coltext=p_bound_values[j];
	      if (coltext) 
	       {
	        result=smallstringconversion(replace_all("\x0D\x0A",coltext,"\n"),p_database->databasecharset(),"");
	       }
              datarow[j].length=(coltext?result.size()+1:0);
              char* dt=NULL;
              if (coltext)
	       {
	       dt=new char[datarow[j].length];
               strcpy(dt,result.c_str());
	       }

              datarow[j].data=dt;
 }
//store data
	insert_data(datarow);

 return true;
}


bool hk_mdbtable::datasource_close(void)
{
free_handles();
 return true;
}


void hk_mdbtable::free_handles(void)
{
  if (p_bound_values)g_free(p_bound_values);
  if (p_bound_lens)g_free(p_bound_lens);
  if (p_table)mdb_free_tabledef(p_table);
  p_table=NULL;
  p_bound_values=NULL;
  p_bound_lens=NULL;

}

