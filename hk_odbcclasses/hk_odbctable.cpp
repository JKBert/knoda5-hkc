// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// copyright (c) 2020 Patrik Hanak <hanakp@users.sourceforge.net>
// This file is part of the hk_odbcclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.21 $
#include "hk_odbctable.h"
#include "hk_odbcdatabase.h"
#include "hk_odbcconnection.h"
#include "hk_actionquery.h"
#include <sqltypes.h>
#include <sqlext.h>
#include <string.h>

hk_odbctable::hk_odbctable(hk_odbcdatabase* db,hk_presentation* p):hk_odbcdatasource(db,p)

{
    p_readonly=false;

}


hk_datasource::enum_datasourcetypes hk_odbctable::type() const
{
    return ds_table;
}


bool hk_odbctable::driver_specific_name(const hk_string& )
{
    return true;
}


bool hk_odbctable::driver_specific_enable(void)
{
//  p_readonly=false;
    return hk_odbcdatasource::driver_specific_enable();

}


bool hk_odbctable::driver_specific_create_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbctable::driver_specific_create_table_now");
#endif
    hk_string csql="CREATE TABLE ";
    primarystring="";
    csql+=p_identifierdelimiter+name()+p_identifierdelimiter;
    csql+=" ( ";
    csql+=internal_new_fields_arguments(false);
    csql+=getprimarystring(false)+" ) ";

    cerr <<"CREATE definition: "<<endl<<csql<<endl;
    hk_actionquery* query= p_database->new_actionquery();
    if (!query) return false;
    query->set_sql(csql.c_str(),csql.size());
    bool result=query->execute();

    delete query;
    return result;
}


hk_string hk_odbctable::getprimarystring(bool alter)
{
    if (primarystring.size()==0)
    {
        return "";
    }
    hk_string key=", ";
    if (alter) key+="ADD ";
    key+="PRIMARY KEY ( ";
    key=key+primarystring+" )";
    return key;

}


hk_string hk_odbctable::internal_new_fields_arguments(bool alter)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbctable::internal_new_fields_arguments");
#endif
    hk_string csql;
    hk_string line;
    hk_string fields;

    list<hk_column*>::iterator it=p_newcolumns.begin();
    while (it!=p_newcolumns.end())
    {
        it++;
    }

    it=p_newcolumns.begin();
    while (it!=p_newcolumns.end())
    {
#ifdef HK_DEBUG
        hkdebug("while START");
#endif
        hk_string n=(*it)->name();
        if (n.size()>0)
        {
            if (line.size()>0)line+=" , ";
            if (alter) fields =" ADD COLUMN "; else fields="";
            fields+=((*it)->name().size()>0?p_identifierdelimiter+(*it)->name()+p_identifierdelimiter:"");
            if (fields.size()==0)
            {
                return "";
            }
            fields+=" ";
            fields+=field2string((*it)->columntype(),longint2string((*it)->size()<256?(*it)->size():255));
	    fields=replace_all("%LENGTH%",fields,longint2string((*it)->size()<256?(*it)->size():255));
	    fields=replace_all("%SCALE%",fields,longint2string(15));
	    fields=replace_all("%PRECISION%",fields,longint2string(15));
#ifdef HK_DEBUG
            hkdebug("nach field2string");
#endif

            if ((*it)->is_primary())
            {
                if (primarystring.size()>0)primarystring+=" , ";
                primarystring+=p_identifierdelimiter+(*it)->name()+p_identifierdelimiter;
            }

            if (((*it)->is_notnull()||(*it)->is_primary()))
                fields+=" NOT NULL ";
            line+=fields;
        }
        else
        {
            show_warningmessage(hk_translate("Warning: Column with no name!"));
        }
        it++;
#ifdef HK_DEBUG
        hkdebug("while ENDE");
#endif
    }

    csql=csql+line+p_sql_delimiter;
#ifdef HK_DEBUG
    hkdebug("hk_odbctable::internal_new_fields_arguments   ENDE");
#endif
    return csql;
}


hk_string hk_odbctable::field2string(hk_column::enum_columntype f,const hk_string& )
{
#ifdef HK_DEBUG
    hkdebug("hk_odbctable::field2string");
#endif
    if (p_odbcdatabase->columntypeparameter(f).size()==0)return "CHAR(255)";

    hk_string fields;

    switch (f)
    {
        case hk_column::auto_inccolumn :
			{
            		switch (p_odbcdatabase->connection()->backendtype())
            		   {
				case hk_odbcconnection::mysql:
										return "BIGINT(1) NOT NULL AUTO_INCREMENT";
										break;
				case hk_odbcconnection::postgresql:
				case hk_odbcconnection::sapdb:
										return "SERIAL";
										break;
				case hk_odbcconnection::mssql:			return "IDENTITY";
										break;
				
				default:			   return "INTEGER";
			   }
							}
        case hk_column::integercolumn : return p_odbcdatabase->columntypeparameter(hk_column::integercolumn);
        case hk_column::smallintegercolumn :    return p_odbcdatabase->columntypeparameter(hk_column::smallintegercolumn);
        case hk_column::floatingcolumn :    return p_odbcdatabase->columntypeparameter(hk_column::floatingcolumn);
        case hk_column::smallfloatingcolumn :   return p_odbcdatabase->columntypeparameter(hk_column::smallfloatingcolumn);
        case hk_column::datecolumn :    return p_odbcdatabase->columntypeparameter(hk_column::datecolumn);
        case hk_column::datetimecolumn :    return p_odbcdatabase->columntypeparameter(hk_column::datetimecolumn);
        case hk_column::timecolumn :    return p_odbcdatabase->columntypeparameter(hk_column::timecolumn);
        //case hk_column::binarycolumn :  return "LONGBLOB";
        case hk_column::memocolumn :    return p_odbcdatabase->columntypeparameter(hk_column::memocolumn) ;
        case hk_column::boolcolumn :    return p_odbcdatabase->columntypeparameter(hk_column::boolcolumn);
        case hk_column::textcolumn :    return p_odbcdatabase->columntypeparameter(hk_column::textcolumn);
        default :       return "CHAR(255)";

    }

}


bool hk_odbctable::driver_specific_alter_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbctable::driver_specific_alter_table_now");
#endif
    primarystring="";
    hk_string asql="ALTER TABLE ";
    hk_string csql;
    asql+=p_identifierdelimiter+name()+p_identifierdelimiter;

    hk_string r=internal_new_fields_arguments(true);
#ifdef HK_DEBUG
    hkdebug("hk_odbctable::primary index nach new_fields_arguments");
#endif
//  cerr <<"primary index nach new_fields_arguments: "<<primarystring<<endl;
    if (r.size()>0)  csql+=r;
    r=internal_alter_fields_arguments();
//  cerr <<"primary index nach alter_fields_arguments: "<<primarystring<<endl;
    if (r.size()>0)
    {
        if (csql.size()>0)csql+=" , ";
        csql+=r;
    }
    r=internal_delete_fields_arguments();
//  cerr <<"primary index nach delete_fields_arguments: "<<primarystring<<endl;
    if (r.size()>0)
    {
        if (csql.size()>0)csql+=" , ";
        csql+=r;
    }
    list<hk_column*>* l=columns();
    bool dropoldprimary=false;
    if (l!=NULL)
    {
        list<hk_column*>::iterator it=l->begin();
        while (it!=l->end())
        {
            if (!is_deletedfield((*it)->name())&&!is_alteredfield((*it)->name()))
            {

                if ((*it)->columntype()==hk_column::auto_inccolumn||((*it)->is_primary()))
                {
                    if (primarystring.size()>0)primarystring+=" , ";
                    primarystring+=(*it)->name();
                }
            }
            if ((*it)->is_primary())dropoldprimary=true;
            it++;

        }

    }
//    cerr <<"primary index nach normalen Feldern: "<<primarystring<<endl;

    hk_string pp=getprimarystring(true);
    if (pp.size()>0||dropoldprimary)asql+=" DROP PRIMARY KEY , ";
    csql=asql+csql;
    csql+=pp;

    cerr <<"ALTER definition: "<<endl<<csql<<endl;
    hk_actionquery* query= p_database->new_actionquery();
    if (!query) return false;

    query->set_sql(csql.c_str(),csql.size());
    bool result=query->execute();
    if (result) cerr<<"tabelle geändert"; else cerr<<"fehler";cerr <<endl;
    delete query;
    return result;

}


hk_string  hk_odbctable::internal_delete_fields_arguments(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbctable::internal_delete_fields_arguments");
#endif

    if (p_deletecolumns.size()==0)return "";
    hk_string result;

    list<hk_string>::iterator it=p_deletecolumns.begin();
    while (it!=p_deletecolumns.end())
    {
        if (result.size()>0) result+=" , ";
        result+=" DROP COLUMN ";
        result+=(*it);
        it++;
    }
    return result;
}


hk_string  hk_odbctable::internal_alter_fields_arguments(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbctable::internal_alter_fields_arguments");
#endif
    if (p_altercolumns.size()==0)return "";
    char* m= new char[50];
    hk_string result;
    hk_string nm;
    list<class_altercolumns>::iterator it=p_altercolumns.begin();
    while (it!=p_altercolumns.end())
    {
        hk_column*c =column_by_name((*it).name.c_str());
        if (c!=NULL)
        {
            if ((*it).size<0)sprintf(m,"%ld",(c->size()<256)?c->size():255);
            else sprintf(m,"%ld",((*it).size<256)?(*it).size:255);
            if (result.size()>0)result+=" , ";
            result+=" CHANGE COLUMN ";
            result+=(*it).name;
            result+=" ";
            if ((*it).newname=="HK_NULL") nm=(*it).name;
            else nm=(*it).newname;
            result+=nm;
            result+=" ";
            hk_column::enum_columntype newcolumntype=(*it).type;
            if (newcolumntype==hk_column::othercolumn) newcolumntype=c->columntype();
            result+=field2string(newcolumntype,m);
	    result=replace_all("%LENGTH%",result,longint2string((*it).size<256?(*it).size:255));
	    result=replace_all("%SCALE%",result,longint2string(15));
	    result=replace_all("%PRECISION%",result,longint2string(15));

        }
        it++;
    }
    delete[] m;
    return result;
}


bool hk_odbctable::is_alteredfield(const hk_string f)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbctable::is_alteredfield");
#endif
    list<class_altercolumns>::iterator it=p_altercolumns.begin();
    while (it!=p_altercolumns.end())
    {
        if ((*it).name==f)return true;
        it++;
    }
    return false;
}


bool hk_odbctable::is_deletedfield(const hk_string f)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbctable::is_deletedfield");
#endif
    list<hk_string>::iterator it=p_deletecolumns.begin();
    while (it!=p_deletecolumns.end())
    {
        if ((*it)==f)return true;
        it++;
    }
    return false;
}


list<hk_datasource::indexclass>* hk_odbctable::driver_specific_indices(void)
{
    p_indices.erase(p_indices.begin(),p_indices.end());
    SQLHSTMT  result;
    SQLRETURN ret= SQLAllocHandle(SQL_HANDLE_STMT,p_odbcdatabase->connection()->connectionhandle(),&result);
    if (ret == SQL_ERROR || ret == SQL_INVALID_HANDLE) return &p_indices;
    int unique;
    SQLLEN indicator;
    const int buffersize=100;
    char* buffer=new char[buffersize];
    SQLCHAR* namebuffer=new SQLCHAR[name().size()+1];
    strncpy((char*)namebuffer,name().c_str(),name().size()+1);
     cerr <<"BEFORE SQLStatistics '"<<name()<<"' buffer ='"<<namebuffer<<"'"<<endl;
    ret=SQLStatistics( result, 0, 0, 0, 0, namebuffer, SQL_NTS, 0, 0 );
    cerr <<"AFTER SQLStatistics"<<endl;
    delete[] namebuffer;
    hk_string currentindexname;
    indexclass* index=NULL;
     cerr<<"Start FETCHING"<<endl;
    if (ret==SQL_SUCCESS)
    {
// now fetch the index definitions
        ret = SQLFetch( result );
        while ( ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO )
        {
            ret = SQLGetData( result, 6/*index name*/, SQL_C_CHAR, (SQLCHAR*)buffer, buffersize, &indicator );
            ret = SQLGetData( result, 4/* unique*/, SQL_C_LONG, &unique, sizeof(unique), &indicator );
            if (currentindexname!=buffer)
            {
                if (index)  p_indices.insert(p_indices.end(),*index);
                delete index;
                index = new indexclass;
                index->name=buffer;
                index->unique=unique;
                currentindexname=buffer;
            }
            ret = SQLGetData( result, 9/*field name*/, SQL_C_CHAR, (SQLCHAR*)buffer, buffersize, &indicator );
            if (index) index->fields.insert(index->fields.end(),buffer);

            ret = SQLFetch( result );
        }                                         //end while

        if (index)
        {
            p_indices.insert(p_indices.end(),*index);
            delete index;
        }

    }
   cerr <<"BEFORE GET PRIMARY"<<endl;
    //now get Primary keys
        SQLFreeHandle(SQL_HANDLE_STMT,result);
    SQLAllocHandle(SQL_HANDLE_STMT,p_odbcdatabase->connection()->connectionhandle(),&result);

    ret=SQLPrimaryKeys( result, 0, 0, 0, 0, (SQLCHAR*)const_cast<char*>(name().c_str()), SQL_NTS);
cerr <<endl<<"PRIMARYKEYS werden ermittelt"<<endl<<endl;
    if (ret==SQL_SUCCESS)
    {  cerr <<"innerhalb Primarykeys"<<endl;
// now fetch the index definitions
        ret = SQLFetch( result );
        while ( ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO )
        {
            ret = SQLGetData( result, 4/*column name*/, SQL_C_CHAR, (SQLCHAR*)buffer, buffersize, &indicator );
             cerr <<"prim="<<buffer<<endl;
            hk_column* col=column_by_name(buffer);
	    if (col)
	       {
		bool indefinitionmode=col->in_definitionmode();
		col->set_definitionmode(true);
		col->set_primary(true);
		p_primary_key_used=true;

		col->set_definitionmode(indefinitionmode);
	       }else cerr <<"col ist leer!!!"<<endl;


            ret = SQLFetch( result );
        }                                         //end while

cerr <<"END Primarykeys"<<endl;
    }else cerr <<"Fehler bei SQLPrimarykeys!!!"<<endl<<endl;

    delete[] buffer;
    buffer=NULL;
    SQLFreeHandle(SQL_HANDLE_STMT,result);
    return &p_indices;
}


list<hk_datasource::indexclass>::iterator hk_odbctable::findindex(const hk_string& i)
{

    list<hk_datasource::indexclass>::iterator it=p_indices.begin();
    while (it!=p_indices.end())
    {
        if ((*it).name==i)return it;
        it++;
    }
    return p_indices.end();
}


bool    hk_odbctable::driver_specific_drop_index(const hk_string& i)
{
    hk_string s="DROP INDEX ";
    s+=p_identifierdelimiter+i+p_identifierdelimiter;
    hk_actionquery* query= p_database->new_actionquery();
    if (query==NULL) return false;
    query->set_sql(s.c_str(),s.size());
    bool res =query->execute();
    delete query;

    return res;
}


bool    hk_odbctable::driver_specific_create_index(const hk_string& i,bool unique, list<hk_string>& fields)
{
    hk_string s="ALTER TABLE ";
    s+=p_identifierdelimiter+name()+p_identifierdelimiter;
    s+=unique?" ADD UNIQUE ":" ADD INDEX ";
    s+=p_identifierdelimiter+i+p_identifierdelimiter;
    s+="( ";
    hk_string f;
    list<hk_string>::iterator it=fields.begin();
    while (it!=fields.end())
    {
        if (f.size()>0)f+=" , ";
        f+=p_identifierdelimiter+(*it)+p_identifierdelimiter;
        it++;
    }

    s+=f+" )";
    hk_actionquery* query= p_database->new_actionquery();
    if (query==NULL) return false;
// cerr <<"Create Index: "<<s<<endl;
    query->set_sql(s.c_str(),s.size());
    bool res =query->execute();
    delete query;

    return res;
}


bool hk_odbctable::driver_specific_create_columns(void)
{
    if (!hk_odbcdatasource::driver_specific_create_columns()) return false;
//  return true;
    SQLHSTMT r;
    SQLRETURN ret= SQLAllocHandle(SQL_HANDLE_STMT,p_odbcdatabase->connection()->connectionhandle(),&r);
    ret=SQLColumns( r, 0, 0, 0, 0,(SQLCHAR*) const_cast<char*>(name().c_str()), SQL_NTS, 0, 0 );
    SQLINTEGER nCol=0;
    SQLCHAR szColumnName[101]="";

    ret = SQLFetch( r );
    while ( (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) && nCol < (signed)columns()->size() )
    {
        ret = SQLGetData( r, 4/*COLUMN_NAME*/, SQL_C_CHAR, szColumnName, sizeof(szColumnName), 0 );
        if ( ret != SQL_SUCCESS )
            strcpy( (char *)szColumnName, "" );

        int size=0;
        SQLSMALLINT nullable;
        ret = SQLGetData( r, 7/*DATA SIZE*/, SQL_C_LONG, &size, sizeof(size), 0 );
        ret = SQLGetData( r, 11/*NULLABLE*/, SQL_C_SSHORT, &nullable, sizeof(nullable), 0 );
        hk_column* colorig= column_by_name((const char*)szColumnName);
        hk_odbccolumn* col=dynamic_cast<hk_odbccolumn*>(colorig);
        if (col)
        {
            col->set_definitionmode(true);
            if (col->columntype()==hk_column::textcolumn && size==0) size=-1;
	    col->set_size(size);
            if (nullable==SQL_NO_NULLS) col->set_notnull(true);
            col->set_definitionmode(false);
        }
	else cerr <<"column :"<<(const char*)szColumnName<<" nicht gefunden!"<<endl;
        ret = SQLFetch( r );
        nCol++;
    }
    SQLFreeHandle(SQL_HANDLE_STMT,r);

//now set primary key
    ret= SQLAllocHandle(SQL_HANDLE_STMT,p_odbcdatabase->connection()->connectionhandle(),&r);
    ret=SQLPrimaryKeys( r, 0, 0, 0, 0,(SQLCHAR*)const_cast<char*>( name().c_str()), SQL_NTS);
    nCol=0;

    ret = SQLFetch( r );
    while ( (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) && nCol < (signed)columns()->size() )
    {
        ret = SQLGetData( r, 4/*COLUMN_NAME*/, SQL_C_CHAR, szColumnName, sizeof(szColumnName), 0 );
	
        hk_odbccolumn* column=dynamic_cast<hk_odbccolumn*>(column_by_name((const char*)szColumnName));
        if (column)
        {
            column->set_definitionmode(true);
            column->set_primary(true);
            column->set_definitionmode(false);
            p_primary_key_used=true;
        }
        ret = SQLFetch( r );
    }
    SQLFreeHandle(SQL_HANDLE_STMT,r);

    if (p_odbcdatabase->connection()->backendtype()==hk_odbcconnection::postgresql)
    {
        hk_string s= "SELECT a.*,typname, pg_get_expr(d.adbin, d.adrelid) as defaultvalue from pg_class t,pg_type y , pg_attribute a \
   LEFT JOIN pg_attrdef d ON a.attnum=d.adnum and a.attrelid=d.adrelid WHERE  t.oid=a.attrelid \
   and a.attnum>0 and y.oid = a.atttypid and relname='"+name()+"'";
        hk_datasource* p_tempdatasource=database()->new_resultquery();
        p_tempdatasource->set_sql(s);
        p_tempdatasource->enable();
        hk_column* defaultfield=p_tempdatasource->column_by_name("defaultvalue");
        hk_column* namefield=p_tempdatasource->column_by_name("attname");
        unsigned int z=0;
        while (  (z <p_tempdatasource->max_rows() ) )
        {
            hk_column* colorig= column_by_name(namefield->asstring());
            hk_odbccolumn* col=dynamic_cast<hk_odbccolumn*>(colorig);
            if (col)
            {
                col->set_definitionmode(true);
                hk_string n="nextval(";
                hk_string::size_type pos=defaultfield->asstring().find(n);
                if(pos!=hk_string::npos)
                {
                    col->set_columntype(hk_column::auto_inccolumn);
                    col->p_autoincdefault=defaultfield->asstring();
                    col->p_autoincdefault.replace(pos,n.size()-1,"currval");
		    p_primary_key_used=true;
		    col->set_primary(true);
                }
                col->set_definitionmode(false);

            }
            p_tempdatasource->goto_next();z++;
        };                                        //end while

    }


    if (p_odbcdatabase->connection()->backendtype()==hk_odbcconnection::sapdb)
    {
        hk_string s= "SELECT \"COLUMNNAME\", \"DEFAULT\" from COLUMNS WHERE \"TABLENAME\"='"+name()+"'";
        hk_datasource* p_tempdatasource=database()->new_resultquery();
        p_tempdatasource->set_sql(s);
        p_tempdatasource->enable();
        hk_column* defaultfield=p_tempdatasource->column_by_name("DEFAULT");
        hk_column* namefield=p_tempdatasource->column_by_name("COLUMNNAME");
        unsigned int z=0;
        while (  (z <p_tempdatasource->max_rows() && namefield && defaultfield) )
        {
            hk_column* colorig= column_by_name(namefield->asstring());
            hk_odbccolumn* col=dynamic_cast<hk_odbccolumn*>(colorig);
            if (col)
            {
                col->set_definitionmode(true);
                hk_string n="SERIAL";
		hk_string value=string2upper(defaultfield->asstring());
                hk_string::size_type pos=value.find(n);
                if(pos!=hk_string::npos)
                {
                    col->set_columntype(hk_column::auto_inccolumn);
                    col->p_autoincdefault=defaultfield->asstring();
		    p_primary_key_used=true;
		 col->set_primary(true);
                }
                col->set_definitionmode(false);

            }
            p_tempdatasource->goto_next();z++;
        };                                        //end while

    }




    return true;

}
