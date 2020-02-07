// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_paradoxclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.26 $
#include "hk_paradoxtable.h"
#include "hk_paradoxdatabase.h"
#include "hk_actionquery.h"
#include "hk_datetime.h"
#include "hk_paradoxconnection.h"
#include "hk_paradoxdatabase.h"
#include "hk_url.h"
//#include <pxversion.h>

hk_paradoxtable::hk_paradoxtable(hk_paradoxdatabase* db,hk_presentation* p):hk_paradoxdatasource(db,p)

{
p_currow=0;
data=NULL;
dataptr=NULL;
 p_readonly=false;

file=NULL;
}


hk_paradoxtable::~hk_paradoxtable()
{
if ( p_paradoxhandle )
{
if (data)p_paradoxhandle->free(p_paradoxhandle,data);
data=NULL;
        PX_close(p_paradoxhandle);
       	PX_delete(p_paradoxhandle);
       	p_paradoxhandle=NULL;
}
if (file) fclose(file);
file=NULL;

}


hk_datasource::enum_datasourcetypes hk_paradoxtable::type() const
{
    return ds_table;
}

bool hk_paradoxtable::driver_specific_enable(void)
{
     bool p_cancel=false;
     int max=progressinterval();
     if (!datasource_open())
      {
       cerr<<"hk_paradoxtable::driver_specific_enable !datasource_open() !!!"<<endl;
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


bool hk_paradoxtable::driver_specific_disable()
{

if ( p_paradoxhandle )
{
if (data)p_paradoxhandle->free(p_paradoxhandle,data);
data=NULL;
        PX_close(p_paradoxhandle);
       	PX_delete(p_paradoxhandle);
       	p_paradoxhandle=NULL;
}
if (file) fclose(file);
file=NULL;
return hk_paradoxdatasource::driver_specific_disable();
}

bool hk_paradoxtable::datasource_open(void)
{
   if (p_print_sqlstatements)   print_sql();
   if (p_enabled) return true;
   if  (!p_paradoxdatabase->connection()->is_connected()) return false;
   p_currow=0;
   hk_url p_url=name();
   hk_url durl=p_paradoxdatabase->name();
   cerr <<"databasepath="<<durl.url()<<endl;
   cerr <<"name="<<p_url.url()<<endl;
   if (durl.directory().size()==0) durl=p_paradoxdatabase->database_path()+"/";
   hk_string n= p_url.directory().size()>0? p_url.directory(): durl.url()+"/"+name()+(durl.extension().size()>0?"."+durl.extension():".db");

   hk_string blobfile= p_url.directory().size()>0? p_url.url(): durl.url()+"/"+name()+".mb";

//    cerr <<"open: #"<<n<<"#"<<endl;
if ( p_paradoxhandle )
{
        PX_close(p_paradoxhandle);
       	PX_delete(p_paradoxhandle);
       	p_paradoxhandle=NULL;
}
   cerr <<"openfile:"<<n<<endl;
   file=fopen(n.c_str(),"r+");
   if (!file) return false;
    p_paradoxhandle=PX_new2(errorhandler,NULL,NULL,NULL);
   if (PX_open_fp(p_paradoxhandle,file)<0)
   {
     show_warningmessage("Could not open file'"+n+"'");
     return false;
   }
   p_paradoxheader=p_paradoxhandle->px_head;
   if (p_paradoxheader)
   {
     //set_maxrows(p_paradoxheader->px_numrecords);
     p_codepage="CP"+longint2string(p_paradoxheader->px_doscodepage);
   }
   PX_set_tablename(p_paradoxhandle,smallstringconversion(name(),"",p_codepage).c_str());
   if (accessmode()==batchwrite)
        {
            clear_columnlist();
            driver_specific_create_columns();
            return true;
        }

  driver_specific_create_columns();
  if (file_exists(blobfile))
    {
      if (PX_set_blob_file(p_paradoxhandle,blobfile.c_str())!=0)
      {
        show_warningmessage("Could not open blobfile:'"+blobfile+"'");
      }
 /*     else 
        show_warningmessage("Use blobfile:'"+blobfile+"'");*/
    }

  return true;
}





bool hk_paradoxtable::datasource_fetch_next_row(void)
{
   if (!p_paradoxheader) return false;
   if (p_currow>=p_paradoxheader->px_numrecords)
 {
  return false;
 }

  if (dataptr) p_paradoxhandle->free(p_paradoxhandle,dataptr);
  dataptr=NULL;

 if (!(dataptr=PX_retrieve_record(p_paradoxhandle,p_currow)))
 {
  cerr <<"PX_retrieve_record gibt NULL zurck"<<endl;
  return false;
 }
 int nf=p_paradoxheader->px_numfields;
 struct_raw_data* datarow=new struct_raw_data[nf];
   pxfield_t*pxf=p_paradoxheader->px_fields;
   int offset=0;
   bool binary=false;
   int binarysize=0;
   for (int i=0;i<nf;++i)
   {

               //create new data row
              char* value=NULL;
	      hk_string txt;
    switch(dataptr[i]->type)
    {
     case pxfLogical :
	       {
	        if (dataptr[i]->isnull!=1)
	        {
	         txt=longint2string(dataptr[i]->value.lval);
	         value=const_cast<char*>(txt.c_str());
	         }
 	       }
	      break;
     case pxfShort :
     case pxfAutoInc :
     case pxfLong:
     		{

	       txt=longint2string(dataptr[i]->value.lval);
	       value=const_cast<char*>(txt.c_str());
	      break;}

     case pxfCurrency:
     case pxfNumber:
     		{

	       txt=format_standard_number(dataptr[i]->value.dval);
	       value=const_cast<char*>(txt.c_str());
	      break;}

     case pxfDate:
     		{
     		  int year,month,day;

               PX_SdnToGregorian(dataptr[i]->value.lval+1721425,&year,&month,&day);
	       hk_datetime dt;
	       dt.set_date(day,month,year);
	       txt=dt.date_asstring();
	       value=const_cast<char*>(txt.c_str());
	       binarysize=txt.size();
	      break;}

     case pxfTimestamp:
     		{

	       txt=PX_timestamp2string(p_paradoxhandle,dataptr[i]->value.dval,"H:i:s d.m.Y");
	       value=const_cast<char*>(txt.c_str());
	      break;}

     case pxfTime:
     		{
     		  int h,m,s;
	       hk_datetime dt;
               h=dataptr[i]->value.lval/3600000;
               m=dataptr[i]->value.lval/60000%60;
               s=dataptr[i]->value.lval%60000/1000;
	       dt.set_time(h,m,s);
	       txt=dt.time_asstring();
	       value=const_cast<char*>(txt.c_str());
	      break;}
     case pxfBLOb:
     case pxfGraphic:
     case pxfOLE:
     case pxfMemoBLOb:
     case pxfFmtMemoBLOb:
     		{
     		binary=true;
     		value=dataptr[i]->value.str.val;
		binarysize=dataptr[i]->value.str.len;
	      break;}

     default:
     		value=dataptr[i]->value.str.val;
		binarysize=dataptr[i]->value.str.len;
	      if (value)
	      {  
	        txt=smallstringconversion(value,p_codepage,"");
	      }
    }
    //cerr <<pxf->px_fname<<"="<<txt<<endl;
    datarow[i].length=(binary?binarysize:(value?txt.size()+1:0));
    char* dt=NULL;
    if (value )
	 {
	   if (binary)
	   {
	    dt=new char[datarow[i].length];
                for (unsigned int tk=0;tk<datarow[i].length;tk++) dt[tk]=value[tk];
//             p_paradoxhandle->free(p_paradoxhandle,value);
            value=NULL;

	   }
	   else
	   {
	    dt=new char[datarow[i].length];
            strcpy(dt,txt.c_str());
	   }
	  }

    datarow[i].data=dt;

    offset+=pxf->px_flen;
    ++pxf;
   }
//store data
insert_data(datarow);
++p_currow;
 return true;

}




bool hk_paradoxtable::datasource_close(void)
{
  p_paradoxheader=NULL;
  if (data)p_paradoxhandle->free(p_paradoxhandle,data);
/* if ( p_paradoxhandle )
{
        PX_close(p_paradoxhandle);
       	PX_delete(p_paradoxhandle);
       	p_paradoxhandle=NULL;
}*/

 data = NULL;
  return true;
}


list<hk_column*>* hk_paradoxtable::driver_specific_columns(void)
{
if (p_columns==NULL&&p_name.size()>0)
 {
   if (!p_paradoxheader)
   {
   hk_url p_url=name();
   hk_url durl=p_paradoxdatabase->name();
//    cerr <<"databasepath="<<durl.directory()<<endl;
   if (durl.directory().size()==0) durl=p_paradoxdatabase->database_path()+"/";
   hk_string n= p_url.directory().size()>0? p_url.directory(): durl.url()+"/"+name()+".db";
//    cerr <<"open: #"<<n<<"#"<<endl;
    if (!p_paradoxhandle)p_paradoxhandle=PX_new2(errorhandler,NULL,NULL,NULL);
      file=fopen(n.c_str(),"r+");
   if (!file)
   {
     show_warningmessage("Could not open file'"+n+"'");
     return false;
   }
   if (PX_open_fp(p_paradoxhandle,file)<0)
   {
     show_warningmessage("Could not open file'"+n+"'");
     return false;
   }
   p_paradoxheader=p_paradoxhandle->px_head;
   if (p_paradoxheader)
   {
     //set_maxrows(p_paradoxheader->px_numrecords);
     p_codepage="CP"+longint2string(p_paradoxheader->px_doscodepage);
     driver_specific_create_columns();
   }
   else p_columns=new list<hk_column*>;
 }
 }
return p_columns;
}


bool hk_paradoxtable::driver_specific_create_columns(void)
{
  if (!p_paradoxheader) return false;
  clear_columnlist();
  p_columns=new list<hk_column*>;
   int z = 0;//counting the fieldnumbers
   int nf=p_paradoxheader->px_numfields;
   pxfield_t*pxf=p_paradoxheader->px_fields;
   for (int f=0;f<nf;++f)
   {
    hk_paradoxcolumn *col= new hk_paradoxcolumn(this,p_true,p_false);
    col->set_fieldnumber(z++);
    col->set_name(smallstringconversion(pxf->px_fname,p_codepage,""));
    int size=pxf->px_flen;
    hk_column::enum_columntype coltype=hk_column::othercolumn;
    switch(pxf->px_ftype)
    {
	case pxfAlpha: coltype=hk_column::textcolumn;break;
        case pxfLogical:  coltype=hk_column::boolcolumn;break;
	case pxfLong:  coltype=hk_column::integercolumn;break;
	case pxfShort:  coltype=hk_column::smallintegercolumn;break;
	case pxfDate:  coltype=hk_column::datecolumn;break;
	case pxfMemoBLOb:  coltype=hk_column::memocolumn;break;
	case pxfBLOb:  coltype=hk_column::binarycolumn;break;
	case pxfNumber:  coltype=hk_column::floatingcolumn;break;
	case pxfTime:    coltype=hk_column::timecolumn;break;
	case pxfAutoInc:   coltype=hk_column::auto_inccolumn;break;
			coltype=hk_column::integercolumn;break;
    }
    col->set_columntype(coltype);
    col->set_size(size);
    p_columns->insert(p_columns->end(),col);
    ++pxf;
    }
return true;
}

bool hk_paradoxtable::driver_specific_create_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxtable::driver_specific_create_table_now");
#endif
// cerr <<"paradox create_table_now"<<endl;
if (!p_paradoxhandle)
   p_paradoxhandle=PX_new2(errorhandler,NULL,NULL,NULL);

//     cerr <<"p_paradoxhandle"<<p_paradoxhandle<<endl;
        p_paradoxheader=p_paradoxhandle->px_head;
     p_codepage="CP1252";
    pxfield_t* schema= (pxfield_t *) p_paradoxhandle->malloc(p_paradoxhandle, p_newcolumns.size()*sizeof(pxfield_t), "Could not get memory for field definitions.");
    if (!schema)
    {
      return false;

    }
    list<hk_column*>::iterator it=p_newcolumns.begin();
    int i=0;
    bool use_blob=false;
    while (it!=p_newcolumns.end())
    {
        char t=pxfAlpha;
	int size=(*it)->size();
	int s2=0;
	switch ((*it)->columntype())
	{

	 case hk_column::auto_inccolumn: 	t=pxfAutoInc;
	 					size=4;
	 					break;
	 case hk_column::integercolumn: 	t=pxfLong;
	 					size=4;
	 					break;
	 case hk_column::smallintegercolumn: 	t=pxfShort;
	 					size=2;
	 					break;
	 case hk_column::floatingcolumn: 	t=pxfNumber;
	 					size=8;
	 					break;
	 case hk_column::smallfloatingcolumn: 	t=pxfNumber;
	 					size=8;
	 					break;
	 case hk_column::datecolumn: 		t=pxfDate;
	 					size=4;
	 					break;
	 case hk_column::timecolumn: 		t=pxfTime;
	 					size=4;
	 					break;
	 case hk_column::boolcolumn: 		t=pxfLogical;
	 					size=1;
	 					break;
	 case hk_column::memocolumn: 		t=pxfMemoBLOb;
	 					use_blob=true;
	 					size=10;
	 					break;
	 case hk_column::binarycolumn: 		t=pxfBLOb;
	 					size=10;
	 					use_blob=true;
	 					break;
         default: size=255;
	}
// 	cerr <<"create p_codepage="<<p_codepage<<endl;
	if (size==0) size=1;
	schema[i].px_fname=PX_strdup(p_paradoxhandle,smallstringconversion((*it)->name(),"",p_codepage).c_str());
	schema[i].px_ftype=t;
	schema[i].px_flen=size;
	schema[i].px_fdc=s2;
	//cerr <<"add col="<<schema[i].px_fname<<" type:"<<schema[i].px_ftype<<endl;
	++it;++i;
    }

   hk_url p_url=name();
   hk_url durl=p_paradoxdatabase->name();
//    cerr <<"databasepath="<<durl.directory()<<endl;
   if (durl.directory().size()==0) durl=p_paradoxdatabase->database_path()+"/";
   hk_string tname= p_url.directory().size()>0? p_url.url(): durl.url()+"/"+name()+(durl.extension().size()>0?"."+durl.extension():".db");
cerr <<"TNAME="<<tname<<endl;
   hk_string blobfile= p_url.directory().size()>0? p_url.url(): durl.directory()+"/"+name()+".mb";
//    cerr<<"tname="<<tname<<endl;
   if (PX_create_file(p_paradoxhandle,schema,p_newcolumns.size(),tname.c_str(),pxfFileTypNonIndexDB)<0)
   {  show_warningmessage("Error creating table");
//       p_paradoxhandle->free(p_paradoxhandle,schema);

        PX_close(p_paradoxhandle);
       	PX_delete(p_paradoxhandle);
       	p_paradoxhandle=NULL;

     return false;
   }
//   p_paradoxhandle->free(p_paradoxhandle,schema);
        if (use_blob)
        {
         if (PX_set_blob_file(p_paradoxhandle,blobfile.c_str())!=0)
            show_warningmessage("Could not create blobfile: '"+blobfile+"'");
        }
        PX_close(p_paradoxhandle);
       	PX_delete(p_paradoxhandle);
       	p_paradoxhandle=NULL;
p_paradoxheader=NULL;
//   cerr<<"table #"<<tname<<"# created"<<endl;

    return true;

}



bool    hk_paradoxtable::insert_row(enum_interaction interaction)
{
//   cerr <<"hk_paradoxtable::insert_row"<<endl;
  //datasource_open();
    inform_visible_objects_before_insert_row();
   p_paradoxheader=p_paradoxhandle->px_head;
      if (p_paradoxheader)
   {
     //set_maxrows(p_paradoxheader->px_numrecords);
     p_codepage="CP"+longint2string(p_paradoxheader->px_doscodepage);
   }

  if (!p_paradoxheader)
  {
  cerr <<"hk_paradoxtable::insert_row no header!"<<endl;
   return false;
  }
//   int offset=0; 
  int numfields=PX_get_num_fields(p_paradoxhandle);
  if (dataptr) p_paradoxhandle->free(p_paradoxhandle,dataptr);
  dataptr=(pxval_t**) p_paradoxhandle->malloc(p_paradoxhandle,numfields*sizeof(pxval_t*),"allocate memory");
  if (!dataptr)
  {
    cerr <<"hk_paradoxtable::insert_row memory allocation failed"<<endl;
    return false;
  }
  for(int k=0;k<numfields;++k)
   {
   MAKE_PXVAL(p_paradoxhandle,dataptr[k]);
   }
   pxfield_t*pxf=p_paradoxheader->px_fields;
  list<hk_column*>::iterator it=p_columns->begin();
  int i=0;
        while ( it!=p_columns->end() )
        {
          hk_column* xc=*it;          
            switch (pxf[i].px_ftype)
            {
                 case pxfAutoInc :
     		{  
                  dataptr[i]->type=pxfAutoInc;
                  dataptr[i]->value.str.val=NULL;
                  dataptr[i]->value.str.len=0;
                  dataptr[i]->value.lval=0;
                  break;
     		}
                 case pxfLogical :
     		{  //cerr<<"l"<<endl;
                  dataptr[i]->type=pxfLogical;
                  dataptr[i]->value.lval=(xc->changed_data_asbool()?1:0);
		  break;
		}
                 case pxfShort :
     		{//cerr<<"s"<<endl;
                  dataptr[i]->type=pxfShort;
                  dataptr[i]->value.lval=xc->changed_data_asinteger();
		  break;
		}
                 case pxfLong :
     		{//cerr<<"L"<<endl;
                  dataptr[i]->type=pxfLong;
                  dataptr[i]->value.lval=xc->changed_data_asinteger();                  
                  break;
		}
                 case pxfTime :
     		{//cerr<<"L"<<endl;
                  dataptr[i]->type=pxfTime;
                  hk_datetime d;
                  //d.set_timeformat(p_timeformat);
                  d.set_time_asstring(xc->changed_data_asstring()); 
                  px_val* r=PX_make_time(p_paradoxhandle,d.hour(),d.minute(),d.second());                
                  dataptr[i]->value.lval=r->value.lval;
                  break;
		}
                 case pxfDate :
     		{//cerr<<"L"<<endl;
                  dataptr[i]->type=pxfDate;
                  hk_datetime d;
                  //d.set_dateformat(p_dateformat);
                  d.set_date_asstring(xc->changed_data_asstring()); 
                  px_val* r=PX_make_date(p_paradoxhandle,d.year(),d.month(),d.day());                
                  dataptr[i]->value.lval=r->value.lval;
                  break;
		}
                 case pxfNumber :
     		{//cerr<<"N"<<endl;
                  dataptr[i]->type=pxfNumber;
                  dataptr[i]->value.dval=xc->changed_data_asdouble();                  
		  break;
		}
                 case pxfAlpha :
     		{//cerr<<"a"<<endl;
		  char* value=NULL;
		  if (!xc->changed_data_is_nullvalue())
		     value=strdup(const_cast<char*>(smallstringconversion(xc->changed_data_asstring(),"",p_codepage).c_str()));

                  dataptr[i]->type=pxfAlpha;
                  dataptr[i]->value.str.val=value;
                  dataptr[i]->value.str.len=(value?strlen(value):0);//strlen(dataptr[i]->value.str.val);
		  break;
		}
                 case pxfMemoBLOb :
                 case pxfFmtMemoBLOb :
     		{//cerr<<"a"<<endl;
                  const struct_raw_data* d=xc->changed_data();

                  dataptr[i]->type=pxfMemoBLOb;
                  if (xc->changed_data_is_nullvalue()||!xc->has_changed())
                  { cerr <<"insert memo mit NULLVALUE"<<endl;
                  dataptr[i]->value.str.val=NULL;
                  dataptr[i]->value.str.len=0;
                  }
                  else
                  {
                  dataptr[i]->value.str.val=d->data;
                  dataptr[i]->value.str.len=d->length;
                  }
		  break;
		}
		
                 case pxfBLOb:
                 case pxfGraphic:
     		{//cerr<<"a"<<endl;
                  const struct_raw_data* d=xc->changed_data();

                  dataptr[i]->type=pxfBLOb;
                  dataptr[i]->value.str.val=d->data;
                  dataptr[i]->value.str.len=d->length;
		  break;
		}
		
		}
            
                    dataptr[i]->isnull=((xc->changed_data_is_nullvalue()
                    			||!xc->has_changed()
                    			||pxf[i].px_ftype==pxfAutoInc)?1:0);
          ++i;
          ++it;
        }
   
   int result=PX_insert_record(p_paradoxhandle,dataptr);
   free (dataptr);
   
   dataptr=NULL;
   p_paradoxheader=NULL;
   bool ret=(result>=0);
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
   if (!ret)
   {
   p_paradoxdatabase->paradoxconnection()->servermessage(lasterror());
   hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row could NOT be inserted!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
   if (interaction==interactive) show_warningmessage(reason);
   }
//    cerr <<"result:"<<((result>=0)?"TRUE":"FALSE")<<endl;
   return ret;

}



bool hk_paradoxtable::driver_specific_insert_data(void)
{

// cerr <<"hk_paradoxtable::driver_specific_insert_data"<<endl;
    hk_string new_autoinc;

    struct_raw_data* datarow=new struct_raw_data[p_columns->size()];
    list<hk_column*>::iterator col_it;
    col_it=p_columns->begin();
    unsigned int spalte=0;
    while (spalte<p_columns->size())
    {
         const struct_raw_data* changed_data=(*col_it)->changed_data();
        if ((*col_it)->columntype()==hk_column::auto_inccolumn &&p_paradoxhandle->px_head)
        {
            new_autoinc=format_number((int)p_paradoxhandle->px_head->px_autoinc,false,0); 
            const int bsize=new_autoinc.size()+1;
            char* newdata=new char[bsize];
            strcpy(newdata,new_autoinc.c_str());
            datarow[spalte].data=newdata;
            datarow[spalte].length=strlen(newdata);
        }
        else
        { //not a autoinc column
        if (!changed_data)
        { 
         show_warningmessage((*col_it)->name()+": Warnung Spalte changed_data==NULL!");
         continue;
        }
        
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


bool hk_paradoxtable::delete_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxtable::delete_row()");
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

    if (!blockserversignals())
    {
        if (subbool)
        {   //cerr <<"loesche Zeile:"<<row_position()<<endl;
            int r=PX_delete_record(p_paradoxhandle,row_position());
            //cerr <<"result loeschen:"<<r<<endl;
            exec_ok=(r>-1);
            if (exec_ok)
            {
#ifdef HK_DEBUG
                hkdebug("L�chen hat funktioniert");
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
            hkdebug("FEHLER kein L�chen");
#endif
            p_paradoxdatabase->paradoxconnection()->servermessage(lasterror());
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




bool hk_paradoxtable::update_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug( "hk_paradoxtable::update_row()");
#endif
   p_paradoxheader=p_paradoxhandle->px_head;
   if (p_paradoxheader)
   {
     //set_maxrows(p_paradoxheader->px_numrecords);
     p_codepage="CP"+longint2string(p_paradoxheader->px_doscodepage);
   }

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


  int numfields=PX_get_num_fields(p_paradoxhandle);
  if (dataptr) p_paradoxhandle->free(p_paradoxhandle,dataptr);
  dataptr=(pxval_t**) p_paradoxhandle->malloc(p_paradoxhandle,numfields*sizeof(pxval_t*),"allocate memory");
  if (!dataptr)
  {
    cerr <<"hk_paradoxtable::update_row memory allocation failed"<<endl;
    return false;
  }
  for(int k=0;k<numfields;++k)
   {
   MAKE_PXVAL(p_paradoxhandle,dataptr[k]);
   }
   pxfield_t*pxf=p_paradoxheader->px_fields;


  list<hk_column*>::iterator it=p_columns->begin();
  int i=0;
        while ( it!=p_columns->end() )
        {
          hk_column* xc=*it;//cerr <<x->name()<<": #"<<x->changed_data_asstring()<<"#"<<endl;
//     cerr <<" pxf="<<pxf[i].px_fname<<endl;

	if (pxf[i].px_ftype==pxfAutoInc)
	{
                  dataptr[i]->type=pxfAutoInc;
                  dataptr[i]->value.lval=xc->curval_asinteger();
	}
	else
	{

            switch (pxf[i].px_ftype)
            {
                 case pxfLogical :
     		{  //cerr<<"l"<<endl;
                  dataptr[i]->type=pxfLogical;
                  dataptr[i]->value.lval=(xc->curval_asbool()?1:0);
		  break;
		}
                 case pxfShort :
     		{//cerr<<"s"<<endl;
                  dataptr[i]->type=pxfShort;
                  dataptr[i]->value.lval=xc->curval_asinteger();
		  break;
		}
                 case pxfLong :
     		{//cerr<<"L"<<endl;
                  dataptr[i]->type=pxfLong;
                  dataptr[i]->value.lval=xc->curval_asinteger();                  
                  break;
		}
                 case pxfNumber :
     		{//cerr<<"N"<<endl;
                  dataptr[i]->type=pxfNumber;
                  dataptr[i]->value.dval=xc->curval_asdouble();                  
		  break;
		}
                 case pxfAlpha :
     		{//cerr<<"a"<<endl;
     		  char* value=strdup(const_cast<char*>(smallstringconversion(xc->curval_asstring(),"",p_codepage).c_str()));
                  dataptr[i]->type=pxfAlpha;
                  dataptr[i]->value.str.val=value;
                  dataptr[i]->value.str.len=strlen(dataptr[i]->value.str.val);

		  break;
		}
                 case pxfTime :
     		{//cerr<<"L"<<endl;
                  dataptr[i]->type=pxfTime;
                  hk_datetime d;
                  //d.set_timeformat(p_timeformat);
                  d.set_time_asstring(xc->changed_data_asstring()); 
                  px_val* r=PX_make_time(p_paradoxhandle,d.hour(),d.minute(),d.second());                
                  dataptr[i]->value.lval=r->value.lval;
                  break;
		}
                 case pxfDate :
     		{//cerr<<"L"<<endl;
                  dataptr[i]->type=pxfDate;
                  hk_datetime d;
                  //d.set_dateformat(p_dateformat);
                  d.set_date_asstring(xc->changed_data_asstring()); 
                  px_val* r=PX_make_date(p_paradoxhandle,d.year(),d.month(),d.day());                
                  dataptr[i]->value.lval=r->value.lval;
                  break;
		}
		 case pxfMemoBLOb :
                 case pxfFmtMemoBLOb :
     		{//cerr<<"a"<<endl;
                  const struct_raw_data* d=xc->changed_data();

                  dataptr[i]->type=pxfMemoBLOb;
                  dataptr[i]->value.str.val=d->data;
                  dataptr[i]->value.str.len=d->length;
		  break;
		}
		 
		 
		 case pxfBLOb:
                 case pxfGraphic:
     		{//cerr<<"a"<<endl;
                  const struct_raw_data* d=xc->changed_data();

                  dataptr[i]->type=pxfBLOb;
                  dataptr[i]->value.str.val=d->data;
                  dataptr[i]->value.str.len=d->length;
		  break;
		}


         }
          dataptr[i]->isnull=((xc->has_changed()?xc->changed_data_is_nullvalue():xc->is_nullvalue())?1:0);
	 }


          ++i;
          ++it;
        }




    set_has_not_changed();

	bool r=true;
         if (!blockserversignals())
        {

   int result=PX_update_record(p_paradoxhandle,dataptr,row_position());
//         cerr <<"nach update_record:"<<result<<endl;
   //datasource_close();
     for(int k=0;k<numfields;++k)
   {
      if (dataptr[k]->type==pxfAlpha)
          free (dataptr[k]->value.str.val);
      free (dataptr[k]);
   }
   free (dataptr);
   
   dataptr=NULL;

   p_paradoxheader=NULL;
                if (result>-1)
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
	            p_paradoxdatabase->paradoxconnection()->servermessage(lasterror());
                hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row could NOT be changed!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
                    if (interaction==interactive) show_warningmessage(reason);
		    r=false;
                }

        }

inform_depending_ds_after_update_row();
return r;
}


bool hk_paradoxtable::delete_rows(unsigned long from,unsigned long to,bool check_depending_datasources,enum_interaction c)
{
#ifdef HK_DEBUG
    hkdebug("hk_paradoxtable::delete_rows(unsigned long from,unsigned long to)");
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
        hk_actionquery* a = p_database->new_actionquery();
        if (a==NULL) return false;


////////////////////
    execute_visible_object_before_delete();

    if (!blockserversignals())
    {
            

	    for(long int i=t;i>=(long int)f &&exec_ok;i--)
	    {
	     int r=PX_delete_record(p_paradoxhandle,i);
            //cerr <<"result loeschen:"<<r<<endl;
            exec_ok=(r>-1);
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
            p_paradoxdatabase->paradoxconnection()->servermessage(lasterror());
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

