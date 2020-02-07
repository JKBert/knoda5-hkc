// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_qbe.h"
#include "hk_datasource.h"
#include "hk_database.h"
#include "hk_dsdatavisible.h"
#include "hk_connection.h"

class hk_qbeprivate
{
public:
hk_qbeprivate()
	{
	   p_querytype=hk_qbe::qt_select;
	   p_distinct=false;
	}
list<hk_qbe::hk_qbedataclass> p_definitionlist;
hk_qbe::enum_querytype p_querytype;
bool p_distinct;
};


hk_qbe::hk_qbe(void):hk_presentation()
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::hk_qbe");
#endif
    p_private=new hk_qbeprivate;
    p_presentationtype=hk_presentation::qbe;
}


hk_qbe::~hk_qbe()
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::~hk_qbe");
#endif
   delete p_private;
}



void hk_qbe::set_querytype(enum_querytype qt,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::set_querytype");
#endif
enum_querytype buffer=p_private->p_querytype;
p_private->p_querytype=qt;
has_changed(registerchange);
widget_specific_querytype_has_changed(buffer,qt);
}

hk_qbe::enum_querytype hk_qbe::querytype(void) const
{
   return p_private->p_querytype;
}


void hk_qbe::set_distinct(bool d,bool registerchange)
{
  p_private->p_distinct=d;
  has_changed(registerchange);
}

bool hk_qbe::distinct(void) const
{
  return p_private->p_distinct;
}

bool hk_qbe::create_sql(hk_string& sql)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_sql");
#endif
  bool result=true;
   switch(p_private->p_querytype)
   {
     case qt_select: sql=create_select_sql(result);break;
     case qt_groupselect:sql=create_groupselect_sql(result);break;
     case qt_update:sql=create_update_sql(result);break;
     case qt_delete:sql=create_delete_sql(result);break;

   }
   if (!result) cerr <<"hk_qbe: Warning error while creating sql statement"<<endl;

   return result;

}

void hk_qbe::savedata(ostream& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::savedata");
#endif
   start_mastertag(s,"QBE");
   hk_presentation::savedata(s);
   hk_string buffer;
   if (p_private->p_querytype==qt_update) buffer="UPDATE";
   else
   if (p_private->p_querytype==qt_groupselect) buffer="GROUPSELECT";
   else
   if (p_private->p_querytype==qt_delete) buffer="DELETE";
   else
   buffer="SELECT";
   set_tagvalue(s,"QUERYTYPE",buffer);
   set_tagvalue(s,"DISTINCT",p_private->p_distinct);
   list<hk_qbe::hk_qbedataclass>::iterator it=p_private->p_definitionlist.begin();
   while (it!=p_private->p_definitionlist.end())
   {
    start_mastertag(s,"QBEDEFINITION");
      set_tagvalue(s,"FIELD",(*it).field);
      set_tagvalue(s,"TABLE",(*it).table);
      set_tagvalue(s,"ALIAS",(*it).alias);
      switch ((*it).order)
      {
        case none : buffer="NONE";break;
        case ascending : buffer="ASCENDING";break;
        case descending : buffer="DESCENDING";break;
      }
      set_tagvalue(s,"ORDER",buffer);

      switch ((*it).functiontype)
      {
         case ft_condition : buffer="CONDITION";break;
        case ft_group : buffer="GROUP";break;
        case ft_sum : buffer="SUM";break;
        case ft_count : buffer="COUNT";break;
        case ft_min : buffer="MIN";break;
        case ft_max : buffer="MAX";break;
        case ft_avg : buffer="AVERAGE";break;
      }
      set_tagvalue(s,"FUNCTIONTYPE",buffer);

      set_tagvalue(s,"SHOW",(*it).show);
      set_tagvalue(s,"UPDATEVALUE",(*it).updatevalue);
         start_mastertag(s,"CONDITIONS");
	   vector<hk_string>::iterator sit=(*it).conditions.begin();
	   while (sit!=(*it).conditions.end())
	     {
              set_tagvalue(s,"CONDITION",(*sit));
		++sit;
	     }
         end_mastertag(s,"CONDITIONS");

    end_mastertag(s,"QBEDEFINITION");
    ++it;
   }
   end_mastertag(s,"QBE");
}



void hk_qbe::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::loaddata");
#endif
   hk_presentation::loaddata(definition);
   clear_definition(false);
   int i=1;
   hk_string buffer,value;
   get_tagvalue(definition,"QUERYTYPE",buffer);
   enum_querytype typebuffer=qt_select;
   if (buffer=="UPDATE") typebuffer=qt_update;
   else
   if (buffer=="GROUPSELECT") typebuffer=qt_groupselect;
   else
   if (buffer=="DELETE") typebuffer=qt_delete;
   p_private->p_distinct=false;
   get_tagvalue(definition,"DISTINCT",p_private->p_distinct);
    xmlNodePtr v=NULL;
   while ((v=get_tagvalue(definition,"QBEDEFINITION",value,i,mastertag)))
   {
      hk_string field;
      int table;
      hk_string alias;
      enum_order order=none;
      bool      show=true;
      enum_functiontype functiontype=ft_condition;
      hk_string updatevalue;
      vector<hk_string> conditionlist;
      get_tagvalue(v,"FIELD",field);
      get_tagvalue(v,"TABLE",table);
      get_tagvalue(v,"ALIAS",alias);
      if(get_tagvalue(v,"ORDER",buffer))
          {

	     if (buffer=="ASCENDING") order=ascending;
	     else
	     if (buffer=="DESCENDING") order=descending;
          }

      if (get_tagvalue(v,"FUNCTIONTYPE",buffer))
      {
          if (buffer=="GROUP") functiontype=ft_group;
	  else
          if (buffer=="SUM") functiontype=ft_sum;
	  else
          if (buffer=="COUNT") functiontype=ft_count;
	  else
          if (buffer=="MIN") functiontype=ft_min;
	  else
          if (buffer=="MAX") functiontype=ft_max;
	  else
          if (buffer=="AVERAGE") functiontype=ft_avg;
      }
      get_tagvalue(v,"UPDATEVALUE",updatevalue);
      get_tagvalue(v,"SHOW",show);

      int c=1;
      hk_string cvalue;
	while (get_tagvalue(v,"CONDITION",buffer,c))
	{
	  conditionlist.insert(conditionlist.end(),buffer);
	  ++c;
	}

	++i;
     add_definition(field,table,alias,functiontype,order,show,updatevalue,conditionlist,false);
   }
   set_querytype(typebuffer,false);

   reset_has_changed();

}




void hk_qbe::clear_definition(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::clear_definition");
#endif
p_private->p_definitionlist.erase(p_private->p_definitionlist.begin(),p_private->p_definitionlist.end());
has_changed(registerchange);
}




void hk_qbe::add_definition(  hk_qbedataclass* cl,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::add_definition(hk_qbedataclass*)");
#endif
  if (!cl) return;
  add_definition(cl->field,cl->table,cl->alias,cl->functiontype,
  		 cl->order,cl->show,cl->updatevalue,
		 cl->conditions,registerchange);
}







void hk_qbe::add_definition(	const hk_string& field,
			int table,
			const hk_string& alias,
			enum_functiontype functiontype,
			enum_order order,
			bool show,
			const hk_string& updatevalue,
			vector<hk_string>& conditions,
			bool registerchange
			)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::add_definition(const hk_string& ...)");
#endif

  if (field.size()==0) return;

  hk_qbedataclass c;
  c.field=field;
  c.table=table;
  c.alias=alias;
  c.functiontype=functiontype;
  c.order=order;
  c.show=show;
  c.updatevalue=updatevalue;
  c.conditions=conditions;
  p_private->p_definitionlist.insert(p_private->p_definitionlist.end(),c);
  has_changed(registerchange);
}






hk_string hk_qbe::create_where(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_where");
#endif
  hk_string result;
  unsigned int max=0;
    list<hk_qbe::hk_qbedataclass>::iterator it=p_private->p_definitionlist.begin();
  while (it!=p_private->p_definitionlist.end())
  {
  if ((*it).conditions.size()>max)
      max=(*it).conditions.size();
  ++it;
  }
  for(unsigned int i=0;i<max;++i)
  {
   it=p_private->p_definitionlist.begin();
   hk_string where;
   while (it!=p_private->p_definitionlist.end())
   {
    vector<hk_string> condi=(*it).conditions;

    if (i<condi.size() && condi[i].size()>0)
    {
          if (where.size()>0) where+=")\n  AND (";
	  hk_qbedataclass* cl=&(*it);

    if (cl->table>-1)
     {
       hk_string fname;
	 switch (p_private->p_querytype)
  	  {
  		case qt_update:
  		case qt_delete:
  		  {
		 fname="\""+cl->field+"\" ";
// 		 where+="\""+cl->field+"\" "+condi[i];
		  break;
		  }
  		default:
  		   fname="\""+unique_shortdatasourcename((cl->table))
	         +"\".\""+cl->field+"\" ";
/*  		   where+="\""+unique_shortdatasourcename((cl->table))
	         +"\".\""+cl->field+"\" "+condi[i];*/
  	  }
  	  hk_string r=condi[i];
  	  hk_connection* c=NULL;
  	  if (database()) c=database()->connection();
  	  hk_dsdatavisible::parsed_value(r,c);
  	  cerr <<"r="<<r<<"=";
  	  r=replace_all("%VALEND%",replace_all("%COLNAME%",r,fname),"");
  	  cerr <<r<<endl;
  	  where+=r;
    }
   }
    ++it;
   }
  if (where.size()>0)
     {
     where="(("+where+"))";
     if (result.size()>0) result+="\n  OR "+where;
     else result=where;
     }
  }  // end for
  hk_string depending=create_depending_where();
  if (result.size() > 0 )
  {
    if(depending.size() > 0)
        result="("+result+")\n  AND ("+depending+" )";
  }
  else result=depending;
  return result;
}


//




hk_string hk_qbe::create_depending_where(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_depending_where");
#endif
  list<hk_datasource*>::iterator it=datasources()->begin();
  hk_string newwhere;
  while (it!=datasources()->end())
  {
    
    if (it==datasources()->begin())
           if ((*it)->database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_JOINS)) return "";

    hk_datasource* p_depending_on_datasource=(*it)->depending_on();
    if (p_depending_on_datasource!=NULL &&!((*it)->depending_on_is_left_join()))
    {
        list<hk_string>::iterator this_ds;
        list<hk_string>::iterator master_ds;
        master_ds=(*it)->depending_on_masterfields()->begin();

        for (this_ds=(*it)->depending_on_thisfields()->begin();this_ds!=(*it)->depending_on_thisfields()->end();this_ds++)
        {
            if (newwhere=="")
	     newwhere="(";
	     else
             newwhere=newwhere+")\n  AND (";
            newwhere=newwhere+"\""+unique_shortdatasourcename((*it)->presentationnumber())+"\".\""+*this_ds+"\"=";
	    newwhere=newwhere+"\""+unique_shortdatasourcename(p_depending_on_datasource->presentationnumber())+"\""
            +"."+"\""+*master_ds+"\"";

            master_ds++;

        }

    }                                             //endif depending_on_datasource !=NULL

    ++it;
  }
  if (newwhere.size()>0) newwhere+=")";


  return newwhere;
}

hk_string hk_qbe::create_from(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_from");
#endif

hk_string result;

  list<hk_datasource*>::iterator it=datasources()->begin();
  bool needs_table_as=false;
  bool supports_left_join=false;
  while (it!=datasources()->end())
  {
   (*it)->depending_on();
   ++it;
  }
  it=datasources()->begin();
  while (it!=datasources()->end())
  {
  if (it==datasources()->begin())
    {
     needs_table_as=(*it)->database()->connection()->server_needs(hk_connection::NEEDS_TABLE_ALIAS_AS);
     supports_left_join=(*it)->database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_JOINS);
    }
  switch (p_private->p_querytype)
  {
  case qt_update:
  case qt_delete:
  		  if (result.size()>0) result+=" , ";
  		  result+="\""+(*it)->name()+"\"";
		  break;
  default: 
  	   if (supports_left_join && (*it)->depending_on()) break;
  	     if (result.size()>0) result+=" ,\n";

           result+="\""+(*it)->name()+"\""
          +(needs_table_as?" AS ":" ");
          result+="\""+unique_shortdatasourcename((*it)->presentationnumber())+"\"";
 
 	list <hk_datasource*>* l=(*it)->dependinglist();
 	if (l->size()>0)
 	{
 	list <hk_datasource*>::iterator dit=l->begin();
 	while (dit!=l->end())
 	 {
   	  result+=joindefinition(*dit);
   	  ++dit;
 	 }
 	}
  
  	   
  }
  ++it;
  }
  return result;

}


hk_string hk_qbe::joindefinition(hk_datasource* ds)
{
 if (!ds || !ds->database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_JOINS)) return "";
 hk_string result;
  bool   needs_table_as=ds->database()->connection()->server_needs(hk_connection::NEEDS_TABLE_ALIAS_AS);
  bool   supports_left_join=ds->database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_JOINS);
  	   
 if (supports_left_join && ds->depending_on_presentationdatasource()>-1 && ds->depending_on_is_left_join())
     {
  	     result="\n"+ds->database()->connection()->left_join_statement();
     }
     else
     {
       result="\n"+ds->database()->connection()->inner_join_statement();
     }



       hk_string malias=unique_shortdatasourcename(ds->depending_on_presentationdatasource());
       hk_string salias=unique_shortdatasourcename(ds->presentationnumber());
       result+="\""+ds->name()+"\""
          +(needs_table_as?" AS ":" ");
          result+="\""+salias+"\"\n ON ";
           
           
       list<hk_string>::iterator this_ds;
       list<hk_string>::iterator master_ds;
       master_ds=ds->depending_on_masterfields()->begin();
       hk_string newwhere;
       for (this_ds=ds->depending_on_thisfields()->begin();
            this_ds!=ds->depending_on_thisfields()->end();this_ds++)
            {
            if (newwhere=="")
	     newwhere="(";
	     else
             newwhere=newwhere+")\n  AND (";
            newwhere=newwhere+"\""+salias+"\".\""+*this_ds+"\"=";
	    newwhere=newwhere+"\""+malias+"\""
            +"."+"\""+*master_ds+"\"";

            master_ds++;

           }

            if (newwhere.size()>0) newwhere+=")";
            result+=newwhere;
           
 

 list <hk_datasource*>* l=ds->dependinglist();
 if (l->size()>0)
 {
 list <hk_datasource*>::iterator dit=l->begin();
 while (dit!=l->end())
 {
   result+=joindefinition(*dit);
   ++dit;
 }
 }
 
 return result;

}




/*hk_string hk_qbe::tablename(hk_qbedataclass* cl)
{ //not used function , delete later
   hk_datasource* myds=get_datasource(cl->table);
   if (myds) return "\""+myds->name()+"\"";
   else return "";
}*/





hk_string hk_qbe::create_what(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_what");
#endif
  hk_string result;
  list<hk_qbe::hk_qbedataclass>::iterator it=p_private->p_definitionlist.begin();
  while (it!=p_private->p_definitionlist.end())
  {

   if ((p_private->p_querytype==qt_select && (*it).show)||
       (p_private->p_querytype==qt_groupselect &&
      (*it).functiontype!=ft_condition ))
   {
     if (result.size()>0) result+=" , ";
     result+=fieldname(&(*it),true);
   }
  ++it;
  }

  return result;

}




hk_string hk_qbe::create_update_set(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_update_set");
#endif
 hk_string result;
  list<hk_qbe::hk_qbedataclass>::iterator it=p_private->p_definitionlist.begin();
  while (it!=p_private->p_definitionlist.end())
  {

   if ((*it).updatevalue.size()>0 )
   {
     if (result.size()>0) result+=" , ";
     result+=fieldname(&(*it),true)+" = "+(*it).updatevalue;
   }
  ++it;
  }

  return result;
}


hk_string hk_qbe::create_order_by(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_order_by");
#endif
  hk_string result;
  list<hk_qbe::hk_qbedataclass>::iterator it=p_private->p_definitionlist.begin();
  while (it!=p_private->p_definitionlist.end())
  {
   if ((*it).order!=none)
   {
     if (result.size()>0) result+=" , ";
     result+=(*it).alias.size()==0?fieldname(&(*it)):"\""+(*it).alias+"\"";
     if ((*it).order==descending) result+=" DESC";
   }
  ++it;
  }

  return result;

}


hk_string hk_qbe::create_group_by(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_group_by");
#endif
  hk_string result;
  list<hk_qbe::hk_qbedataclass>::iterator it=p_private->p_definitionlist.begin();
  while (it!=p_private->p_definitionlist.end())
  {
   if ((*it).functiontype==ft_group)
   {
     if (result.size()>0) result+=" , ";
     result+=fieldname(&(*it));
   }
  ++it;
  }

  return result;

}





void hk_qbe::widget_specific_querytype_has_changed(enum_querytype ,enum_querytype )
{

}





const list<hk_qbe::hk_qbedataclass>* hk_qbe::definitionlist(void)
{
   return &p_private->p_definitionlist;
}





hk_string hk_qbe::create_select_sql(bool& valid)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_select_sql");
#endif
  hk_string result=create_what();
  if (result.size()==0)
     {
       show_warningmessage(hk_translate("Error!\nNo output fields selected"));
       valid=false;
       return "";
     }
  hk_string from=create_from();
  if (from.size()==0)
     {
       show_warningmessage(hk_translate("Error!\nNo tables defined"));
       valid=false;
       return "";
     }

  result=(p_private->p_distinct?"SELECT DISTINCT ":"SELECT ")+result+"\nFROM\n" +from;
  hk_string where=create_where();
  if (where.size()>0)
     {
       result+="\nWHERE "+where;
     }
  hk_string orderby=create_order_by();
  if (orderby.size()>0)
     {
       result+="\nORDER BY "+orderby;
     }
     valid=true;
  return result;
}




hk_string hk_qbe::create_groupselect_sql(bool& valid)
{

#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_groupselect_sql");
#endif
  hk_string result=create_what();
  if (result.size()==0)
     {
       show_warningmessage(hk_translate("Error!\nNo output fields selected"));
       valid=false;
       return "";
     }
  hk_string from=create_from();
  if (from.size()==0)
     {
       show_warningmessage(hk_translate("Error!\nNo tables defined"));
       valid=false;
       return "";
     }



  result=(p_private->p_distinct?"SELECT DISTINCT ":"SELECT ")+result+"\nFROM\n" +from;
  hk_string group_by=create_group_by();
  if (group_by.size()==0)
     {
       show_warningmessage(hk_translate("Error!\nNo 'group by' fields selected"));
       valid=false;
       return "";
     }
  result+="\nGROUP BY\n"+group_by;

  hk_string where=create_where();
  if (where.size()>0)
     {
       result+="\nWHERE\n "+where;
     }else cerr <<"WHERE.size()==0!"<<endl;
  hk_string orderby=create_order_by();
  if (orderby.size()>0)
     {
       result+="\nORDER BY "+orderby;
     }
     valid=true;

  return result;
}






hk_string hk_qbe::create_update_sql(bool& valid)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_update_sql");
#endif
  hk_string from=create_from();
  if (from.size()==0)
     {
       show_warningmessage(hk_translate("Error!\nNo tables defined"));
       valid=false;
       return "";
     }

  hk_string set=create_update_set();
  if (set.size()==0)
     {
       show_warningmessage(hk_translate("Error!\nNo new values defined"));
       valid=false;
       return "";
     }

  hk_string result="UPDATE "+from+" SET "+set;
  hk_string where=create_where();
  if (where.size()>0)
     {
       result+=" WHERE "+where;
     }else cerr <<"WHERE.size()==0!"<<endl;


  valid=true;
  return result;
}






hk_string hk_qbe::create_delete_sql(bool& valid)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::create_delete_sql");
#endif
  hk_string from=create_from();
  if (from.size()==0)
     {
       show_warningmessage(hk_translate("Error!\nNo tables defined"));
       valid=false;
       return "";
     }
  hk_string result="DELETE FROM "+from;
  hk_string where=create_where();
  if (where.size()>0)
     {
       result+="\nWHERE\n "+where;
     }



  valid=true;
  return result;
}

hk_string hk_qbe::fieldname(hk_qbedataclass* cl,bool withalias)
{
#ifdef HK_DEBUG
    hkdebug("hk_qbe::fieldname");
#endif
  hk_string result;
  if (cl->table>-1)
  {
  switch (p_private->p_querytype)
  {
  case qt_update:
  case qt_delete:
  		  {
		  /*hk_datasource* ds=get_datasource(cl->table);
		  if (ds)
		  result="\""+ds->name()+"\"";*/
		  break;
		  }
  default:   result="\""+unique_shortdatasourcename((cl->table))+"\"";

  }
  if (result.size()>0)
  result+=cl->field=="*"?".*":".\""+cl->field+"\"";
  else
  result=cl->field=="*"?"*":"\""+cl->field+"\"";

    if (p_private->p_querytype==qt_groupselect)
    {
     hk_string function;
     switch  (cl->functiontype)
     {
        case ft_sum:function="SUM(";break;
	case ft_count:function="COUNT(";break;
	case ft_avg:function="AVG(";break;
	case ft_min:function="MIN(";break;
	case ft_max:function="MAX(";break;
        default:;
     }
     if (function.size()>0) result=function+result+")";
    }
  }
  else
  { // no table name set
    result=cl->field;
  }

  if (cl->alias.size()>0&& withalias)
   {
     result+=" AS \""+cl->alias+"\"";
   }
  return result;
}


