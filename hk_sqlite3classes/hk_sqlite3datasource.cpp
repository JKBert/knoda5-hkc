// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_sqlite3classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_sqlite3datasource.h"
#include "hk_sqlite3database.h"
#include "hk_sqlite3connection.h"
#include "hk_sqlite3column.h"
#include "hk_sqlite3actionquery.h"
#include <sqlite3.h>
#include <exception>
#include <new>

hk_sqlite3datasource::hk_sqlite3datasource(hk_sqlite3database* d,hk_presentation* p):hk_storagedatasource(d,p)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_sqlite3datasource::constructor");
#endif
   vm=0;
   colnums=0;
    p_sqlitedatabase=d;
    p_actionquery=new hk_sqlite3actionquery(d);
    p_enabled=false;
}


hk_sqlite3datasource::~hk_sqlite3datasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3datasource::destructor");
#endif
    if (accessmode()!=standard &&is_enabled()) driver_specific_batch_disable();
    list<coltest*>::iterator it=p_coltest.begin();
    while (it!=p_coltest.end())
    {
      coltest* t=(*it);
      ++it;
      delete t;
    }
}


list<hk_column*>* hk_sqlite3datasource::driver_specific_columns(void)
{
#ifdef HK_DEBUG
                                                  //cout <<"hk_sqlite3datasource::driver_specific_columns"<<endl;
    hkdebug("hk_sqlite3datasource::driver_specific_columns");
#endif
    if (p_columns==NULL&&type()==ds_table&&p_name.size()>0&&p_sqlitedatabase->dbhandler())
    {
        hk_string s="SELECT * FROM '"  +p_name+"' WHERE 0=1";
	vm=0;

	if (sqlite3_prepare(p_sqlitedatabase->dbhandler() ,s.c_str(),s.size(),&vm,0 ))
	{ //cerr <<"type="<<type()<<endl;
          p_sqlitedatabase->sqliteconnection()->servermessage(sqlite3_errmsg(p_sqlitedatabase->dbhandler()));
	  cerr<< "driver_specific_columns compile problem"<<s<<endl;
	  return p_columns;
	}

	colnums=0;
	int result=0;
	if (vm)
	{
	result=sqlite3_step(vm);
	colnums=sqlite3_column_count(vm);
        driver_specific_create_columns();
	sqlite3_finalize(vm);
	}


	if (result && p_sqlitedatabase->dbhandler())
        {
	  p_sqlitedatabase->sqliteconnection()->servermessage(sqlite3_errmsg(p_sqlitedatabase->dbhandler()));
  	}
	vm=0;
    }
    return p_columns;

}


bool hk_sqlite3datasource::driver_specific_create_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3datasource::driver_specific_create_columns");
#endif
  clear_columnlist();
  p_columns=new list<hk_column*>;
  list<coltest*>::iterator pit=p_coltest.begin();
  while (pit!=p_coltest.end())
  {
   delete(*pit);
  ++pit;
  }
  p_coltest.clear();
  if (type()==ds_table)
     parse_createstatement();


   int z = 0;                             //counting the fieldnumbers
   for(int i=0;i<colnums;++i)
   {
    hk_sqlite3column *col= new hk_sqlite3column(this,p_true,p_false);
    col->set_fieldnumber(z++);
    col->set_name(sqlite3_column_name(vm,i));

    hk_string coltype;
    if (sqlite3_column_decltype(vm,i)!=0)coltype=string2lower(sqlite3_column_decltype(vm,i));
    else coltype="memo";
    hk_column::enum_columntype ct=hk_column::textcolumn;
    int cs=255;
    if (coltype.find("smallint")!=hk_string::npos)
    {
      ct=hk_column::smallintegercolumn;
    }
    else
    if (coltype.find("int")!=hk_string::npos)
    {
      ct=hk_column::integercolumn;

     }
  //   delete r;

    //}
    else
    if (coltype.find("smallfloat")!=hk_string::npos)
    {
      ct=hk_column::smallfloatingcolumn;
    }
    else
    if (coltype.find("float")!=hk_string::npos)
    {
      ct=hk_column::smallfloatingcolumn;
    }
    else
    if (coltype.find("double")!=hk_string::npos)
    {
      ct=hk_column::floatingcolumn;
    }
    else
    if (coltype.find("real")!=hk_string::npos)
    {
      ct=hk_column::floatingcolumn;
    }
    else
    if (coltype.find("numeric")!=hk_string::npos)
    {
      ct=hk_column::floatingcolumn;
    }
    else
    if (coltype.find("decimal")!=hk_string::npos)
    {
      ct=hk_column::floatingcolumn;
    }
    else
    if (coltype.find("memo")!=hk_string::npos)
    {
       ct=hk_column::memocolumn;
       cs=100000;
    }
    else
    if (coltype.find("blob")!=hk_string::npos)
    {
       ct=hk_column::binarycolumn;
       cs=100000;
    }
    else
    if (coltype.find("binary")!=hk_string::npos)
    {
       ct=hk_column::binarycolumn;
       cs=100000;
    }
    else
    if (coltype.find("bool")!=hk_string::npos)
    {
       ct=hk_column::boolcolumn;
       cs=1;
    }
    else
    if (coltype.find("datetime")!=hk_string::npos)
    {
       ct=hk_column::datetimecolumn;
    }
    else
    if (coltype.find("time")!=hk_string::npos)
    {
       ct=hk_column::timecolumn;

    }
    else
    if (coltype.find("date")!=hk_string::npos)
    {
       ct=hk_column::datecolumn;

    }

    if (type()==ds_table)
    {
      list<coltest*>::iterator it=p_coltest.begin();
      while(it!=p_coltest.end())
      {
        if ((*it)->p_name==col->name())
	 {
	  if ((*it)->p_autoinc)
	    {
	      ct=hk_column::auto_inccolumn;
	      col->set_primary(true);
	      col->set_notnull(true);
	      p_primary_key_used=true;
	    }

	  if ((*it)->p_primary)
	    {
	      col->set_primary(true);
	      col->set_notnull(true);
	      p_primary_key_used=true;
	    }

	  if ((*it)->p_notnull)
	    {
	      col->set_notnull(true);
	    }

	  break;
	 }

	++it;
      }


    }




    p_columns->insert(p_columns->end(),col);
    col->set_columntype(ct);
    col->set_size(cs);


   }
   return true;
}




bool hk_sqlite3datasource::driver_specific_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3datasource::driver_specific_enable");
#endif
     bool p_cancel=false;
     int max=progressinterval();
     if (!datasource_open())
      {
       return false;
      }


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




hk_column* hk_sqlite3datasource::driver_specific_new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("driver_specific_new_column");
#endif
    hk_column* col=new hk_sqlite3column(this,p_true,p_false);

    return col;
}


bool hk_sqlite3datasource::driver_specific_insert_data(void)
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
            new_autoinc=format_number(sqlite3_last_insert_rowid(p_sqlitedatabase->dbhandler()),false,0);
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


hk_sqlite3connection* hk_sqlite3datasource::sqliteconnection(void)
{
    return p_sqlitedatabase->connection();

}


bool hk_sqlite3datasource::driver_specific_batch_enable(void)
{
   p_counter=0;

if (!datasource_open())
 return false;

if (accessmode()==batchwrite)   return true;
if (datasource_fetch_next_row())
  set_maxrows(1);
else set_maxrows(0);
return true;
}


bool hk_sqlite3datasource::driver_specific_batch_disable(void)
{
return datasource_close();
 }


bool hk_sqlite3datasource::driver_specific_batch_goto_next(void)
{
if (hk_storagedatasource::driver_specific_batch_goto_next()) return true;
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





void hk_sqlite3datasource::parse_createstatement(void)
{
hk_string createsql;
sqlite3_stmt* tmpvm=0;

hk_string s="SELECT sql FROM sqlite_master WHERE type='table' AND name='"+name()+"'";
if( sqlite3_prepare(p_sqlitedatabase->dbhandler() ,s.c_str(),s.size(),&tmpvm,0 ))
{
  p_sqlitedatabase->sqliteconnection()->servermessage(sqlite3_errmsg(p_sqlitedatabase->dbhandler()));
  cerr<< "internal_is_autoinc_column compile problem"<<endl;print_sql();
  return ;
}


int result=0;
if (tmpvm)
{//cerr <<"vor sqlite_step"<<endl;
int tmpcolnums=sqlite3_column_count(tmpvm);
result=sqlite3_step(tmpvm);
//cerr<<"sqlite_step result="<<result<<endl;

if (tmpcolnums==1&&result==SQLITE_ROW)
{
createsql=(const char*)sqlite3_column_text(tmpvm,0);
}





  hk_string::size_type startpos=createsql.find_first_of("(");
  hk_string::size_type endpos=createsql.find_last_of(")");
  if (startpos!=hk_string::npos && endpos!=hk_string::npos)
   createsql=trim(createsql.substr(startpos+1,endpos-startpos-1));

sqlite3_finalize(tmpvm);
tmpvm=0;
if (result==SQLITE_ERROR)
       {
	  p_sqlitedatabase->sqliteconnection()->servermessage(sqlite3_errmsg(p_sqlitedatabase->dbhandler()));
         return ;
	}







    hk_string::size_type offset=0;
    hk_string quotedtype;
    enum
    {
	S_START,S_IN_DOUBLEQUOTE,S_IN_QUOTE,S_IN_TAG, S_IN_BRACKETS
    } state=S_START;



//parser begin
int row=1;
int col=1;
list<hk_string> tags;
hk_string tag;

while (offset<createsql.size())
{
	hk_string xc(1,createsql[offset]);
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
			if (xc=="(")
			   {
			     tag=xc;
			     state=S_IN_BRACKETS;
			   }
			else
			if (xc=="'")
			 state=S_IN_QUOTE;
			else
			if (xc==",")
			 {
			   tag=",";
			   tags.insert(tags.end(),tag);

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
 			   tags.insert(tags.end(),tag);
			    state=S_START;
			   }
			  else tag+=xc;

			break;


	case S_IN_DOUBLEQUOTE:
			  if (xc=="\"")
			   {
 			   tags.insert(tags.end(),tag);
			    state=S_START;
			   }
			  else tag+=xc;

			break;

	case S_IN_BRACKETS:
			  if (xc==")")
			   {
			    tag+=xc;
 			   tags.insert(tags.end(),tag);
			    state=S_START;
			   }
			  else tag+=xc;

			break;


	case S_IN_TAG:
			if (isspace(xc[0]))
			 {
			   tags.insert(tags.end(),tag);
			   tag="";
			   state=S_START;

			 }
			 else
			if (xc=="(")
			{
			 tag+=xc;
			 state=S_IN_BRACKETS;
			}
			else
			if (xc==",")
			 {
			   tags.insert(tags.end(),tag);
			   tags.insert(tags.end(),",");
			   state=S_START;
			 }
			else
			 tag+=xc;
			break;
	}
	++offset;++col;

} // endloop

tags.insert(tags.end(),tag);


list<coltest*> p_cols;
coltest* currcol=NULL;
list<hk_string>::iterator it=tags.begin();
//cerr <<"createsql="<<createsql<<endl;

    enum
    {
	T_START,T_NOT,T_INTEGER,T_INTEGERPRIMARY, T_ANYTAG
    } tagstate=T_START;


while (it!=tags.end())
{
 //cerr <<"tag=#"<<*it<<"#"<<endl;
 if (currcol==NULL||*it==",")
  {
    if (currcol)
    {
     p_coltest.insert(p_coltest.end(),currcol);
     //cerr <<"currcol anh�gen"<<endl;
     tagstate=T_START;
    }
    currcol=new coltest;//cerr <<"currcol anlegen"<<currcol<<endl;
  }
  hk_string t=string2upper(*it);
  switch (tagstate)
  {
    case T_START:
	    if (currcol&& *it!=",")
	     {
	      currcol->p_name=*it;
   	      tagstate=T_ANYTAG;
	     }
    			break;
    case T_NOT:
    			if (t=="NULL")
			  currcol->p_notnull=true;
			tagstate=T_ANYTAG;
    			break;
    case T_INTEGER:	if (t=="PRIMARY")
    			  tagstate=T_INTEGERPRIMARY;
			else
			  tagstate=T_ANYTAG;
    			break;
    case T_INTEGERPRIMARY:
			  currcol->p_primary=true;
    			if (t=="KEY")
			  currcol->p_autoinc=true;
			tagstate=T_ANYTAG;
    			break;
    case T_ANYTAG:
    		if (t=="INTEGER") tagstate=T_INTEGER;
		else
    		if (t=="NOT") tagstate=T_NOT;
		else
    		if (t=="PRIMARY") currcol->p_primary=true;

    			break;
  }






 ++it;

}

if (currcol)      p_coltest.insert(p_coltest.end(),currcol);

/*
 list<coltest*>::iterator it2=p_coltest.begin();
 while(it2!=p_coltest.end())
 {
  cerr <<"it2: #"<<(*it2)->p_name<<"# \tnotnull: "<<(*it2)->p_notnull<<" \tprimary: "<<(*it2)->p_primary<<" \tautoinc: "<<(*it2)->p_autoinc<<endl;
  ++it2;
 }
*/
}
}


bool hk_sqlite3datasource::insert_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlite3datasource::insert_row()");
#endif
bool result=true;
    inform_visible_objects_before_insert_row();

    if (p_columns!=NULL)
    {   hk_string inssql;
        hk_string fields;
        list<hk_column*>::iterator it=p_columns->begin();
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
            }
        }

        if (fields.size()>0) fields+=" )";

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
                {
		    if (xc->columntype()==hk_column::binarycolumn)
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
        inssql+=")"+p_sql_delimiter;
       if (p_print_sqlstatements)cerr<<"INSSQL="<<inssql<<endl;

        if (!blockserversignals())
        {

        sqlite3_stmt* avm=0;
	if( sqlite3_prepare(p_sqlitedatabase->dbhandler() ,inssql.c_str(),inssql.size(),&avm,0 ))
	{
          p_sqlitedatabase->sqliteconnection()->servermessage(sqlite3_errmsg(p_sqlitedatabase->dbhandler()));
	  cerr <<sqlite3_errmsg(p_sqlitedatabase->dbhandler())<<" ";
	  cerr<< "compile problem"<<endl;
	  return false;
	}

	//cerr <<"sqlite_compile result="<<result<<endl;
	int rslt=0;
	if (avm)
	{
	it=p_columns->begin();
	int c=1;
        while ( it!=p_columns->end() )
        {
            hk_column* xc=*it;
            ++it;

            if (xc->has_changed()&&(xc->transformed_changed_data()->data!=NULL) &&xc->columntype()==hk_column::binarycolumn)
                {
		 sqlite3_bind_blob(avm,c,xc->transformed_changed_data()->data,
			xc->transformed_changed_data()->length,SQLITE_TRANSIENT);
		 c++;
                }


        }

	rslt=sqlite3_step(avm);
	//cerr <<"sqlite_step result="<<result<<endl;
	}
	sqlite3_finalize(avm);
	avm=0;
                if (rslt!=SQLITE_ERROR)
                {
#ifdef HK_DEBUG
                    hkdebug("Einfgen hat funktioniert");
#endif
                    driver_specific_insert_data();
                    set_has_not_changed();
                    transaction_commit();
                    goto_last();
                    inform_visible_objects_row_add();
		    result=true;
                }
                else
                {
#ifdef HK_DEBUG
                    hkdebug("FEHLER keine Datenspeicherung");
#endif
                    transaction_rollback();
	            p_sqlitedatabase->sqliteconnection()->servermessage(sqlite3_errmsg(p_sqlitedatabase->dbhandler()));
                    hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row could NOT be inserted!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
                    if (interaction==interactive) show_warningmessage(reason);
		    result=false;
                }
		execute_visible_object_after_insert();

        }
        if (result) set_has_not_changed();

    }

    if (result)
    {
    setmode_normal();
    set_has_not_changed();
    }
    return result;
}



bool hk_sqlite3datasource::update_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug( "hk_sqlite3datasource::update_row()");
#endif
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
    {
        if (interaction==interactive)show_warningmessage(replace_all("%1",hk_translate("Row in datasource '%1' could not be changed due to depending datasource(s)"),name()));

        p_mode=mode_normal;
        set_has_not_changed();

        return false;
    }
    transaction_begin();
 inform_depending_ds_before_update_row();
hk_string upsql;

   if (p_columns!=NULL)
    {
        list<hk_column*>::iterator it;
        it=p_columns->begin();
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

		    if (xc->columntype()==hk_column::binarycolumn)
		    {
			upsql+=" ? ";
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



           if (p_print_sqlstatements)cerr<<"UPSQL="<<upsql<<endl;
    set_has_not_changed();

	bool r=true;
         if (!blockserversignals())
        {

        sqlite3_stmt* avm=0;
	if( sqlite3_prepare(p_sqlitedatabase->dbhandler() ,upsql.c_str(),upsql.size(),&avm,0 ))
	{
          p_sqlitedatabase->sqliteconnection()->servermessage(sqlite3_errmsg(p_sqlitedatabase->dbhandler()));
	  cerr <<sqlite3_errmsg(p_sqlitedatabase->dbhandler())<<" ";
	  cerr<< "compile problem"<<endl;
	  return false;
	}

	//cerr <<"sqlite_compile result="<<result<<endl;
       int sqlresult=0;
	if (avm)
	{
	list<hk_column*>::iterator it=p_columns->begin();
	int c=1;
        while ( it!=p_columns->end() )
        {
            hk_column* xc=*it;
            ++it;

            if (xc->has_changed()&&(xc->transformed_changed_data()->data!=NULL) &&xc->columntype()==hk_column::binarycolumn)
                {
		 sqlite3_bind_blob(avm,c,xc->transformed_changed_data()->data,
			xc->transformed_changed_data()->length,SQLITE_TRANSIENT);
		 c++;
                }


        }

	sqlresult=sqlite3_step(avm);
	//cerr <<"sqlite_step result="<<result<<endl;
	}
	sqlite3_finalize(avm);
	avm=0;
                if (sqlresult!=SQLITE_ERROR)
                {
#ifdef HK_DEBUG
                    hkdebug("�dern hat funktioniert");
#endif
                driver_specific_update_data();
                transaction_commit();
		r=true;
                }
                else
                {
#ifdef HK_DEBUG
                    hkdebug("FEHLER keine Datenspeicherung");
#endif
                    transaction_rollback();
	            p_sqlitedatabase->sqliteconnection()->servermessage(sqlite3_errmsg(p_sqlitedatabase->dbhandler()));
                hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row could NOT be changed!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
                    if (interaction==interactive) show_warningmessage(reason);
		    r=false;
                }

        }

inform_depending_ds_after_update_row();
return r;
}


bool hk_sqlite3datasource::datasource_open(void)
{
   if (p_print_sqlstatements)   print_sql();
    if (!p_enabled)
    {
        if  (!p_sqlitedatabase||!p_sqlitedatabase->dbhandler())
	 {
	  cerr <<"error p_sqlitedatabase==NULL||p_sqlitedatabase->dbhandler()"<<endl;
	  cerr <<"db="<<p_sqlitedatabase<<" handler="<<p_sqlitedatabase->dbhandler()<<endl;
	  return false;
	 }
        vm=0;
	if (sqlite3_prepare(p_sqlitedatabase->dbhandler() ,p_sql.c_str(),p_sql.size(),&vm,0 ))
	{
          p_sqlitedatabase->sqliteconnection()->servermessage(sqlite3_errmsg(p_sqlitedatabase->dbhandler()));
	  cerr<< "driver_specific_enable compile problem"<<endl;print_sql();
	  return false;
	}
	colnums=0;
	if (vm)
	{
	colnums=sqlite3_column_count(vm);
	driver_specific_create_columns();
        }
	else return false;


     }

 return true;
}






bool hk_sqlite3datasource::datasource_fetch_next_row(void)
{
int result=0;
if (!((result=sqlite3_step(vm))==SQLITE_ROW))
{
	if (result==SQLITE_ERROR)
        {
	  p_sqlitedatabase->sqliteconnection()->servermessage(sqlite3_errmsg(p_sqlitedatabase->dbhandler()));
 	}
  return false;
}
	  list<hk_column*>::iterator colit=p_columns->begin();
          struct_raw_data* datarow=new struct_raw_data[colnums];
	   for (int col=0;col<colnums;++col)
	   {
            //neue Datenzeile erzeugen
	      const char* colvalue=NULL;
              char* dt=NULL;
           if ((*colit)->columntype()==hk_column::binarycolumn)
		{
		  colvalue=(const char*)sqlite3_column_blob(vm,col);
		  datarow[col].length=sqlite3_column_bytes(vm,col);
              if (colvalue)
	       {
	       dt=new char[datarow[col].length];
               memcpy(dt,colvalue,datarow[col].length);
	       }

		}
		else
		{
		 colvalue=(const char*)sqlite3_column_text(vm,col);
 	         hk_string n;
	         if (colvalue) n=smallstringconversion(colvalue,p_database->databasecharset(),"");
             	datarow[col].length=(colvalue?strlen(n.c_str())+1:0);
                if (colvalue)
	        {
	         dt=new char[datarow[col].length];
                 strcpy(dt,n.c_str());
	         }
		}
              //leave NULL pointer if there is no data

//	       for (unsigned int tk=0;tk<datarow[col].length;tk++) dt[tk]=data[col][tk];
              datarow[col].data=dt;
	      ++colit;
	}
//Daten speichern
	insert_data(datarow);

 return true;
}


bool hk_sqlite3datasource::datasource_close(void)
{
	sqlite3_finalize(vm);
	vm=0;



   return true;
}

