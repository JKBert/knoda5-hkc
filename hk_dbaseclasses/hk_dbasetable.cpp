// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_dbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.4 $
#include "hk_dbasetable.h"
#include "hk_dbasedatabase.h"
#include "hk_actionquery.h"
#include "hk_dbaseconnection.h"


hk_dbasetable::hk_dbasetable(hk_dbasedatabase* db,hk_presentation* p):hk_dbasedatasource(db,p)

{

}


hk_datasource::enum_datasourcetypes hk_dbasetable::type() const
{
    return ds_table;
}

bool hk_dbasetable::driver_specific_enable(void)
{
     if (!datasource_open())
      {
       cerr<<"hk_dbasetable::driver_specific_enable !datasource_open() !!!"<<endl;
       return false;
      }


        try
       {
 	 while (datasource_fetch_next_row())
	 {
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

bool hk_dbasetable::driver_specific_disable(void)
{
xbShort rc=p_dbfdatasource->PackDatabase(XB_LOCK);
 rc=p_dbfdatasource->CloseDatabase();
 return hk_dbasedatasource::driver_specific_disable();
}

bool hk_dbasetable::driver_specific_create_table_now(void)
{
    xbSchema* schema= new xbSchema  [p_newcolumns.size()+1] ;

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
	 					size=17;s2=8;
	 					break;
	 case hk_column::smallfloatingcolumn: 	t='F';
	 					size=8;s2=4;
	 					break;
	 case hk_column::datecolumn: 		t='D';
	 					size=8;
	 					break;
	 case hk_column::boolcolumn: 		t='L';
	 					size=1;
	 					break;
	 case hk_column::timecolumn: 		t='C';
	 					size=10;
	 					break;
	 case hk_column::datetimecolumn: 	t='C';
	 					size=25;
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
// 	cerr <<"add col="<<schema[i].FieldName<<" type:"<<schema[i].Type<<endl;
	++it;++i;
    }
   
	schema[i].FieldName[0]=0;
	schema[i].Type=0;
	schema[i].FieldLen=0;
	schema[i].NoOfDecs=0;
   
   
   xbDbf newtable(p_dbasedatabase->dbhandle());
   newtable.SetVersion(4);
   
   hk_url p_url=name();
   hk_url durl=p_dbasedatabase->name();
//    cerr <<"databasepath="<<durl.directory()<<endl;
   if (durl.directory().size()==0) durl=p_dbasedatabase->database_path()+"/";
   hk_string tname= p_url.directory().size()>0? p_url.directory(): durl.url()+"/"+name()+(durl.extension().size()>0?"."+durl.extension():".dbf");
   

   if (newtable.CreateDatabase(tname.c_str(),schema,XB_OVERLAY)!=XB_NO_ERROR)
   {  show_warningmessage("Error creating table");
   cerr <<"Error creating '"<<tname<<"'"<<endl;
   cerr <<"dbase path:"<<database()->database_path()<<endl;
     return false;
   } 
   newtable.OpenDatabase(tname.c_str());
   int max=newtable.FieldCount(); 
   int c=0;
   while (c<max)
   {
//    cerr <<"name #"<<newtable.GetFieldName(c) <<"# type: "<<newtable.GetFieldType(c)  <<endl;
   ++c;
    }
    return true; 
}

list<hk_column*>* hk_dbasetable::driver_specific_columns(void)
{
if (p_columns==NULL&&p_name.size()>0)
 {
     driver_specific_create_columns();
   if (!p_columns) p_columns=new list<hk_column*>;
 }
 
return p_columns;
}

bool hk_dbasetable::driver_specific_create_columns(void)
{
  clear_columnlist();
  p_columns=new list<hk_column*>;
   int z = 0;//counting the fieldnumbers
   int nf=p_dbfdatasource->FieldCount();
   for (int f=0;f<nf;++f)
   {
    hk_dbasecolumn *col= new hk_dbasecolumn(this,p_true,p_false);
    col->set_fieldnumber(z++);
    col->set_name(smallstringconversion(p_dbfdatasource->GetFieldName(f),p_database->databasecharset(),""));
    int size=p_dbfdatasource->GetFieldLen(f);
    hk_column::enum_columntype coltype=hk_column::othercolumn;
    switch(p_dbfdatasource->GetFieldType(f))
    {
	case XB_CHAR_FLD: coltype=hk_column::textcolumn;break;
        case XB_LOGICAL_FLD:  coltype=hk_column::boolcolumn;break;
	case XB_NUMERIC_FLD: 	
			{
				xbShort fs=p_dbfdatasource->GetFieldDecimal(f);
				if (fs==0)
				coltype=hk_column::integercolumn;
				else
				coltype=hk_column::textcolumn;
				break;
			}
	case XB_FLOAT_FLD:  coltype=hk_column::floatingcolumn;break;
	case XB_DATE_FLD:  coltype=hk_column::datecolumn;break;
	case XB_MEMO_FLD:  coltype=hk_column::memocolumn;break;
	
    }
    col->set_columntype(coltype);
    col->set_size(size);
    p_columns->insert(p_columns->end(),col);
    }
return true;
}

bool hk_dbasetable::datasource_open(void)
{
   if (p_print_sqlstatements)   print_sql();
   if (p_enabled) return true;
   if  (!p_dbasedatabase->connection()->is_connected()) return false;
    p_currow=0;
   hk_url p_url=name();
   hk_url durl=p_dbasedatabase->name();
   if (durl.directory().size()==0) durl=p_dbasedatabase->database_path();
   hk_string n= p_url.directory().size()>0? p_url.url(): durl.url()+"/"+name()+(durl.extension().size()>0?"."+durl.extension():".dbf");

//    hk_string blobfile= p_url.directory().size()>0? p_url.directory(): durl.directory()+"/"+name()+".mb";

   if (!p_dbfdatasource) 
   {
   	cerr <<"p_dbfdatasource==NULL!!!"<<endl;
   	return false;   
   }
   xbShort rc;
  if ((rc=p_dbfdatasource->OpenDatabase(n.c_str()))!=XB_NO_ERROR)
   {
     p_dbasedatabase->connection()->servermessage(p_dbasedatabase->dbhandle()->GetErrorMessage(rc));
     show_warningmessage(p_dbasedatabase->dbhandle()->GetErrorMessage(rc));
     return false;
   }
  driver_specific_create_columns();
  return true;
}

bool hk_dbasetable::datasource_fetch_next_row(void)
{
 
 if (p_currow==0)
 {
 if (p_dbfdatasource->GetFirstRecord()!= XB_NO_ERROR)
 {
  return false;
 }
 }
 else
 {
 if (p_dbfdatasource->GetNextRecord()!= XB_NO_ERROR)
 {
  return false;
 }
 }
 int nf=p_dbfdatasource->FieldCount();
 struct_raw_data* datarow=new struct_raw_data[nf];
   for (int i=0;i<nf;++i)
   {

               //create new data row
              char* value=NULL;
	      hk_string txt;
    switch(p_dbfdatasource->GetFieldType(i))
    {
        case XB_LOGICAL_FLD:
        {		
        	xbShort l=p_dbfdatasource->GetLogicalField(i);
//         	cerr <<"load bool:"<<l<<endl;
	        txt=(l==0?"F":"T");
		break;
	}
	case XB_FLOAT_FLD:
        {
        	xbFloat l=p_dbfdatasource->GetFloatField(i);
//         	cerr <<"load float:"<<l<<endl;
	        txt=format_standard_number(l,false,4);
		break;
	}
	case XB_MEMO_FLD:
	{
		
		if (p_dbfdatasource->MemoFieldExists(i))
		{
		 int s=p_dbfdatasource->GetMemoFieldLen(i)+1;
		value=new char[s];
		p_dbfdatasource->GetMemoField(i,s,value,0);
	      	}
	      	if (value)
	      	{  xbLong l=p_dbfdatasource->GetMemoFieldLen(i);
	      	   hk_string r(value,l);
	        	txt=r;
// 	        	cerr <<"load memo:"<<txt<<endl;
	        	delete[] value;
	        	value=NULL;
	      	}
		break;
	}
/*	case XB_CHAR_FLD: 
	case XB_NUMERIC_FLD:
	case XB_DATE_FLD:;
	case XB_MEMO_FLD:;*/
	default:
	{
		value=new char[p_dbfdatasource->GetFieldLen(i)+1];
		p_dbfdatasource->GetField(i,value);
	      	if (value)
	      	{  
	        	txt=trimright(value);
	        	delete[] value;
	        	value=NULL;
	      	}
		break;
	}
    }
    txt=smallstringconversion(txt,p_database->databasecharset(),"");
    //cerr <<pxf->px_fname<<"="<<txt<<endl;
    datarow[i].length=txt.size()+1;
    char* dt=NULL;
	    dt=new char[datarow[i].length];
            strcpy(dt,txt.c_str());

    datarow[i].data=dt;

   }

insert_data(datarow);
++p_currow;

return true;
}

bool hk_dbasetable::datasource_close(void)
{
return false;
}

bool hk_dbasetable::driver_specific_insert_data(void)
{
//    cerr <<"driver_specific_insert"<<endl;
   
   struct_raw_data* datarow=new struct_raw_data[p_columns->size()];
    list<hk_column*>::iterator col_it;
    col_it=p_columns->begin();
    unsigned int spalte=0;
//     cerr <<" while"<<endl;
    while (spalte<p_columns->size())
    {
        const struct_raw_data* changed_data=(*col_it)->changed_data();
            datarow[spalte].length=changed_data->length;;
            char* data=NULL;  
//             cerr <<"d:"<<(*col_it)->changed_data_asstring()<<" ";
            if (changed_data->data)
            {
                data=new char[datarow[spalte].length];
                for (unsigned int tk=0;tk<datarow[spalte].length;tk++) data[tk]=changed_data->data[tk];
//             cerr <<"anfügen: "<<data<<endl;
            }
            else cerr <<"data=NULL ";
            datarow[spalte].data=data;
//         cerr <<spalte<<" ";
        spalte++;
        col_it++;
    }
//     cerr <<endl;
    insert_data(datarow);
    return true;
}


bool    hk_dbasetable::insert_row(enum_interaction interaction)
{
    inform_visible_objects_before_insert_row();

  p_dbfdatasource->BlankRecord();
  
  set_colvalues(false);
   
   xbShort rc=p_dbfdatasource->AppendRecord();
   bool ret=(rc==XB_NO_ERROR);
   if (ret)
   {
     ret=driver_specific_insert_data();
     if (ret)
     {
     set_has_not_changed();
     transaction_commit();
     goto_last();
     inform_visible_objects_row_add();
     setmode_normal();
     execute_visible_object_after_insert();
     setmode_normal();
     set_has_not_changed();
     }

   }
   else
   {
     p_dbasedatabase->connection()->servermessage(p_dbasedatabase->dbhandle()->GetErrorMessage(rc));
   hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row could NOT be inserted!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
   if (interaction==interactive) show_warningmessage(reason);
   }
   return ret;

}





void hk_dbasetable::set_colvalues(bool is_update)
{
// cerr <<"colvals: ";
  list<hk_column*>::iterator it=p_columns->begin();
  int i=0;
  hk_string value;
        while ( it!=p_columns->end() )
        {
          hk_column* xc=*it;          
            switch (p_dbfdatasource->GetFieldType(i))
            {
        case XB_LOGICAL_FLD:
        {	//cerr <<" logic:"<<xc->changed_data_asbool()<<endl;	
		value=smallstringconversion(((is_update?xc->curval_asbool():xc->changed_data_asbool())?"T":"F"),"",database()->databasecharset());
		p_dbfdatasource->PutField(i,value.c_str());
		
		break;
	}
	case XB_FLOAT_FLD:
        {       //cerr <<" float:"<<xc->changed_data_asdouble();
		p_dbfdatasource->PutFloatField(i,(is_update?xc->curval_asdouble():xc->changed_data_asdouble()));
		break;
	}
	case XB_MEMO_FLD:
	{       //cerr <<" memo:"<<xc->changed_data_asstring();
		value=smallstringconversion(is_update?xc->curval_asstring():xc->changed_data_asstring(),"",database()->databasecharset());

		p_dbfdatasource->UpdateMemoData(i,value.size(),value.c_str(),XB_LOCK);
				break;
	}
	case XB_NUMERIC_FLD: 	
			{
				xbShort fs=p_dbfdatasource->GetFieldDecimal(i);
				if (fs==0)
				 {
					p_dbfdatasource->PutLongField(i,(is_update?xc->curval_asinteger():xc->changed_data_asinteger()));
// 					cerr <<" numLong:"<<xc->changed_data_asinteger();
				 }
				 else
				 {
					value=smallstringconversion(is_update?xc->curval_asstring():xc->changed_data_asstring(),"",database()->databasecharset());
					p_dbfdatasource->PutField(i,value.c_str());
// 					cerr <<"numeric:"<<xc->changed_data_asstring();
				 }
				 break;
			}

	case XB_DATE_FLD:
			{
				hk_datetime d;
				d.set_date_asstring((is_update?xc->curval_asstring():xc->changed_data_asstring())); 
				d.set_dateformat("YMD");
				p_dbfdatasource->PutField(i,smallstringconversion(d.date_asstring(),"",database()->databasecharset()).c_str());
// 				cerr <<"date:"<<xc->changed_data_asstring();
				break;
			}

/*	case XB_CHAR_FLD: 
	case XB_DATE_FLD:;
*/
	default:
	{
		value=smallstringconversion(is_update?xc->curval_asstring():xc->changed_data_asstring(),"",database()->databasecharset());
		p_dbfdatasource->PutField(i,value.c_str());
// 		cerr <<"default:"<<xc->changed_data_asstring();
		break;
	}
		
	    }
//             cerr <<endl;
          ++i;
          ++it;
        }

}



bool hk_dbasetable::update_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug( "hk_dbasetable::update_row()");
#endif
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
    
    set_colvalues(true);
    
    set_has_not_changed();

	bool r=true;
         if (!blockserversignals())
        {

   xbShort rc=p_dbfdatasource->PutRecord(row_position()+1);

//         cerr <<"nach update_record:"<<result<<endl;
   //datasource_close();
        if (rc==XB_NO_ERROR)
                {
#ifdef HK_DEBUG
                    hkdebug("Ändern hat funktioniert");
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
	            p_dbasedatabase->connection()->servermessage(p_dbasedatabase->dbhandle()->GetErrorMessage(rc));
                hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row could NOT be changed!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
                    if (interaction==interactive) show_warningmessage(reason);
		    r=false;
                }

        }

inform_depending_ds_after_update_row();
return r;
}





bool hk_dbasetable::delete_rows(unsigned long from,unsigned long to,bool check_depending_datasources,enum_interaction c)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbasetable::delete_rows(unsigned long from,unsigned long to)");
#endif
    if (is_readonly()) return false;


    if (from==to && from<max_rows())
    {                                             // only a single row, so handle it as a single row
        goto_row(from);
        create_actual_row_where_statement();
        return delete_row(c);
    }
    if (c==interactive)
    {
        hk_string txt=hk_translate("Delete the records %FROM% to %TO% ?");
        txt=replace_all("%FROM%",txt,longint2string(from+1));
        txt=replace_all("%TO%",txt,longint2string(to+1));
        if(!show_yesnodialog(txt,true))
        {
#ifdef HK_DEBUG
            hkdebug("don't delete");
#endif
            return false;
        }
    }
//check if depending datasources allow deletion
    if (check_depending_datasources)
    {
        bool dep_bool=true;
        list<hk_datasource*>::iterator dep_it=p_dependinglist.begin();
        if (dependingmode()!=depending_nohandle)
            while (dep_it!=p_dependinglist.end())
        {
            if ((*dep_it)->dependingmode()!=depending_nohandle) dep_bool=false;
            dep_it++;
        }

        if (!dep_bool)
        {
            show_warningmessage(hk_translate("Rows could not be deleted due to depending datasource(s).\n\
	  Use single row deletion instead!"));
            return false;
        }
    }
//end check

    bool exec_ok=true;
    if (max_rows()==0||type()!=ds_table)
    {
//	  cout <<"pcounter: "<<p_counter<<endl<<"type: "<<type()<<endl;
        return false;
    }
    unsigned long f,t,f_orig;
    if (from>to)
    {
        t=from;
        f=to;
    }
    else
    {
        t=to;
        f=from;
    }
    if (f>=max_rows()) return false;
    if (t>=max_rows()) t=max_rows()-1;
    hk_string b;
    f_orig=f;
    if (!blockserversignals())
    {
        execute_visible_object_before_delete();


////////////////////
    execute_visible_object_before_delete();
xbShort rc=XB_NO_ERROR;
    if (!blockserversignals())
    {
            

	    for(long int i=t;i>=(long int)f &&exec_ok;i--)
	    {
	     p_dbfdatasource->GetRecord(i+1);
	     rc=p_dbfdatasource->DeleteRecord();
// 	     int r=PX_delete_record(p_paradoxhandle,i);
            //cerr <<"result loeschen:"<<r<<endl;
            exec_ok=(rc==XB_NO_ERROR);
	    }
            if (exec_ok)
            {
#ifdef HK_DEBUG
                hkdebug("Löschen hat funktioniert");
#endif
                driver_specific_delete_data_at(p_counter);
                inform_visible_objects_row_delete();
                transaction_commit();
                if ((max_rows()<=p_counter)&&(p_counter>0))p_counter=max_rows()-1;
            }
        if (!exec_ok)
        {
#ifdef HK_DEBUG
            hkdebug("FEHLER kein Löschen");
#endif

            p_dbasedatabase->connection()->servermessage(p_dbasedatabase->dbhandle()->GetErrorMessage(rc));
            transaction_rollback();
            hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row was NOT deleted!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
            if (c==interactive) show_warningmessage(reason);
        }
    }
    execute_visible_object_after_delete();
    set_has_not_changed();
//     cerr <<"p_counter:"<<p_counter<<endl;
    }
    unsigned long pp=p_counter;
    if (exec_ok)
    {
        bool r=true;
        while ((t>=f_orig) &&r)
        {
            driver_specific_delete_data_at(t); //cerr <<"deleteat: "<<t<<endl;
            if (t==0) r=false;
            t--;
        }

    } else cerr <<"res==false!!!!!!!!!!!"<<endl;
    inform_visible_objects_row_delete();
    if (max_rows()==0) p_counter=0;
    else if (p_counter>=max_rows()) p_counter=max_rows()-1;
    goto_row(pp);
    inform_depending_ds_after_store_changed_data();
    inform_visible_objects_after_store_changed_data();
    return exec_ok;
}


bool hk_dbasetable::delete_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbasetable::delete_row()");
#endif
    unsigned long oldp_counter =p_counter;
    if(interaction==interactive &&!show_yesnodialog(hk_translate("Delete this record?"),true))
    {
#ifdef HK_DEBUG
        hkdebug("don't delete");
#endif
        p_mode=mode_normal;
        set_has_not_changed();
        return true;
    }

#ifdef HK_DEBUG
    hkdebug("delete");
#endif

    inform_before_row_change();
    list<hk_datasource*>::iterator dep_it;
    dep_it=p_dependinglist.begin();
    bool dep_bool=true;
    if (dependingmode()!=depending_nohandle)
        while (dep_it!=p_dependinglist.end())
    {
        if(!(*dep_it)->depending_on_datasource_deleterow_ok()) dep_bool=false;
        dep_it++;
    }

    if (!dep_bool)
    {
        if (interaction==interactive) show_warningmessage(hk_translate("Row could not be deleted due to depending datasource(s)"));
        p_mode=mode_normal;
        set_has_not_changed();

        return false;
    }
    execute_visible_object_before_delete();
    transaction_begin();
    dep_it=p_dependinglist.begin();
    bool subbool=true;
    while (dep_it!=p_dependinglist.end())
    {
        if (!(*dep_it)->depending_on_datasource_before_delete_row())
            subbool=false;
        dep_it++;
    }

    bool exec_ok=false;
xbShort rc=XB_NO_ERROR;

    if (!blockserversignals())
    {
        if (subbool)
        {   //cerr <<"loesche Zeile:"<<row_position()<<endl;
            
	     p_dbfdatasource->GetRecord(row_position()+1);
	     rc=p_dbfdatasource->DeleteRecord();
            
            //cerr <<"result loeschen:"<<r<<endl;
            exec_ok=(rc==XB_NO_ERROR);
            if (exec_ok)
            {
#ifdef HK_DEBUG
                hkdebug("Löschen hat funktioniert");
#endif
                driver_specific_delete_data_at(p_counter);
                inform_visible_objects_row_delete();
                transaction_commit();
                if ((max_rows()<=p_counter)&&(p_counter>0))p_counter=max_rows()-1;
            }
        }
        if (!subbool ||!exec_ok)
        {
#ifdef HK_DEBUG
            hkdebug("FEHLER kein Löschen");
#endif
            p_dbasedatabase->connection()->servermessage(p_dbasedatabase->dbhandle()->GetErrorMessage(rc));
            transaction_rollback();
            hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row was NOT deleted!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
            if (interaction==interactive) show_warningmessage(reason);
        }
    }
    execute_visible_object_after_delete();
    set_has_not_changed();
//     cerr <<"p_counter:"<<p_counter<<endl;
    if (p_counter>0)
    {
        p_mode=mode_normal;
        goto_row(p_counter);
        if (p_counter==oldp_counter)
            inform_depending_ds_goto_row();

    }
    else setmode_insertrow();
    return exec_ok;

}







