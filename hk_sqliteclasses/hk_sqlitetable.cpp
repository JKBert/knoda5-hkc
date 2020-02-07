// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_sqliteclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.12 $
#include "hk_sqlitetable.h"
#include "hk_sqlitedatabase.h"
#include <hk_actionquery.h>
hk_sqlitetable::hk_sqlitetable(hk_sqlitedatabase* db,hk_presentation* p):hk_sqlitedatasource(db,p)

{
    p_readonly=false;

}


hk_datasource::enum_datasourcetypes hk_sqlitetable::type() const
{
    return ds_table;
}


bool hk_sqlitetable::driver_specific_name(const hk_string& )
{
    return true;
}


bool hk_sqlitetable::driver_specific_enable(void)
{
//  p_readonly=false;
    return hk_sqlitedatasource::driver_specific_enable();

}


bool hk_sqlitetable::driver_specific_create_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitetable::driver_specific_create_table_now");
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


hk_string hk_sqlitetable::getprimarystring(bool alter)
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


hk_string hk_sqlitetable::internal_new_fields_arguments(bool alter)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitetable::internal_new_fields_arguments");
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

            if ((*it)->columntype()!=hk_column::auto_inccolumn&&((*it)->is_primary()))
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
    hkdebug("hk_sqlitetable::internal_new_fields_arguments   ENDE");
#endif
    return csql;
}


hk_string hk_sqlitetable::field2string(hk_column::enum_columntype f,const hk_string& m)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitetable::field2string");
#endif
    hk_string fields;
    switch (f)
    {
        case hk_column::auto_inccolumn :    return "INTEGER PRIMARY KEY";
        case hk_column::integercolumn : return "INTEGER";
        case hk_column::smallintegercolumn :    return "SMALLINT";
        case hk_column::floatingcolumn :    return "DOUBLE";
        case hk_column::smallfloatingcolumn :   return "FLOAT";
        case hk_column::datecolumn :    return "DATE";
        case hk_column::datetimecolumn :    return "DATETIME";
        case hk_column::timestampcolumn :   return "TIMESTAMP";
        case hk_column::timecolumn :    return "TIME";
        case hk_column::binarycolumn :  return "BLOB";
        case hk_column::memocolumn :    return "MEMO";
        case hk_column::boolcolumn :    return "BOOL";
        case hk_column::textcolumn :    fields +="CHAR(";
        fields+=m;
        fields+=") ";
        return fields;
        default :       return "CHAR(255)";

    }

}


bool hk_sqlitetable::driver_specific_alter_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitetable::driver_specific_alter_table_now");
#endif
return false;
}


hk_string  hk_sqlitetable::internal_delete_fields_arguments(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitetable::internal_delete_fields_arguments");
#endif
   return "";
}


hk_string  hk_sqlitetable::internal_alter_fields_arguments(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitetable::internal_alter_fields_arguments");
#endif
return "";
}


bool hk_sqlitetable::is_alteredfield(const hk_string f)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitetable::is_alteredfield");
#endif
    list<class_altercolumns>::iterator it=p_altercolumns.begin();
    while (it!=p_altercolumns.end())
    {
        if ((*it).name==f)return true;
        it++;
    }
    return false;
}


bool hk_sqlitetable::is_deletedfield(const hk_string f)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitetable::is_deletedfield");
#endif
    list<hk_string>::iterator it=p_deletecolumns.begin();
    while (it!=p_deletecolumns.end())
    {
        if ((*it)==f)return true;
        it++;
    }
    return false;
}


list<hk_datasource::indexclass>* hk_sqlitetable::driver_specific_indices(void)
{
    hk_datasource* d= database()->new_resultquery();
    
    if (d==NULL)
    {
        return NULL;
    }
    hk_string s="SELECT * FROM sqlite_master WHERE type='index' AND tbl_name='"+name()+"'";
    d->set_sql(s);
    d->enable();
    p_indices.erase(p_indices.begin(),p_indices.end());
    unsigned long max=d->max_rows();
    hk_column* namecol=d->column_by_name("name");
    hk_column* sqlcol=d->column_by_name("sql");
    if (!namecol || !sqlcol)
    {
       show_warningmessage(hk_translate("ERROR hk_sqlitetable::driver_specific_indices System columns not found!"));
       delete d;
       return &p_indices;
    }
    
    for(unsigned long i=0;i<max;++i)
    {
    
     if (sqlcol->asstring().size()>0)
     {

    indexclass index;
     index.name=namecol->asstring();
     list<hk_string> tokens;
     
     hk_string::size_type startpos=sqlcol->asstring().find_first_of("(");
     hk_string::size_type endpos=sqlcol->asstring().find_last_of(")");
     
     if (startpos!=hk_string::npos && endpos!=hk_string::npos)
     {
     parse_indices(tokens,sqlcol->asstring().substr(0,startpos-1));
     list<hk_string>::iterator it=tokens.begin();
     if (tokens.size()>2)
     {
      ++it;
      index.unique=(string2upper(*it)=="UNIQUE");
     }
     parse_indices(tokens,sqlcol->asstring().substr(startpos+1,endpos-startpos-1));
     it=tokens.begin();
     while (it!=tokens.end())
     {
      index.fields.insert(index.fields.end(),*it);
       ++it;
     }
     
       
     }

     p_indices.insert(p_indices.end(),index);
     }
     d->goto_next();
    }
    
    delete d;

    return &p_indices;
}


list<hk_datasource::indexclass>::iterator hk_sqlitetable::findindex(const hk_string& i)
{

    list<hk_datasource::indexclass>::iterator it=p_indices.begin();
    while (it!=p_indices.end())
    {
        if ((*it).name==i)return it;
        it++;
    }
    return p_indices.end();
}


bool    hk_sqlitetable::driver_specific_drop_index(const hk_string& i)
{
    hk_string s;
        s+=" DROP INDEX ";
        s+=p_identifierdelimiter+i+p_identifierdelimiter;
    
    hk_actionquery* query= p_database->new_actionquery();
    if (query==NULL) return false;
    query->set_sql(s.c_str(),s.size());
    bool res =query->execute();
    delete query;

    return res;
}


bool    hk_sqlitetable::driver_specific_create_index(const hk_string& i,bool unique, list<hk_string>& fields)
{
    hk_string s="CREATE ";
    if (unique)s+="UNIQUE ";
    s+="INDEX ";
    s+=p_identifierdelimiter+i+p_identifierdelimiter;
    s+=" ON ";
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
// cout <<"Create Index: "<<s<<endl;
    query->set_sql(s.c_str(),s.size());
    bool res =query->execute();
    delete query;

    return res;
}


void hk_sqlitetable::parse_indices(list<hk_string>& tokens, const hk_string & indiceslist)
{

    hk_string::size_type offset=0;

    enum
    {
	S_START,S_IN_DOUBLEQUOTE,S_IN_QUOTE,S_IN_TAG
    } state=S_START;

tokens.clear();
//cerr <<"   indices="<<indices<<endl;
//parser begin
int row=1;
int col=1;
hk_string tag;

while (offset<indiceslist.size())
{
	hk_string xc(1,indiceslist[offset]);
	switch (state)
	{
	case S_START:   
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
			if (xc==",")
			 {
			   //tag=",";
			   //tokens.insert(tokens.end(),tag);
			   
			 } 
			else
			 {
			  state=S_IN_TAG;
			  tag=xc;
			 }
			break;	
	case S_IN_QUOTE:
			  if (xc=="'")
			   {
 			   tokens.insert(tokens.end(),tag);
			    state=S_START;
			   } 
			  else tag+=xc;
 
			break;		
	
	
	case S_IN_DOUBLEQUOTE:
			  if (xc=="\"")
			   {
 			   tokens.insert(tokens.end(),tag);
			    state=S_START;
			   } 
			  else tag+=xc;
 
			break;		
	
	
	
	case S_IN_TAG:
			if (isspace(xc[0]))
			 {
			   tokens.insert(tokens.end(),tag);
			   tag="";
			   state=S_START;
			     
			 }
			 else
			if (xc==",")
			 {
			   tokens.insert(tokens.end(),tag);
			   state=S_START;
			 }
			else
			 tag+=xc;
			break;
	}
	++offset;++col;

} // endloop
  
if (state!=S_START)
   tokens.insert(tokens.end(),tag);

/*list<hk_string>::iterator it=tokens.begin();
while (it!=tokens.end())
{
  cerr << "token1="<<*it<<endl;
  ++it;
}
cerr <<endl;
*/
}


