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
#include "hk_odbcdatasource.h"
#include "hk_odbcdatabase.h"
#include "hk_odbcconnection.h"
#include "hk_odbccolumn.h"
#include "hk_odbcactionquery.h"
#include <sqltypes.h>
#include <sqlext.h>
#include <exception>
#include <new>

hk_odbcdatasource::hk_odbcdatasource(hk_odbcdatabase* d,hk_presentation* p):hk_storagedatasource(d,p)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcdatasource::constructor");
#endif
    p_odbcdatabase=d;
    p_columns=NULL;
    p_length=NULL;
    p_debug_counter=0;
    p_enabled=false;
    p_actionquery=new hk_odbcactionquery(d);
    
    if (p_odbcdatabase->connection()->backendtype()==hk_odbcconnection::mssql)
    {
    p_true="1";
    p_false="0";
    }
    else
    {
    p_true="TRUE";
    p_false="FALSE";
    }
    SQLCHAR delimiter[50]="";
    SQLSMALLINT size;
    SQLGetInfo(p_odbcdatabase->connection()->connectionhandle(),SQL_IDENTIFIER_QUOTE_CHAR,delimiter,sizeof(delimiter),&size);
    p_identifierdelimiter=(char*) &delimiter;
    cerr <<"ODBC IDENTIFIERDELIMITER='"<<delimiter<<"'"<<endl;

    SQLRETURN ret= SQLAllocHandle(SQL_HANDLE_STMT,p_odbcdatabase->connection()->connectionhandle(),&p_result);cerr <<"SQL: "<<p_sql<<endl;
    ret=SQLGetTypeInfo(p_result,SQL_CHAR);cerr <<"ret="<<ret<<endl;
    if ((ret==SQL_SUCCESS ||ret==SQL_SUCCESS_WITH_INFO))
    
    {
    if ((ret=SQLFetch(p_result))==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO)
                {
            SQLLEN indicator=0;
            SQLGetData(p_result,5/*LITERAL_SUFFIX*/,SQL_C_CHAR, delimiter,sizeof(delimiter),&indicator);
	    p_sqltextdelimiter=(char*) &delimiter;
	    cerr <<"TEXTDELIMITER='"<<delimiter<<"'"<<endl;
                }
    SQLFreeHandle(SQL_HANDLE_STMT,p_result);
    }
    else
      {
      
	cerr <<"WARNING:hk_odbcdatasource::hk_odbcdatasource SQLGetTypeInfo ERROR"<<endl;
      }

}


hk_odbcdatasource::~hk_odbcdatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcdatasource::destructor");
#endif
    if (accessmode()!=standard &&is_enabled()) driver_specific_batch_disable();
    clear_result();
// if (p_SQL_Connection) odbc_close(p_SQL_Connection);
// p_SQL_Connection=NULL;
}


list<hk_column*>* hk_odbcdatasource::driver_specific_columns(void)
{

    if (p_columns==NULL&&type()==ds_table&&p_name.size()>0&&p_odbcdatabase->connection()->connectionhandle())
    {
        SQLRETURN ret= SQLAllocHandle(SQL_HANDLE_STMT,p_odbcdatabase->connection()->connectionhandle(),&p_result);cerr <<"SQL: "<<p_sql<<endl;
        if (ret!=SQL_SUCCESS)
        { cerr <<"hk_odbcdatasource::driver_specific_columns : could not allocate handle"<<endl;
            clear_result();
            return p_columns;
        }
        hk_string sql_string= "SELECT * FROM "+p_identifierdelimiter+p_name+p_identifierdelimiter+" WHERE 1=0";
        ret= SQLExecDirect(p_result ,(SQLCHAR*)const_cast<char*>((sql_string.c_str())) ,SQL_NTS);
        if (ret!=SQL_SUCCESS)
        {cerr <<"hk_odbcdatasource::driver_specific_columns : could not select fields with 'WHERE 1=0'"<<endl;
            clear_result();
            return p_columns;

        }

        if (!driver_specific_create_columns()) cerr <<"hk_odbcdatasource::driver_specific_columns create_columns failed"<<endl;
        clear_result();
    }
    return p_columns;
}


bool hk_odbcdatasource::driver_specific_create_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("create_columns");
#endif
//   unsigned int num_fields;
    if (p_result!=NULL)
    {
        clear_columnlist();
        p_columns=new list<hk_column*>;

        SQLSMALLINT num_fields;
        SQLRETURN result=SQLNumResultCols(p_result,&num_fields);
        if (result!=SQL_SUCCESS)
        {
            clear_result();
            cerr <<"Fehler1"<<endl;
            return false;

        }
        int z = 0;                                //counting the fieldnumbers
        SQLCHAR txtbuffer[101]="";
        SQLLEN intbuffer;
        for (int c=0;c<num_fields;c++)
        {
            hk_odbccolumn *col= new hk_odbccolumn(this,p_true,p_false);
#ifdef HK_DEBUG
            {
                col->hkclassname("odbccolumn "+longint2string(p_debug_counter++));
                hkdebug("New Column");
            }
#endif
            SQLColAttribute(p_result,c+1,SQL_DESC_LABEL,txtbuffer,sizeof(txtbuffer),0,0);
            if (p_odbcdatabase->connection()->use_unicodenames())
	    	col->set_name(u2l((const char*)txtbuffer));
	    else
	    	col->set_name((const char*)txtbuffer);
	    //cerr <<"new_columnname= "<<col->name()<<endl;
            col->set_columntype(hk_column::textcolumn);
            col->set_fieldnumber(z++);
            result=SQLColAttribute(p_result,c+1,SQL_DESC_LENGTH, 0,SQL_IS_INTEGER,0,&intbuffer);
            col->set_size(intbuffer);

            result=SQLColAttribute(p_result,c+1,SQL_DESC_CONCISE_TYPE, 0,SQL_IS_INTEGER,0,&intbuffer);
            if (p_datetimeformat.size()>0) col->set_datetimeformat(p_datetimeformat.c_str());
            if (p_dateformat.size()>0) col->set_dateformat(p_dateformat.c_str());
            if (p_timeformat.size()>0) col->set_timeformat(p_timeformat.c_str());

            switch(intbuffer)
            {
                case SQL_DECIMAL:
                case SQL_NUMERIC:
                case SQL_FLOAT :
                case SQL_REAL:
                    col->set_columntype(hk_column::smallfloatingcolumn) ;
                    break;
                case SQL_DOUBLE:
                    col->set_columntype(hk_column::floatingcolumn) ;
                    break;
                case SQL_INTEGER:
                case SQL_BIGINT:            col->set_columntype(hk_column::integercolumn);
                break;
                case SQL_SMALLINT:
                case SQL_TINYINT:               col->set_columntype(hk_column::smallintegercolumn);
                break;

                case SQL_LONGVARCHAR:    col->set_columntype(hk_column::memocolumn);
		break;
		case SQL_CHAR:
                case SQL_VARCHAR:       if (col->size()>255)
		 			col->set_columntype(hk_column::memocolumn);
					else
		 			col->set_columntype(hk_column::textcolumn);
                break;
                case SQL_TYPE_DATE:     col->set_columntype(hk_column::datecolumn);
                break;
                case SQL_TYPE_TIME:     col->set_columntype(hk_column::timecolumn);
                break;
                case SQL_BINARY:
                case SQL_VARBINARY:
                case SQL_LONGVARBINARY:
                    col->set_columntype(hk_column::binarycolumn);
                    break;
		case SQL_BIT:
                    col->set_columntype(hk_column::boolcolumn);
                    break;

                default:        col->set_columntype(hk_column::othercolumn);

            }                                     //endswitch

            result=SQLColAttribute(p_result,c+1,SQL_DESC_AUTO_UNIQUE_VALUE, 0,SQL_IS_INTEGER,0,&intbuffer);
            if (intbuffer==SQL_TRUE)
	       {
	         col->set_columntype(hk_column::auto_inccolumn);
		p_primary_key_used=true;
		col->set_primary(true);

	        }	 

            result=SQLColAttribute(p_result,c+1,SQL_DESC_NULLABLE, 0,SQL_IS_INTEGER,0,&intbuffer);
            if (intbuffer==SQL_NO_NULLS)
            {
                col->set_notnull(true);
//			   if ( col->set_notnull(true))cerr <<"notnull ERFOLGREICH gesetzt "<<col->name()<<endl;
//			   else cerr <<"FEHLER beim SETZEN NOTNULL"<<endl;
            }

            col->set_definitionmode(false);
            p_columns->insert(p_columns->end(),col);

        };                                        //end for

        return true;
    }
    return false;

}


bool hk_odbcdatasource::driver_specific_enable(void)
{

#ifdef HK_DEBUG
    hkdebug("driver_specific_enable");
#endif
   if (p_print_sqlstatements)print_sql();
    if (!p_enabled)
    {
        if  (p_odbcdatabase==NULL)  return false;
        if (accessmode()==batchwrite) return true;
        if  (!p_odbcdatabase->connection()->is_connected()) return false;
        SQLRETURN ret= SQLAllocHandle(SQL_HANDLE_STMT,p_odbcdatabase->connection()->connectionhandle(),&p_result);cerr <<"SQL: "<<p_sql<<endl;
        if (ret!=SQL_SUCCESS)
        {
            clear_result();
            return false;
        }

    hk_string separator=" \t\n";
    hk_string::size_type startpos=hk_string::npos;
    startpos=p_sql.find_first_not_of(separator);
   hk_string value;
   hk_string::size_type endpos=hk_string::npos;
   if (startpos!=hk_string::npos)
   {
   endpos=p_sql.find_first_of(separator,startpos+1);
    value=p_sql.substr(startpos,endpos-startpos);
   }
//cerr <<"startpos:"<<startpos<<" endpos:"<<endpos<<endl;
  hk_string value2;
   if (endpos!=hk_string::npos)
  { startpos=p_sql.find_first_not_of(separator,endpos+1);
   if (startpos!=hk_string::npos)
   {
    endpos=p_sql.find_first_of(separator,startpos+1);
    value2=p_sql.substr(startpos,endpos-startpos);
    //cerr <<"value2 startpos:"<<startpos<<" endpos:"<<endpos<<endl;
   }
}
//cerr <<"v1="<<value<<" v2="<<value2<<endl;

	if (value=="ODBCFIELDS")
	  ret=SQLGetTypeInfo(p_result,SQL_ALL_TYPES);
	else
	if (value=="PRIMARYKEYS")
	ret= SQLPrimaryKeys(p_result,0,0,0,0 ,(SQLCHAR*)const_cast<char*>((value2.c_str())) ,SQL_NTS);
	else
	if (value=="STATISTICS")
	ret= SQLStatistics( p_result, 0, 0, 0, 0, (SQLCHAR*)const_cast<char*>((value2.c_str())), SQL_NTS, 0, 0 );
	else
	if (value=="COLUMNS")
	ret= SQLColumns( p_result, 0, 0, 0, 0,(SQLCHAR*) const_cast<char*>(value2.c_str()), SQL_NTS, 0, 0 );
	else
	ret= SQLExecDirect(p_result ,(SQLCHAR*)const_cast<char*>((p_sql.c_str())) ,SQL_NTS);
        if (ret!=SQL_SUCCESS)
        {
            clear_result();
            return false;

        }
     bool p_cancel=false;
     int max=progressinterval();
     int i=1;
        if (ret==SQL_SUCCESS)
        {
            SQLSMALLINT num_fields;
            ret=SQLNumResultCols(p_result,&num_fields);
            if (ret!=SQL_SUCCESS)
            {
                clear_result();
                return false;
            }
            if (!driver_specific_create_columns()) return false;
// now add data
            try
            {
                while (((ret=SQLFetch(p_result))==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO) && !p_cancel)
                {
		 if (progressdialog() && (i %15000 ==0))
		 {
	          p_cancel=progressdialog()(i,max,hk_translate("Executing query ..."));
		 }
		 ++i;
		 if (i>max-30000)max+=10000;  
                 add_data(num_fields);
                }                                 //end fetchdata
            }                                     //end try
            catch (std::bad_alloc errormessage)
            {
                memory_error();
                clear_result();
                return true;
            }
            clear_result();
            return true;
        }
    }
    return false;
}


void hk_odbcdatasource::add_data(SQLSMALLINT num_fields)
{
//neue Datenzeile erzeugen
    struct_raw_data* datarow=new struct_raw_data[num_fields];
    unsigned int spalte=0;
   long int bufferlength=100000;
   char* buffer= new char[bufferlength];
    while (spalte<(unsigned int)num_fields)
    {
        buffer[0]='\0';
	hk_column* col=NULL;
	list<hk_column*>::iterator it=p_columns->begin();
   	while (col==NULL&&it!=p_columns->end())
	  {
		if ((*it)->fieldnumber()==spalte) col=(*it);
		++it;
	  }
     SQLLEN indicator=0;
     SQLRETURN result=SQLGetData(p_result,spalte+1,SQL_C_CHAR, (SQLPOINTER) buffer,bufferlength,&indicator);
            if (!(result==SQL_SUCCESS||result==SQL_SUCCESS_WITH_INFO) )
	      {
               set_servermessage();
	       break;
	      }
	long int length=strlen(buffer)+1;
	char *b=new char[length];
	strcpy(b,buffer);
	      
	if (col)
	{
	 if (col->columntype()==hk_column::boolcolumn)
		{
		hk_string value=b;
		delete[] b;
		int s=(value!="0"?4:5);
		b= new char[6];
		strcpy(b,((s==4)?"TRUE":"FALSE"));
		length=s;
		}
	      }
	  if (indicator==SQL_NULL_DATA)
	  {
            datarow[spalte].length=0;
            datarow[spalte].data=NULL;
	  }
	  else
	  {    
            datarow[spalte].length=length;
            datarow[spalte].data=b;
	  }  

        spalte++;
    }
//Daten speichern
    insert_data(datarow);
    delete[] buffer;
}


hk_column* hk_odbcdatasource::driver_specific_new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("driver_specific_new_column");
#endif
    hk_column* col=new hk_odbccolumn(this,p_true,p_false);

    return col;
}


bool hk_odbcdatasource::driver_specific_insert_data(void)
{
    struct_raw_data* datarow=new struct_raw_data[p_columns->size()];
    list<hk_column*>::iterator col_it;
    col_it=p_columns->begin();
    unsigned int spalte=0;
    while (spalte<p_columns->size())
    {
        const struct_raw_data* changed_data=(*col_it)->changed_data();
        hk_string new_autoinc;
        if ((*col_it)->columntype()==hk_column::auto_inccolumn)
        {

            hk_string autoincquery;
            hk_string colname;
            bool use_autoinc=true;
            switch (p_odbcdatabase->connection()->backendtype())
            {
                case hk_odbcconnection::mysql:
                    autoincquery="SELECT LAST_INSERT_ID()";
                    colname="last_insert_id()";
                    break;
                case hk_odbcconnection::postgresql:
                {
                    hk_odbccolumn* odbccol=dynamic_cast<hk_odbccolumn*>((*col_it));
                    if (odbccol)
                    {
                        autoincquery="SELECT "+odbccol->p_autoincdefault;
                    }
                    colname="currval";
                }
                break;
                case hk_odbcconnection::sapdb:
                {
                    hk_odbccolumn* odbccol=dynamic_cast<hk_odbccolumn*>((*col_it));
                    if (odbccol)
                    {
                        autoincquery="select  "+p_identifierdelimiter+name()+p_identifierdelimiter+".currval as currval from "+p_identifierdelimiter+name()+p_identifierdelimiter;
                    }
                    colname="CURRVAL";
                }
                break;
                case hk_odbcconnection::mssql:
                    autoincquery="SELECT @@IDENTITY as currval";
                    colname="currval";
                    break;
                default:use_autoinc=false;
            }
            if (use_autoinc)
            {
                hk_datasource* d= database()->new_resultquery();
                if (d)
                {
                    d->set_sql(autoincquery);
                    d->enable();
                    hk_column* c=d->column_by_name(colname);
                    if (c)
                    {
                        new_autoinc=c->asstring();cerr <<"autoincwert: "<<new_autoinc<<endl;

                    } else cerr <<"kein autoinccolumn"<<endl;
                }
                delete d;

                const int bsize=new_autoinc.size()+1;
                char* data=new char[bsize];
                strcpy(data,new_autoinc.c_str());
                datarow[spalte].data=data;
                datarow[spalte].length=strlen(data);

            }
        }
        else
        {                                         // not a autoinc column
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


bool hk_odbcdatasource::driver_specific_batch_enable(void)
{

    if (!p_enabled)
    {
        set_maxrows(0);
        if  (p_odbcdatabase==NULL)  return false;
        if  (!p_odbcdatabase->connection()->is_connected()) return false;
        if (accessmode()==batchwrite)
        {
            p_enabled=true;
            return true;
        }
        SQLRETURN ret= SQLAllocHandle(SQL_HANDLE_STMT,p_odbcdatabase->connection()->connectionhandle(),&p_result);cerr <<"SQL: "<<p_sql<<endl;
        if (ret!=SQL_SUCCESS)
        {
            clear_result();
            return false;
        }
        ret= SQLExecDirect(p_result ,(SQLCHAR*)const_cast<char*>((p_sql.c_str())) ,SQL_NTS);
        if (ret!=SQL_SUCCESS)
        {
            clear_result();
            return false;

        }
        if (ret==SQL_SUCCESS)
        {
            SQLSMALLINT num_fields;
            ret=SQLNumResultCols(p_result,&num_fields);
            if (ret!=SQL_SUCCESS)
            {
                clear_result();
                return false;
            }
            if (!driver_specific_create_columns()) return false;
// now add data
            try
            {
                if ((ret=SQLFetch(p_result))==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO)
                {
                    add_data(num_fields);
                    set_maxrows(1);
                }
            }                                     //end try
            catch (std::bad_alloc errormessage)
            {
                memory_error();
                return true;
            }
            return true;
        }
    }
    set_maxrows(0);
    return false;
}


bool hk_odbcdatasource::driver_specific_batch_disable(void)
{
    delete_data();
    if (accessmode()==batchwrite) return true;
    clear_result();
    return true;
}


bool hk_odbcdatasource::driver_specific_batch_goto_next(void)
{
    if (hk_storagedatasource::driver_specific_batch_goto_next()) return true;
    SQLRETURN ret;

    if ((ret=SQLFetch(p_result))==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO)
    {
        SQLSMALLINT num_fields;
        ret=SQLNumResultCols(p_result,&num_fields);
//neue Datenzeile erzeugen
        try
        {
            set_maxrows(max_rows()+1);
            add_data(num_fields);

        }                                         //end try
        catch (std::bad_alloc errormessage)
        {
            memory_error();
            clear_result();
            return true;
        }
        ++p_counter;
        return true;
    }
    else
    {
        clear_result();
    }
    return false;
}


void hk_odbcdatasource::clear_result(void)
{

    set_servermessage();
    SQLFreeHandle(SQL_HANDLE_STMT,p_result);

}


void hk_odbcdatasource::set_servermessage(void)
{
    const int buffersize=300;
    SQLCHAR* buffer=new SQLCHAR[buffersize];
    *buffer=0;
    SQLSMALLINT ressize;

    SQLGetDiagField(SQL_HANDLE_STMT,p_result,1,SQL_DIAG_MESSAGE_TEXT,buffer,buffersize,&ressize);
    p_odbcdatabase->connection()->set_last_servermessage((const char*)buffer);
//     cerr <<"SERVERMESSAGE: "<<(const char*)buffer<<endl<<endl;
    delete[] buffer;

}
