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
#include "hk_mysqldatasource.h"
#include "hk_mysqldatabase.h"
#include "hk_mysqlconnection.h"
#include "hk_mysqlcolumn.h"
#include "hk_mysqlactionquery.h"
#include <exception>
#include <new>
#include <string.h>

hk_mysqldatasource::hk_mysqldatasource(hk_mysqldatabase* d,hk_presentation* p):hk_storagedatasource(d,p)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_mysqldatasource::constructor");
#endif
    p_mysqldatabase=d;
//p_rows=0;
    p_columns=NULL;
    p_length=NULL;
    p_debug_counter=0;
    p_enabled=false;
    p_actionquery=new hk_mysqlactionquery(d);
    p_true="1";
    p_false="0";
    p_identifierdelimiter="`";
    p_SQL_Connection=NULL;

}


hk_mysqldatasource::~hk_mysqldatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqldatasource::destructor");
#endif
    if (accessmode()!=standard &&is_enabled()) driver_specific_batch_disable();
    if (p_SQL_Connection) mysql_close(p_SQL_Connection);
    p_SQL_Connection=NULL;
}


list<hk_column*>* hk_mysqldatasource::driver_specific_columns(void)
{
#ifdef HK_DEBUG
                                                  //cout <<"hk_mysqldatasource::driver_specific_columns"<<endl;
    hkdebug("hk_mysqldatasource::driver_specific_columns");
#endif
    if (p_columns==NULL&&type()==ds_table&&p_name.size()>0&&p_mysqldatabase->connection()->dbhandler())
    {
        p_result=mysql_list_fields(p_mysqldatabase->connection()->dbhandler(),p_name.c_str(),NULL);
        driver_specific_create_columns();
        mysql_free_result(p_result);
        p_result=NULL;

    }
    return p_columns;

}


bool hk_mysqldatasource::driver_specific_create_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("create_columns");
#endif
    unsigned int num_fields;
    MYSQL_FIELD *fields;
    if (p_result!=NULL)
    {
        clear_columnlist();
        p_columns=new list<hk_column*>;

        num_fields=mysql_num_fields(p_result);
        int z = 0;                                //counting the fieldnumbers
        while ((fields=mysql_fetch_field(p_result)))
        {
            hk_mysqlcolumn *col= new hk_mysqlcolumn(this,p_true,p_false);
#ifdef HK_DEBUG
            {
                col->hkclassname("Mysqlcolumn "+longint2string(p_debug_counter++));
                hkdebug("New Column");
            }
#endif
            col->set_name(fields->name);
            col->set_size(fields->length);
            if (fields->table)col->p_resulttablename=fields->table;
            col->set_fieldnumber(z++);
            if (p_datetimeformat.size()>0) col->set_datetimeformat(p_datetimeformat);
            if (p_dateformat.size()>0) col->set_dateformat(p_dateformat);
            if (p_timeformat.size()>0) col->set_timeformat(p_timeformat);
            switch(fields->type)
            {
                case FIELD_TYPE_FLOAT :
                    col->set_columntype(hk_column::smallfloatingcolumn) ;
                    break;
                case FIELD_TYPE_DOUBLE:
		    col->p_driverspecific_digits=fields->decimals;
                    col->set_columntype(hk_column::floatingcolumn) ;
                    break;
                case FIELD_TYPE_LONG:
                case FIELD_TYPE_INT24:
                case FIELD_TYPE_LONGLONG:
                    col->set_columntype(hk_column::integercolumn);
                    if (fields->flags & AUTO_INCREMENT_FLAG)
                        col->set_columntype(hk_column::auto_inccolumn);
                    break;
                case FIELD_TYPE_TINY:
					if (database()->connection()->booleanemulation())
					   col->set_columntype(hk_column::boolcolumn);
					else
					   col->set_columntype(hk_column::smallintegercolumn);
                break;
                case FIELD_TYPE_SHORT:           col->set_columntype(hk_column::smallintegercolumn);

                break;

		case FIELD_TYPE_DECIMAL:	col->p_driverspecific_digits=fields->decimals;
                case FIELD_TYPE_ENUM:
                case FIELD_TYPE_SET:
                case FIELD_TYPE_STRING:
                case FIELD_TYPE_VAR_STRING:   col->set_columntype(hk_column::textcolumn);
                break;
                case FIELD_TYPE_DATETIME:
                    col->set_columntype(hk_column::datetimecolumn);
                    break;
                case FIELD_TYPE_TIMESTAMP:
                    col->set_columntype(hk_column::timestampcolumn);
                    break;
                case FIELD_TYPE_NEWDATE:
                case FIELD_TYPE_DATE:       col->set_columntype(hk_column::datecolumn);
                break;
                case FIELD_TYPE_TIME:       col->set_columntype(hk_column::timecolumn);
                break;
                case FIELD_TYPE_TINY_BLOB:
                case FIELD_TYPE_MEDIUM_BLOB:
                case FIELD_TYPE_LONG_BLOB:
                case FIELD_TYPE_BLOB:       if (fields->flags & BINARY_FLAG)
                col->set_columntype(hk_column::binarycolumn);
                else
                    col->set_columntype(hk_column::memocolumn);
                break;
                default:        col->set_columntype(hk_column::othercolumn);

            }                                     //endswitch

            if (fields->flags & PRI_KEY_FLAG)
            {
                col->set_primary(true);
                p_primary_key_used=true;
            }
            if (fields->flags & NOT_NULL_FLAG) col->set_notnull(true);
            col->set_definitionmode(false);

            p_columns->insert(p_columns->end(),col);
        };                                        //end while

        delete fields;
// now check if there are ambiguous names
        list<hk_column*>::iterator it=p_columns->begin();
        while (it!=p_columns->end())
        {
            if (columnname_occurances((*it)->name())>1) set_uniquenames(it);
            ++it;
        }

        return true;
    }
    return false;

}


void hk_mysqldatasource::set_uniquenames(list<hk_column*>::iterator i)
{

    if (i==p_columns->end())return;
    hk_string nm=(*i)->name();
    list<hk_column*>::iterator it=i;
    while (it!=p_columns->end())
    {
        hk_mysqlcolumn* col = static_cast<hk_mysqlcolumn*>((*it));
        if (col)col->set_definitionmode(true);
        if ((*it)->name()==nm  && (((hk_mysqlcolumn*)(*it))->p_resulttablename.size()>0))
            (*it)->set_name(((hk_mysqlcolumn*)(*it))->p_resulttablename +"."+(*it)->name());
        if (col) col->set_definitionmode(false);

        ++it;
    }

}


bool hk_mysqldatasource::driver_specific_enable(void)
{
    int i;
#ifdef HK_DEBUG
    hkdebug("driver_specific_enable");
#endif
   if (p_print_sqlstatements)   print_sql();
    if (!dbhandler())return false;
    if (!p_enabled)
    {
        if  (p_mysqldatabase==NULL)  return false;
        if (accessmode()==batchwrite)
        {
            clear_columnlist();
            driver_specific_create_columns();
            return true;
        }
        if  (!p_mysqldatabase->connection()->is_connected()) return false;
     bool p_cancel=false;
     int max=progressinterval();
     int pi=1;
        i= mysql_query(dbhandler() ,p_sql.c_str() );
        if (i==0)
        {
            p_result=mysql_use_result(dbhandler());
            if (p_result==NULL) return false;
//p_rows=mysql_num_rows(p_result);
            unsigned int num_fields=mysql_num_fields(p_result);
            driver_specific_create_columns();
            unsigned long zeile=0;
            try
            {
                while ((p_row=mysql_fetch_row(p_result))&&!p_cancel)
                {
//Jede einzelne Zeile durchhoppeln
#ifdef HK_DEBUG
//hkdebug("Zeile", zeile++);
#endif
                    zeile++;
                    p_length=mysql_fetch_lengths(p_result);
//neue Datenzeile erzeugen
                    add_data(num_fields);
                	 if (progressdialog() && (pi %15000 ==0))
	 {
          p_cancel=progressdialog()(pi,max,hk_translate("Executing query ..."));
	 }
	 ++pi;
	if (pi>max-30000)max+=10000;  

                }
            }                                     //end try
            catch (std::bad_alloc errormessage)
            {
                memory_error();
                mysql_free_result(p_result);
                return true;
            }

            mysql_free_result(p_result);
            p_result=NULL;
//p_rows=zeile;
            return true;
        }
        else
        {
            p_mysqldatabase->connection()->servermessage();
            return false;
        }
    }

    return false;
}


void hk_mysqldatasource::add_data(unsigned int num_fields)
{
//neue Datenzeile erzeugen
    struct_raw_data* datarow=new struct_raw_data[num_fields];
    unsigned int spalte=0;
    while (spalte<num_fields)
    {
        datarow[spalte].length=p_length[spalte];
        char* data=NULL;
                                                  //leave NULL pointer if there is no data
        if (p_row[spalte])data=new char[datarow[spalte].length];
#ifdef HK_DEBUG
//	 	   hkdebug("Spaltenl�ge: ",(int)datarow[spalte].length);
#endif
        if (data) for (unsigned int tk=0;tk<datarow[spalte].length;tk++) data[tk]=p_row[spalte][tk];
        datarow[spalte].data=data; ;
#ifdef HK_DEBUG
//	 	   hkdebug("Spalte ",(int)spalte);
#endif
        spalte++;
    }
//Daten speichern
    insert_data(datarow);

}


hk_column* hk_mysqldatasource::driver_specific_new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("driver_specific_new_column");
#endif
    hk_column* col=new hk_mysqlcolumn(this,p_true,p_false);

    return col;
}


bool hk_mysqldatasource::driver_specific_insert_data(void)
{
    if (!dbhandler())return false;
    struct_raw_data* datarow=new struct_raw_data[p_columns->size()];
    list<hk_column*>::iterator col_it;
    col_it=p_columns->begin();
    unsigned int spalte=0;
    while (spalte<p_columns->size())
    {
        const struct_raw_data* changed_data=(*col_it)->changed_data();
        unsigned long new_autoinc=mysql_insert_id(p_mysqldatabase->connection()->dbhandler());
        if ((*col_it)->columntype()==hk_column::auto_inccolumn)
        {
            const int bsize=100;
            char* data=new char[bsize];
            snprintf(data,bsize,"%ld",new_autoinc);
            datarow[spalte].data=data;
            datarow[spalte].length=strlen(data);
        }
        else
        {
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


hk_mysqlconnection* hk_mysqldatasource::mysqlconnection(void)
{
    return p_mysqldatabase->connection();

}


bool hk_mysqldatasource::driver_specific_batch_enable(void)
{

    if (!dbhandler())return false;
    if (!p_enabled)
    {
        set_maxrows(0);
        if  (p_mysqldatabase==NULL)  return false;
        if  (!p_mysqldatabase->connection()->is_connected()) return false;
        if (accessmode()==batchwrite)
        {
            p_enabled=true;
            return true;
        }
        int i= mysql_query(dbhandler() ,p_sql.c_str() );
        if (i==0)
        {
            p_result=mysql_use_result(dbhandler());
            if (p_result==NULL)
            {
                return false;
            }
            unsigned int num_fields=mysql_num_fields(p_result);
            driver_specific_create_columns();
            try
            {
                if((p_row=mysql_fetch_row(p_result)))
                {
// Add first row
                    p_length=mysql_fetch_lengths(p_result);
                    add_data(num_fields);
                    set_maxrows(mysql_num_rows(p_result));
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


bool hk_mysqldatasource::driver_specific_batch_disable(void)
{
    delete_data();
    if (accessmode()==batchwrite) return true;
    if (!p_result) return false;
    while (mysql_fetch_row(p_result));
    mysql_free_result(p_result);
    p_result=NULL;
    return true;
}


bool hk_mysqldatasource::driver_specific_batch_goto_next(void)
{
    if (hk_storagedatasource::driver_specific_batch_goto_next()) return true;

    if (p_result&&(p_row=mysql_fetch_row(p_result)))
    {
        unsigned int num_fields=mysql_num_fields(p_result);
        p_length=mysql_fetch_lengths(p_result);
//neue Datenzeile erzeugen
        set_maxrows(mysql_num_rows(p_result));
        try
        {
            add_data(num_fields);
            ++p_counter;
        }                                         //end try
        catch (std::bad_alloc errormessage)
        {
            memory_error();
            return true;
        }
        return true;
    }
    else
    {
        mysql_free_result(p_result);
        p_result=NULL;
    }
    return false;
}


MYSQL* hk_mysqldatasource::dbhandler()
{
    set_handle();
    return p_SQL_Connection;
}


void hk_mysqldatasource::set_handle(void)
{
    if (p_mysqldatabase->connection()->is_connected())
    {
        if (!p_SQL_Connection)
        {
            p_SQL_Connection=mysql_init(p_SQL_Connection);
            char* xc=0;
            bool p_connected=mysql_real_connect(p_SQL_Connection,p_mysqldatabase->connection()->host().c_str(),p_mysqldatabase->connection()->user().c_str(),p_mysqldatabase->connection()->password().c_str(),xc,p_mysqldatabase->connection()->tcp_port(),xc,0);
            mysql_select_db(p_SQL_Connection,p_mysqldatabase->name().c_str());
            if (!p_connected)
            {
                mysql_close(p_SQL_Connection);
                p_SQL_Connection=NULL;
            }

        }

    }
    else
    {
//connection not connected
        mysql_close(p_SQL_Connection);
        p_SQL_Connection=NULL;
    }

}


void    hk_mysqldatasource::set_name(const hk_string& n,bool registerchange)
{
hk_string nm=replace_all("\\",replace_all("/",n,"_"),"_");
 hk_datasource::set_name(nm,registerchange);
}


