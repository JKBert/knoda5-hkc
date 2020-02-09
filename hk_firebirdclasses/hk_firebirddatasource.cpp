// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_firebirdclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_firebirddatasource.h"
#include "hk_firebirddatabase.h"
#include "hk_firebirdconnection.h"
#include "hk_firebirdcolumn.h"
#include "hk_firebirdactionquery.h"
#include <hk_datetime.h>
#include <time.h>
#include <exception>
#include <new>
#include <string.h>

#define MAXLEN 1024


hk_firebirddatasource::hk_firebirddatasource(hk_firebirddatabase* d,hk_presentation* p):hk_storagedatasource(d,p)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_firebirddatasource::constructor");
#endif
    p_enabled=false;
    p_firebirddatabase=d;
    p_sqlhandle=NULL;
    p_transaction=NULL;
    p_stmt=NULL;
    p_currow=0;
    p_actionquery=new hk_firebirdactionquery(d);

}


hk_firebirddatasource::~hk_firebirddatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatasource::destructor");
#endif
    if (accessmode()!=standard &&is_enabled()) driver_specific_batch_disable();
}





bool hk_firebirddatasource::driver_specific_enable(void)
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

hk_column* hk_firebirddatasource::driver_specific_new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatasource::driver_specific_new_column");
#endif
    hk_column* col=new hk_firebirdcolumn(this,p_true,p_false);

    return col;
}











list<hk_column*>* hk_firebirddatasource::driver_specific_columns(void)
{
if (!p_columns) p_columns=new list<hk_column*>;
return p_columns;
}



bool hk_firebirddatasource::driver_specific_create_columns(void)
{
  clear_columnlist();
  p_columns=new list<hk_column*>;
  if (!p_sqlhandle)return false;
  short num_cols=p_sqlhandle->sqld;
  int z = 0;//counting the fieldnumbers
  for (short i=0;i<num_cols;++i)
  {
    hk_firebirdcolumn* col= new hk_firebirdcolumn(this,p_true,p_false);
    col->set_fieldnumber(z++);
    hk_column::enum_columntype coltype=hk_column::othercolumn;
    short dtype=p_sqlhandle->sqlvar[i].sqltype &~1;
    hk_string n(p_sqlhandle->sqlvar[i].aliasname,p_sqlhandle->sqlvar[i].aliasname_length);
    col->set_name(trim(n));
    short len=0;
    switch (dtype)
    {
      case SQL_TEXT:
      case SQL_VARYING:
      			{
      			  coltype=hk_column::textcolumn;
      			  if (database()->connection()->booleanemulation()&&p_sqlhandle->sqlvar[i].sqllen==1)
      			  coltype=hk_column::boolcolumn;
      			  break;
      			}
      case SQL_SHORT:
      			{
      			  coltype=hk_column::smallintegercolumn;
      			  break;
      			}
      case SQL_LONG:
      			{
      			  coltype=hk_column::integercolumn;
      			  break;
      			}
      case SQL_INT64:
      			{
      			  coltype=hk_column::integercolumn;
      			  break;
      			}
      case SQL_FLOAT:
      			{
      			  coltype=hk_column::smallfloatingcolumn;
      			  break;
      			}
      case SQL_DOUBLE:
      			{
      			  coltype=hk_column::floatingcolumn;
      			  break;
      			}
      case SQL_TIMESTAMP:
      			{
      			  coltype=hk_column::timestampcolumn;
      			  break;
      			}
      case SQL_TYPE_DATE:
      			{
      			  coltype=hk_column::datecolumn;
      			  break;
      			}
      case SQL_TYPE_TIME:
      			{
      			  coltype=hk_column::timecolumn;
      			  len=p_sqlhandle->sqlvar[i].sqllen;
      			  break;
      			}
      case SQL_BLOB:
      			{
      			  coltype=hk_column::binarycolumn;
      			  if (p_sqlhandle->sqlvar[i].sqlsubtype==1)
      			    coltype=hk_column::memocolumn;
      			  break;
      			}
      case SQL_ARRAY:

      default: ;

    }

    col->set_columntype(coltype);
    col->set_size(p_sqlhandle->sqlvar[i].sqllen);


    p_columns->insert(p_columns->end(),col);
  }
return true;
}


bool hk_firebirddatasource::driver_specific_insert_data(void)
{
    hk_string new_autoinc;

    struct_raw_data* datarow=new struct_raw_data[p_columns->size()];
    list<hk_column*>::iterator col_it;
    col_it=p_columns->begin();
    unsigned int spalte=0;
    while (spalte<p_columns->size())
    {
        const struct_raw_data* changed_data=(*col_it)->changed_data();
        if ((*col_it)->columntype()==hk_column::auto_inccolumn)
        {

            hk_datasource* tmpds=database()->new_resultquery();
            hk_firebirdcolumn* fc=dynamic_cast<hk_firebirdcolumn*>((*col_it));
            if (tmpds && fc)
            {
              hk_string s="SELECT GEN_ID(\""+fc->p_generatorname+"\",0) AS CURVAL FROM RDB$DATABASE";
              tmpds->set_sql(s);
              if (!tmpds->enable())
              show_warningmessage("GEN_ID query didn't work!");
              hk_column* c=tmpds->column_by_name("CURVAL");
              if (c)
              {
               new_autoinc=format_number(c->asinteger(),false,0);
              }

            }
            if (tmpds)
            {
             tmpds->disable();
             delete tmpds;
             tmpds=NULL;
            }
            //new_autoinc=format_number(sqlite3_last_insert_rowid(p_sqlitedatabase->dbhandler()),false,0);
            const int bsize=new_autoinc.size()+1;
            char* data=new char[bsize];
            strcpy(data,new_autoinc.c_str());
            datarow[spalte].data=data;
            datarow[spalte].length=strlen(data);
        }
        else
        {                                         //not a autoinc column
            datarow[spalte].length=changed_data->length;;
            char* data=NULL;
            if (changed_data->data)
            {
                data=new char[datarow[spalte].length];
                for (unsigned int tk=0;tk<datarow[spalte].length;tk++) data[tk]=changed_data->data[tk];
            }
            datarow[spalte].data=data;
        }
        spalte++;
        col_it++;
    }
    insert_data(datarow);
return true;

}




bool hk_firebirddatasource::driver_specific_batch_enable(void)
{
if (!datasource_open()) return false;
if (accessmode()==batchwrite)   return true;
p_counter=0;
if (datasource_fetch_next_row())
  set_maxrows(1);
else set_maxrows(0);
return true;
}


bool hk_firebirddatasource::driver_specific_batch_disable(void)
{
return datasource_close();

}


bool hk_firebirddatasource::driver_specific_batch_goto_next(void)
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


bool hk_firebirddatasource::datasource_open(void)
{
   if (p_print_sqlstatements)   print_sql();
   if (p_enabled)
   {
    return true;
   }
   p_currow=0;
   if (!p_firebirddatabase->firebirdconnection()->handle())
   {
     cerr <<"no firebirdconnectionhandle"<<endl;
     return false;
   }
   if (p_sqlhandle) free(p_sqlhandle);
   const int fnr=20;
   p_sqlhandle=(XSQLDA*)malloc(XSQLDA_LENGTH(fnr));
   p_sqlhandle->sqln=fnr;
   p_sqlhandle->version=1;
   isc_db_handle h=p_firebirddatabase->firebirdconnection()->handle();
   if (isc_dsql_allocate_statement(p_status,&h,&p_stmt))
   {
   set_error();
   free_handles();
   return false;
   }


   if (!p_transaction)
   {
     if (isc_start_transaction(p_status,&p_transaction,1,&h,0,NULL))
     {
   	set_error();
   	free_handles();
   	return false;
     }
   }

   if (isc_dsql_prepare(p_status,&p_transaction,&p_stmt,0,const_cast<char*>(p_sql.c_str()),SQL_DIALECT_V6,p_sqlhandle))
   {
   	set_error();
   	free_handles();
   	return false;
   }


   char infobuffer[20];
   static char stmt_info[]={isc_info_sql_stmt_type};
   //short L;
   //long statementtype;
   if (!isc_dsql_sql_info(p_status,&p_stmt,sizeof(stmt_info),stmt_info,sizeof(infobuffer),infobuffer))
   {
    //L=(short) isc_vax_integer((char*) infobuffer +1,2);
    //statementtype=isc_vax_integer((char*) infobuffer+3,1);
   }

   short num_cols=p_sqlhandle->sqld;
   if (p_sqlhandle->sqln <num_cols)
   {
    p_sqlhandle=(XSQLDA*) realloc(p_sqlhandle,XSQLDA_LENGTH(num_cols));
    p_sqlhandle->sqln=num_cols;
    p_sqlhandle->version=1;
    if (isc_dsql_describe(p_status,&p_stmt,SQL_DIALECT_V6,p_sqlhandle))
    {
   	set_error();
   	free_handles();
   	return false;
    }
    num_cols=p_sqlhandle->sqld;
   }

   XSQLVAR* var;
   short offset=0;
   short i;
   long buffer[MAXLEN];
   for (var=p_sqlhandle->sqlvar,i=0;i<num_cols;var++,i++)
   {
     short length,alignment;
     length=alignment=var->sqllen;
     short tp=var->sqltype & ~1;

     if (tp == SQL_TEXT)
     alignment = 1;
     else if (tp == SQL_VARYING)
        {
            length += sizeof (short) + 1;
            alignment = sizeof (short);
        }
        /*  RISC machines are finicky about word alignment
        **  So the output buffer values must be placed on
        **  word boundaries where appropriate
        */
#ifdef FB_ALIGN
        offset = FB_ALIGN(offset, alignment);
#endif        
        var->sqldata = (char *) buffer + offset;
        offset += length;
#ifdef FB_ALIGN        
        offset = FB_ALIGN(offset, sizeof (short));
#endif
        var->sqlind = (short*) ((char *) buffer + offset);
        offset += sizeof  (short);


   }
   if (isc_dsql_execute(p_status,&p_transaction,&p_stmt,SQL_DIALECT_V6,NULL))
   {
   	set_error();
   	datasource_close();
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

bool hk_firebirddatasource::datasource_fetch_next_row(void)
{
if (!p_sqlhandle) return false;

long fetchstatus=isc_dsql_fetch(p_status,&p_stmt,SQL_DIALECT_V6,p_sqlhandle);
if (fetchstatus!=0) return false;
  unsigned int num_cols=(unsigned int)p_sqlhandle->sqld;
 struct_raw_data* datarow=new struct_raw_data[num_cols];
  for (unsigned int i=0;i<num_cols;++i)
  {
    XSQLVAR var=p_sqlhandle->sqlvar[i];
              datarow[i].length=(var.sqllen);
              char* data=var.sqldata;
              char* dt=NULL;
              //leave NULL pointer if there is no data
              if (!((var.sqltype &1)&&(*var.sqlind<0)))
	       {
	         short dtype=var.sqltype &~1;
	         switch (dtype)
	         {
	           case SQL_TEXT:
	       				{
	       				dt=new char[datarow[i].length+1];
	       				for (unsigned int tk=0;tk<datarow[i].length;tk++) dt[tk]=data[tk];
	       				break;
	       				}
	           case SQL_VARYING:
	       				{
	       				typedef struct vary
	       				{
	       				 short vary_length;
	       				 char vary_string[1];
	       				} VARY;
	       				VARY* vary=(VARY*)var.sqldata;
	       				datarow[i].length=vary->vary_length;
	       				dt=new char[datarow[i].length+1];
	       				for (unsigned int tk=0;tk<datarow[i].length;tk++) dt[tk]=vary->vary_string[tk];
	       				break;
	       				}
	           case SQL_SHORT:
	           case SQL_LONG:
	           case SQL_INT64:
	       				{
	       				long n;
	       				if (dtype==SQL_LONG)n=*(long*)var.sqldata;
	       				else
	       				if (dtype==SQL_SHORT) n=(long)*(short*)var.sqldata;
	       				else n=(long)*(ISC_INT64*)var.sqldata;
	       				hk_string number=longint2string(n);
	       				dt=new char[number.size()+1];
	       				strcpy(dt,number.c_str());
	       				datarow[i].length=number.size();
	       				break;
	       				}
	           case SQL_FLOAT:
	           case SQL_DOUBLE:
	       				{
	       				double n;
	       				if (dtype==SQL_FLOAT)n=(double)*(float*)var.sqldata;
	       				else n=*(double*)var.sqldata;
	       				hk_string number=format_standard_number(n);
	       				dt=new char[number.size()+1];
	       				strcpy(dt,number.c_str());
	       				datarow[i].length=number.size();
	       				break;
	       				}
	           case SQL_TYPE_DATE:
	       				{

	       				struct tm times;
	       				isc_decode_sql_date((ISC_DATE*)var.sqldata,&times);
	       				hk_datetime d;
	       				d.set_dateformat("M.D.Y");
	       				d.set_date(times.tm_mday,times.tm_mon+1,times.tm_year+1900);
	       				hk_string date=d.date_asstring();
	       				dt=new char[date.size()+1];
	       				strcpy(dt,date.c_str());
	       				datarow[i].length=date.size();
	       				break;
	       				}
	           case SQL_TYPE_TIME:
	       				{

	       				struct tm times;
	       				isc_decode_sql_time((ISC_TIME*)var.sqldata,&times);
	       				hk_datetime d;
	       				d.set_timeformat("h:m:s");
	       				d.set_time(times.tm_hour,times.tm_min,times.tm_sec);
	       				hk_string time=d.time_asstring();
	       				dt=new char[time.size()+1];
	       				strcpy(dt,time.c_str());
	       				datarow[i].length=time.size();
	       				break;
	       				}
	           case SQL_TIMESTAMP:
	       				{

	       				struct tm times;
	       				isc_decode_timestamp((ISC_TIMESTAMP*)var.sqldata,&times);
	       				hk_datetime d;
	       				d.set_datetimeformat("M.D.Y h:m:s");
	       				d.set_datetime(times.tm_mday,times.tm_mon+1,times.tm_year+1900,times.tm_hour,times.tm_min,times.tm_sec);
	       				hk_string datetime=d.datetime_asstring();
	       				dt=new char[datetime.size()+1];
	       				strcpy(dt,datetime.c_str());
	       				datarow[i].length=datetime.size();
	       				break;
	       				}
	           case SQL_BLOB:
	       				{
	       				isc_blob_handle blob_handle=NULL;
	       				isc_db_handle h=p_firebirddatabase->firebirdconnection()->handle();
	       				ISC_QUAD blob_id=*(ISC_QUAD*) var.sqldata;
	       				if (!isc_open_blob(p_status,&h,&p_transaction,&blob_handle,&blob_id))
	       				{
	       				//first query the total size of the blob
	       				#ifdef HAVE_FIREBIRD2
					ISC_LONG max_size=0;
					#else
					SLONG max_size=0;
					#endif
	       				char blob_items[]={isc_info_blob_total_length};
	       				char res_buffer[20],*p,item;
	       				isc_blob_info(p_status,&blob_handle,sizeof(blob_items),blob_items,sizeof(res_buffer),res_buffer);
	       				if (p_status[0]==1 &&p_status[1])
	       				{
	       				 set_error();
	       				 isc_close_blob(p_status,&blob_handle);
	       				 break;
	       				}
	       				p=res_buffer;
	       				//for (p=res_buffer;*p!=isc_info_end;)
	       				{
	       				  item=*p++;
	       				  short length=(short) isc_vax_integer(p,2);
	       				  p+=2;
	       				  if (item==isc_info_blob_total_length)
	       				    max_size=isc_vax_integer(p,length);

	       				}
	       				datarow[i].length=max_size;
	       				dt=new char[max_size];
	       				char* pointer=dt;


	       				short blob_seg_len;
	       				char blob_segment[1024];
	       				long blob_stat=isc_get_segment(p_status,&blob_handle,(unsigned short *)&blob_seg_len,sizeof(blob_segment),blob_segment);

	       				while (blob_stat==0 || p_status[1] ==isc_segment)
	       				{
	       				for (unsigned int tk=0;tk<(unsigned int)blob_seg_len;tk++) pointer[tk]=blob_segment[tk];
	       				pointer+=blob_seg_len;

						blob_stat=isc_get_segment(p_status,&blob_handle,(unsigned short *)&blob_seg_len,sizeof(blob_segment),blob_segment);

	       				}
	       				if (p_status[1]==isc_segstr_eof)
	       				{
	       				 if (isc_close_blob(p_status,&blob_handle))
	       				     set_error();
	       				 break;
	       				}
	       				else
	       				   set_error();

	       				}
	       				break;
	       				}
	           default:		{
	           			datarow[i].length=0;
	           			datarow[i].data=NULL;
	           			}
	         }


	       }
	       else
		{ //NULL pointer, so make sure that length is 0
		datarow[i].length=0;
		}
              datarow[i].data=dt;
  }
 insert_data(datarow);

 return true;
}


bool hk_firebirddatasource::datasource_close(void)
{
   if (p_stmt)
   if( isc_dsql_free_statement(p_status,&p_stmt,DSQL_close))
   {
   	set_error();
   	return false;
   }
   if (p_transaction)
   {
    if (isc_commit_transaction(p_status,&p_transaction))
    {
      set_error();
      free_handles();
      return false;
    }
   }
free_handles();
 return true;
}

void hk_firebirddatasource::free_handles(void)
{
   if (p_sqlhandle) free(p_sqlhandle);
   p_sqlhandle=NULL;
   p_transaction=NULL;
   p_stmt=NULL;

}


void hk_firebirddatasource::set_error(void)
{
#ifdef HAVE_FIREBIRD2
  const ISC_STATUS* pvector=p_status;
#else
  long* pvector=p_status;
#endif
  char msg[512];
  hk_string errors;
  while (
#ifdef HAVE_FIREBIRD2
  fb_interpret(msg,sizeof(msg),&pvector)
#else
  isc_interprete(msg,&pvector)
#endif 
  )
  {
    errors+=msg;
    errors+="\n";
  }
  p_firebirddatabase->firebirdconnection()->servermessage(errors);
}





bool hk_firebirddatasource::insert_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirddatasource::insert_row()");
#endif
cerr <<"insert_row"<<endl;
bool result=true;
int count_changed_fields=0;
isc_tr_handle transaction=NULL;
    inform_visible_objects_before_insert_row();

    if (p_columns!=NULL)
    {   hk_string inssql;
        hk_string fields;
        list<hk_column*>::iterator it=p_columns->begin();
        cerr <<"vor fields"<<endl;
        while ( it!=p_columns->end() )
        {
            hk_column* xc=*it;
            ++it;
            if (xc!=NULL)
                if (xc->has_changed())
            {
                if (fields.size()==0) fields=" (";
                else
                {
                    fields+=" , ";
                }
                fields+=p_identifierdelimiter+xc->name()+p_identifierdelimiter;
                if (xc->columntype()==hk_column::binarycolumn||xc->columntype()==hk_column::memocolumn)
                   ++count_changed_fields;
            }
        }

        cerr <<"nach fields"<<endl;
        if (fields.size()>0) fields+=" )";
        cerr <<"vor create INSERT INTO"<<endl;

	it=p_columns->begin();
        while ( it!=p_columns->end() )
        {
            hk_column* xc=*it;
            ++it;
            if (xc->has_changed())
            {
                if (inssql.size()==0)
                {
                inssql="INSERT INTO ";
		inssql+=p_identifierdelimiter+name()+p_identifierdelimiter+fields+" VALUES (";
                }
                else
                {
                    inssql+=" , ";
                }

//write data
                if((xc->transformed_changed_data()->data!=NULL))
                {   cerr <<"column:"<<xc->name()<<" type="<<xc->columntype()<<" is Binary:"<<(xc->columntype()==hk_column::binarycolumn?"ja":"nein")<<endl;

		    if (xc->columntype()==hk_column::binarycolumn||xc->columntype()==hk_column::memocolumn)
		    {
			inssql+=" ? ";
		    }
		    else
		    {
			hk_string tf(xc->transformed_changed_data()->data,xc->transformed_changed_data()->length);
			inssql+=xc->get_delimiter()+tf+xc->get_delimiter();
		    }
                }
                else
                {
                    inssql+="NULL";
                }

            }
        }
                cerr <<"nach create INSERT INTO"<<endl;

        inssql+=")"+p_sql_delimiter;
        cerr<<"INSSQL="<<inssql<<endl;
        if (!blockserversignals())
        {

        XSQLDA* xsqlda=(XSQLDA*)malloc(XSQLDA_LENGTH(count_changed_fields));
        xsqlda->version=SQLDA_VERSION1;
        xsqlda->sqln=count_changed_fields;
        xsqlda->sqld=count_changed_fields;
        ISC_QUAD blob_id[count_changed_fields];
        isc_blob_handle blob_handle[count_changed_fields];
	for (int c=0;c<count_changed_fields;++c)
	{
	  xsqlda->sqlvar[c].sqldata=(char*)(&blob_id[c]);
	  xsqlda->sqlvar[c].sqltype=SQL_BLOB;
	  xsqlda->sqlvar[c].sqllen=sizeof(ISC_QUAD);
	  blob_handle[c]=NULL;
	}
    cerr <<"Anzahl variables: "<<count_changed_fields<<endl;
   isc_db_handle h=p_firebirddatabase->firebirdconnection()->handle();

   if (!transaction)
   {
     if (isc_start_transaction(p_status,&transaction,1,&h,0,NULL))
     {
   	set_error();
   	    free (xsqlda);

   	return false;
     }
   }



	it=p_columns->begin();
	int c=0;
        while ( it!=p_columns->end() )
        {
            hk_column* xc=*it;
            ++it;

            if (xc->has_changed()
                &&(xc->transformed_changed_data()->data!=NULL)
                &&(xc->columntype()==hk_column::binarycolumn||xc->columntype()==hk_column::memocolumn))
                {
                  if (isc_create_blob(p_status,&h,&transaction,&blob_handle[c],&blob_id[c]))
   			{
   			  set_error();
   			      free (xsqlda);

   			  return false;
   			}
   			int s=0;
   		// Now writing data in chunks
   		char* dp=xc->transformed_changed_data()->data;
   		unsigned long int dl=xc->transformed_changed_data()->length;
   		int segmentsize=dl>32767?32767:dl;
   		while (segmentsize>0)
   		{
		  if(isc_put_segment(p_status,&blob_handle[c],segmentsize,dp))
   			{ cerr <<"schreibe segment:"<<s++<<endl;
   			  set_error();
   			  free(xsqlda);
   			  return false;
   			}
   		  dp+=segmentsize;
   		  dl-=segmentsize;
   		  segmentsize=dl>32767?32767:dl;
   		}
   		  if (isc_close_blob(p_status,&blob_handle[c]))
   			{
   			  set_error();
   			      free (xsqlda);

   			  return false;
   			}
		 c++;
                }
        } // end while p_columns
	bool rslt=true;
        if (isc_dsql_execute_immediate(p_status,&h,&transaction,0,const_cast<char*>(inssql.c_str()),SQL_DIALECT_V6,xsqlda))
   			{
   			  set_error();
   			  rslt= false;
   			}

                if (rslt)
                {
#ifdef HK_DEBUG
                    hkdebug("Einfügen hat funktioniert");
#endif
   			if (transaction)
   			{
    			 if (isc_commit_transaction(p_status,&transaction))
    			 {
      			  set_error();
      			  free (xsqlda);
      			  return false;
    			 }
   			}
                    driver_specific_insert_data();
                    set_has_not_changed();
                    transaction_commit();
                    goto_last();
                    inform_visible_objects_row_add();
		    rslt=true;
                }
                else
                {
#ifdef HK_DEBUG
                    hkdebug("FEHLER keine Datenspeicherung");
#endif
                    transaction_rollback();
                    hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row could NOT be inserted!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
                    if (interaction==interactive) show_warningmessage(reason);
		    rslt=false;
                }
		execute_visible_object_after_insert();
          free (xsqlda);

        }
        if (!result) set_has_not_changed();

    }

    if (result)
    {
    setmode_normal();
    set_has_not_changed();
    }
    return result;
}



bool hk_firebirddatasource::update_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug( "hk_firebirddatasource::update_row()");
#endif
int count_changed_fields=0;
isc_tr_handle transaction=NULL;
    if(p_actual_row_where.size()==0)
    {
        if (interaction==interactive)show_warningmessage("Internal Error: update_row() p_actual_row_where is empty");
        return false ;
    }
    bool dep_bool=true;
    list<hk_datasource*>::iterator dep_it=p_dependinglist.begin();

    while (dep_it!=p_dependinglist.end())
    {
        if(!(*dep_it)->depending_on_datasource_updaterow_ok()) dep_bool=false;
        dep_it++;
    }

    if (!dep_bool)
    {                if (interaction==interactive)show_warningmessage(replace_all("%1",hk_translate("Row in datasource '%1' could not be changed due to depending datasource(s)"),name()));
        p_mode=mode_normal;
        set_has_not_changed();

        return false;
    }
    transaction_begin();
 inform_depending_ds_before_update_row();
hk_string upsql;

list<hk_column*>::iterator it=p_columns->begin();

   if (p_columns!=NULL)
    {

	bool hchanged=false;
       while ( it!=p_columns->end() )
        {
            hk_column* xc=*it;
            ++it;
			// Figure out the column type
            if (xc->has_changed())
            {   hchanged=true;
                if (upsql.size()==0)
                {
                    upsql="UPDATE "+p_identifierdelimiter+name()+p_identifierdelimiter+" SET ";
                }
                else
                {
                    upsql+=" , ";
                }
		upsql+=p_identifierdelimiter+xc->name()+p_identifierdelimiter+" = ";
                if (xc->transformed_changed_data()->data)
                {

		    if (xc->columntype()==hk_column::binarycolumn||xc->columntype()==hk_column::memocolumn)
		    {
			upsql+=" ? ";
			++count_changed_fields;

		    }
		    else
		    {
			hk_string tf(xc->transformed_changed_data()->data,xc->transformed_changed_data()->length);
			upsql+=xc->get_delimiter()+tf+xc->get_delimiter();
		    }


                }
                else
                {
                    upsql+="NULL";
                }

            }
        }
	if (!hchanged)
	 {
            if (interaction==interactive)show_warningmessage(hk_translate("Bug: datasource has changed, but no column has changed"));
	    return false;
	 }
    }

    upsql+=p_actual_row_where;



        cerr<<"UPSQL="<<upsql<<endl;
    set_has_not_changed();

	bool r=true;
         if (!blockserversignals())
        {

        XSQLDA* xsqlda=(XSQLDA*)malloc(XSQLDA_LENGTH(count_changed_fields));
        xsqlda->version=SQLDA_VERSION1;
        xsqlda->sqln=count_changed_fields;
        xsqlda->sqld=count_changed_fields;
        ISC_QUAD blob_id[count_changed_fields];
        isc_blob_handle blob_handle[count_changed_fields];
	for (int c=0;c<count_changed_fields;++c)
	{
	  xsqlda->sqlvar[c].sqldata=(char*)(&blob_id[c]);
	  xsqlda->sqlvar[c].sqltype=SQL_BLOB;
	  xsqlda->sqlvar[c].sqllen=sizeof(ISC_QUAD);
	  blob_handle[c]=NULL;
	}
    cerr <<"Anzahl variables: "<<count_changed_fields<<endl;
   isc_db_handle h=p_firebirddatabase->firebirdconnection()->handle();

   if (!transaction)
   {
     if (isc_start_transaction(p_status,&transaction,1,&h,0,NULL))
     {
   	set_error();
   	    free (xsqlda);

   	return false;
     }
   }



	it=p_columns->begin();
	int c=0;
        while ( it!=p_columns->end() )
        {
            hk_column* xc=*it;
            ++it;

            if (xc->has_changed()
                &&(xc->transformed_changed_data()->data!=NULL)
                &&(xc->columntype()==hk_column::binarycolumn||xc->columntype()==hk_column::memocolumn))
                {
                  if (isc_create_blob(p_status,&h,&transaction,&blob_handle[c],&blob_id[c]))
   			{
   			  set_error();
   			      free (xsqlda);

   			  return false;
   			}
   			int s=0;
   		// Now writing data in chunks
   		char* dp=xc->transformed_changed_data()->data;
   		unsigned long int dl=xc->transformed_changed_data()->length;
   		int segmentsize=dl>32767?32767:dl;
   		while (segmentsize>0)
   		{
		  if(isc_put_segment(p_status,&blob_handle[c],segmentsize,dp))
   			{ cerr <<"schreibe segment:"<<s++<<endl;
   			  set_error();
   			  free(xsqlda);
   			  return false;
   			}
   		  dp+=segmentsize;
   		  dl-=segmentsize;
   		  segmentsize=dl>32767?32767:dl;
   		}
   		  if (isc_close_blob(p_status,&blob_handle[c]))
   			{
   			  set_error();
   			      free (xsqlda);

   			  return false;
   			}
		 c++;
                }
        } // end while p_columns
	bool result=true;
        if (isc_dsql_execute_immediate(p_status,&h,&transaction,0,const_cast<char*>(upsql.c_str()),SQL_DIALECT_V6,xsqlda))
   			{
   			  set_error();
   			  result= false;
   			}

                if (result)
                {
#ifdef HK_DEBUG
                    hkdebug("Ändern hat funktioniert");
#endif
   			if (transaction)
   			{
    			 if (isc_commit_transaction(p_status,&transaction))
    			 {
      			  set_error();
      			  free (xsqlda);
      			  return false;
    			 }
   			}
                driver_specific_update_data();
                transaction_commit();
		result=true;
                }
                else
                {
#ifdef HK_DEBUG
                    hkdebug("FEHLER keine Datenspeicherung");
#endif
                    transaction_rollback();
                    hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row could NOT be inserted!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
                    if (interaction==interactive) show_warningmessage(reason);
		    result=false;
                }
          free (xsqlda);
        }
        if (r) set_has_not_changed();

inform_depending_ds_after_update_row();
return r;
}

