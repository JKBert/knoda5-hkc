// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_xbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.4 $
#include "hk_xbasetable.h"
#include "hk_xbasedatabase.h"
#include <hk_actionquery.h>
hk_xbasetable::hk_xbasetable(hk_xbasedatabase* db,hk_presentation* p):hk_xbasedatasource(db,p)

{

}


hk_datasource::enum_datasourcetypes hk_xbasetable::type() const
{
    return ds_table;
}

bool hk_xbasetable::driver_specific_create_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasetable::driver_specific_create_table_now");
#endif
    
    
/*    xbSchema* schema= new xbSchema  [p_newcolumns.size()+1] ;

    list<hk_column*>::iterator it=p_newcolumns.begin();
    int i=0;
    while (it!=p_newcolumns.end())
    {
        char t='C';
	int size=(*it)->size();
	int s2=0;
	switch ((*it)->columntype())
	{
	 case hk_column::integercolumn: 	t='N';
	 					size=17;s2=0;
	 					break;
	 case hk_column::smallintegercolumn: 	t='N';
	 					size=6;s2=0;
	 					break;
	 case hk_column::floatingcolumn: 	t='F';
	 					size=17;
	 					break;
	 case hk_column::smallfloatingcolumn: 	t='F';
	 					size=8;
	 					break;
	 case hk_column::datecolumn: 		t='D';
	 					size=8;
	 					break;
	 case hk_column::boolcolumn: 		t='L';
	 					size=1;
	 					break;
	 case hk_column::memocolumn: 		t='M';
	 					size=10;
	 					break;
         default:;	
	}
	strncpy(schema[i].FieldName,(*it)->name().c_str(),sizeof(xbSchema));
	schema[i].Type=t;
	schema[i].FieldLen=size;
	schema[i].NoOfDecs=s2;
	cerr <<"add col="<<schema[i].FieldName<<" type:"<<schema[i].Type<<endl;
	++it;++i;
    }
   
	schema[i].FieldName[0]=0;
	schema[i].Type=0;
	schema[i].FieldLen=0;
	schema[i].NoOfDecs=0;
   
   
   xbXBase x;
   xbDbf newtable(&x);
   newtable.SetVersion(4);
   hk_string tname=database()->database_path()+"/"+name()+".dbf";
   if (newtable.CreateDatabase(tname.c_str(),schema,XB_OVERLAY)!=XB_NO_ERROR)
   {  show_warningmessage("Error creating table");
     return false;
   } 
   newtable.OpenDatabase(tname.c_str());
   int max=newtable.FieldCount(); 
   int c=0;
   while (c<max)
   {
   cerr <<"name #"<<newtable.GetFieldName(c) <<"# type: "<<newtable.GetFieldType(c)  <<endl;
   ++c;
    }
    return true; 
 */   
    
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

hk_string hk_xbasetable::getprimarystring(bool alter)
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


hk_string hk_xbasetable::internal_new_fields_arguments(bool alter)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasetable::internal_new_fields_arguments");
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
/*            if (((*it)->is_notnull()||(*it)->is_primary())&&(*it)->columntype()!=hk_column::auto_inccolumn)
                fields+=" NOT NULL ";*/
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
    hkdebug("hk_sqlite3table::internal_new_fields_arguments   ENDE");
#endif
    return csql;
}


hk_string hk_xbasetable::field2string(hk_column::enum_columntype f,const hk_string& m)
{
#ifdef HK_DEBUG
    hkdebug("hk_xbasetable::field2string");
#endif
    hk_string fields;
    switch (f)
    {
        case hk_column::integercolumn : return "INT";
        case hk_column::smallintegercolumn :    return "INT";
        case hk_column::floatingcolumn :    return "DOUBLE";
        case hk_column::smallfloatingcolumn :   return "DOUBLE";
        case hk_column::datecolumn :    return "DATE";
        case hk_column::memocolumn :    return "BLOB";
        case hk_column::boolcolumn :    return "bool";
        case hk_column::textcolumn :    fields +="CHAR(";
        fields+=m;
        fields+=") ";
        return fields;
        default :       return "CHAR(255)";

    }

}

