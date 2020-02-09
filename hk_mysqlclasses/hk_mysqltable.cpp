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
//$Revision: 1.28 $
#include "hk_mysqltable.h"
#include "hk_mysqldatabase.h"
#include "hk_actionquery.h"
#include <string.h>

hk_mysqltable::hk_mysqltable(hk_mysqldatabase* db,hk_presentation* p):hk_mysqldatasource(db,p)

{
    p_readonly=false;

}


hk_datasource::enum_datasourcetypes hk_mysqltable::type() const
{
    return ds_table;
}


bool hk_mysqltable::driver_specific_name(const hk_string& )
{
    return true;
}


bool hk_mysqltable::driver_specific_enable(void)
{
//  p_readonly=false;
    return hk_mysqldatasource::driver_specific_enable();

}


bool hk_mysqltable::driver_specific_create_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqltable::driver_specific_create_table_now");
#endif
    hk_string csql="CREATE TABLE ";
    primarystring="";
//    csql+=replace_all(" ",name(),"_");
   csql+= p_identifierdelimiter+name()+p_identifierdelimiter;
    csql+=" ( ";
    csql+=internal_new_fields_arguments(false);
    csql+=getprimarystring(false)+" ) ";

    hk_actionquery* query= p_database->new_actionquery();
    if (!query) return false;
    query->set_sql(csql.c_str(),csql.size());
    bool result=query->execute();

    delete query;
    return result;
}


hk_string hk_mysqltable::getprimarystring(bool alter)
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


hk_string hk_mysqltable::internal_new_fields_arguments(bool alter)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqltable::internal_new_fields_arguments");
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
#ifdef HK_DEBUG
            hkdebug("nach field2string");
#endif

            if ((*it)->columntype()==hk_column::auto_inccolumn||((*it)->is_primary()))
            {
                if (primarystring.size()>0)primarystring+=" , ";
                primarystring+=p_identifierdelimiter+(*it)->name()+p_identifierdelimiter;
            }
//else
            if (((*it)->is_notnull()||(*it)->is_primary())&&(*it)->columntype()!=hk_column::auto_inccolumn)
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
    hkdebug("hk_mysqltable::internal_new_fields_arguments   ENDE");
#endif
    return csql;
}


hk_string hk_mysqltable::field2string(hk_column::enum_columntype f,const hk_string& m)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqltable::field2string");
#endif
    hk_string fields;
    switch (f)
    {
        case hk_column::auto_inccolumn :    return "BIGINT(1) NOT NULL AUTO_INCREMENT";
        case hk_column::integercolumn : return "BIGINT";
        case hk_column::smallintegercolumn :    return "SMALLINT";
        case hk_column::floatingcolumn :    return "DOUBLE(255,8)";
        case hk_column::smallfloatingcolumn :   return "FLOAT(255,8)";
        case hk_column::datecolumn :    return "DATE";
        case hk_column::datetimecolumn :    return "DATETIME";
        case hk_column::timestampcolumn :   return "TIMESTAMP";
        case hk_column::timecolumn :    return "TIME";
        case hk_column::binarycolumn :  return "LONGBLOB";
        case hk_column::memocolumn :    return "LONGTEXT";
        case hk_column::boolcolumn :    return "TINYINT";
        case hk_column::textcolumn :    fields +="CHAR(";
        fields+=m;
        fields+=") ";
        return fields;
        default :       return "CHAR(255)";

    }

}


bool hk_mysqltable::driver_specific_alter_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqltable::driver_specific_alter_table_now");
#endif
    primarystring="";
    hk_string asql="ALTER TABLE ";
    hk_string csql;
    asql+=p_identifierdelimiter+name()+p_identifierdelimiter;

    hk_string r=internal_new_fields_arguments(true);
#ifdef HK_DEBUG
    hkdebug("hk_mysqltable::primary index nach new_fields_arguments");
#endif
//  cout <<"primary index nach new_fields_arguments: "<<primarystring<<endl;
    if (r.size()>0)  csql+=r;
    r=internal_alter_fields_arguments();
//  cout <<"primary index nach alter_fields_arguments: "<<primarystring<<endl;
    if (r.size()>0)
    {
        if (csql.size()>0)csql+=" , ";
        csql+=r;
    }
    r=internal_delete_fields_arguments();
//  cout <<"primary index nach delete_fields_arguments: "<<primarystring<<endl;
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
                    primarystring+=p_identifierdelimiter+(*it)->name()+p_identifierdelimiter;
                }
            }
            if ((*it)->is_primary())dropoldprimary=true;
            it++;

        }

    }
//    cout <<"primary index nach normalen Feldern: "<<primarystring<<endl;

    hk_string pp=getprimarystring(true);
    if ((pp.size()>0&&dropoldprimary)||(pp.size()==0 &&dropoldprimary))asql+=" DROP PRIMARY KEY , ";

    //if (pp.size()>0||dropoldprimary)asql+=" DROP PRIMARY KEY , ";
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


hk_string  hk_mysqltable::internal_delete_fields_arguments(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqltable::internal_delete_fields_arguments");
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


hk_string  hk_mysqltable::internal_alter_fields_arguments(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqltable::internal_alter_fields_arguments");
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
            result+=p_identifierdelimiter+(*it).name+p_identifierdelimiter;
            result+=" ";
            if ((*it).newname=="HK_NULL") nm=(*it).name;
            else nm=(*it).newname;
            result+=p_identifierdelimiter+nm+p_identifierdelimiter;
            result+=" ";
            hk_column::enum_columntype newcolumntype=(*it).type;
            if (newcolumntype==hk_column::othercolumn) newcolumntype=c->columntype();
            result+=field2string(newcolumntype,m);
            if (((*it).primary||(*it).notnull)&&(*it).type!=hk_column::auto_inccolumn)
                result+=" NOT NULL ";
            if (newcolumntype==hk_column::auto_inccolumn||((*it).primary))
            {
                if (primarystring.size()>0)primarystring+=" , ";
                primarystring+=p_identifierdelimiter+(((*it).newname=="HK_NULL")?(*it).name:(*it).newname)+p_identifierdelimiter;
            }

        }
        it++;
    }
    delete[] m;
    return result;
}


bool hk_mysqltable::is_alteredfield(const hk_string f)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqltable::is_alteredfield");
#endif
    list<class_altercolumns>::iterator it=p_altercolumns.begin();
    while (it!=p_altercolumns.end())
    {
        if ((*it).name==f)return true;
        it++;
    }
    return false;
}


bool hk_mysqltable::is_deletedfield(const hk_string f)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqltable::is_deletedfield");
#endif
    list<hk_string>::iterator it=p_deletecolumns.begin();
    while (it!=p_deletecolumns.end())
    {
        if ((*it)==f)return true;
        it++;
    }
    return false;
}


list<hk_datasource::indexclass>* hk_mysqltable::driver_specific_indices(void)
{
    hk_datasource* d= database()->new_resultquery();
    if (d==NULL)
    {
        return NULL;
    }
    p_indices.erase(p_indices.begin(),p_indices.end());
    hk_string q="SHOW INDEX FROM ";
    q+=p_identifierdelimiter+name()+p_identifierdelimiter;
    d->set_sql(q.c_str(),true);
    d->enable();
//d->dump_data();
    unsigned long m=d->max_rows();
    hk_column* keyname=d->column_by_name("Key_name");
    hk_column* fieldname=d->column_by_name("Column_name");
    hk_column* uniquefield=d->column_by_name("Non_unique");
    if (keyname==NULL||fieldname==NULL||uniquefield==NULL)
    {
        delete d;
        return NULL;
    }

    indexclass index;
    hk_string indexname;
    for (unsigned long i=0;i<m;i++)
    {
        hk_string n=keyname->asstring(false);
        if (n!=indexname&& n!="PRIMARY")
        {
            indexname=n;
            index.name=n;
            index.unique=(strcmp(uniquefield->asstring(false).c_str(),"0")==0);
            p_indices.insert(p_indices.end(),index);
        }

        d->goto_next();
    }
    d->goto_first();
    for (unsigned long i=0;i<m;i++)
    {
        hk_string n=keyname->asstring(false);
        list<hk_datasource::indexclass>::iterator it=findindex(n);
        if (it!=p_indices.end())
        {
            (*it).fields.insert((*it).fields.end(),fieldname->asstring(false));

        }
        d->goto_next();
    }
    delete d;

    return &p_indices;
}


list<hk_datasource::indexclass>::iterator hk_mysqltable::findindex(const hk_string& i)
{

    list<hk_datasource::indexclass>::iterator it=p_indices.begin();
    while (it!=p_indices.end())
    {
        if ((*it).name==i)return it;
        it++;
    }
    return p_indices.end();
}


bool    hk_mysqltable::driver_specific_drop_index(const hk_string& i)
{
    hk_string s="ALTER TABLE ";
    s+=p_identifierdelimiter+name()+p_identifierdelimiter;
    if (i=="PRIMARY") s+=" DROP PRIMARY KEY ";
    else
    {
        s+=" DROP INDEX ";
        s+=p_identifierdelimiter+i+p_identifierdelimiter;
    }
    hk_actionquery* query= p_database->new_actionquery();
    if (query==NULL) return false;
    query->set_sql(s.c_str(),s.size());
    bool res =query->execute();
    delete query;

    return res;
}


bool    hk_mysqltable::driver_specific_create_index(const hk_string& i,bool unique, list<hk_string>& fields)
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
// cout <<"Create Index: "<<s<<endl;
    query->set_sql(s.c_str(),s.size());
    bool res =query->execute();
    delete query;

    return res;
}
