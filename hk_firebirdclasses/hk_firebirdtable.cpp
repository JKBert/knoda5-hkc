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
//$Revision: 1.14 $
#include "hk_firebirdtable.h"
#include "hk_firebirddatabase.h"
#include "hk_firebirdcolumn.h"
#include "hk_actionquery.h"
#include "hk_connection.h"

hk_firebirdtable::hk_firebirdtable(hk_firebirddatabase* db,hk_presentation* p):hk_firebirddatasource(db,p)

{
   p_tempdatasource=database()->new_resultquery();
   p_fieldset=false;
}

hk_firebirdtable::~hk_firebirdtable()
{
}

hk_datasource::enum_datasourcetypes hk_firebirdtable::type() const
{
    return ds_table;
}

list<hk_column*>* hk_firebirdtable::driver_specific_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdtable::driver_specific_columns");
#endif
    hk_string s="select RDB$RELATION_FIELDS.RDB$FIELD_NAME as FIELDNAME, RDB$FIELDS.RDB$FIELD_TYPE as FIELDTYPE, \
RDB$FIELDS.RDB$FIELD_SUB_TYPE as SUBTYPE, \
RDB$FIELDS.RDB$FIELD_LENGTH as \"SIZE\", \
RDB$FIELDS.RDB$CHARACTER_LENGTH as \"CHARSIZE\", \
RDB$RELATION_FIELDS.RDB$NULL_FLAG as IS_NULLABLE, \
RDB$RELATION_FIELDS.RDB$FIELD_POSITION as \"POSITION\", \
RDB$RELATION_FIELDS.RDB$SYSTEM_FLAG as \"SYSTEMFLAG\" \
from RDB$RELATION_FIELDS, RDB$FIELDS \
WHERE RDB$FIELD_SOURCE=RDB$FIELDS.RDB$FIELD_NAME \
AND RDB$RELATION_NAME='"+name()+"' ORDER BY RDB$RELATION_FIELDS.RDB$FIELD_POSITION";
   p_tempdatasource->set_sql(s);

  if (!p_columns)
  {
   p_tempdatasource->enable();
   hk_column* fieldname=p_tempdatasource->column_by_name("FIELDNAME");
   hk_column* fieldtype=p_tempdatasource->column_by_name("FIELDTYPE");
   hk_column* fieldsubtype=p_tempdatasource->column_by_name("SUBTYPE");
   hk_column* fieldnullable=p_tempdatasource->column_by_name("IS_NULLABLE");
   hk_column* systemfield=p_tempdatasource->column_by_name("SYSTEMFLAG");
   hk_column* sizefield=p_tempdatasource->column_by_name("SIZE");
   if (!fieldname ||!fieldtype ||!fieldsubtype || !fieldnullable ||!systemfield ||!sizefield)
     {
       cerr <<"fieldname "<<fieldname
       <<" fieldtype "<<fieldtype
       <<" fieldsubtype "<<fieldsubtype
       <<" fieldnullable "<<fieldnullable
       <<" systemfield "<<systemfield
       <<" sizefield "<<sizefield
       <<endl;
       cerr<<"hk_firebirdtable::driver_specific_columns system error: Could not load system columns!"<<endl;
       p_tempdatasource->disable();
       return p_columns;
     }
    p_columns=new list<hk_column*>;
    if (!p_columns||name().size()==0)return p_columns;
    unsigned int z=0;
    while (  (z <p_tempdatasource->max_rows() ) )
    {
      hk_firebirdcolumn* col= new hk_firebirdcolumn(this,p_true,p_false);
      col->set_fieldnumber(z);
      col->set_name(trim(fieldname->asstring()));
      hk_column::enum_columntype coltype=hk_column::othercolumn;
      long int ft=fieldtype->asinteger();
      //cerr <<"COLNAME="<<fieldname->asstring()<<" TYPE="<<ft<<endl;
      switch (ft)
      {
       case 7 : {
       		 coltype=hk_column::smallintegercolumn;
       		 break;
       		}
       case 8 :
       case 16: {
       		 coltype=hk_column::integercolumn;
       		 break;
       		}
       case 12 : {
       		 coltype=hk_column::datecolumn;
       		 break;
       		}
       case 13 : {
       		 coltype=hk_column::timecolumn;
       		 break;
       		}
       case 14 :
       case 37 : {
       		 coltype=hk_column::textcolumn;
      	         if (database()->connection()->booleanemulation()&&sizefield->asinteger()==1)
      		 coltype=hk_column::boolcolumn;
       		 break;
       		}
       case 10 : {
       		 coltype=hk_column::smallfloatingcolumn;
       		 break;
       		}
       case 27 : {
       		 coltype=hk_column::floatingcolumn;
       		 break;
       		}
       case 35 : {
       		 coltype=hk_column::timestampcolumn;
       		 break;
       		}
       case 261 : {
       		 if (fieldsubtype->asinteger()==1)
       		 coltype=hk_column::memocolumn;
       		 else
       		 coltype=hk_column::binarycolumn;
       		 break;
       		}
        default:;
      }

      col->set_columntype(coltype);
      col->set_size(sizefield->asinteger());
      p_columns->insert(p_columns->end(),col);
      p_tempdatasource->goto_next();z++;
    }

  }

  if (!p_fieldset)
  {

  if (!p_tempdatasource->is_enabled()) p_tempdatasource->enable();
   hk_column* fieldname=p_tempdatasource->column_by_name("FIELDNAME");
   hk_column* fieldtype=p_tempdatasource->column_by_name("FIELDTYPE");
   hk_column* fieldsubtype=p_tempdatasource->column_by_name("SUBTYPE");
   hk_column* fieldnullable=p_tempdatasource->column_by_name("IS_NULLABLE");
   hk_column* systemfield=p_tempdatasource->column_by_name("SYSTEMFLAG");
   hk_column* sizefield=p_tempdatasource->column_by_name("SIZE");
   if (!fieldname ||!fieldtype ||!fieldsubtype || !fieldnullable ||!systemfield ||!sizefield)
     {
       cerr <<"fieldname "<<fieldname
       <<" fieldtype "<<fieldtype
       <<" fieldsubtype "<<fieldsubtype
       <<" fieldnullable "<<fieldnullable
       <<" systemfield "<<systemfield
       <<" sizefield "<<sizefield
       <<endl;
       cerr<<"hk_firebirdtable::driver_specific_columns system error: Could not load system columns for table '"+name()+"!"<<endl;
       p_tempdatasource->disable();
       return p_columns;
     }
  p_tempdatasource->goto_first();
  unsigned long m=p_tempdatasource->max_rows();
  for (unsigned long i=0;i<m;i++)
  {
  	hk_column* c=NULL;
        list<hk_column*>::iterator it;
        it=p_columns->begin();
        hk_string searchname=trim(fieldname->asstring());
        while ( it!=p_columns->end())
        {
            hk_column* xc=*it;
            ++it;
            if (xc->name()==searchname) c=xc;

        }
  	if (c)
  	{
  	c->set_notnull(fieldnullable->asinteger()==1);
  	}
  	else
  	{
  	 show_warningmessage("Column: '"+trim(fieldname->asstring()+"' not found"));
  	}
  	p_tempdatasource->goto_next();

  }
  // now check for auto_inc
  p_tempdatasource->disable();
  s="SELECT RDB$TRIGGER_SOURCE AS DEFINITION FROM RDB$TRIGGERS WHERE RDB$TRIGGER_TYPE=1 AND RDB$RELATION_NAME ='"+name()+"'";
  p_tempdatasource->set_sql(s);
  p_tempdatasource->enable();
   hk_column* triggerdef=p_tempdatasource->column_by_name("DEFINITION");
   if (!triggerdef)
   {
       show_warningmessage("hk_firebirdtable::driver_specific_columns system error: Could not load trigger system column!");
       p_tempdatasource->disable();
       return p_columns;
   }

    unsigned long z=0;
    while (  (z <p_tempdatasource->max_rows() ) )
    {
      hk_string generator, column;
      //cerr <<"Suche in: "<<triggerdef->asstring()<<endl<<endl;
      if (parse_for_generatorname(triggerdef->asstring(),generator,column))
      {
       z=p_tempdatasource->max_rows();
       //cerr <<"gefundener generator="<<generator<<" Spalte: "<<column<<endl;
  	hk_firebirdcolumn* c=NULL;
        list<hk_column*>::iterator it;
        it=p_columns->begin();
        hk_string searchname=trim(column);
        while ( it!=p_columns->end())
        {
            hk_column* xc=*it;
            ++it;
            if (xc->name()==searchname) c=dynamic_cast<hk_firebirdcolumn*>(xc);

        }
  	if (c)
  	{
  	  c->p_generatorname=generator;
  	  c->set_columntype(hk_column::auto_inccolumn);
  	}
      }
      else
      {
//         cerr <<"kein generator"<<endl;
      }
     p_tempdatasource->goto_next();
     ++z;
    }





  // now check for primary key
  p_tempdatasource->disable();
  s="SELECT C.RDB$CONSTRAINT_NAME AS NAME, \
F.RDB$FIELD_NAME AS FIELD \
FROM RDB$RELATION_CONSTRAINTS C, RDB$INDICES I, RDB$INDEX_SEGMENTS F \
WHERE C.RDB$CONSTRAINT_TYPE='PRIMARY KEY' AND C.RDB$INDEX_NAME=I.RDB$INDEX_NAME \
AND I.RDB$INDEX_NAME=F.RDB$INDEX_NAME AND C.RDB$RELATION_NAME='"+name()+"'";
  p_tempdatasource->set_sql(s);
  p_tempdatasource->enable();
   hk_column* keyname=p_tempdatasource->column_by_name("NAME");
   hk_column* keyfield=p_tempdatasource->column_by_name("FIELD");

     if (!keyname ||!keyfield)
     {
       cerr <<"keyname "<<keyname
       <<" keyfield "<<keyfield
       <<endl;
       show_warningmessage("hk_firebirdtable::driver_specific_columns system error: Could not load primary system columns!");
       p_tempdatasource->disable();
       return p_columns;
     }
  m=p_tempdatasource->max_rows();
  for (unsigned long i=0;i<m;i++)
  {     if (i==0) p_primary_key_name=trim(keyname->asstring());
  	hk_column* c=NULL;
        list<hk_column*>::iterator it;
        it=p_columns->begin();
        hk_string searchname=trim(keyfield->asstring());
        while ( it!=p_columns->end())
        {
            hk_column* xc=*it;
            ++it;
            if (xc->name()==searchname) c=xc;

        }
  	if (c)
  	{
  	c->set_primary(true);
  	p_primary_key_used=true;
  	}
  	else
  	{
  	 show_warningmessage("Column: '"+trim(fieldname->asstring()+"' not found"));
  	}
  	p_tempdatasource->goto_next();

  }



  p_fieldset=true;
  } //END p_fielset
  p_tempdatasource->disable();
return p_columns;
}


bool hk_firebirdtable::driver_specific_enable(void)
{
p_fieldset=false;
  return hk_firebirddatasource::driver_specific_enable();
}


list<hk_datasource::indexclass>* hk_firebirdtable::driver_specific_indices(void)
{
//return NULL; //for testing reasons !! remove later

    if (p_tempdatasource==NULL)
    {
        return NULL;
    }
        p_tempdatasource->disable();
    hk_string s="SELECT I.RDB$INDEX_NAME as INDEXNAME , \
I.RDB$UNIQUE_FLAG as IS_UNIQUE, S.RDB$FIELD_NAME as FIELDNAME \
FROM RDB$INDICES I, RDB$INDEX_SEGMENTS S WHERE  I.RDB$INDEX_NAME=S.RDB$INDEX_NAME \
AND RDB$RELATION_NAME='"+name()+"'";
    p_tempdatasource->set_sql(s);
    p_tempdatasource->enable();


    p_indices.erase(p_indices.begin(),p_indices.end());
    unsigned long m=p_tempdatasource->max_rows();
    hk_column* keyname=p_tempdatasource->column_by_name("INDEXNAME");
    hk_column* fieldname=p_tempdatasource->column_by_name("FIELDNAME");
    hk_column* uniquefield=p_tempdatasource->column_by_name("IS_UNIQUE");
     if (!keyname ||!fieldname||!uniquefield)
     {
       cerr <<"keyname "<<keyname
       <<" fieldname "<<fieldname
       <<" uniquefield "<<uniquefield
       <<endl;
       show_warningmessage("hk_firebirdtable::driver_specific_indices system error: Could not load primary system columns!");
       p_tempdatasource->disable();
       return NULL;
     }

    indexclass index;                             //cout <<"Anzahl indices: "<<m<<endl;
    hk_string indexname;
    for (unsigned long i=0;i<m;i++)
    {
        hk_string n=trim(keyname->asstring());
        if (n!=indexname && n!=p_primary_key_name)
        {
            indexname=trim(n);
            index.name=indexname;
            index.unique=uniquefield->asbool();
            p_indices.insert(p_indices.end(),index);
        }
        p_tempdatasource->goto_next();
    }
    p_tempdatasource->goto_first();
    for (unsigned long i=0;i<m;i++)
    {
        hk_string n=trim(keyname->asstring());
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


list<hk_datasource::indexclass>::iterator hk_firebirdtable::findindex(const hk_string& i)
{

    list<hk_datasource::indexclass>::iterator it=p_indices.begin();
    while (it!=p_indices.end())
    {
        if ((*it).name==i)return it;
        it++;
    }
    return p_indices.end();
}


bool hk_firebirdtable::driver_specific_create_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdtable::driver_specific_create_table_now");
#endif
    hk_string csql="CREATE TABLE ";
    primarystring="";
    //csql+=replace_all(" ",name(),"_");
    csql+=p_identifierdelimiter+name()+p_identifierdelimiter;
    csql+=" ( ";
    csql+=internal_new_fields_arguments(false);
    csql+=getprimarystring(false)+" ) ";

// cout <<"hk_firebirdtable::driver_specific_create_table_now CREATE definition: "<<endl<<csql<<endl;
    hk_actionquery* query= p_database->new_actionquery();
    query->set_sql(csql.c_str(),csql.size());
    bool result=query->execute();
// if (result) cout<<"tabelle angelegt"; else cout<<"fehler";cout <<endl;
    if (result)
     {
      list<hk_column*>::iterator it=p_newcolumns.begin();
      while (it!=p_newcolumns.end())
      {
        if ((*it)->columntype()==hk_column::auto_inccolumn)
           create_autoinc_field((*it)->name());
        ++it;
      }


     }
    delete query;
    return result;
}


hk_string hk_firebirdtable::getprimarystring(bool alter)
{
    if (primarystring.size()==0)return "";
    hk_string key=", ";
    if (alter) key+="ADD ";
    key+="CONSTRAINT PK_"+name()+" PRIMARY KEY ( ";
    key=key+primarystring+" )";
    return key;

}


hk_string hk_firebirdtable::internal_new_fields_arguments(bool alter)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdtable::internal_new_fields_arguments");
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
            if (alter) fields =" ADD "; else fields="";
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
                if (((*it)->is_notnull()||(*it)->is_primary())||(*it)->columntype()==hk_column::auto_inccolumn)
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
    hkdebug("hk_firebirdtable::internal_new_fields_arguments   ENDE");
#endif
    return csql;
}


hk_string hk_firebirdtable::field2string(hk_column::enum_columntype f,const hk_string& m)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdtable::field2string");
#endif
    hk_string fields;
    switch (f)
    {
        case hk_column::auto_inccolumn :    return "BIGINT";
        case hk_column::integercolumn : return "BIGINT";
        case hk_column::smallintegercolumn :    return "SMALLINT";
        case hk_column::floatingcolumn :    return "DOUBLE PRECISION";
        case hk_column::smallfloatingcolumn :   return "FLOAT";
        case hk_column::datecolumn :    return "DATE";
        case hk_column::timestampcolumn :   return "TIMESTAMP";
        case hk_column::timecolumn :    return "TIME";
        case hk_column::binarycolumn :  return "BLOB SUB_TYPE 0";
        case hk_column::memocolumn :    return "BLOB SUB_TYPE 1";
        case hk_column::boolcolumn :    return "CHAR(1)";
        case hk_column::textcolumn :
        				{
        				fields ="VARCHAR("+m+") ";
        				return fields;
        				}
        default :       return "VARCHAR(255)";

    }

}


bool hk_firebirdtable::driver_specific_alter_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdtable::driver_specific_alter_table_now");
#endif
    hk_string asql="ALTER TABLE ";
    hk_string csql;
    asql+=p_identifierdelimiter+name()+p_identifierdelimiter;
    hk_string r=internal_new_fields_arguments(true);
    if (r.size()>0)  csql+=r;
    r=internal_alter_fields_arguments();
//  cout <<"primary index nach alter_fields_arguments: "<<primarystring<<endl;
    if (r.size()>0)
    {
        if (csql.size()>0)csql=" , "+csql;
        csql+=r;
    }
    r=internal_delete_fields_arguments();
//  cout <<"primary index nach delete_fields_arguments: "<<primarystring<<endl;
    if (r.size()>0)
    {
        if (csql.size()>0)csql+=" , ";
        csql+=r;
    }
    csql=asql+csql;

    hk_actionquery* query= p_database->new_actionquery();
    if (!query) return false;

    query->set_sql(csql.c_str(),csql.size());
    bool result=query->execute();


        if (result)
     {
      list<hk_column*>::iterator it=p_newcolumns.begin();
      while (it!=p_newcolumns.end())
      {
        if ((*it)->columntype()==hk_column::auto_inccolumn)
           create_autoinc_field((*it)->name());
        ++it;
      }

    list<class_altercolumns>::iterator ait=p_altercolumns.begin();
    while (ait!=p_altercolumns.end())
    {
        hk_column*c =column_by_name((*ait).name.c_str());
        if (c!=NULL)
        {
          if ((*ait).type==hk_column::auto_inccolumn && c->columntype()!=(*ait).type==hk_column::auto_inccolumn)
          {
          create_autoinc_field((*ait).name);
          }
        }
     ++ait;
    }

     }

    delete query;
    return result;
}




hk_string  hk_firebirdtable::internal_alter_fields_arguments(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdtable::internal_alter_fields_arguments");
#endif
    if (p_altercolumns.size()==0)return "";
  //  char* m= new char[50];
  hk_string m;
    hk_string result;
    
    list<class_altercolumns>::iterator it=p_altercolumns.begin();
    while (it!=p_altercolumns.end())
    {
        hk_column*c =column_by_name((*it).name.c_str());
        if (c!=NULL)
        {
            if ((*it).size<0)m=longint2string((c->size()<256)?c->size():255);
            else m=longint2string((*it).size);

            if (result.size()>0)result+=" , ";
            result+=" ALTER COLUMN ";
            result+=p_identifierdelimiter+(*it).name+p_identifierdelimiter;
            result+=" ";
            if ((*it).newname!="HK_NULL" &&(*it).newname.size()>0 && (*it).name!=(*it).newname) result+=" TO "+p_identifierdelimiter+(*it).newname+p_identifierdelimiter;
            result+=" ";
            hk_column::enum_columntype newcolumntype=(*it).type;

            if ((newcolumntype!=c->columntype() && newcolumntype!=hk_column::othercolumn)
               ||(c->size()!=(*it).size&&(*it).size>0)
            )
            {
            if (newcolumntype==hk_column::othercolumn)newcolumntype=c->columntype();
            result+=" TYPE "+field2string(newcolumntype,m);
            }
            if (((*it).primary||(*it).notnull)&&(*it).type!=hk_column::auto_inccolumn)
                result+=" NOT NULL ";

        }
        it++;
    }
//    delete[] m;
    return result;
}


hk_string  hk_firebirdtable::internal_delete_fields_arguments(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_firebirdtable::internal_delete_fields_arguments");
#endif

    if (p_deletecolumns.size()==0)return "";
    hk_string result;

    list<hk_string>::iterator it=p_deletecolumns.begin();
    while (it!=p_deletecolumns.end())
    {
        if (result.size()>0) result+=" , ";
        result+=" DROP ";
        result+=p_identifierdelimiter+(*it)+p_identifierdelimiter;
        it++;
    }
    return result;
}


bool    hk_firebirdtable::driver_specific_create_index(const hk_string& i,bool unique, list<hk_string>& fields)
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


bool    hk_firebirdtable::driver_specific_drop_index(const hk_string& i)
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


bool hk_firebirdtable::parse_for_generatorname(const hk_string & t, hk_string& generator, hk_string& column)
{
    hk_string::size_type offset=0;

    enum
    {
	S_START,S_IN_DOUBLEQUOTE,S_IN_QUOTE,S_IN_TAG
    } state=S_START;

int row=1;
int col=1;
hk_string tag;
bool in_gen_id=false;
bool get_columnname=false;
while (offset<t.size())
{
	hk_string xc(1,t[offset]);
	switch (state)
	{
	case S_START:  //cerr<<"S_START"<<endl;
			tag="";
			if (isspace(xc[0]))
				{
					if (xc=="\n")
					{
						++row;col=0;
					}
				  break;
				}
			else
			if (xc=="\"")
			   {
			     state=S_IN_DOUBLEQUOTE;
			   }
			else
			if (xc=="'")
			 state=S_IN_QUOTE;
			else
			if (xc=="(" && in_gen_id)
			{
			  state=S_IN_TAG;
			}
			else
			 {
			  state=S_IN_TAG;
			  tag=string2upper(xc);
			 }
			break;
	case S_IN_QUOTE://cerr<<"S_IN_QUOTE"<<endl;
			  if (xc=="'")
			   {
 			    if (in_gen_id)
 			    {
				generator=tag;
				return true;
 			    }
 			    else
 			    {
 			    tag="";
			    state=S_START;
			    }
			   }
			  else
			  {
			  tag+=xc;
			  //cerr<<"tag=#"<<tag<<"#"<<endl;
			  }

			break;


	case S_IN_DOUBLEQUOTE://cerr<<"S_IN_DOUBLE_QUOTE"<<endl;
			  if (xc=="\"")
			   {
			  if (column.size()>0)
			  {
			   if (in_gen_id)
			   {
			    generator=tag;
			    return true;
			   }
			  }
 			   else
 			   {
			  if (get_columnname)
			  {
			    column=tag;//cerr <<"SET COLUMN="<<column<<endl;
			    state=S_START;
			    get_columnname=false;
			    break;
			  }
 			   tag="";
			    state=S_START;
			   }
			   }
			  else
			  {
			  tag+=xc;
			  //cerr<<"tag=#"<<tag<<"#"<<endl;
			  }

			break;



	case S_IN_TAG://cerr<<"S_IN_TAG"<<endl;
			if (xc=="." && string2upper(tag)=="NEW")
			{
			    get_columnname=true;
			    //cerr <<"NEW found! get_columnname=true"<<endl;
			    state=S_START;
			    break;
			}
			if (xc=="(" &&string2upper(tag)=="GEN_ID")
			 {
			  	in_gen_id=true;
			   tag="";
			   state=S_START;
                         }
			if (isspace(xc[0])|| tag==",")
			 {
			  if (get_columnname)
			  {
			    column=tag;//cerr <<"SET COLUMN="<<column<<endl;
			    state=S_START;
			    get_columnname=false;
			    break;
			  }


			  if (column.size()>0)
			  {
			   if (in_gen_id)
			   {
			    generator=tag;
			    return true;
			   }
			  }
			   if (string2upper(tag)=="GEN_ID")
			     	in_gen_id=true;
			   tag="";
			   state=S_START;

			 }
			else
			{
			  if (in_gen_id &&xc==")")
			  {
			    generator=tag;
			    return true;
			  }
			  else
  			  if (in_gen_id &&xc==",")
			  {
			    generator=tag;
			    return true;
			  }

			}
			  {
			  tag+=string2upper(xc);
			  //cerr<<"tag=#"<<tag<<"#"<<endl;
			  }
			break;
	}
	++offset;++col;

} // endloop

return false;
}

void hk_firebirdtable::create_autoinc_field(const hk_string& field)
{
   hk_actionquery* aq=database()->new_actionquery();
   if (!aq)
    {
      show_warningmessage("hk_firebirdtable::create_autoinc_field  Error: no actionquery!");
      return;
    }

    p_tempdatasource->disable();
    p_tempdatasource->set_sql("SELECT RDB$GENERATOR_NAME AS NAME FROM RDB$GENERATORS");
    if (!p_tempdatasource->enable())
    {
     show_warningmessage("Error could not enable RDB$GENERATORS query!");
     delete aq;
     return;
    }
    hk_column* gname=p_tempdatasource->column_by_name("NAME");
    if (! gname)
     {
        show_warningmessage("Could not find RDB$GENERATORS system column");
        delete aq;
        return;
     }
     int i=0;
     bool newname=false;
     hk_string newgenerator;
     while (!newname)
     {
       newgenerator=name()+field+longint2string(i)+"_GEN";
       newname=(gname->find(newgenerator)>p_tempdatasource->max_rows());
       ++i;
     }
     hk_string s="CREATE GENERATOR \""+newgenerator+"\"";
     aq->set_sql(s.c_str(),s.size());
     if (!aq->execute())
      {
         show_warningmessage("Warning: Generator '"+newgenerator+"' could not be created!");
      }

     i=0;
     newname=false;
     hk_string newtrigger;
     while (!newname)
     {
       newtrigger=name()+field+longint2string(i)+"_BI";
       newname=(gname->find(newtrigger)>p_tempdatasource->max_rows());
     }
     s="CREATE TRIGGER \""+newtrigger+"\" FOR \""+name()+"\" ACTIVE BEFORE INSERT AS\n";
     hk_string nf="NEW.\""+field+"\"";
     s+="BEGIN IF ("+nf+" IS NULL ) THEN "+nf+" = GEN_ID(\""+newgenerator+"\", 1); END";
     aq->set_sql(s.c_str(),s.size());
     if (!aq->execute())
      {
         show_warningmessage("Warning: Trigger '"+newtrigger+"' could not be created!");
      }





    delete aq;
}


void hk_firebirdtable::driver_specific_referenceslist(void)
{

hk_string foreignkeys="select C.RDB$CONSTRAINT_NAME RELNAME,C.RDB$RELATION_NAME CHILD,F.RDB$UPDATE_RULE UPDATERULE, F.RDB$DELETE_RULE DELETERULE from RDB$RELATION_CONSTRAINTS C,RDB$REF_CONSTRAINTS F WHERE C.RDB$CONSTRAINT_TYPE='FOREIGN KEY' AND C.RDB$CONSTRAINT_NAME=F.RDB$CONSTRAINT_NAME  AND C.RDB$RELATION_NAME='"+name()+"'";


hk_string child="SELECT C.RDB$CONSTRAINT_NAME , C.RDB$RELATION_NAME CHILD, S1.RDB$FIELD_NAME KEYON \
FROM RDB$RELATION_CONSTRAINTS C, RDB$INDICES I1,RDB$INDEX_SEGMENTS S1 \
WHERE C.RDB$CONSTRAINT_TYPE='FOREIGN KEY' AND I1.RDB$INDEX_NAME=C.RDB$INDEX_NAME AND I1.RDB$INDEX_NAME=S1.RDB$INDEX_NAME  order by S1.RDB$FIELD_POSITION ";




hk_string parent="select C.RDB$CONSTRAINT_NAME ,S2.RDB$FIELD_NAME REFERENCEDFIELD,I2.RDB$RELATION_NAME PARENT \
from RDB$RELATION_CONSTRAINTS C, RDB$INDICES I1, RDB$INDICES I2, RDB$INDEX_SEGMENTS S2 \
WHERE C.RDB$CONSTRAINT_TYPE='FOREIGN KEY' AND I1.RDB$INDEX_NAME=C.RDB$INDEX_NAME AND I2.RDB$INDEX_NAME=S2.RDB$INDEX_NAME AND I1.RDB$FOREIGN_KEY=I2.RDB$INDEX_NAME ORDER BY S2.RDB$FIELD_POSITION";


hk_datasource*fk =database()->new_resultquery();
hk_datasource*d =database()->new_resultquery();
hk_datasource*p =database()->new_resultquery();
if (!fk||!d||!p )
{
  show_warningmessage("Error:hk_firebirddatasource::driver_specific_referenceslist\nNo datasource");
  return;
}
    fk->set_sql(foreignkeys);
    d->set_sql(child);
    d->set_depending_on(fk);
    d->add_depending_fields("RDB$CONSTRAINT_NAME","RELNAME");
    p->set_sql(parent);
    p->set_depending_on(fk);
    p->add_depending_fields("RDB$CONSTRAINT_NAME","RELNAME");
    fk->enable();
    if (fk->max_rows()==0)
    {
      fk->disable();
      delete d;
      delete p;
      delete fk;
      return;
    }
    hk_column* ref_is_on=fk->column_by_name("RELNAME");
    hk_column* updaterule=fk->column_by_name("UPDATERULE");
    hk_column* deleterule=fk->column_by_name("DELETERULE");
    unsigned int fkpos=0;
    while (fkpos<fk->max_rows()) // main while loop
    {
     hk_column* keyon=d->column_by_name("KEYON");

     hk_column* referencedfield=p->column_by_name("REFERENCEDFIELD");
     hk_column* parentname=p->column_by_name("PARENT");
     if (!ref_is_on||!keyon||!referencedfield||!parentname||!updaterule||!deleterule)
     {
      cerr <<"ref_is_on"<<ref_is_on<<" keyon"<<keyon<<" referencedfield"<<referencedfield<<" parentname"<<parentname<<" UPDATERULE"<<updaterule<<" DELETERULE"<<deleterule<<endl;
      show_warningmessage("Error:hk_firebirddatasource::driver_specific_referenceslist\nNo system table!");
      return;
     }

      referentialclass ref;
      ref.p_masterdatasource=trim(parentname->asstring());
      ref.p_name=trim(ref_is_on->asstring());
      //cerr <<"datasource:"<<name()<<" master:"<<ref.p_masterdatasource<<endl;
      ref.p_updatecascade=(trim(updaterule->asstring())=="CASCADE");
      //cerr <<"upd:"<<updaterule->asstring()<<endl;
      ref.p_deletecascade=(trim(deleterule->asstring())=="CASCADE");
      //cerr <<"del:"<<deleterule->asstring()<<endl;
      int i=0;
      while (i<(int)d->max_rows())
       {
        dependingclass dep;
        dep.masterfield=trim(referencedfield->asstring_at(i));
        dep.dependingfield=trim(keyon->asstring_at(i));
        ref.p_fields.push_back(dep);
        //cerr <<"  masterfield:#"<<dep.masterfield<<"#"<<endl;
        //cerr <<"  dependingfield:#"<<dep.dependingfield<<"#"<<endl;
        ++i;
       }

      p_references.push_back(ref);
      //cerr <<"add_reference:"<<ref.p_name<<endl;
      //cerr <<"update cascade:"<<ref.p_updatecascade<<endl;
      //cerr <<"delete cascade:"<<ref.p_deletecascade<<endl;
      //cerr <<ref.p_masterdatasource<<endl;
      ++fkpos;
      fk->goto_next();
    } // end main while
    fk->disable();
    delete fk;
    delete d;
    delete p;
    d=NULL;
    p=NULL;
    fk=NULL;
}



