// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// edited by Anirban Biswas <utpal@cal2.vsnl.net.in>
// copyright (c) 2020 Patrik Hanak <hanakp@users.sourceforge.net>
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.30 $

#include "hk_postgresqltable.h"
#include "hk_postgresqldatabase.h"
#include "hk_postgresqlcolumn.h"
#include <hk_actionquery.h>

typedef signed int int32;                         /* == 32 bits */
#define VARHDRSZ        ((int32) sizeof(int32))

hk_postgresqltable::hk_postgresqltable(hk_postgresqldatabase* db,hk_presentation* p):hk_postgresqldatasource(db,p)

{
//wanna_debug(true);
    p_readonly=false;

}


hk_datasource::enum_datasourcetypes hk_postgresqltable::type() const
{
    return ds_table;
}


bool hk_postgresqltable::driver_specific_name(const hk_string& )
{
    return true;
}


bool hk_postgresqltable::driver_specific_enable(void)
{
//  p_readonly=false;
    driver_specific_indices();                    //to select if table uses primarykey
    return hk_postgresqldatasource::driver_specific_enable();

}


bool hk_postgresqltable::driver_specific_create_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqltable::driver_specific_create_table_now");
#endif
    hk_string csql="CREATE TABLE ";
    primarystring="";
    //csql+=replace_all(" ",name(),"_");
    csql+=p_identifierdelimiter+name()+p_identifierdelimiter;
    csql+=" ( ";
    csql+=internal_new_fields_arguments(false);
    csql+=getprimarystring(false)+" ) ";

// cout <<"hk_postgresqltable::driver_specific_create_table_now CREATE definition: "<<endl<<csql<<endl;
    hk_actionquery* query= p_database->new_actionquery();
    query->set_sql(csql.c_str(),csql.size());
    bool result=query->execute();
// if (result) cout<<"tabelle angelegt"; else cout<<"fehler";cout <<endl;
    delete query;
    return result;
}


hk_string hk_postgresqltable::getprimarystring(bool alter)
{
    if (primarystring.size()==0)return "";
    hk_string key=", ";
    if (alter) key+="ADD ";
    key+="PRIMARY KEY ( ";
    key=key+primarystring+" )";
    return key;

}


hk_string hk_postgresqltable::internal_new_fields_arguments(bool alter)
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqltable::internal_new_fields_arguments");
#endif
    hk_string csql;
    hk_string line;
    hk_string fields;

    list<hk_column*>::iterator it=p_newcolumns.begin();
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
#ifdef HK_DEBUG
            hkdebug("nach field2string");
#endif

            if (!alter)
            {
                if ((*it)->columntype()==hk_column::auto_inccolumn||((*it)->is_primary()))
                {
                    if (primarystring.size()>0)primarystring+=" , ";
                    primarystring+=p_identifierdelimiter+(*it)->name()+p_identifierdelimiter;

                }
                if (((*it)->is_notnull()||(*it)->is_primary())&&(*it)->columntype()!=hk_column::auto_inccolumn)
                    fields+=" NOT NULL ";
            }
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
    hkdebug("hk_postgresqltable::internal_new_fields_arguments   ENDE");
#endif
    return csql;
}


bool hk_postgresqltable::is_unallowed_alter(void)
{
    list<class_altercolumns>::iterator it=p_altercolumns.begin();
    while (it!=p_altercolumns.end())
    {
        hk_column* c=column_by_name((*it).name);
        if (c)
        {
            if (((*it).primary!=c->is_primary())) return true;
        }
        it++;
    }
    return false;

}


hk_string hk_postgresqltable::field2string(hk_column::enum_columntype f,const hk_string& m)
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqltable::field2string");
#endif
    hk_string fields;
    switch (f)
    {
        case hk_column::auto_inccolumn :    return "SERIAL";
//BIGINT is not supported before pg 7.1
        case hk_column::integercolumn : return "INT8";
        case hk_column::smallintegercolumn :    return "SMALLINT";
        case hk_column::floatingcolumn :    return "FLOAT8";
        case hk_column::smallfloatingcolumn :   return "FLOAT4";
        case hk_column::datecolumn :    return "DATE";
        case hk_column::timestampcolumn :   return "TIMESTAMP";
        case hk_column::timecolumn :    return "TIME";
        case hk_column::binarycolumn :  return "BYTEA";
        case hk_column::memocolumn :    return "TEXT";
        case hk_column::boolcolumn :    return "BOOLEAN";
        case hk_column::textcolumn :    fields ="VARCHAR("+m+") ";
        return fields;
        default :       return "VARCHAR(255)";

    }

}


bool hk_postgresqltable::driver_specific_alter_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqltable::driver_specific_alter_table_now");
#endif
    if (is_unallowed_alter() )
    {
        show_warningmessage(hk_translate("Error: Postgres does not have the ability to modify the primary key!"));
        return false;

    }
    if ( !internal_alter_fields_arguments()) return false;
    hk_string asql="ALTER TABLE ";
    asql+=p_identifierdelimiter+name()+p_identifierdelimiter;
    list<hk_column*>::iterator it=p_newcolumns.begin();
    hk_actionquery* query= p_database->new_actionquery();
    if (!query) return false;
    hk_string n;
    bool result=true;
    while (it!=p_newcolumns.end()&&result)
    {
        result=true;
        if ((*it)->name().size()>0)
        {
            hk_string f=p_identifierdelimiter+(*it)->name()+p_identifierdelimiter+" "+field2string((*it)->columntype(),longint2string( ((*it)->size()<256?(*it)->size():255 )));
            if (f.size()>0)
            {
                if (n.size() >0)
                n+=" , ";
                n+=" ADD COLUMN "+f;
            }
        }
        it++;
    }
    
    
    
    list<class_altercolumns>::iterator ait=p_altercolumns.begin();
    
    while (ait!=p_altercolumns.end()&&result)
    {
    
        hk_column* c=column_by_name((*ait).name);
        if (c)
        {
        
        if ((*ait).type!=c->columntype()       //try to alter columntype
            && (*ait).type!=hk_column::othercolumn
            || (((*ait).size>-1)&&(*ait).size!=c->size()) //try to change size
            )
          {
            hk_string f=p_identifierdelimiter+(*ait).name+p_identifierdelimiter+" TYPE "+field2string((*ait).type,longint2string( ((*ait).size<256?(*ait).size:255 )));
            if (f.size()>0)
            {
                if (n.size()>0) n+=" , ";
                n+=" ALTER COLUMN "+f;
            }
        
        
          }
        }
    ++ait;
    }
    
    hk_string del=internal_delete_fields_arguments();
    if (del.size()>0)
    {
      if (n.size()>0) n+=" , ";
      n+=del;
    
    }
    n=asql+n;
    query->set_sql(n.c_str(),n.size());
    result=query->execute();
    delete query;
    return result;

}

hk_string  hk_postgresqltable::internal_delete_fields_arguments(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqltable::internal_delete_fields_arguments");
#endif

    if (p_deletecolumns.size()==0)return "";
    hk_string result;

    list<hk_string>::iterator it=p_deletecolumns.begin();
    while (it!=p_deletecolumns.end())
    {
        if (result.size()>0) result+=" , ";
        result+=" DROP COLUMN ";
        result+=p_identifierdelimiter+(*it)+p_identifierdelimiter;
        it++;
    }
    return result;
}


bool  hk_postgresqltable::internal_alter_fields_arguments(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqltable::internal_alter_fields_arguments");
#endif
    if (p_altercolumns.size()==0)
    {
        return true;
    }
    hk_string result;
    list<class_altercolumns>::iterator it=p_altercolumns.begin();
    hk_actionquery* query= p_database->new_actionquery();
    if (!query) return false;
    bool r=true;
    while (it!=p_altercolumns.end()&&r)
    {
        hk_column*c =column_by_name((*it).name.c_str());
        if (c!=NULL)
        {
            if ((*it).newname!="HK_NULL"&& (*it).newname!=(*it).name &&(*it).newname.size()>0)
            {
                result=" ALTER TABLE ";
                result+=p_identifierdelimiter+name()+p_identifierdelimiter;

                result+=" RENAME COLUMN ";
                result+=p_identifierdelimiter+(*it).name+p_identifierdelimiter+" TO "+p_identifierdelimiter+(*it).newname+p_identifierdelimiter;
                query->set_sql(result.c_str(),result.size());
                r=query->execute();
                if (!r) show_warningmessage(replace_all("%COLUMN%",hk_translate("Column '%COLUMN%' could not be renamed"),(*it).name));
            }
            if (r && (*it).notnull!=c->is_notnull())
            {
                if ((*it).notnull)
                {
                    result="SELECT * FROM "+p_identifierdelimiter+name()+p_identifierdelimiter+" WHERE "+p_identifierdelimiter+((*it).newname!="HK_NULL"?(*it).newname:(*it).name)+p_identifierdelimiter+" IS NULL ";
                    hk_datasource*d =database()->new_resultquery();
                    if (d)
                    {
                        d->set_sql(result);
                        d->enable();
                        if (d->max_rows()>0) r=false;
//d->dump_data();
//cout <<"MAXROWS: "<<d->max_rows();
                        d->disable();
                        delete d;
                    }

                    if (!r) show_warningmessage(replace_all("%COLUMN%",hk_translate("Column '%COLUMN%' has NULL values, so NOTNULL is not possible"),(*it).name));
                }
                if (r)
                {
                    result="UPDATE pg_attribute SET attnotnull=";
                    result+=((*it).notnull?"TRUE":"FALSE");
                    result+=" WHERE attname=";
                    result+=p_sqltextdelimiter+(((*it).newname!="HK_NULL"?(*it).newname:(*it).name))+p_sqltextdelimiter;
                    result+=" AND attrelid=(SELECT oid FROM pg_class WHERE relname='"+name()+"')";
                    cerr <<"NOT NULL Anpassung: "<<result<<endl;
                    query->set_sql(result.c_str(),result.size());
                    r=query->execute();
                }

            }

        }
        it++;
    }
    delete query;
    return r;
}


bool hk_postgresqltable::is_alteredfield(const hk_string& f)
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqltable::is_alteredfield");
#endif
    list<class_altercolumns>::iterator it=p_altercolumns.begin();
    while (it!=p_altercolumns.end())
    {
        if ((*it).name==f)return true;
        it++;
    }
    return false;
}


bool hk_postgresqltable::is_deletedfield(const hk_string& f)
{
#ifdef HK_DEBUG
    hkdebug("hk_postgresqltable::is_deletedfield");
#endif
    list<hk_string>::iterator it=p_deletecolumns.begin();
    while (it!=p_deletecolumns.end())
    {
        if ((*it)==f)return true;
        it++;
    }
    return false;
}


list<hk_datasource::indexclass>* hk_postgresqltable::driver_specific_indices(void)
{
//return NULL; //for testing reasons !! remove later

    if (!p_tempdatasource) p_tempdatasource=database()->new_resultquery();
    if (p_tempdatasource==NULL)
    {
        return NULL;
    }
    set_indexquery();
    p_indices.erase(p_indices.begin(),p_indices.end());
    unsigned long m=p_tempdatasource->max_rows();
    hk_column* keyname=p_tempdatasource->column_by_name("indexname");
    hk_column* fieldname=p_tempdatasource->column_by_name("columnname");
    hk_column* uniquefield=p_tempdatasource->column_by_name("is_unique");
    hk_column* primaryfield=p_tempdatasource->column_by_name("is_primary");
    if (keyname==NULL||fieldname==NULL||uniquefield==NULL||primaryfield==NULL)
    {
        p_tempdatasource->disable();
        return NULL;
    }

    indexclass index;                             //cout <<"Anzahl indices: "<<m<<endl;
    hk_string indexname;
    for (unsigned long i=0;i<m;i++)
    {
        hk_string n=keyname->asstring();
        if (n!=indexname&& !primaryfield->asbool())
        {
            indexname=n;
            index.name=n;
            index.unique=uniquefield->asbool();
            p_indices.insert(p_indices.end(),index);
        }
        if (primaryfield->asbool())
        {
            p_primary_key_used=true;

        }
        p_tempdatasource->goto_next();
    }
    p_tempdatasource->goto_first();
    for (unsigned long i=0;i<m;i++)
    {
        hk_string n=keyname->asstring();
        list<hk_datasource::indexclass>::iterator it=findindex(n);
        if (it!=p_indices.end())
        {
            (*it).fields.insert((*it).fields.end(),fieldname->asstring());

        }
        p_tempdatasource->goto_next();
    }

    p_tempdatasource->disable();
    return &p_indices;
}


void hk_postgresqltable::set_indexquery(void)
{
    if (!p_tempdatasource) p_tempdatasource=database()->new_resultquery();
    if (!p_tempdatasource) return;
    hk_string q="select t1.relname as indexname, indisunique as is_unique ,indisprimary as is_primary ,\
 attname as columnname\
 from pg_index,pg_class t1,pg_class t2 ,pg_attribute a\
 where indexrelid=t1.oid and indrelid =t2.oid and a.attrelid=t1.oid and t2.relname ='";
    q+=name()+"'";
    p_tempdatasource->disable();
    p_tempdatasource->set_sql(q,true);
    p_tempdatasource->enable();
// p_tempdatasource->dump_data();

}


list<hk_datasource::indexclass>::iterator hk_postgresqltable::findindex(const hk_string& i)
{

    list<hk_datasource::indexclass>::iterator it=p_indices.begin();
    while (it!=p_indices.end())
    {
        if ((*it).name==i)return it;
        it++;
    }
    return p_indices.end();
}


bool    hk_postgresqltable::driver_specific_drop_index(const hk_string& i)
{
    hk_string s=" DROP INDEX ";
    s+=p_identifierdelimiter+i+p_identifierdelimiter;

    hk_actionquery* query= p_database->new_actionquery();
    if (query==NULL) return false;
    query->set_sql(s.c_str(),s.size());
    bool res =query->execute();
    delete query;

    return res;
}


bool    hk_postgresqltable::driver_specific_create_index(const hk_string& i,bool unique, list<hk_string>& fields)
{
    hk_string s="CREATE ";
    s+=(unique?"UNIQUE ":"");
    s+="INDEX ";
    s+=p_identifierdelimiter+i+p_identifierdelimiter+" ON ";
    s+=p_identifierdelimiter+name()+p_identifierdelimiter;
    s+=" ( ";
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
//cout <<"Create Index: "<<s<<endl;
    query->set_sql(s.c_str(),s.size());
    bool res =query->execute();
    delete query;

    return res;
}


bool hk_postgresqltable::driver_specific_create_columns(void)
{
    if (!p_tempdatasource) p_tempdatasource=database()->new_resultquery();
    bool result=hk_postgresqldatasource::driver_specific_create_columns();

    if (!result||!p_columns||name().size()==0)return false;
    hk_string s= "SELECT a.*,typname, pg_get_expr(d.adbin, d.adrelid) as defaultvalue from pg_class t,pg_type y , pg_attribute a \
   LEFT JOIN pg_attrdef d ON a.attnum=d.adnum and a.attrelid=d.adrelid WHERE  t.oid=a.attrelid \
   and a.attnum>0 and y.oid = a.atttypid and relname='"+name()+"'";
    if (p_tempdatasource==NULL) return false;
    p_tempdatasource->disable();
    p_tempdatasource->set_sql(s);
    p_tempdatasource->enable();
    hk_column* namefield=p_tempdatasource->column_by_name("attname");
    hk_column* isnotnullfield=p_tempdatasource->column_by_name("attnotnull");
    hk_column* defaultfield=p_tempdatasource->column_by_name("defaultvalue");
    hk_column* sizefield=p_tempdatasource->column_by_name("atttypmod");
    unsigned long z=0;
    while (  (z <p_tempdatasource->max_rows() ) )
    {
        hk_column* colorig= column_by_name(namefield->asstring());
        hk_postgresqlcolumn* col=dynamic_cast<hk_postgresqlcolumn*>(colorig);
        if (col)
        {
            col->set_definitionmode(true);
            col->set_notnull(isnotnullfield->asbool());
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
            if (col->columntype()==hk_column::textcolumn)col->set_size(localestring2int(sizefield->asstring())-VARHDRSZ);
            col->set_definitionmode(false);

        }
        p_tempdatasource->goto_next();z++;
    };                                            //end while

    p_tempdatasource->disable();

// now query for the is_primary values
    set_indexquery();
    if (!p_tempdatasource) return result;

    z=0;
    hk_column* fieldname=p_tempdatasource->column_by_name("columnname");
    hk_column* primaryfield=p_tempdatasource->column_by_name("is_primary");
    while (  (z <p_tempdatasource->max_rows() ) )
    {
        if (primaryfield->asbool())
        {

            p_primary_key_used=true;
            hk_postgresqlcolumn* col= dynamic_cast<hk_postgresqlcolumn*>(column_by_name(fieldname->asstring()));
            if (col)
            {
                col->set_definitionmode(true);
                col->set_primary(true);
                col->set_definitionmode(false);
            }
        }
        p_tempdatasource->goto_next();z++;
    };                                            //end while
    p_tempdatasource->disable();

    return result;
}


void hk_postgresqltable::driver_specific_after_copy_table()
{
    if (p_tempdatasource==NULL) return;
    p_tempdatasource->disable();
    hk_string s= "SELECT a.*,typname, pg_get_expr(d.adbin, d.adrelid) as defaultvalue from pg_class t,pg_type y , pg_attribute a \
   LEFT JOIN pg_attrdef d ON a.attnum=d.adnum and a.attrelid=d.adrelid WHERE  t.oid=a.attrelid \
   and a.attnum>0 and y.oid = a.atttypid and relname='"+name()+"'";
    p_tempdatasource->set_sql(s);
    p_tempdatasource->enable();
    hk_column* namefield=p_tempdatasource->column_by_name("attname");
    hk_column* defaultfield=p_tempdatasource->column_by_name("defaultvalue");
    unsigned long z=0;
    while (  (z <p_tempdatasource->max_rows() ) )
    {
        hk_column* colorig= column_by_name(namefield->asstring());
        hk_postgresqlcolumn* col=dynamic_cast<hk_postgresqlcolumn*>(colorig);
        if (col)
        {
            hk_string n="nextval(";
            hk_string::size_type pos=defaultfield->asstring().find(n);
            if(pos!=hk_string::npos)
            {
                cerr <<defaultfield->asstring()<<endl;
                hk_string setval=defaultfield->asstring();
                setval.replace(pos,n.size()-1,"SELECT setval");
                pos=setval.find_last_of(")");
                hk_string max=",(SELECT max(\""+namefield->asstring()+"\") FROM \""+name()+"\"))";
                setval.replace(pos,n.size()-1,max);
                hk_actionquery* query= p_database->new_actionquery();
                query->set_sql(setval.c_str(),setval.size());
                query->execute();
                delete query;
            }
        }
        p_tempdatasource->goto_next();z++;
    };                                            //end while

    p_tempdatasource->disable();
}




void hk_postgresqltable::driver_specific_referenceslist(void)
{
hk_string s="select \
	c1.relname as \"constraint_is_on\" ,\
	c2.relname as \"referenced_table\",\
	c1.oid as \"thisoid\",\
	c2.oid as \"masteroid\",\
	conname,\
	confupdtype,\
	confdeltype,\
	substring(array_dims(confkey) from \':(.*)\\\\]\') as numberitems,\
	confkey[1] as confkey1,\
	confkey[2] as confkey2,\
	confkey[3] as confkey3,\
	confkey[4] as confkey4,\
	confkey[5] as confkey5,\
	confkey[6] as confkey6,\
	confkey[7] as confkey7,\
	confkey[8] as confkey8,\
	confkey[9] as confkey9,\
	confkey[10] as confkey10,\
	conkey[1] as conkey1,\
	conkey[2] as conkey2,\
	conkey[3] as conkey3,\
	conkey[4] as conkey4,\
	conkey[5] as conkey5,\
	conkey[6] as conkey6,\
	conkey[7] as conkey7,\
	conkey[8] as conkey8,\
	conkey[9] as conkey9,\
	conkey[10] as conkey10\
 from \
	pg_constraint,\
	pg_class as c1,\
	pg_class as c2 \
 where \
	c1.oid=conrelid\
	and c2.oid=confrelid\
	and contype='f'\
	AND c1.relname='"+name()+"'";
hk_datasource*d =database()->new_resultquery();
hk_datasource* d2=database()->new_resultquery();
hk_datasource* d3=database()->new_resultquery();
if (!d || !d2 ||!d3)
{
  show_warningmessage("Error:hk_postgresqldatasource::driver_specific_referenceslist\nNo datasource");
  return;
}

    d->set_sql(s);
    cerr <<"sql1="<<endl<<s<<endl<<endl;
    d->enable();
    unsigned long i=0;
    hk_column* ref_is_on= d->column_by_name("referenced_table");
    hk_column* conname=d->column_by_name("conname");
    hk_column* oid=d->column_by_name("thisoid");
    hk_column* masteroid=d->column_by_name("masteroid");
    hk_column* numberitems=d->column_by_name("numberitems");
    hk_column* onupdate=d->column_by_name("confupdtype");
    hk_column* ondelete=d->column_by_name("confdeltype");
    if (!ref_is_on||!conname||!numberitems||!oid||!masteroid||!onupdate||!ondelete)
    {
      cerr <<"ref_is_on"<<ref_is_on<<" conname"<<conname<<" oid"<<oid<<" numberitems"<<numberitems<<" masteroid"<<masteroid<<" onupdate"<<onupdate<<" ondelete"<<ondelete<<endl;
      show_warningmessage("Error:hk_postgresqldatasource::driver_specific_referenceslist\nNo system table!");
      return;
    }


    while (i<d->max_rows())
    {
      referentialclass ref;
      ref.p_masterdatasource=ref_is_on->asstring_at(i);
      ref.p_name=conname->asstring_at(i);
      ref.p_updatecascade=onupdate->asstring()=="c";
      ref.p_deletecascade=ondelete->asstring()=="c";
      int numitems=numberitems->asinteger_at(i);
      if (numitems>10)numitems=10;
      int n=0;
      hk_string sql2="select attnum,attname from pg_attribute where attnum >=0 and attrelid=";
      sql2+=format_number(oid->asinteger_at(i),false,false);
      hk_string sql3="select attnum,attname from pg_attribute where attnum >=0 and attrelid=";
      sql3+=format_number(masteroid->asinteger_at(i),false,false);
      d2->set_sql(sql2);
      d2->enable();
      d3->set_sql(sql3);
      d3->enable();

      hk_column* attnum=d2->column_by_name("attnum");
      hk_column* attname=d2->column_by_name("attname");
      hk_column* masterattnum=d3->column_by_name("attnum");
      hk_column* masterattname=d3->column_by_name("attname");
      if (attnum && attname && masterattnum && masterattname)
      {
       while (n<numitems)
       {
        hk_string num=format_number(n+1,false,false);
	hk_string c1name="confkey"+num;
	hk_string c2name="conkey"+num;
	hk_column* c1=d->column_by_name(c1name);
	hk_column* c2=d->column_by_name(c2name);
	if (c1&&c2)
	  {
	    dependingclass dep;
	    unsigned long cresult=masterattnum->find(c1->asstring());
	    if (cresult<d3->max_rows())dep.masterfield=masterattname->asstring_at(cresult);

	    cresult=attnum->find(c2->asstring());
	    if (cresult<d2->max_rows())dep.dependingfield=attname->asstring_at(cresult);;

	    ref.p_fields.push_back(dep);
	  }
	else
	 {
	   show_warningmessage("Error:hk_postgresqldatasource::driver_specific_referenceslist\nconfkey column not found");
	   cerr <<c1name <<" "<<c2name<<endl;
	 }
        ++n;
       }
      } // end attnum && attname

      p_references.push_back(ref);
      cerr <<"add_reference:"<<ref.p_name<<endl;
      cerr <<ref.p_masterdatasource<<endl;

      list<dependingclass>::iterator it=ref.p_fields.begin();
      while (it!=ref.p_fields.end())
      {
       cerr <<"m="<<(*it).masterfield<<" s="<<(*it).dependingfield<<endl;
       ++it;
      }
      ++i;
      d2->disable();
      d3->disable();
    }
    d->disable();
    delete d;
    delete d2;
    delete d3;
    d=NULL;
    d2=NULL;
    d3=NULL;



}



