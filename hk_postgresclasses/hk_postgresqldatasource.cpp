// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include <hk_postgresqldatasource.h>
#include <hk_postgresqldatabase.h>
#include <hk_postgresqlconnection.h>
#include <hk_postgresqlcolumn.h>
#include <hk_postgresqlactionquery.h>
#include <hk_column.h>
#include <exception>
#include <new>
#include <string.h>

hk_postgresqldatasource::hk_postgresqldatasource(hk_postgresqldatabase* d,hk_presentation* p):hk_storagedatasource(d,p)
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqldatasource::constructor");
#endif
    p_tempdatasource=NULL;
    p_postgresqldatabase=d;
    p_columns=NULL;
    p_debug_counter=0;
    p_enabled=false;
    p_actionquery=new hk_postgresqlactionquery(d);
    p_true="t";
    p_false="f";
    p_sql_delimiter.append("\0");
    p_casesensitive=true;
    p_identifierdelimiter="\"";
    zeile=0;

}


hk_postgresqldatasource::~hk_postgresqldatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqldatasource::destructor");
#endif
}


list<hk_column*>* hk_postgresqldatasource::driver_specific_columns(void)
{

    if (p_columns==NULL&&type()==ds_table&&p_name.size()>0&&p_postgresqldatabase->connection()->dbhandler())
    {
        if( PQstatus( p_postgresqldatabase->connection()->dbhandler() ) == CONNECTION_OK )
        {
            hk_string sql_string= "SELECT * FROM \""+p_name+"\" WHERE 1=0";
            p_result = PQexec(p_postgresqldatabase->connection()->dbhandler() ,  sql_string.c_str());
        }
        else cerr <<"BAD CONECTION"<<endl;

        driver_specific_create_columns();
// postgresql_free_result(p_result);
        PQclear(p_result);
        p_result=NULL;

    }
    return p_columns;
}


bool hk_postgresqldatasource::driver_specific_create_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("create_columns");
#endif
    unsigned int num_fields;

    if (p_result!=NULL)
    {
        clear_columnlist();
        p_columns=new list<hk_column*>;

        num_fields=PQnfields(p_result);
        unsigned int z = 0;

        while (  (z <num_fields ) )
        {
            hk_postgresqlcolumn *col= new hk_postgresqlcolumn(this,p_true,p_false);
#ifdef HK_DEBUG
            {
                col->hkclassname("Postgrescolumn "+longint2string(p_debug_counter++));
                hkdebug("New Column");
            }
#endif

            col->set_name( PQfname(p_result , z) );
            col->set_size( PQfsize(p_result , z) );
            col->fieldnr(z);
            if (p_datetimeformat.size()>0) col->set_datetimeformat(p_datetimeformat.c_str());
            if (p_dateformat.size()>0) col->set_dateformat(p_dateformat.c_str());
            if (p_timeformat.size()>0) col->set_timeformat(p_timeformat.c_str());
//	if ( PQftype(p_result , z) == VARCHAROID)
            Oid p_columntype=PQftype(p_result , z);
//cout << "Field: "<<col->name()<<" Field type = " << p_columntype<< endl;
            switch(p_columntype)
            { // numbers taken from postgres file: server/catalog/pg_type.h
                case 1021 :                       //float4
                case 700:
                    col->set_columntype(hk_column::smallfloatingcolumn) ;
                    break;
                case 1022:                        //float8
                case 701:
		case 1231: //numeric
		case 1700: //numeric
                    col->set_columntype(hk_column::floatingcolumn) ;
                    break;
                case 20:
                case 23:
                case 1016:                        //int8
                case 1007:                        //int4
                case 26:                          //object identifier
                case 28:                          //transaction id
                    col->set_columntype(hk_column::integercolumn);
                    break;
                case 21:
                case 1005:                        //int2
                    col->set_columntype(hk_column::smallintegercolumn);
                    break;
                case 1000:                        //bool
                case 16:
                    col->set_columntype(hk_column::boolcolumn);
                    break;

                case 1043:                        //varchar
                case 1042:                        //char
                case 1002:                        //char
                    if (col->size()<1) col->set_size(255);
                    col->set_columntype(hk_column::textcolumn);
                    break;
                case 17:
                    col->set_columntype(hk_column::binarycolumn);
                    break;
                case 25:
                    col->set_columntype(hk_column::memocolumn);
                    break;
                case 1083:                        //time
                    col->set_columntype(hk_column::timecolumn);
                    break;
                case 1082:                        //date
                    col->set_columntype(hk_column::datecolumn);
                    break;
                case 1114:                        //timestamp
                case 1184:                        //timestamptz
                    col->set_columntype(hk_column::timestampcolumn);
                    break;

                default:        col->set_columntype(hk_column::othercolumn);

            }                                     //endswitch

            col->set_definitionmode(false);
            p_columns->insert(p_columns->end(),col);
            z++;

        };                                        //end while

        return true;
    }
    return false;

}


bool hk_postgresqldatasource::driver_specific_enable(void)
{
    

#ifdef HK_DEBUG
    hkdebug("hk_postgresqldatasource::driver_specific_enable");
#endif
   if (p_print_sqlstatements)print_sql();
    zeile=0;
    if (!p_enabled)
    {
        if  (p_postgresqldatabase==NULL)  return false;
        if  (!p_postgresqldatabase->connection()->is_connected()) return false;
        p_result = PQexec(p_postgresqldatabase->connection()->dbhandler() ,p_sql.c_str());
      bool p_cancel=false;
     int max=PQntuples(p_result);
     int i=1;
       if (PQresultStatus(p_result) == PGRES_TUPLES_OK)
        {
            unsigned int num_fields=PQnfields(p_result);
            try
            {

                driver_specific_create_columns();
                while ( zeile < (unsigned long int)PQntuples(p_result)&& !p_cancel )
                {
                    add_data(num_fields);
                    zeile++;
                    if (progressdialog() && (i %15000 ==0))
	 		{
          		p_cancel=progressdialog()(i,max,hk_translate("Executing query ..."));
	 		}
	 		++i;

                }                                 //End of outer while
                PQclear(p_result);
            }                                     //end try
            catch (std::bad_alloc errormessage)
            {
                memory_error();
                PQclear(p_result);
                return true;
            }
            p_result=NULL;                        //                    dump_data();
            return true;
        }
        else
        {
            p_postgresqldatabase->connection()->servermessage();
            PQclear(p_result);
            p_result=NULL;
            cerr <<"Datei konnte nicht geöffnet werden"<<endl;
            return false;
        }
    }
    return false;
}


void hk_postgresqldatasource::add_data(unsigned int num_fields)
{

    struct_raw_data* datarow=new struct_raw_data[num_fields];
    unsigned int spalte=0;
    list<hk_column*>::iterator it=p_columns->begin();
    while (spalte<num_fields)
    {
        char* data=NULL;

        if (!PQgetisnull(p_result,zeile,spalte))
        {
            char* pgres= PQgetvalue( p_result , zeile ,  spalte) ;
            size_t size=0;
	    if (it!=p_columns->end() && (*it)->columntype()!=hk_column::binarycolumn)
	    {// handle non-binary data
            if (pgres)datarow[spalte].length=strlen(pgres);          //PQgetlength(p_result,zeile,spalte)+1;
            data=new char[datarow[spalte].length];
            if (pgres)
                for (unsigned int tk=0;tk<datarow[spalte].length;tk++) data[tk]=pgres[tk];
//              cerr <<"value:"<<pgres<<endl;
	    }
	    else
	    {// handle binary data;
            char* pgunescape=(char*) unescapeBytea((unsigned char*)pgres,&size);
            datarow[spalte].length=size;          //PQgetlength(p_result,zeile,spalte)+1;
            data=new char[datarow[spalte].length];
            if (pgunescape)
                for (unsigned int tk=0;tk<datarow[spalte].length;tk++) data[tk]=pgunescape[tk];
            delete[] pgunescape;
	   }
        }
	if (it!=p_columns->end()) ++it;
        datarow[spalte].data=data;
        spalte++;

    }
//Daten speichern
    insert_data(datarow);

}


hk_column* hk_postgresqldatasource::driver_specific_new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("driver_specific_new_column");
#endif
    hk_column* col=new hk_postgresqlcolumn(this,p_true,p_false);

    return col;
}


bool hk_postgresqldatasource::driver_specific_insert_data(void)
{
    if (!p_tempdatasource) p_tempdatasource=database()->new_resultquery();

    hk_string new_autoinc;

    struct_raw_data* datarow=new struct_raw_data[p_columns->size()];
    list<hk_column*>::iterator col_it;
    col_it=p_columns->begin();
    unsigned int spalte=0;
    while (spalte<p_columns->size())
    {
        const struct_raw_data* changed_data=(*col_it)->changed_data();
//unsigned long new_autoinc=postgresql_insert_id(p_postgresqldatabase->connection()->dbhandler());
        hk_postgresqlcolumn* postcol=dynamic_cast<hk_postgresqlcolumn*>((*col_it));
        if (postcol&&postcol->columntype()==hk_column::auto_inccolumn)
        {

            hk_string n="SELECT "+postcol->p_autoincdefault;
            hk_datasource* d= database()->new_resultquery();
            if (d)
            {
                d->set_sql(n);
                d->enable();
                hk_column* c=d->column_by_name("currval");
                if (c)
                {
                    new_autoinc=c->asstring();

                }
            }
            delete d;

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


void hk_postgresqldatasource::before_source_vanishes(void)
{
    hk_datasource::before_source_vanishes();
    if (p_tempdatasource)
    {
        delete p_tempdatasource;
        p_tempdatasource=NULL;
    }

}


void hk_postgresqldatasource::driver_specific_before_drop_table(void)
{

}


bool hk_postgresqldatasource::driver_specific_batch_enable(void)
{
    zeile=0;
    if (!p_enabled)
    {
        set_maxrows(0);
        if  (p_postgresqldatabase==NULL)  return false;
        if  (!p_postgresqldatabase->connection()->is_connected()) return false;
        if (accessmode()==batchwrite)
        {
            p_enabled=true;
            return true;
        }
        p_result = PQexec(p_postgresqldatabase->connection()->dbhandler() ,p_sql.c_str());
        if (PQresultStatus(p_result) == PGRES_TUPLES_OK)
        {
            try
            {
                driver_specific_create_columns();
                unsigned int num_fields=PQnfields(p_result);
                unsigned int rows=PQntuples(p_result);
                set_maxrows(rows);
                if (rows>0)
                {
                    add_data(num_fields);
                    zeile++;
                }                                 //End of outer while
            }                                     //end try
            catch (std::bad_alloc errormessage)
            {
                memory_error();
                return true;
            }
            return true;
        }
        else
        {
            p_postgresqldatabase->connection()->servermessage();
            PQclear(p_result);
            p_result=NULL;
            cerr <<"Datei konnte nicht ge�fnet werden"<<endl;
            return false;
        }
    }
    return false;
}


bool hk_postgresqldatasource::driver_specific_batch_disable(void)
{
    delete_data();
    if (accessmode()==batchwrite) return true;
    if (!p_result) return false;
    PQclear(p_result);
    p_result=NULL;
    return true;

}


bool hk_postgresqldatasource::driver_specific_batch_goto_next(void)
{

    if (hk_storagedatasource::driver_specific_batch_goto_next()) return true;
    if (p_result&&(zeile<(unsigned long int)PQntuples(p_result)))
    {
//neue Datenzeile erzeugen
        try
        {
            add_data(PQnfields(p_result));
            zeile++;
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
        PQclear(p_result);
        p_result=NULL;
    }
    return false;

}


/*
 * The following functions have been taken (and  slightly modified to work with c++)
 * from Postgres file: fe-exec.c  Original names are PQescapeBytea and PQunescapeBytea
with the following copyright notice
 *-------------------------------------------------------------------------
 *
 * fe-exec.c
 *	  functions related to sending a query down to the backend
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
*
*
* IDENTIFICATION
 *	  Header: /cvsroot/pgsql-server/src/interfaces/libpq/fe-exec.c,v 1.121 2002/08/24 15:00:47 tgl Exp
 *
 *-------------------------------------------------------------------------
 *

 */

/*
 *		PQescapeBytea	- converts from binary string to the
 *		minimal encoding necessary to include the string in an SQL
 *		INSERT statement with a bytea type column as the target.
 *
 *		The following transformations are applied
 *		'\0' == ASCII  0 == \\000
 *		'\'' == ASCII 39 == \'
 *		'\\' == ASCII 92 == \\\\
 *		anything >= 0x80 ---> \\ooo (where ooo is an octal expression)
 */
unsigned char * escapeBytea(unsigned char *bintext, size_t binlen, size_t *bytealen)
{
    unsigned char *vp;
    unsigned char *rp;
    unsigned char *result;
    size_t      i;
    size_t      len;

/*
 * empty string has 1 char ('\0')
 */
    len = 1;

    vp = bintext;
    for (i = binlen; i > 0; i--, vp++)
    {
        if (*vp == 0 || *vp >= 0x80)
            len += 5;                             /* '5' is for '\\ooo' */
        else if (*vp == '\'')
            len += 2;
        else if (*vp == '\\')
            len += 4;
        else
            len++;
    }

    rp = result = new unsigned char[len];
    if (rp == NULL)
        return NULL;

    vp = bintext;
    *bytealen = len;

    for (i = binlen; i > 0; i--, vp++)
    {
        if (*vp == 0 || *vp >= 0x80)
        {
            (void)sprintf((char*)rp,"\\\\%03o",*vp);
            rp += 5;
        }
        else if (*vp == '\'')
        {
            rp[0] = '\\';
            rp[1] = '\'';
            rp += 2;
        }
        else if (*vp == '\\')
        {
            rp[0] = '\\';
            rp[1] = '\\';
            rp[2] = '\\';
            rp[3] = '\\';
            rp += 4;
        }
        else
            *rp++ = *vp;
    }
    *rp = '\0';

    return result;
}



/*
 *		PQunescapeBytea	- converts the null terminated string representation
 *		of a bytea, strtext, into binary, filling a buffer. It returns a
 *		pointer to the buffer which is NULL on error, and the size of the
 *		buffer in retbuflen. The pointer may subsequently be used as an
 *		argument to the function free(3). It is the reverse of PQescapeBytea.
 *
 *		The following transformations are reversed:
 *		'\0' == ASCII  0 == \000
 *		'\'' == ASCII 39 == \'
 *		'\\' == ASCII 92 == \\
 *
 *		States:
 *		0	normal		0->1->2->3->4
 *		1	\			   1->5
 *		2	\0			   1->6
 *		3	\00
 *		4	\000
 *		5	\'
 *		6	\\
 */
unsigned char * unescapeBytea(unsigned char *strtext, size_t *retbuflen)
{
    size_t buflen;
    unsigned char *buffer, *sp, *bp;
    unsigned int state=0;

    if(strtext == NULL)return NULL;
    buflen = strlen((char*)strtext);              /* will shrink, also we discover if strtext */
    buffer = new unsigned char[buflen];           /* isn't NULL terminated */
    if(buffer == NULL)return NULL;
    for(bp = buffer, sp = strtext; *sp != '\0'; bp++, sp++)
    {
        switch(state)
        {
            case 0:
                if(*sp == '\\')state=1;
                *bp = *sp;
                break;
            case 1:
                if(*sp == '\'')                   /* state=5 */
                {                                 /* replace \' with 39 */
                    bp--;
                    *bp = '\'';
                    buflen--;
                    state=0;
                }
                else if(*sp == '\\')              /* state=6 */
                {                                 /* replace \\ with 92 */
                    bp--;
                    *bp = '\\';
                    buflen--;
                    state=0;
                }
                else
                {
                    if(isdigit(*sp))state=2;
                    else state=0;
                    *bp = *sp;
                }
                break;
            case 2:
                if(isdigit(*sp))state=3;
                else state=0;
                *bp = *sp;
                break;
            case 3:
                if(isdigit(*sp))                  /* state=4 */
                {
                    int v;
                    bp -= 3;
                    sscanf((char*)sp-2, "%03o", &v);
                    *bp = v;
                    buflen -= 3;
                    state=0;
                }
                else
                {
                    *bp = *sp;
                    state=0;
                }
                break;
        }
    }
//	buffer = (unsigned char*) realloc(buffer,buflen);
    if (buffer == NULL)
        return NULL;

    *retbuflen=buflen;
    return buffer;
}




