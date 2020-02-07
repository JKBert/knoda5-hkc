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
#include "hk_sqlitedatasource.h"
#include "hk_sqlitedatabase.h"
#include "hk_sqliteconnection.h"
#include "hk_sqlitecolumn.h"
#include "hk_sqliteactionquery.h"
#include <sqlite.h>
#include <exception>
#include <new>

hk_sqlitedatasource::hk_sqlitedatasource(hk_sqlitedatabase* d,hk_presentation* p):hk_storagedatasource(d,p)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_sqlitedatasource::constructor");
#endif
   vm=0;
   data=0;
   colnums=0;
   colnames=0;
    p_sqlitedatabase=d;
    p_actionquery=new hk_sqliteactionquery(d);
    p_enabled=false;
}


hk_sqlitedatasource::~hk_sqlitedatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitedatasource::destructor");
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


list<hk_column*>* hk_sqlitedatasource::driver_specific_columns(void)
{
#ifdef HK_DEBUG
                                                  //cout <<"hk_sqlitedatasource::driver_specific_columns"<<endl;
    hkdebug("hk_sqlitedatasource::driver_specific_columns");
#endif
    if (p_columns==NULL&&type()==ds_table&&p_name.size()>0&&p_sqlitedatabase->dbhandler())
    {
	char* errormsg=0;
        hk_string s="SELECT * FROM '"  +p_name+"' WHERE 0=1"; 
	vm=0;
	int result= sqlite_compile(p_sqlitedatabase->dbhandler() ,s.c_str(),0,&vm,&errormsg );
	if (result!=SQLITE_OK)
	{ //cerr <<"type="<<type()<<endl;
          p_sqlitedatabase->sqliteconnection()->servermessage(errormsg);
	  sqlite_freemem(errormsg);
	  errormsg=0;
	  cerr<< "driver_specific_columns compile problem"<<s<<endl;
	  return p_columns;
	}
            
	colnums=0;
	data=0;
	colnames=0;
	if (vm)
	{
	result=sqlite_step(vm,&colnums,&data,&colnames);
	}
	
	
        driver_specific_create_columns();
	sqlite_finalize(vm,&errormsg);
	vm=0;
	if (result==SQLITE_ERROR) 
        {
	  p_sqlitedatabase->sqliteconnection()->servermessage(errormsg);
	  sqlite_freemem(errormsg);
	  errormsg=0;
  	}  

    }
    return p_columns;

}


bool hk_sqlitedatasource::driver_specific_create_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitedatasource::driver_specific_create_columns");
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
if (!colnames) return false;
        int z = 0;                                //counting the fieldnumbers
   for(int i=0;i<colnums;++i)
   {
    hk_sqlitecolumn *col= new hk_sqlitecolumn(this,p_true,p_false);
    col->set_fieldnumber(z++);
    col->set_name(colnames[i]);
    //cerr <<colnames[i]<<"("<<colnames[colnums+i]<<")"<<endl;
    hk_string coltype;
    if (colnames[colnums+i])
    coltype=string2lower(colnames[colnums+i]);
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
    if (coltype.find("real")!=hk_string::npos)
    {
      ct=hk_column::floatingcolumn;
    }
else
    if (coltype.find("double")!=hk_string::npos)
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




bool hk_sqlitedatasource::driver_specific_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_sqlitedatasource::driver_specific_enable");
#endif
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
        if (accessmode()==batchwrite)
        {
            clear_columnlist();
            driver_specific_create_columns();
            return true;
        }
	char* errormsg=0;
	int result= sqlite_compile(p_sqlitedatabase->dbhandler() ,p_sql.c_str(),0,&vm,&errormsg );
	if (result!=SQLITE_OK)
	{
          p_sqlitedatabase->sqliteconnection()->servermessage(errormsg);
	  sqlite_freemem(errormsg);
	  errormsg=0;
	  cerr<< "driver_specific_enable compile problem"<<endl;print_sql();
	  return false;
	}
            
	colnums=0;
	data=0;
	colnames=0;
	if (vm)
	{
	int rc=p_enablefunctioncounter;
     bool p_cancel=false;
     int max=progressinterval();
     int i=1;
	while ((result=sqlite_step(vm,&colnums,&data,&colnames))==SQLITE_ROW && !p_cancel)
	{ 
          struct_raw_data* datarow=new struct_raw_data[colnums];
	   for (int col=0;col<colnums;++col)
	   {
            //neue Datenzeile erzeugen
	      hk_string n;
	      if (data[col]) n=smallstringconversion(data[col],p_database->databasecharset(),"");
              datarow[col].length=(data[col]?strlen(n.c_str())+1:0); 
              char* dt=NULL;
              //leave NULL pointer if there is no data
              if (data[col])
	       {
	       dt=new char[datarow[col].length];
               strcpy(dt,n.c_str());
	       }
		
//	       for (unsigned int tk=0;tk<datarow[col].length;tk++) dt[tk]=data[col][tk];
              datarow[col].data=dt;
	      if (p_enablefunctioncounter>0 &&p_enablefunction)
	      {
	        if (rc>0)--rc;
		else
		{
		  rc=p_enablefunctioncounter;
		  p_enablefunction();
		}
	      }
	   }
	 if (progressdialog() && (i %15000 ==0))
	 {
          p_cancel=progressdialog()(i,max,hk_translate("Executing query ..."));
	 }
	 ++i;
	if (i>max-30000)max+=10000;  
//Daten speichern
	insert_data(datarow);
        }
       driver_specific_create_columns();
	
	sqlite_finalize(vm,&errormsg);
	vm=0;
	if (result==SQLITE_ERROR) 
        {
	  p_sqlitedatabase->sqliteconnection()->servermessage(errormsg);
	  sqlite_freemem(errormsg);
	  errormsg=0;
         return false;
	}  
	}
	    
   }
   return true;	    
}




hk_column* hk_sqlitedatasource::driver_specific_new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("driver_specific_new_column");
#endif
    hk_column* col=new hk_sqlitecolumn(this,p_true,p_false);

    return col;
}


bool hk_sqlitedatasource::driver_specific_insert_data(void)
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
            new_autoinc=format_number(sqlite_last_insert_rowid(p_sqlitedatabase->dbhandler()),false,0);
            const int bsize=new_autoinc.size()+1;
            char* newdata=new char[bsize];
            strcpy(newdata,new_autoinc.c_str());
            datarow[spalte].data=newdata;
            datarow[spalte].length=strlen(newdata);
        }
        else
        {                                         //not a autoinc column
            datarow[spalte].length=changed_data->length;;
            char* newdata=NULL;
            if (changed_data->data)
            {
                newdata=new char[datarow[spalte].length];
                for (unsigned int tk=0;tk<datarow[spalte].length;tk++) newdata[tk]=changed_data->data[tk];
            }
            datarow[spalte].data=newdata;
        }
        spalte++;
        col_it++;
    }
    insert_data(datarow);
    return true;
}


hk_sqliteconnection* hk_sqlitedatasource::sqliteconnection(void)
{
    return p_sqlitedatabase->connection();

}


bool hk_sqlitedatasource::driver_specific_batch_enable(void)
{
   p_counter=0;
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
	char* errormsg=0;
	int result= sqlite_compile(p_sqlitedatabase->dbhandler() ,p_sql.c_str(),0,&vm,&errormsg );
	if (result!=SQLITE_OK)
	{
          p_sqlitedatabase->sqliteconnection()->servermessage(errormsg);
	  sqlite_freemem(errormsg);
	  errormsg=0;
	  cerr<< "driver_specific_enable compile problem"<<endl;print_sql();
	  return false;
	}
            
	colnums=0;
	data=0;
        if (accessmode()==batchwrite)   return true; 
	colnames=0;
	if (vm)
	{
	result=sqlite_step(vm,&colnums,&data,&colnames);
       driver_specific_create_columns();
	if(result==SQLITE_ROW&& colnums>0)
	{
          struct_raw_data* datarow=new struct_raw_data[colnums];
	   for (int col=0;col<colnums;++col)
	   {
            //neue Datenzeile erzeugen
              datarow[col].length=(data[col]?strlen(data[col])+1:0); 
              char* dt=NULL;
              //leave NULL pointer if there is no data
              if (data[col])
	       {
	       dt=new char[datarow[col].length];
               strcpy(dt,data[col]);
	       }
		
//	       for (unsigned int tk=0;tk<datarow[col].length;tk++) dt[tk]=data[col][tk];
              datarow[col].data=dt;
	   }
//Daten speichern
        
	insert_data(datarow);
	set_maxrows(1);
	
       } 
	return true;
	}
	    
   }
   set_maxrows(0);
   return false;	    
}


bool hk_sqlitedatasource::driver_specific_batch_disable(void)
{
    delete_data();
    if (accessmode()==batchwrite) return true;
    char* errormsg=0;
    if (vm)
    {
    int result=	sqlite_finalize(vm,&errormsg);
	vm=0;
	if (result==SQLITE_ERROR) 
        {
	  p_sqlitedatabase->sqliteconnection()->servermessage(errormsg);
	  sqlite_freemem(errormsg);
	  errormsg=0;
         return false;
	}  
     }	
     data=0;
     colnames=0;
     colnums=0;
     vm=0;
    return true;
 }


bool hk_sqlitedatasource::driver_specific_batch_goto_next(void)
{
    if (hk_storagedatasource::driver_specific_batch_goto_next()) return true;
  	if (vm)
	{
	if (sqlite_step(vm,&colnums,&data,&colnames)==SQLITE_ROW)
	{ 
	  set_maxrows(max_rows()+1);
          struct_raw_data* datarow=new struct_raw_data[colnums];
 
	   for (int col=0;col<colnums;++col)
	   {
	               //neue Datenzeile erzeugen
	      hk_string n;
	      if (data[col]) n=smallstringconversion(data[col],p_database->databasecharset(),"");
              datarow[col].length=(data[col]?strlen(n.c_str())+1:0); 
              char* dt=NULL;
              //leave NULL pointer if there is no data
              if (data[col])
	       {
	       dt=new char[datarow[col].length];
               strcpy(dt,n.c_str());
	       }
		
              datarow[col].data=dt;
	   }
//Daten speichern
	insert_data(datarow);
 	  ++p_counter;
	return true;
        }
	else
	 {
           char* errormsg=0;
           int result=	sqlite_finalize(vm,&errormsg);
	   vm=0;
	   if (result==SQLITE_ERROR) 
            {
	    p_sqlitedatabase->sqliteconnection()->servermessage(errormsg);
	    sqlite_freemem(errormsg);
	    errormsg=0;
            return false;
	   }  
	 }
       
       }
    return false;
}




void hk_sqlitedatasource::parse_createstatement(void)
{
hk_string createsql;
sqlite_vm* tmpvm=0;
char* errormsg=0;
hk_string s="SELECT sql FROM sqlite_master WHERE type='table' AND name='"+name()+"'";
int result= sqlite_compile(p_sqlitedatabase->dbhandler() ,s.c_str(),0,&tmpvm,&errormsg );
p_coltest.clear();
if (result!=SQLITE_OK)
{
  p_sqlitedatabase->sqliteconnection()->servermessage(errormsg);
  sqlite_freemem(errormsg);
  errormsg=0;
  cerr<< "parse_createstatement compile problem"<<endl;print_sql();
  return ;
}
            
int tmpcolnums=0;
const char** tmpdata=0;
const char** tmpcolnames=0;
if (vm)
{//cerr <<"vor sqlite_step"<<endl;
result=sqlite_step(tmpvm,&tmpcolnums,&tmpdata,&tmpcolnames);
//cerr<<"sqlite_step result="<<result<<endl;
if (tmpcolnums==1&&result==SQLITE_ROW)
{
createsql=tmpdata[0];
}

  hk_string::size_type startpos=createsql.find_first_of("(");
  hk_string::size_type endpos=createsql.find_last_of(")");
  if (startpos!=hk_string::npos && endpos!=hk_string::npos)
   createsql=trim(createsql.substr(startpos+1,endpos-startpos-1));

sqlite_finalize(tmpvm,&errormsg);
tmpvm=0;
if (result==SQLITE_ERROR) 
       {
	  p_sqlitedatabase->sqliteconnection()->servermessage(errormsg);
	  sqlite_freemem(errormsg);
	  errormsg=0;
         return ;
	}  
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
   //  cerr <<"currcol anh�gen"<<endl;
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
  
  
 /*list<coltest*>::iterator it2=p_coltest.begin();
 while(it2!=p_coltest.end())
 {
  cerr <<"it2: #"<<(*it2)->p_name<<"# \tnotnull: "<<(*it2)->p_notnull<<" \tprimary: "<<(*it2)->p_primary<<" \tautoinc: "<<(*it2)->p_autoinc<<endl;
  ++it2;
 } 
*/
}
