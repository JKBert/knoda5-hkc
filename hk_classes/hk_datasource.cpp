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
//$Revision: 1.183 $
#include "hk_datasource.h"
#include "hk_database.h"
#include "hk_column.h"
#include "hk_dsvisible.h"
#include "hk_connection.h"
#include "hk_presentation.h"
#include "hk_actionquery.h"
#include "hk_qbe.h"
#include <algorithm>
#include <list>
#include <stdlib.h>
#include <time.h>


 voidfunction* hk_datasource::p_enablefunction=NULL;
 long hk_datasource::p_enablefunctioncounter=0;


class hk_datasourceprivate
{
public:
hk_datasourceprivate(hk_datasource* ds)
	{
	  p_datasource=ds;
	  p_x=10;
	  p_y=10;
	  p_width=150;
	  p_height=150;
	  p_designwidth=1000;
	  p_designheight=1000;
	  p_progressinterval=100000;
	  p_previous_enable_problems=false;
	  p_while_disabling=false;
	  p_while_enabling=false;
	  p_while_goto_row=false;
	  p_progressdialog=NULL;
	}
hk_string sqlconvertdelimiter(const hk_string& s)
     {
     hk_string result=s;
     hk_string p_sqltextdelimiter=p_datasource->textdelimiter();
     hk_string p_identifierdelimiter=p_datasource->identifierdelimiter();
      for(unsigned int i=0;i<result.size();++i)
    		{
		if (result[i]==defaulttextdelimiter[0]&&p_sqltextdelimiter.size()>0)
		    result[i]=p_sqltextdelimiter[0];
		else
		if (result[i]==defaultidentifierdelimiter[0]&&p_identifierdelimiter.size()>0) result[i]=p_identifierdelimiter[0];
		}
    return result;
     }


        hk_string   p_filter;
        hk_string p_temporaryfilter;
        bool p_use_temporaryfilter;
        hk_string p_internalfilter;
        bool p_use_internalfilter;
        hk_string  p_sorting;
        hk_string  p_temporarysorting;
	hk_string p_definedsql;
        bool p_use_temporarysorting;
        bool    p_enable_phase;
        bool    p_rawsql;
        long    p_depending_on_presentationdatasource;
        bool        internal_dsrefresh;
        hk_datasource::enum_dependingmodes p_dependingmode;
        bool p_blockvisiblesignals;
        bool p_blockserversignals;
        bool p_blockdatasourcesignals;
	bool p_while_enabling;
	bool p_while_disabling;
	bool p_while_goto_row;
        hk_datasource::enum_accessmode p_accessmode;
        bool p_use_batchmode;
	bool p_previous_enable_problems;
	int p_x,p_y,p_width,p_height;
	int p_designwidth, p_designheight;
	int p_progressinterval;
	hk_datasource* p_datasource;
	list<hk_string>   p_columnnames;
	progress_dialogtype* p_progressdialog;
};

hk_datasource::hk_datasource(hk_database* db,hk_presentation* p):hk_data()
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::hk_datasource");
//wanna_debug(true);
#endif
    p_private=new hk_datasourceprivate(this);
    p_true="YES";
    p_false="NO";
    p_casesensitive=true;
    p_database=db;
    p_presentation=p;
    p_enabled=false;
    p_depending_on_is_left_join=false;
    p_counter=0;
    p_length=0;
    set_has_not_changed();
    p_primary_key_used=false;
    p_mode=mode_disabled;
    p_ignore_changed_data=false;
    p_depending_on_datasource=NULL;
    p_parsed_sql=NULL;
    p_readonly=false;
    p_private->p_filter="";
    p_automatic_data_update=p_database->is_automatic_data_update();
    p_actionquery=NULL;
    p_columns=NULL;
    p_private->p_enable_phase=false;
    p_tablesql_set=false;
    p_private->p_rawsql=false;
    p_dscounter=-1;
// wanna_debug(true);
    p_private->p_depending_on_presentationdatasource=-1;
    p_private->internal_dsrefresh=false;
    p_depending_on_datasource_react_on_changed_data=false;
    p_private->p_dependingmode=depending_standard;
    p_private->p_blockvisiblesignals=false;
    p_private->p_blockserversignals=false;
    p_private->p_blockdatasourcesignals=false;
    p_private->p_use_temporaryfilter=false;
    p_private->p_use_internalfilter=false;
    p_private->p_use_temporarysorting=false;
    p_private->p_accessmode=standard;
    p_private->p_use_batchmode=false;
    automatic_position_datasource();
}


hk_datasource::~hk_datasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::~hk_datasource");
#endif
    clear_visiblelist();
    clear_columnlist();
    clear_modecolumnlists();
    list<hk_datasource*>::iterator dep_it;
    dep_it=p_dependinglist.begin();
    while (dep_it!=p_dependinglist.end())
    {
        (*dep_it)->p_depending_on_datasource=NULL;
        dep_it++;
    }
    p_database->ds_remove(this);
    if (p_presentation!=NULL) p_presentation->remove_datasource(p_dscounter);
    if (p_depending_on_datasource!=NULL)
        p_depending_on_datasource->depending_ds_remove(this);
    p_depending_on_datasource=NULL;
    if (p_actionquery!=NULL) delete p_actionquery;
    if (p_parsed_sql!=NULL) delete p_parsed_sql;
    delete p_private;
#ifdef HK_DEBUG
    hkdebug("hk_datasource::~hk_datasource ENDE");
#endif

}


void hk_datasource::set_name(const hk_string& n,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::set_name");
    hkclassname(n);
#endif
    if (p_presentation!=NULL&& n.size()>0&&registerchange) p_presentation->set_has_changed();
    hk_data::set_name(n);
    if ( type()==ds_table ||type()==ds_view)
    {
        hk_string t="SELECT * FROM "+defaultidentifierdelimiter+n+defaultidentifierdelimiter+p_sql_delimiter;
        p_tablesql_set=true;
	hk_string buffer=p_viewsql;
        set_sql(t);
	p_viewsql=buffer;
        p_tablesql_set=false;

    }

    if (type()==ds_view && p_database->view_exists(n)) driver_specific_load_view();

}


hk_string hk_datasource::name(void)
{
/*   if (p_casesensitive)return hk_data::name();
   else return string2lower(hk_data::name());
*/
    return hk_data::name();
}


void hk_datasource::setmode_createtable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::setmode_createtable");
#endif
    if (type()!=ds_table || runtime_only())return;
    if (is_enabled()) disable();
    clear_columnlist();
    clear_modecolumnlists();

    p_mode=mode_createtable;
}


void hk_datasource::setmode_normal(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::setmode_normal");
#endif

    if (p_mode==mode_createtable)create_table_now();
    if (p_mode==mode_altertable) alter_table_now();
    if (!is_enabled()&&!p_private->p_enable_phase) enable();
    enum_tablemodes buffermode=p_mode;
    p_mode=mode_normal;
    if (buffermode==mode_insertrow)  inform_visible_objects_row_change();
}


void hk_datasource::setmode_insertrow(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::setmode_insertrow");
#endif
    if (is_readonly())return;
    if (!is_enabled()&&!p_private->p_enable_phase)enable();
    switch (p_mode)
    {
        case mode_normal:
        case mode_insertrow: if(check_store_changed_data())store_changed_data();
        			else p_has_changed=false;

        break;
        case mode_disabled:;
        case mode_altertable:enable();
        default  :;
    }
    p_mode=mode_insertrow;
    if (p_depending_on_datasource!=NULL)
    {
        list<hk_string>::iterator m=depending_master_fields.begin();
        for (list<hk_string>::iterator k=depending_this_fields.begin();k!=depending_this_fields.end();k++)
        {                                         //set the depending fields with the value of the master field
            hk_column* kcol=column_by_name((*k));
            if (kcol!=0)
            {
                hk_column* nk=p_depending_on_datasource->column_by_name((*m));
                if (nk!=0)
                    kcol->set_asstring(nk->asstring(false),true,false);
            }

            m++;
        }
        set_has_not_changed();
    }
//    inform_visible_objects_row_change();
    inform_visible_objects_insertmode();
    inform_depending_ds_insertmode();
    execute_visible_object_before_insert();

}


bool hk_datasource::create_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::create_table_now");
#endif
    if ((p_mode==mode_createtable)&&(p_name.size()>0))
    {
        bool res=driver_specific_create_table_now();
        if (res)
        {
            p_mode=mode_disabled;
            clear_columnlist();
            p_database->inform_datasources_filelist_changes(lt_table);
        }
        else
        {
            hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME% could not be created."),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
            show_warningmessage(reason);
        }
        return res;
    }
    else return false;
}




bool hk_datasource::create_view_now(void)
{
 bool result=driver_specific_create_view_now();
 if (!result)show_warningmessage(hk_translate("View could not be created!")+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage());
 p_database->inform_datasources_filelist_changes(lt_view);

return result;
}


bool hk_datasource::alter_view_now(void)
{

return driver_specific_alter_view_now();
}




void hk_datasource::delete_actualrow(enum_interaction c)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::delete_actualrow");
#endif
    if (p_mode==mode_normal)
    {
        p_mode=mode_deleterow;
        set_has_changed();
        store_changed_data(c);

    }

}


hk_datasource::enum_tablemodes hk_datasource::mode(void)
{
    return p_mode;
}


void hk_datasource::column_remove(hk_column* col)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::column_remove");
#endif
    if (p_columns!=NULL) p_columns->remove(col);
}


void hk_datasource::inform_visible_objects_row_change(void)
{
// Inform the visual Objects that row position has changed
#ifdef HK_DEBUG
    hkdebug("datasource::inform_visible_objects_row_change");
#endif
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it=p_visibles.begin() ;
    while(it!=p_visibles.end())
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->row_change();
    }

}



void    hk_datasource::inform_visible_objects_batch_goto_next(void)
{
#ifdef HK_DEBUG
    hkdebug("datasource::inform_visible_objects_batch_goto_next");
#endif
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it=p_visibles.begin() ;
    while(it!=p_visibles.end())
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->batch_next();
    }

}


void    hk_datasource::inform_visible_objects_batch_goto_previous(void)
{
#ifdef HK_DEBUG
    hkdebug("datasource::inform_visible_objects_batch_goto_previous");
#endif
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it=p_visibles.begin() ;
    while(it!=p_visibles.end())
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->batch_previous();
    }

}



void hk_datasource::inform_visible_objects_before_insert_row()
{
#ifdef HK_DEBUG
    hkdebug("datasource::inform_visible_objects_row_change");
#endif

     if (p_private->p_blockvisiblesignals) return;
    mark_visible_objects_as_not_handled();
    list<hk_dsvisible*>::iterator it=p_visibles.begin() ;
    while(it!=p_visibles.end())
    {
        hk_dsvisible* vi=*it;
        it++;
        if (!vi->p_already_handled)
        {
            vi->before_insert_row();
            vi->p_already_handled=true;
            it=p_visibles.begin();
        }
    }






}


void    hk_datasource::inform_when_table_structure_changes(void)
{
#ifdef HK_DEBUG
    hkdebug("datasource::inform_when_table_structure_changes");
#endif
     if (p_private->p_blockvisiblesignals) return;
    mark_visible_objects_as_not_handled();
    list<hk_dsvisible*>::iterator it=p_visibles.begin() ;
    while(it!=p_visibles.end())
    {
        hk_dsvisible* vi=*it;
        it++;
        if (!vi->p_already_handled)
        {
            vi->table_structure_changes();
            vi->p_already_handled=true;
            it=p_visibles.begin();
        }
    }






}


void    hk_datasource::inform_when_indexlist_changes(void)
{
#ifdef HK_DEBUG
    hkdebug("datasource::inform_when_table_structure_changes");
#endif
    if (p_private->p_blockvisiblesignals) return;

    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->list_changes(lt_index);
    }

}


void hk_datasource::filelist_changes(listtype t)
{
#ifdef HK_DEBUG
    hkdebug("datasource::filelist_changes");
#endif
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        vi->list_changes(t);
        it++;
    }

}


void hk_datasource::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("datasource::before_source_vanishes");
#endif
    if (p_mode==mode_createtable||p_mode==mode_altertable)
        switch (p_mode)
        {
            case mode_altertable :alter_table_now();break;
            case mode_createtable: create_table_now();break;
            default:if (check_store_changed_data()) store_changed_data();
                else p_has_changed=false;

        }
    mark_visible_objects_as_not_handled();
    list<hk_dsvisible*>::iterator it =p_visibles.begin();
    while(it!=p_visibles.end())
    {
        hk_dsvisible* vi=(*it);
        ++it;
        if (!vi->p_already_handled)
        {
            vi->p_already_handled=true;
            vi->before_source_vanishes();
            it=p_visibles.begin();
        }
    }
    if (p_presentation!=NULL) p_presentation->remove_datasource(this,false);
    p_presentation=NULL;
    disable();
#ifdef HK_DEBUG
    hkdebug("ENDE datasource::before_source_vanishes");
#endif




}


void hk_datasource::inform_visible_objects_insertmode(void)
{
// Inform the visual Objects that data has changed
#ifdef HK_DEBUG
    hkdebug("datasource::inform_visible_objects_insertmode");
#endif
    if (p_private->p_blockvisiblesignals) return;

    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->insert_mode();
    }

}


void hk_datasource::inform_visible_objects_ds_disable(void)
{
#ifdef HK_DEBUG
    hkdebug("datasource::inform_visible_objects_ds_disable");
#endif
    if (p_private->p_blockvisiblesignals) return;
    mark_visible_objects_as_not_handled();
    list<hk_dsvisible*>::iterator it=p_visibles.begin() ;
    while(it!=p_visibles.end())
    {
        hk_dsvisible* vi=*it;
        it++;
        if (!vi->p_already_handled)
        {
            vi->p_already_handled=true;
            vi->datasource_disable();
            it=p_visibles.begin();
        }
    }
}


void hk_datasource::inform_visible_objects_ds_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("datasource::inform_visible_objects_ds_enable");
#endif
    if (p_private->p_blockvisiblesignals) return;
    mark_visible_objects_as_not_handled();
    list<hk_dsvisible*>::iterator it=p_visibles.begin() ;
    while(it!=p_visibles.end())
    {
        hk_dsvisible* vi=*it;
        ++it;
        if (!vi->p_already_handled)
        {
            vi->p_already_handled=true;
            vi->datasource_enable();
            it=p_visibles.begin();
        }
    }
}

bool hk_datasource::check_store_changed_data()
{
    if ((p_automatic_data_update)&&(p_has_changed)) return true;
    else
    {
     if (p_has_changed && show_yesnodialog(replace_all("%1",hk_translate("Store changed data in table '%1'?"),name()),true))
      return true;
     else
     if (p_has_changed)
     {
      reset_changed_data();
      p_has_changed=false;
     }
    }
 return false;
}

bool hk_datasource::goto_row(unsigned long r)
{
#ifdef HK_DEBUG
    hkdebug("datasource::gotorow ",ulongint2string(r));
#endif
    if (p_private->p_accessmode!=standard&&p_private->p_use_batchmode) return false;
    bool res=true;
    if (r==p_counter&&p_mode!=mode_insertrow)
        return res;
    p_private->p_while_goto_row=true;
    inform_before_row_change();
    bool result=true;
    if (check_store_changed_data()) result=store_changed_data();
                   else
                    p_has_changed=false;
   if (!result)
   {
     p_private->p_while_goto_row=false;
    return false;
   }

    if (p_mode==mode_insertrow)setmode_normal();
    execute_visible_object_before_row_change();
    if (driver_specific_goto_row(r))
    {
        p_counter=r;
//create_actual_row_where_statement();
    }
    else res=false;
    execute_visible_object_after_row_change();

    inform_depending_ds_goto_row();
    inform_visible_objects_row_change();
    p_private->p_while_goto_row=false;

    return res;

}

bool hk_datasource::while_goto_row(void) const
{
  return     p_private->p_while_goto_row;
}


bool hk_datasource::goto_random(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::goto_random");
#endif
    goto_row(hk_random(0,max_rows()-1));
    return true;
}


bool hk_datasource::goto_first()
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::goto_first");
#endif
    return goto_row(0);
}


bool hk_datasource::goto_next()
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::goto_next");
#endif
    if (p_private->p_accessmode==batchread&&p_private->p_use_batchmode)
    {
     bool result=driver_specific_batch_goto_next();
     inform_visible_objects_batch_goto_next();
     return result;
    }
    if (p_private->p_accessmode==batchwrite&&p_private->p_use_batchmode)
    {
        inform_before_row_change();
	bool result=true;
        if (check_store_changed_data()) result=store_changed_data();
	else p_has_changed=false;
	if (result)
	{
        if (p_mode==mode_insertrow)setmode_normal();
        setmode_insertrow();
	}
        return true;
    }

    if (p_counter<(max_rows()))
        return goto_row(p_counter+1);
    else  return false;
}


bool hk_datasource::goto_previous()
{
#ifdef HK_DEBUG
    hkdebug("datasource::goto_previous");
#endif



    if (p_private->p_accessmode!=standard&&p_private->p_use_batchmode)
        {
     bool result=driver_specific_batch_goto_previous();
     inform_visible_objects_batch_goto_previous();
     return result;
     }
    driver_specific_batch_goto_previous();

    if (p_counter>0)
        return goto_row(p_counter-1);
    else return false;
}


bool hk_datasource::goto_last()
{
    return goto_row(max_rows()-1);
}


unsigned long hk_datasource::max_rows(void)
{

    return driver_specific_max_rows();
}


bool hk_datasource::set_sql(const hk_string& s,bool rawsql,bool registerchange)
{

#ifdef HK_DEBUG
    hkdebug("datasource::set_sql(hk_string)",s);
#endif
    if ((type()==ds_table )&&(!p_tablesql_set)) return false;
    if (!p_tablesql_set) p_private->p_previous_enable_problems=false;
    if ((p_presentation!=NULL&&type()==ds_query) &&registerchange) p_presentation->set_has_changed();
    p_private->p_rawsql=rawsql;
    if (!p_presentation ||(p_presentation && p_presentation->mode()==hk_presentation::designmode))
    	p_private->p_definedsql=s;
    p_sql=s;
    p_original_sql=s;
    if (type()==ds_view) p_viewsql=s;
    parse_sql();
    if (!p_private->p_rawsql)
    {
        create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);
    }
    else
    {
        p_sql=replace_all("%TRUE%",p_sql,p_true);
        p_sql=replace_all("%FALSE%",p_sql,p_false);
        p_sql=replace_dates(p_sql);
//    if (p_identifierdelimiter.size()>0)  p_sql=replace_all("\'",p_sql,p_identifierdelimiter);
//    if (p_sqltextdelimiter.size()>0) p_sql=replace_all("\"",p_sql,p_sqltextdelimiter);
    p_sql=p_private->sqlconvertdelimiter(p_sql);
    }

    return true;
}

hk_string hk_datasource::backendsql(void) const
{
  return p_sql;
}


bool hk_datasource::set_query(hk_qbe* qbe,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("datasource::set_query(hk_qbe)");
#endif

  if (!qbe)
  {
   cerr <<"no qbe set!"<<endl;
    return false;
  }
  if (type()!=ds_query && type()!=ds_view)
    {
     show_warningmessage(hk_translate("hk_datasource::set_query: Datasource is not of type query or view!"));
     return false;
    }
  hk_string s;
  if (!qbe->create_sql(s)) return false;
  if (type()==ds_view) p_viewsql=s;
  return set_sql(s,false,registerchange);
}




hk_string hk_datasource::sql(void) const
{
    if (type()==ds_view) return p_viewsql;
    return p_original_sql;
}


bool hk_datasource::is_rawsql(void)
{
    return p_private->p_rawsql;

}


bool hk_datasource::is_readonly(void) const
{
    if (type()==ds_query||type()==ds_view||p_private->p_accessmode==batchread) return true;
    if (p_private->p_accessmode==batchwrite) return false;
    else return p_readonly;
}


void hk_datasource::set_readonly(bool r)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::set_readonly");
#endif
    if (type()==ds_table)
    {
        p_readonly=r;
    }
}


bool hk_datasource::set_accessmode(enum_accessmode a)
{
    if (is_enabled()) return false;
    if (driver_specific_set_accessmode(a))
    {
        p_private->p_accessmode= a;
        return true;
    }
    else return false;

}


hk_datasource::enum_accessmode hk_datasource::accessmode(void) const
{
    return p_private->p_accessmode;

}


bool hk_datasource::enable(void)
{
#ifdef HK_DEBUG
    hkdebug("datasource::enable");
#endif

    if (p_presentation!=NULL)
    {
        if ( p_private->p_depending_on_presentationdatasource>-1)
            if (p_depending_on_datasource==NULL)
                set_depending_on_presentationdatasource(p_private->p_depending_on_presentationdatasource,p_depending_on_datasource_react_on_changed_data,p_private->p_dependingmode);
    }
   p_has_changed=false;
    if (!p_database->p_connection->is_connected())p_database->p_connection->connect();
    if (p_depending_on_datasource!=NULL)
    {
        if (!p_depending_on_datasource->is_enabled()
            ||(p_depending_on_datasource->mode()==mode_insertrow && p_private->p_dependingmode!=depending_nohandle)
            )
            return false;

    }

    if (!p_enabled)
    {

        p_private->p_while_enabling=true;
	inform_before_enable();
        if (p_columns!=NULL) clear_columnlist();
        if (p_depending_on_datasource!=NULL) create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);
        p_private->p_use_batchmode=false;
        if (p_private->p_accessmode!=standard)
        {
            p_private->p_use_batchmode=driver_specific_batch_enable();
            if (!p_private->p_use_batchmode)p_private->p_accessmode=standard;

        }

        if ((p_private->p_accessmode==standard||(p_private->p_accessmode!=standard&&(!p_private->p_use_batchmode)))?driver_specific_enable():p_private->p_use_batchmode)
        {
            p_private->p_previous_enable_problems=false;
	    p_enabled=true;
//       goto_first();
            p_counter=0;
            inform_visible_objects_new_columns_created();
            setmode_normal();
            inform_depending_ds_enable();
            inform_visible_objects_ds_enable();
            if (max_rows()==0) setmode_insertrow();
            p_private->p_while_enabling=false;

            return true;
        }
        else
	{
	   p_private->p_previous_enable_problems=true;
           p_private->p_while_enabling=false;

	   return false;
	}
    };
    p_private->p_previous_enable_problems=true;
    p_private->p_while_enabling=false;

    return false;
}

bool hk_datasource::while_enabling(void) const
{
  return p_private->p_while_enabling;
}


bool hk_datasource::disable(void)
{
#ifdef HK_DEBUG
    hkdebug("datasource::disable");
#endif
    if (!p_enabled) return true;
    p_private->p_while_disabling=true;
    inform_before_disable();
    inform_before_row_change();
    if (check_store_changed_data()) store_changed_data();
    p_has_changed=false;
    p_enabled=false;
    p_mode=mode_disabled;
    bool erg=((p_private->p_accessmode==standard||(p_private->p_accessmode!=standard &&!p_private->p_use_batchmode))?
        driver_specific_disable()
        :driver_specific_batch_disable());
    p_private->p_use_batchmode=false;
    inform_visible_objects_ds_disable();
    inform_depending_ds_disable();

       if (p_presentation && p_private->p_definedsql!=p_original_sql)
          {  // sql has changed during runtime, e.g a Python script, thus reset the original sql
            set_sql(p_private->p_definedsql,p_private->p_rawsql,false);
	  }
    p_private->p_while_disabling=false;

    return erg;
}

bool hk_datasource::while_disabling(void) const
{
  return p_private->p_while_disabling;
}

bool hk_datasource::set_enabled(bool e)
{

    if (e)  return enable();
    else return disable();

}


bool hk_datasource::is_enabled(void) const
{
    return p_enabled;

}


unsigned long hk_datasource::row_position(void)
{
    return p_counter;
}


list<hk_column*>* hk_datasource::columns(void)
{
    return driver_specific_columns();
}


void hk_datasource::clear_columnlist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::clear_columnlist");
#endif
    inform_visible_objects_before_columns_deleted();
    if (p_columns!=NULL)
    {
        list<hk_column*>::iterator it;
        it=p_columns->begin();
        while ( it!=p_columns->end() )
        {
            hk_column* xc=*it;
            ++it;
            delete xc;
        }
        delete p_columns;
    }
    p_columns=NULL;
    p_primary_key_used=false;
}


void    hk_datasource::clear_modecolumnlists(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::clear_modecolumnlists");
#endif
    list<hk_column*>::iterator it;
    it=p_newcolumns.begin();
    while ( it!=p_newcolumns.end() )
    {
        hk_column* xc=*it;
        ++it;
        delete xc;
    }
    p_newcolumns.erase(p_newcolumns.begin(),p_newcolumns.end());
    p_altercolumns.erase(p_altercolumns.begin(),p_altercolumns.end());
    p_deletecolumns.erase(p_deletecolumns.begin(),p_deletecolumns.end());

}


hk_column* hk_datasource::new_column(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::new_column");
#endif
    if ((p_mode==mode_createtable)||(p_mode==mode_altertable))
    {
        hk_column* c= driver_specific_new_column();
        if (c!=NULL)   p_newcolumns.insert(p_newcolumns.end(),c);
        return c;
    }
    else return NULL;

}


void hk_datasource::visible_add(hk_dsvisible* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::visible_add");
#endif
    p_visibles.insert(p_visibles.end(),v);
}


void hk_datasource::visible_remove(hk_dsvisible* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::visible_remove");
#endif
    p_visibles.remove(v);
}


void hk_datasource::clear_visiblelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::clear_visiblelist");
#endif
    list<hk_dsvisible*>::iterator it;
    it=p_visibles.begin();
    mark_visible_objects_as_not_handled();
    while ( it!=p_visibles.end() )
    {
        hk_dsvisible* dv=*it;
        ++it;
        if (!dv->p_already_handled)
        {
            dv->p_already_handled=true;
            dv->datasource_delete();
            p_visibles.remove(dv);
            it=p_visibles.begin();
        }
    }

}


hk_column* hk_datasource::column_by_name(const hk_string& c)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::column_by_name(char) ",c);
#endif
    return column_by_name(c,1);
}


hk_column* hk_datasource::column_by_name(const hk_string& c,int number)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::column_by_name(char,int) ",c);
#endif
    list<hk_column*>* p_l =columns();
    hk_string nc=c.substr(0,p_database->connection()->maxfieldnamesize()-1);
    int counter=1;
    if (number<1)number=1;
    hk_column* result=NULL;
    if (p_l!=NULL)
    {
#ifdef HK_DEBUG
        hkdebug("hk_datasource::column_by_name ","p_columns!=NULL");
#endif

        list<hk_column*>::iterator it;
        it=p_l->begin();
        while ( it!=p_l->end())
        {
            hk_column* xc=*it;
            ++it;
            bool r=false;
            if (p_casesensitive)   {if (  (mode()==mode_altertable?xc->p_originalcolumnname:xc->name())==nc) r=true;}
            else {if (string2lower(mode()==mode_altertable?xc->p_originalcolumnname:xc->name())==string2lower(nc)) r=true;}
            if (r)
            {
                result=xc;
                if (counter==number) return xc;
                else ++counter;

            }

        }
    }
    else
    {
#ifdef HK_DEBUG
        hkdebug("hk_datasource::column_by_name ","p_columns=NULL");
#endif

    }
#ifdef HK_DEBUG
    hkdebug("hk_datasource::column_by_name ","return=NULL!!!");
#endif
    return result;

}


int hk_datasource::columnname_occurances(const hk_string& colname)
{
    list<hk_column*>* p_l =columns();
    hk_string c=colname.substr(0,p_database->connection()->maxfieldnamesize()-1);
    if (!p_l) return 0;

    int counter=0;
    list<hk_column*>::iterator it=p_l->begin();
    while ( it!=p_l->end())
    {
        bool r=false;
        if (p_casesensitive)   {if ((*it)->name()==c) r=true;}
        else {if (string2lower((*it)->name())==string2lower(c)) r=true;}
        if (r) ++counter;
        ++it;

    }
    return counter;

}


int hk_datasource::columnname_occurance(hk_column* col)
{
    if (!col) return 0;
    list<hk_column*>* p_l =columns();
    if (!p_l) return 0;

    int counter=0;
    list<hk_column*>::iterator it=p_l->begin();
    while ( it!=p_l->end())
    {
        bool r=false;
        if (p_casesensitive)   {if ((*it)->name()==col->name()) r=true;}
        else {if (string2lower((*it)->name())==string2lower(col->name())) r=true;}
        if (r)
        {
            ++counter;
            if ((*it)==col) return counter;
        }
        ++it;

    }
    return 0;

}


bool hk_datasource::store_changed_data(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::store_changed_data");
    if (p_has_changed)hkdebug("p_ds_datachanged = true"); else hkdebug("p_ds_datachanged = false");
#endif
    if (!p_database->connection()->is_connected()||is_readonly())
    {
#ifdef HK_DEBUG
        hkdebug("hk_datasource::store_changed_data  connection not connected!");
#endif
        return false;
    }
    inform_visible_objects_before_store_changed_data();
    if ((p_ignore_changed_data) ||(p_readonly)||(!p_has_changed))
    {
#ifdef HK_DEBUG
        hkdebug("Ignore changed Data!");
#endif
        p_ignore_changed_data=false;
        reset_changed_data();
        if (p_mode==mode_insertrow&&max_rows()>0)
        {
            p_mode=mode_normal;
            goto_row(p_counter);
        }
        ;
        inform_depending_ds_after_store_changed_data();
        inform_visible_objects_after_store_changed_data();
        return true;
    };
    execute_visible_object_script_before_update();
    create_actual_row_where_statement();
    bool result=true;
    switch (p_mode)
    {
        case mode_normal:   result=update_row(interaction);
        break;
        case mode_deleterow:    result=delete_row(interaction);
        break;
        case mode_insertrow:    result=insert_row(interaction);
        break;
        default:        ;
    }
    if (result)
    {
    reset_changed_data();
    execute_visible_object_script_after_update();
    inform_depending_ds_after_store_changed_data();
    inform_visible_objects_after_store_changed_data();
    }
    //else     reset_changed_data();

    return result;
}


bool hk_datasource::update_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug( "hk_datasource::update_row()");
#endif
    if(p_actual_row_where.size()==0)
    {
        if (interaction==interactive)show_warningmessage("Internal Error: update_row() p_actual_row_where is empty");
        return false ;
    }
    char* p_changed_data=NULL;
    char* p_it=NULL;
    unsigned long new_length=p_actual_row_where.length()+name().size()+12+p_sql_delimiter.size()+2*p_identifierdelimiter.size();
// 12 for 'UPDATE '=7 + " SET " =5
    if (p_database->connection()->server_needs(hk_connection::NEEDS_NULL_TERMINATED_SQL)) ++new_length;
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

    if (p_columns!=NULL)
    {
        list<hk_column*>::iterator it;
        it=p_columns->begin();
        bool firstcolumn=true;
        while ( it!=p_columns->end() )
        {                                         //get Length of new data
            hk_column* xc=*it;
            ++it;
            if (xc!=NULL)
                if (xc->has_changed())
            {
#ifdef HK_DEBUG
                hkdebug("Änderungslänge ",(int)(xc->transformed_changed_data()->length));
#endif
                if (xc->transformed_changed_data()->data)
                    new_length+=xc->transformed_changed_data()->length+3+2*xc->get_delimiter().size()+xc->name().size();
//3 for ' = '
	            else 						      //7 for ' = NULL'
                    new_length+=7+xc->name().size();
		    new_length+=2*p_identifierdelimiter.size();
                new_length+=(firstcolumn?0:3);    //3 for ' , '

                firstcolumn=false;
            }
        }
       p_changed_data=new char[new_length];
        *p_changed_data=0;
        p_it=p_changed_data;

        it=p_columns->begin();
	bool hchanged=false;
        while ( it!=p_columns->end() )
        {
            hk_column* xc=*it;
            ++it;
			// Figure out the column type
            if (xc->has_changed())
            {   hchanged=true;
                if (*p_changed_data==0)
                {
                    memcpy(p_changed_data,"UPDATE ",7);
                    p_it+=7;

		memcpy(p_it,p_identifierdelimiter.c_str(),p_identifierdelimiter.size());
		p_it+=p_identifierdelimiter.size();
                    memcpy(p_it,name().c_str(),name().size());
                    p_it+=name().size();
		memcpy(p_it,p_identifierdelimiter.c_str(),p_identifierdelimiter.size());
		p_it+=p_identifierdelimiter.size();
                    memcpy(p_it," SET ",5);
                    p_it+=5;
#ifdef HK_DEBUG
// hkdebug(p_changed_data);
#endif
                }
                else
                {
                    memcpy(p_it," , ",3);
                    p_it+=3;
                }
		memcpy(p_it,p_identifierdelimiter.c_str(),p_identifierdelimiter.size());
		p_it+=p_identifierdelimiter.size();
                memcpy(p_it,xc->name().c_str(),xc->name().size());
                p_it+=xc->name().size();
		memcpy(p_it,p_identifierdelimiter.c_str(),p_identifierdelimiter.size());
		p_it+=p_identifierdelimiter.size();
                memcpy(p_it," = ",3);
                p_it+=3;
                if (xc->transformed_changed_data()->data)
                {
                    memcpy(p_it,xc->get_delimiter().c_str(),xc->get_delimiter().size());
                    p_it+=xc->get_delimiter().size();
                    memcpy(p_it,xc->transformed_changed_data()->data,xc->transformed_changed_data()->length);
                    p_it+=xc->transformed_changed_data()->length;
                    memcpy(p_it,xc->get_delimiter().c_str(),xc->get_delimiter().size());
                    p_it+=xc->get_delimiter().size();
                }
                else
                {
                    memcpy(p_it,"NULL",4);
                    p_it+=4;
                }

            }
        }
	if (!hchanged)
	 {
            if (interaction==interactive)show_warningmessage(hk_translate("Bug: datasource has changed, but no column has changed"));
	    return false;
	 }
    }
    memcpy(p_it,p_actual_row_where.c_str(),p_actual_row_where.size());
    p_it+=p_actual_row_where.size();
    memcpy(p_it,p_sql_delimiter.c_str(),p_sql_delimiter.size());
	if (p_database->connection()->server_needs(hk_connection::NEEDS_NULL_TERMINATED_SQL))
   	           *p_it=0;
#ifdef HK_DEBUG
    hkdebug("new_length ",(int)new_length);
//hkdebug("Change hk_string ",p_changed_data);
#endif
    set_has_not_changed();
    bool result=false;
    if (!p_private->p_blockserversignals)
    {
        if (p_actionquery!=NULL)
        {
//     cout <<"benötigt "<<p_it-p_changed_data<<" freier Platz: "<<new_length<<endl;;
            p_actionquery->set_sql(p_changed_data,new_length);
            if (p_actionquery->execute())
            {
#ifdef HK_DEBUG
                hkdebug("Anpassung hat funktioniert");
#endif
                driver_specific_update_data();
                transaction_commit();
		result=true;
            }
            else
            {
#ifdef HK_DEBUG
                hkdebug("FEHLER keine Datenspeicherung");
#endif
                transaction_rollback();
                hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row could NOT be changed!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
                if (interaction==interactive)show_warningmessage(reason);
		result =false;
            }
        }
    }
    delete[] p_changed_data;
    inform_depending_ds_after_update_row();
return result;
}







void    hk_datasource::inform_depending_ds_before_update_row(void)
{

    list<hk_datasource*>::iterator dep_it=p_dependinglist.begin();
    while (dep_it!=p_dependinglist.end())
    {
        (*dep_it)->depending_on_datasource_before_update_row();
        dep_it++;
    }
}

void    hk_datasource::inform_depending_ds_after_update_row(void)
{
    list<hk_datasource*>::iterator dep_it=p_dependinglist.begin();

    while (dep_it!=p_dependinglist.end())
    {
        (*dep_it)->depending_on_datasource_after_update_row();
        dep_it++;
    }

}








bool hk_datasource::insert_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::insert_row()");
#endif
bool result=true;
    int new_length=22+name().size()+p_sql_delimiter.size()+2*p_identifierdelimiter.size();
    if (p_database->connection()->server_needs(hk_connection::NEEDS_NULL_TERMINATED_SQL)) ++new_length;
//cout <<"Anfang: "<<new_length<<endl;
//22= INSERT_INTO_ _VALUES_()

    char* p_it;
    char* p_changed_data;
    inform_visible_objects_before_insert_row();
    if (p_columns!=NULL)
    {
        list<hk_column*>::iterator it;
        it=p_columns->begin();
        hk_string fields;
        while ( it!=p_columns->end() )
        {                                         //get Length of new data
            hk_column* xc=*it;
            ++it;
            if (xc!=NULL)
                if (xc->has_changed())
            {
                                                  //4 for NULL
                new_length+=xc->transformed_changed_data()->data?xc->transformed_changed_data()->length:4;
                if (fields.size()==0) fields=" (";
                else
                {
                    fields+=" , ";
                    new_length+=3;
                }
                fields+=p_identifierdelimiter+xc->name()+p_identifierdelimiter;
                if (xc->transformed_changed_data()->data)new_length+=2*xc->get_delimiter().size();
            }
        }

        if (fields.size()>0) fields+=" )";
        new_length+=fields.size();
//	cout <<"fields: "<<fields<<" größe: "<<fields.size()<<endl;
#ifdef HK_DEBUG
        hkdebug("�derungsl�ge Gesamt",(int)new_length);
#endif

//create INSERT SQL statement
        p_changed_data=new char[new_length];
        *p_changed_data=0;
        p_it=p_changed_data;
        it=p_columns->begin();
        while ( it!=p_columns->end() )
        {
            hk_column* xc=*it;
            ++it;
            if (xc->has_changed())
            {
                if (*p_changed_data==0)
                {
                    memcpy(p_changed_data,"INSERT INTO ",12);
                    p_it+=12;
		memcpy(p_it,p_identifierdelimiter.c_str(),p_identifierdelimiter.size());
		p_it+=p_identifierdelimiter.size();

                    memcpy(p_it,name().c_str(),name().size());
                    p_it+=name().size();
		memcpy(p_it,p_identifierdelimiter.c_str(),p_identifierdelimiter.size());
		p_it+=p_identifierdelimiter.size();
                    memcpy(p_it,fields.c_str(),fields.size());
                    p_it+=fields.size();

                    memcpy(p_it," VALUES (",9);
                    p_it+=9;
                }
                else
                {
                    memcpy(p_it," , ",3);
                    p_it+=3;
                }

//write data
                if((xc->transformed_changed_data()->data!=NULL))
                {
                    memcpy(p_it,xc->get_delimiter().c_str(),xc->get_delimiter().size());
                    p_it+=xc->get_delimiter().size();
                    memcpy(p_it,xc->transformed_changed_data()->data,xc->transformed_changed_data()->length);
                    p_it+=xc->transformed_changed_data()->length;
                    memcpy(p_it,xc->get_delimiter().c_str(),xc->get_delimiter().size());
                    p_it+=xc->get_delimiter().size();
                }
                else
                {
                    memcpy (p_it,"NULL",4);
                    p_it+=4;
                }

            }
        }
        memcpy(p_it,")",1);
        p_it+=1;
        memcpy(p_it,p_sql_delimiter.c_str(),p_sql_delimiter.size());
	p_it+=p_sql_delimiter.size();
	if (p_database->connection()->server_needs(hk_connection::NEEDS_NULL_TERMINATED_SQL))
   	           *p_it=0;

#ifdef HK_DEBUG
        hkdebug(p_changed_data);
#endif
        if (!p_private->p_blockserversignals)
        {

	    if (p_actionquery!=NULL)
            {
//     cout <<"action: berechnete Daten "<<p_it-p_changed_data<<" freier Platz: "<<new_length<<endl;;
                p_actionquery->set_sql(p_changed_data,new_length);

                transaction_begin();
                if (p_actionquery->execute())
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
                    hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row could NOT be inserted!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
                    if (interaction==interactive) show_warningmessage(reason);
		    result=false;
                }
				execute_visible_object_after_insert();

            }
        }
        if (result) set_has_not_changed();
        delete[] p_changed_data;

    }

    if (result)
    {
    setmode_normal();
    set_has_not_changed();
    }
    return result;
}


bool hk_datasource::delete_row(enum_interaction interaction)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::delete_row()");
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
    if (p_private->p_dependingmode!=depending_nohandle)
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

    hk_string str="DELETE FROM ";
    str+= p_identifierdelimiter+name()+p_identifierdelimiter+p_actual_row_where+p_sql_delimiter;
#ifdef HK_DEBUG
    hkdebug("DELETE SQL: ",str);
#endif
    if(p_actual_row_where.size()==0)
    {
        if (interaction==interactive) show_warningmessage("Internal Error: delete_row() p_actual_row_where is empty");
        return false;
    }
    bool exec_ok=false;
    p_actionquery->set_sql(str.c_str(),str.size());

    if (!p_private->p_blockserversignals)
    {
        if (subbool)
        {
            exec_ok=p_actionquery->execute();

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
            transaction_rollback();
            hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME%: Row was NOT deleted!"),name())+"\n"+hk_translate("Servermessage: ")+database()->connection()->last_servermessage();
            if (interaction==interactive) show_warningmessage(reason);
        }
    }
    execute_visible_object_after_delete();
    set_has_not_changed();
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


bool hk_datasource::depending_on_datasource_deleterow_ok(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::depending_on_datasource_deleterow_ok");
#endif
    if(p_depending_on_datasource==NULL) return true;
    bool result=true;
    switch (p_private->p_dependingmode)
    {
        case depending_delete:;
        case depending_changedelete:;
        case depending_nohandle: result=true;
        break;
        case depending_standard: if  (max_rows()>0||type()!=ds_table) result=false;
        break;
        default: result=true;
    }
    return result;
}


bool hk_datasource::depending_on_datasource_before_delete_row(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::depending_on_datasource_before_delete_row");
#endif
    if (type()!=ds_table) return false;
    bool result=false;
    switch (p_private->p_dependingmode)
    {
        case depending_delete:;
        case depending_changedelete:
        {
            if (p_private->p_blockserversignals) return true;

            hk_actionquery* a = p_database->new_actionquery();
            if (a==NULL) return false;
            hk_string query = "DELETE FROM ";
            query+=p_identifierdelimiter+name()+p_identifierdelimiter+" ";
            query+=whole_datasource_where_statement()+p_sql_delimiter;
            a->set_sql(query.c_str(),query.size());
            result=a->execute();
            delete a;
            break;
        }
        case depending_nohandle: result=true;
        break;
        case depending_standard: if  (max_rows()>0||type()!=ds_table) result=false;
        break;
        default: result=true;
    }
    return result;
}


bool    hk_datasource::set_depending_on_presentationdatasource(long d,bool react_on_data_changes, enum_dependingmodes newmode ,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::set_depending_on_presentationdatasource");
#endif

    if (p_presentation==NULL)return false;
    if (d==p_dscounter&& p_dscounter!=-1)
    {

        show_warningmessage(hk_translate("Error: Master and depending datasource are the same!"));
        return false;
    }
    if (registerchange) p_presentation->set_has_changed();
    long tmpds=p_private->p_depending_on_presentationdatasource;
    p_private->p_depending_on_presentationdatasource=d;

    if (d>-1)
    {
    p_private->internal_dsrefresh=true;
    hk_datasource* ds=p_presentation->get_datasource(d);
        if (!set_depending_on(ds,react_on_data_changes,newmode))
	{
	 p_private->p_depending_on_presentationdatasource=tmpds;
	 return false;
	}


    p_private->internal_dsrefresh=false;
   }
   else
   if (d==-1)
   {
        if (!set_depending_on(NULL,react_on_data_changes,newmode))
	{
	 p_private->p_depending_on_presentationdatasource=tmpds;
	 return false;
	}

   }

return true;
}


long    hk_datasource::depending_on_presentationdatasource(void)
{
    return p_private->p_depending_on_presentationdatasource;
}


bool hk_datasource::set_depending_on(hk_datasource* d,bool react_on_data_changes,enum_dependingmodes newmode)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::set_depending_on");
#endif
//  if (p_presentation!=NULL &&d!=NULL)if (!p_private->internal_dsrefresh)return;
    if (d==this)
    {

        show_warningmessage(hk_translate("Error: Master and depending datasource are the same!"));
        return false;
    }
    if (d)
    {
      hk_datasource* tmp=d;
      while (tmp)
      {
        if (tmp==this)
	 {
	   show_warningmessage(hk_translate("Error: setting this masterdatasource would lead to a circular dependency!"));
	   return false;

	 }

       tmp=tmp->depending_on();
      }


    }
    p_depending_on_datasource_react_on_changed_data=react_on_data_changes;
    p_private->p_dependingmode=newmode;
    if (p_depending_on_datasource!=NULL)
    {
        p_depending_on_datasource->depending_ds_remove(this);
        p_depending_on_datasource=NULL;

    }
    if (d!=NULL)
    {
        d->depending_ds_add(this);
        p_depending_on_datasource=d;
        if (p_depending_on_datasource->is_enabled()) enable();
        else disable();
    }
    else p_depending_on_datasource=NULL;
  return true;
}


void hk_datasource::clear_depending_fields(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::clear_depending_fields");
#endif
    depending_this_fields.erase(depending_this_fields.begin(),depending_this_fields.end());
    depending_master_fields.erase(depending_master_fields.begin(),depending_master_fields.end());
    if (p_presentation!=NULL&&registerchange) p_presentation->set_has_changed();

}


hk_datasource* hk_datasource::depending_on(void)
{

    if (p_private->p_depending_on_presentationdatasource>-1&&p_depending_on_datasource==NULL&&p_presentation!=NULL)
    {
        set_depending_on_presentationdatasource(p_private->p_depending_on_presentationdatasource,p_depending_on_datasource_react_on_changed_data,p_private->p_dependingmode,false);
    }
    return p_depending_on_datasource;
}


void hk_datasource::depending_ds_add(hk_datasource* d)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::depending_ds_add");
#endif
    if (d!=NULL) p_dependinglist.insert(p_dependinglist.end(),d);

}


void hk_datasource::depending_ds_remove(hk_datasource* d)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::depending_ds_remove");
#endif
    if (d!=NULL) p_dependinglist.remove(d);

}

list<hk_datasource*>* hk_datasource::dependinglist(void)
{
   return &p_dependinglist;
}

void hk_datasource::inform_depending_ds_goto_row(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_depending_ds_goto_row");
#endif
    if (p_private->p_blockdatasourcesignals) return;
    list<hk_datasource*>::iterator it;
    it=p_dependinglist.begin();

    while ( it!=p_dependinglist.end() )
    {
        hk_datasource* dv=*it;
        ++it;

        if (dv!=NULL)
        {
            dv->depending_on_datasource_row_change();
        }
    }

// dump_data();
}


void hk_datasource::inform_depending_ds_store_data(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_depending_ds_store_data");
#endif
    if (p_private->p_blockdatasourcesignals) return;
    list<hk_datasource*>::iterator it;
    it=p_dependinglist.begin();

    while ( it!=p_dependinglist.end() )
    {
        hk_datasource* dv=*it;
        ++it;
        if (dv->is_enabled() && dv->check_store_changed_data()) dv->store_changed_data();
         else dv->p_has_changed=false;

    }

}


void hk_datasource::inform_depending_ds_data_has_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_depending_ds_data_has_changed");
#endif
    if (p_private->p_blockdatasourcesignals) return;
    list<hk_datasource*>::iterator it;
    it=p_dependinglist.begin();

    while ( it!=p_dependinglist.end() )
    {
        hk_datasource* dv=*it;
        ++it;
        dv->depending_on_datasource_data_has_changed();
    }

}


void hk_datasource::inform_depending_ds_after_store_changed_data(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_depending_ds_after_store_changed_data");
#endif
    if (p_private->p_blockdatasourcesignals) return;

    list<hk_datasource*>::iterator it;
    it=p_dependinglist.begin();

    while ( it!=p_dependinglist.end() )
    {
        hk_datasource* dv=*it;
        ++it;
        dv->depending_on_datasource_after_store_changed_data();
    }

}


void hk_datasource::inform_depending_ds_insertmode(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_depending_ds_insertmode");
#endif
    if (p_private->p_blockdatasourcesignals) return;
    list<hk_datasource*>::iterator it;
    it=p_dependinglist.begin();

    while ( it!=p_dependinglist.end() )
    {
        hk_datasource* dv=*it;
        ++it;
        if (dv->is_enabled()) dv->depending_on_datasource_insertmode();
    }

}


void hk_datasource::depending_on_datasource_insertmode(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::depending_on_datasource_insertmode");
#endif
    disable();

}


void hk_datasource::parse_sql(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::parse_sql");
#endif

    hk_string tst_sql,original_sql;
    hk_string::size_type offset=0;
    original_sql=p_private->sqlconvertdelimiter(p_original_sql);
     hk_string::size_type p=original_sql.find_last_not_of(" \n\t");
     if (p!=hk_string::npos)
     {
      if (original_sql[p]==';')
       {
       original_sql.replace(p,1,"");
       }

     }
     // use the driver specific text and field delimiters
     // hk_classes standard is : SELECT "my field" WHERE "my field"='value'
    tst_sql=original_sql;
    hk_string quotedtype;
    int open_brackets=0;
    enum
    {
	S_START,S_IN_TEXT,S_IN_QUOTED, S_IN_BRACKETS_AND_QUOTED, S_IN_BRACKETS
    } state=S_START;

    for (unsigned int tt=0;tt<tst_sql.size();tt++)
    {
        tst_sql[tt]=toupper(tst_sql[tt]);
    }
    if (p_parsed_sql!=NULL)
    {
        delete p_parsed_sql;
        p_parsed_sql=NULL;
    }
    p_parsed_sql=new struct_parsed_sql;

    int    position[6];
    int from_pos, to_pos,pos;
    int size=tst_sql.length();
    list<int> sizelist;
    pos=0;
    for (int k=0;k<6;k++)position[k]=-1;

//parser begin
hk_string tag;
int row=1;
int col=1;
hk_string errormessages;
hk_string error=hk_translate("row %ROW% column %COL% :");
hk_string errorclose=error+hk_translate("Too many closing brackets\n");
hk_string erroropen=error+hk_translate("Too many open brackets\n");

while (offset<=tst_sql.size())
{
	hk_string xs(1,tst_sql[offset]);
	switch (state)
	{
	case S_START:   if (isspace(xs[0]))
				{
					if (xs=="\n")
					{
						++row;col=0;
					}
				  break;
				}
			if (xs=="(")
			{
			state=S_IN_BRACKETS;
			++open_brackets;
			}
			else
			if (xs==p_identifierdelimiter || xs==p_sqltextdelimiter)
			{
				quotedtype=xs;
				state=S_IN_QUOTED;
			}
			else state=S_IN_TEXT;

			tag=xs;
			//cerr<<"S_START"<<" "<<tag<<endl;
			break;

	case S_IN_TEXT:  {
			bool is_tag=false;
			if (isspace(xs[0]))
			{if (xs=="\n")
					{
						++row;col=0;
					}
				state=S_START;
			}
			else
			if (xs==p_identifierdelimiter || xs==p_sqltextdelimiter)
			{
				quotedtype=xs;
				state=S_IN_QUOTED;
			}
			else
			if (xs=="(")
			{
			state=S_IN_BRACKETS;
			++open_brackets;
			}
			if (state!=S_IN_TEXT)
			{
        		if (tag=="SELECT" &&position[0]==-1)
        		{
           			is_tag=true;
				position[0]=offset-6;
        			sizelist.insert(sizelist.end(),position[0]);
        		}
        		else
        		if (tag=="FROM" &&position[1]==-1)
        		{
           			is_tag=true;
        		    position[1]=offset-4;
        		    sizelist.insert(sizelist.end(),position[1]);
        		}
        		else
        		if (tag=="WHERE" &&position[2]==-1)
        		{
           			is_tag=true;
       			    position[2]=offset-5;
       			    sizelist.insert(sizelist.end(),position[2]);
       			}
       			else
       			if (tag=="GROUP" &&position[3]==-1)
       			{
           			is_tag=true;
       			    position[3]=offset-5;
       			    sizelist.insert(sizelist.end(),position[3]);
       			}
       			else
       			if (tag=="HAVING" &&position[4]==-1)
       			{
           			is_tag=true;
       			    position[4]=offset-6;
       			    sizelist.insert(sizelist.end(),position[4]);
       			}
       			else
       			if (tag=="ORDER" &&position[5]==-1)
       			{
           			is_tag=true;
       			    position[5]=offset-5;
       			    sizelist.insert(sizelist.end(),position[5]);
       			}

			if (state==S_IN_BRACKETS||state==S_IN_QUOTED)
			   tag=xs;
			if (state==S_START)
			{
			 tag+=xs;
			 //if(!is_tag)state=S_IN_TEXT;
			}
			}

			else //state==S_IN_TEXT
			{
			  if (xs==")")
			     errormessages+=
			     replace_all("%ROW%",replace_all("%COL%",errorclose,longint2string(col)),longint2string(row));
   			  tag+=xs;
			 }
			//cerr<<"S_IN_TEXT"<<" "<<tag<<endl;
			break;
			}
		case S_IN_QUOTED:
			tag+=xs;
			if (xs=="\n")
					{
						++row;col=0;
					}
			if (xs==quotedtype)
				state=S_IN_TEXT;
			//cerr<<"S_IN_QUOTED"<<" "<<tag<<endl;
			break;
		case S_IN_BRACKETS_AND_QUOTED:
			tag+=xs;
			//cerr<<"S_IN_BRACKETS_AND_QUOTED"<<" "<<tag<<endl;
			if (xs=="\n")
					{
						++row;col=0;
					}
			if (xs==quotedtype)
				state=S_IN_BRACKETS;
			break;
		case S_IN_BRACKETS:
			tag+=xs;
			//cerr<<"S_IN_BRACKETS"<<" "<<tag<<endl;
			if (xs=="\n")
					{
						++row;col=0;
					}
			if (xs==p_identifierdelimiter || xs==p_sqltextdelimiter)
			{
			  state=S_IN_BRACKETS_AND_QUOTED;
			  quotedtype=xs;
			}
			else
			if (xs=="(")++open_brackets;
			else
			if (xs==")")
			{
				if (open_brackets==0)
				   errormessages+=replace_all("%ROW%",replace_all("%COL%",errorclose,longint2string(col)),longint2string(row));
				else
				{
				--open_brackets;
				if (open_brackets==0)
				state=S_IN_TEXT;
				}
			}
			break;
	}
	++offset;++col;



}
if (open_brackets>0)errormessages+=replace_all("%ROW%",replace_all("%COL%",erroropen,longint2string(col)),longint2string(row));
sizelist.insert(sizelist.end(),size);
    sizelist.sort();
    pos=0;
    if (position[pos]!=-1)
    {                                             // SELECT PART
        from_pos=position[pos]+6;
        int z=pos+1;
        to_pos=-1;

        list<int>::iterator iter;
        iter = find(sizelist.begin(),sizelist.end(),position[pos]);
        if(iter!=sizelist.end()) iter++;
        if (iter!=sizelist.end()) to_pos=*iter-1;

        p_parsed_sql->select_part=original_sql.substr(from_pos,to_pos-from_pos+1);
	parse_fieldpart();
//FROM PART
        if (position[++pos]>-1)
        {
            from_pos=position[pos]+4;
            z=pos+1;
            to_pos=-1;
            iter = find(sizelist.begin(),sizelist.end(),position[pos]);
            if(iter!=sizelist.end()) iter++;
            if (iter!=sizelist.end()) to_pos=*iter-1;
            p_parsed_sql->from_part=original_sql.substr(from_pos,to_pos-from_pos+1);

        }
	parse_tablepart();
// WHERE PART
        if (position[++pos]>-1)
        {
            from_pos=position[pos]+5;
            z=pos+1;
            to_pos=-1;
            iter = find(sizelist.begin(),sizelist.end(),position[pos]);
            if(iter!=sizelist.end()) iter++;
            if (iter!=sizelist.end()) to_pos=*iter-1;
            p_parsed_sql->where_part=original_sql.substr(from_pos,to_pos-from_pos+1);
        }
//GROUP BY PART
        if (position[++pos]>-1)
        {
            from_pos=position[pos]+5;
            z=pos+1;
            to_pos=-1;
            iter = find(sizelist.begin(),sizelist.end(),position[pos]);
            if(iter!=sizelist.end()) iter++;
            if (iter!=sizelist.end()) to_pos=*iter-1;
            p_parsed_sql->groupby_part=original_sql.substr(from_pos,to_pos-from_pos+1);
            hk_string buffer=tst_sql.substr(from_pos,to_pos-from_pos+1);
            z=buffer.find("BY");
            if (z>-1 &&p_parsed_sql->groupby_part.size()>(unsigned)(z+2)) p_parsed_sql->groupby_part=p_parsed_sql->groupby_part.substr(z+2,p_parsed_sql->groupby_part.size());
        }

// HAVING PART
        if (position[++pos]>-1)
        {
            from_pos=position[pos]+6;
            z=pos+1;
            to_pos=-1;
            iter = find(sizelist.begin(),sizelist.end(),position[pos]);
            if(iter!=sizelist.end()) iter++;
            if (iter!=sizelist.end()) to_pos=*iter-1;
            p_parsed_sql->having_part=original_sql.substr(from_pos,to_pos-from_pos+1);
        }
// ORDER BY PART
        if (position[++pos]>-1)
        {
            from_pos=position[pos]+5;
            to_pos=-1;
            iter = find(sizelist.begin(),sizelist.end(),position[pos]);
            if(iter!=sizelist.end()) iter++;
            if (iter!=sizelist.end()) to_pos=*iter-1;
            p_parsed_sql->orderby_part=original_sql.substr(from_pos,to_pos-from_pos+1);
            hk_string buffer=tst_sql.substr(from_pos,to_pos-from_pos+1);
            hk_string::size_type zb=buffer.find("BY");
//             z=buffer.find("BY");
            if (zb!=hk_string::npos&& p_parsed_sql->orderby_part.size()>(unsigned)(zb+2))
            {
                p_parsed_sql->orderby_part=p_parsed_sql->orderby_part.substr(zb+2,p_parsed_sql->orderby_part.size());
            }
        }

    }
if (errormessages.size()>0)
{
cerr <<endl<<endl<<"======================="<<endl<<errormessages<<endl;//show_warningmessage(errormessages);
cerr <<"SQL: "<<p_original_sql<<endl<<endl;
cerr <<"SELECT: '"<<p_parsed_sql->select_part<<"'"<<endl;
cerr <<"FROM: '"<<p_parsed_sql->from_part<<"'"<<endl;
cerr <<"WHERE: '"<<p_parsed_sql->where_part<<"'"<<endl;
cerr <<"GROUP BY: '"<<p_parsed_sql->groupby_part<<"'"<<endl;
cerr <<"HAVING: '"<<p_parsed_sql->having_part<<"'"<<endl;
cerr <<"ORDER BY: '"<<p_parsed_sql->orderby_part<<"'"<<endl<<"======================="<<endl<<endl;

}

}







void hk_datasource::parse_fieldpart(void)
{
    //return;
    /*
    The following code does not work correctly yet
    */
    if (!p_parsed_sql) return;
    hk_string::size_type offset=0;
    hk_string quotedtype;
    int open_brackets=0;
    hk_string tststring=p_parsed_sql->select_part;
    enum
    {
	S_START,S_IN_DEFINITION,S_IN_IDENTIFIERQUOTED,S_IN_ALIAS,S_IN_ALIASQUOTED,S_IN_QUOTED, S_IN_BRACKETS_AND_QUOTED, S_IN_BRACKETS
    } state=S_START;



//parser begin
hk_string tag;
hk_string nexttag;
int row=1;
int col=1;
hk_string errormessages;
hk_string error="row %ROW% column %COL% :";
hk_string errorclose=error+"Too many closing brackets\n";
hk_string erroropen=error+"Too many open brackets\n";
// cerr <<"TSTSTRING:#"<<tststring<<"#"<<endl;
fieldoriginclass field;
while (offset<=tststring.size())
{
	hk_string xs(1,tststring[offset]);
// 	cerr <<"xs:#"<<xs<<"# tag:#"<<tag<<endl;
	switch (state)
	{
	case S_START:   
// 			cerr <<"S_START"<<endl;
			nexttag="";
			if (isspace(xs[0]))
				{
					if (xs=="\n")
					{
						++row;col=0;
					}
				  break;
				}
 			 tag=xs;
			if (xs=="(")
			{
			state=S_IN_BRACKETS;
			++open_brackets;

			}
			else
			if (xs==p_identifierdelimiter)
			 {
			   state=S_IN_IDENTIFIERQUOTED;

			 }
			else
			if (xs==p_sqltextdelimiter)
			{
				quotedtype=xs;
				state=S_IN_QUOTED;
			}
			else
			{
			 state=S_IN_DEFINITION;
			}
			break;




	case S_IN_DEFINITION:
	           { 
// 	           cerr <<"S_IN_DEFINITION: nexttag:#"<<nexttag<<"#"<<endl;
			if (isspace(xs[0]))
			{
			 if (xs=="\n")
				{
				 ++row;col=0;
				}
        		if (string2upper(nexttag)=="AS")
        		{
				state=S_IN_ALIAS;
				field.fieldname=trim(tag.substr(0,tag.size()-2));
				tag="";
				break;
        		}
        		else
        		{  
        		  state=S_IN_ALIAS;
				field.fieldname=trim(tag);
				tag="";
				break;
        		  
        		}
        		nexttag="";
				//ignore white spaces
			}
			else
			if (xs==p_identifierdelimiter)
			{
			  state=S_IN_IDENTIFIERQUOTED;
			}
			else
			 if( xs==p_sqltextdelimiter)
			{
				quotedtype=xs;
				state=S_IN_QUOTED;
			}
			else
			if (xs=="(")
			{
			state=S_IN_BRACKETS;
			++open_brackets;
			}
			else

			if (xs==",")
			{
			field.fieldname=trim(tag);
			p_parsed_sql->fieldpart.insert(p_parsed_sql->fieldpart.end(),field);
			field.fieldname="";
			field.alias="";
			tag="";
			state=S_START;

			}

    		        if (isspace(xs[0]))
			  {
			 nexttag="";
			}
			else nexttag+=xs;
    		        tag+=xs;

			break;
			}



	case S_IN_QUOTED:
//  			cerr <<"S_IN_QUOTED"<<endl;
			if (xs=="\n")
					{
						++row;col=0;
					}
			if (xs==p_sqltextdelimiter)
			   state=S_IN_DEFINITION;
			tag=tag+xs;
			break;

	case S_IN_BRACKETS_AND_QUOTED:
// 			cerr <<"S_IN_BRACKETS_AND_QUOTED"<<endl;
			tag+=xs;
			if (xs=="\n")
					{
						++row;col=0;
					}
			if (xs==quotedtype)
				state=S_IN_BRACKETS;
			break;

	case S_IN_BRACKETS:
// 			cerr <<"S_IN_BRACKETS"<<endl;
			nexttag="";
			tag+=xs;
			if (xs=="\n")
					{
						++row;col=0;
					}
			if (xs==p_identifierdelimiter || xs==p_sqltextdelimiter)
			{
			  state=S_IN_BRACKETS_AND_QUOTED;
			  quotedtype=xs;
			}
			else
			if (xs=="(")++open_brackets;
			else
			if (xs==")")
			{
				if (open_brackets==0)
				   errormessages+=replace_all("%ROW%",replace_all("%COL%",errorclose,longint2string(col)),longint2string(row));
				else
				{
				--open_brackets;
				if (open_brackets==0)
				state=S_IN_DEFINITION;
				}
			}
			break;



	case S_IN_IDENTIFIERQUOTED:     
// 					cerr <<"S_IN_IDENTIFIERQUOTED"<<endl;
					nexttag="";
					if (xs==p_identifierdelimiter)
					   state=S_IN_DEFINITION;
					tag=tag+xs;
					break;



	case S_IN_ALIASQUOTED:		
// 					cerr <<"S_IN_ALIASQUOTED"<<endl;
					{
					if (xs!=p_identifierdelimiter)
					{
					  tag=tag+xs;

					}
					else
					{
					hk_string comma=",";
					while(offset<=tststring.size()&&','!=tststring[offset])
					   ++offset;
					state=S_START;
					{

					field.alias=tag;
					p_parsed_sql->fieldpart.insert(p_parsed_sql->fieldpart.end(),field);
					field.fieldname="";
					field.alias="";
					tag="";
					state=S_START;

					}
					}
					break;
					};

	case S_IN_ALIAS:		
// 					cerr<<"S_IN_ALIAS tag:#"<<tag<<"#"<<endl;
					if (tag.size()==0&& xs==p_identifierdelimiter)
					{
					   state=S_IN_ALIASQUOTED;
					   break;


					}
					if (xs=="(")
					{
					   cerr <<"Error! '(' in alias definition!"<<endl;
					   return;
					}
					if (isspace(xs[0])&& string2upper(tag)=="AS")
					{
					  tag="";
					  break;
					}
					
					if (xs==",")
					{

					field.alias=trim(tag);
					p_parsed_sql->fieldpart.insert(p_parsed_sql->fieldpart.end(),field);
					field.fieldname="";
					field.alias="";
					tag="";
					state=S_START;

					}
					else
					{
					if (isspace(xs[0])&& tag.size()==0)
					{
					  ;
					}
					else
					tag+=xs;
					}
					break;



	}
	++offset;++col;



}

  switch (state)
  {
    case S_IN_ALIAS:			field.alias=trim(tag);break;
    default:				field.fieldname=trim(tag);
					break;
  }
  if (field.fieldname.size()>0)
           p_parsed_sql->fieldpart.insert(p_parsed_sql->fieldpart.end(),field);

  return;
  list<fieldoriginclass>::iterator it=p_parsed_sql->fieldpart.begin();

  while (it!=p_parsed_sql->fieldpart.end())
  {
    cerr <<"selectdef:"<<endl;
    cerr <<"=========="<<endl;
    cerr <<"  fieldname: #"<<(*it).fieldname<<"#"<<endl;
    cerr <<"  alias    : #"<<(*it).alias<<"#"<<endl;
    cerr <<"=========="<<endl<<endl;

   ++it;
  }

cerr <<tststring<<endl;
}














void hk_datasource::parse_tablepart(void)
{
    //return;
    /*
    The following code does not work correctly yet
    */
    if (!p_parsed_sql) return;
    hk_string::size_type offset=0;
    hk_string quotedtype;
hk_string nexttag;
    hk_string tststring=p_parsed_sql->from_part;
    enum
    {
	S_START,S_IN_DEFINITION,S_IN_IDENTIFIERQUOTED,S_IN_ALIAS,S_IN_ALIASQUOTED
    } state=S_START;



//parser begin
hk_string tag;
int row=1;
int col=1;
hk_string errormessages;
hk_string error="row %ROW% column %COL% :";
hk_string errorclose=error+"Too many closing brackets\n";
hk_string erroropen=error+"Too many open brackets\n";
pair<hk_string,hk_string> table;
while (offset<=tststring.size())
{
	hk_string xs(1,tststring[offset]);
	switch (state)
	{



	case S_START:
			nexttag="";
			tag="";
			if (isspace(xs[0]))
				{
					if (xs=="\n")
					{
						++row;col=0;
					}
				  break;
				}
			if (xs==p_identifierdelimiter)
			 {
			   state=S_IN_IDENTIFIERQUOTED;
			   break;

			 }
			else
			{
			 state=S_IN_DEFINITION;
			}
 			 tag=xs;
			break;




	case S_IN_DEFINITION:
	           {
			if (isspace(xs[0]))
			{
			 if (xs=="\n")
				{
				 ++row;col=0;
				}
        		if (string2upper(nexttag)=="AS")
        		{
				state=S_IN_ALIAS;
				table.first=trim(tag.substr(0,tag.size()-2));
				tag="";
				break;
        		}
        		else
        		{
				state=S_IN_ALIAS;
				table.first=trim(tag);
				tag="";
				break;
        		}

        		nexttag="";
				//ignore white spaces
			}
			else
			if (xs==p_identifierdelimiter)
			{
			  state=S_IN_IDENTIFIERQUOTED;
			}
			else

			if (xs==",")
			{
			table.first=trim(tag);
			p_parsed_sql->tablepart.insert(p_parsed_sql->tablepart.end(),table);
			table.first="";
			table.second="";
			tag="";
			state=S_START;

			}
    		        if (isspace(xs[0]))
			  {
			 nexttag="";
			}
			else nexttag+=xs;
    		        tag+=xs;

			break;
			}





	case S_IN_IDENTIFIERQUOTED:
					nexttag="";
					if (xs==p_identifierdelimiter)
					{
					   state=S_IN_DEFINITION;
					 }
					else
					{
					tag=tag+xs;
					}
					break;
	case S_IN_ALIASQUOTED:
	{
					if (xs!=p_identifierdelimiter)
					{
					  tag=tag+xs;

					}
					else
					{
					hk_string comma=",";
					while(offset<=tststring.size()&&','!=tststring[offset])
					   ++offset;
					state=S_START;
					{

					table.second=tag;
					p_parsed_sql->tablepart.insert(p_parsed_sql->tablepart.end(),table);
					table.first="";
					table.second="";
					tag="";
					state=S_START;

					}
					}
					break;
					};

	case S_IN_ALIAS:
					if (tag.size()==0&& xs==p_identifierdelimiter)
					{
					   state=S_IN_ALIASQUOTED;
					   break;


					}
					if (xs=="(")
					{
					   cerr <<"Error! '(' in alias definition!"<<endl;
					   return;
					}
					if (xs==",")
					{

					table.second=tag;
					p_parsed_sql->tablepart.insert(p_parsed_sql->tablepart.end(),table);
					table.first="";
					table.second="";
					tag="";
					state=S_START;

					}
				else
					{
					if (isspace(xs[0])&& tag.size()==0)
					{
					  ;
					}
					else
					tag+=xs;
					}
					break;


	}
	++offset;++col;



}

      switch (state)
  {
    case S_IN_ALIAS:			table.second=trim(tag);break;
    default:				table.first=trim(tag);
					break;
  }
  if (table.first.size()>0)
           p_parsed_sql->tablepart.insert(p_parsed_sql->tablepart.end(),table);




   return;
  list<pair<hk_string,hk_string> >::iterator it=p_parsed_sql->tablepart.begin();

  while (it!=p_parsed_sql->tablepart.end())
  {
    cerr <<"selectdef:"<<endl;
    cerr <<"=========="<<endl;
    cerr <<"  tablename : #"<<(*it).first<<"#"<<endl;
    cerr <<"  tablealias: #"<<(*it).second<<"#"<<endl;
    cerr <<"=========="<<endl<<endl;

   ++it;
  }

cerr <<tststring<<endl;
}
















void    hk_datasource::depending_on_datasource_data_has_changed()
{
    if (p_depending_on_datasource!=NULL)
        if (   p_depending_on_datasource_react_on_changed_data
        && p_depending_on_datasource->is_enabled()
        ) reload_data(true);
}


void hk_datasource::depending_on_datasource_after_store_changed_data(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::depending_on_datasource_after_store_changed_data");
#endif

    bool g_r=false;
    hk_datasource* d=p_depending_on_datasource;
    while(d)
    {
     if (d->while_goto_row()) g_r=true;
     d=d->depending_on();
    }

    if (p_depending_on_datasource&& !g_r)depending_on_datasource_row_change();
}


void hk_datasource::depending_on_datasource_row_change(bool take_changed_data)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::depending_on_datasource_row_change");
#endif

//wenn ds sich ändert muss sql statement WHERE angepasst werden
    if (p_depending_on_datasource!=NULL)
    {
        if (check_store_changed_data()) store_changed_data();
            else p_has_changed=false;

        if ((p_depending_on_datasource->is_enabled())&&p_depending_on_datasource->mode()==mode_normal&&!p_depending_on_datasource->while_disabling() &&!p_depending_on_datasource->while_enabling())
        {
            reload_data(take_changed_data);
        }

        else
        {
            disable();
        }
    }
}


void hk_datasource::reload_data(bool take_changed_data)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::reload_data");
#endif
    if(check_store_changed_data()) store_changed_data();
    else p_has_changed=false;
    create_new_sql_statement(take_changed_data);
    inform_before_disable();
    (p_private->p_accessmode==standard||(p_private->p_accessmode!=standard &&!p_private->p_use_batchmode))?driver_specific_disable():driver_specific_batch_disable();
    p_mode=mode_disabled;
    p_enabled=false;
    p_has_changed=false;
    p_private->p_use_batchmode=false;
    if (p_private->p_accessmode!=standard)
    {
        p_private->p_use_batchmode=driver_specific_batch_enable();
        if (!p_private->p_use_batchmode)p_private->p_accessmode=standard;

    }
    if ((p_private->p_accessmode==standard||(p_private->p_accessmode!=standard&&(!p_private->p_use_batchmode)))?driver_specific_enable():false)
    {
        p_private->p_enable_phase=true;
        setmode_normal();
        p_private->p_enable_phase=false;
        if (max_rows()>0)
        {
            p_enabled=true;
            goto_first();
        }
        else
        {
            if (!is_readonly())
            {
                p_enabled=true;
                setmode_insertrow();
            }
            else p_enabled=true;
        }
    }
    if (p_enabled)
    {
    bool w_e=false;
    hk_datasource* d=p_depending_on_datasource;
    while(d)
    {
     if (d->while_enabling()) w_e=true;
     d=d->depending_on();
    }
    inform_visible_objects_new_columns_created();

    if (!w_e)
        inform_depending_ds_goto_row();

    inform_visible_objects_ds_enable();
    }

}


void    hk_datasource::add_depending_fields(const hk_string& this_ds_field, const hk_string& master_ds_field,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::add_depending_fields");
#endif

    if ((this_ds_field=="")||(master_ds_field=="")) return;
    if (p_presentation!=NULL&&registerchange) p_presentation->set_has_changed();
    depending_this_fields.insert(depending_this_fields.end(),this_ds_field);
    depending_master_fields.insert(depending_master_fields.end(),master_ds_field);
#ifdef HK_DEBUG
    hkdebug("hk_datasource::add_depending_fields ENDE");
#endif

}


void hk_datasource::create_new_sql_statement(bool take_changed_data)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::create_new_sql_statement");
#endif
    if (p_private->p_rawsql ||p_parsed_sql==NULL) return;
    hk_string delimiter;
    hk_string buffer,buffer2;
//construct the whole SQL statement;
    buffer2="SELECT "+p_parsed_sql->select_part;
    if (p_parsed_sql->from_part!="")
    {
        buffer2=buffer2+" FROM ";
        buffer2=buffer2+p_parsed_sql->from_part;
    }
    if (database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_WHERE))
    buffer2+=whole_datasource_where_statement(take_changed_data);
    if (p_parsed_sql->groupby_part!="" &&database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_GROUP_BY))
       {buffer2=buffer2+" GROUP BY ";buffer2=buffer2+p_parsed_sql->groupby_part;}
    if (p_parsed_sql->having_part!=""&&database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_HAVING))
       {buffer2=buffer2+" HAVING ";buffer2=buffer2+p_parsed_sql->having_part;}

 if (database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_ORDER_BY))
 {
    bool orderbyset=false;
    if (p_private->p_use_temporarysorting && p_private->p_temporarysorting.size()>0)
    {
        buffer2=buffer2+" ORDER BY ";
        buffer2=buffer2+p_private->sqlconvertdelimiter(p_private->p_temporarysorting);
        orderbyset=true;
    }
    if (p_private->p_sorting!="")
    {
        buffer2=buffer2+(orderbyset?" , ":" ORDER BY ");
        buffer2=buffer2+p_private->sqlconvertdelimiter(p_private->p_sorting);
        orderbyset=true;
    }
    if (p_parsed_sql->orderby_part!="")
    {
        buffer2=buffer2+(orderbyset?" , ":" ORDER BY ");
        buffer2=buffer2+p_private->sqlconvertdelimiter(p_parsed_sql->orderby_part);
    }
 }
#ifdef HK_DEBUG
    hkdebug("BUFFER2: ",buffer2);
#endif
    buffer2+=p_sql_delimiter;
    
   
    p_sql=replace_all("%TRUE%",replace_dates(buffer2),p_true);
    p_sql=replace_all("%FALSE%",p_sql,p_false);
}


hk_string hk_datasource::replace_dates(const hk_string& sqlstatement)
{
    hk_string result;
    enum {S_TXT,S_QUOTED,S_DATE} state=S_TXT;
    hk_string::size_type offset=0;
   hk_string quoted;
   hk_string datval;

    while (offset<sqlstatement.size())
    {
        hk_string xc(1,sqlstatement[offset]);               //row[offset];
//          cerr <<offset<<": #"<<xc<<"# ";
        ++offset;
        switch (state)
        {
         case S_TXT:
         	         if (xc=="\"" ||xc=="'")
         	          {
         	            state=S_QUOTED;
         	            quoted=xc;	
         	          result+=xc;
         	          }
         	         else
         	         if (xc=="#")
         	          {
         	            state=S_DATE;
         	            datval="";
         	          }
         	         else
         	          result+=xc;

         		break;
         case S_QUOTED:
         	         if (xc==quoted)
         	          {
         	            state=S_TXT;
         	            quoted="";	
         	          }
         	          result+=xc;
         		break;
         case S_DATE:
         	         if (xc=="#")
         	          {
         	            state=S_TXT;
         	            hk_datetime dt;
         	            hk_column* c= driver_specific_new_column();

         	            result+="'"+transfer_date(datval,c->p_dateformat,c->p_driverspecific_dateformat)+"'";
         	            delete c;
         	          }
         	          else
         	           datval+=xc;
         		break;
         
        }
     }
  return result; 
}

void hk_datasource::set_filter(const hk_string& f,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::set_filter");
#endif
    if (p_presentation!=NULL&& f.size()>0&&registerchange) p_presentation->set_has_changed();

    p_private->p_filter=f;
    create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);
}


void hk_datasource::set_temporaryfilter(const hk_string&f)
{
    p_private->p_temporaryfilter=f;
    create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);
}


hk_string hk_datasource::temporaryfilter(void)
{
    return p_private->p_temporaryfilter;
}


void hk_datasource::set_use_temporaryfilter(bool use)
{
    p_private->p_use_temporaryfilter=use;
    create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);
}


bool hk_datasource::use_temporaryfilter(void)
{
    return p_private->p_use_temporaryfilter;
}



void hk_datasource::set_internalfilter(const hk_string&f)
{
    p_private->p_internalfilter=f;
    create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);
}


hk_string hk_datasource::internalfilter(void)
{
    return p_private->p_internalfilter;
}

void hk_datasource::set_use_internalfilter(bool use)
{
    p_private->p_use_internalfilter=use;
    create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);
}

bool hk_datasource::use_internalfilter(void)
{
    return p_private->p_use_internalfilter;
}










void hk_datasource::set_sorting(const hk_string& s,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::set_sorting");
#endif
    if (p_presentation!=NULL&& s.size()>0&&registerchange) p_presentation->set_has_changed();
    p_private->p_sorting=s;
    create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);
}


void hk_datasource::set_temporarysorting(const hk_string& s)
{
    p_private->p_temporarysorting=s;

}


hk_string hk_datasource::temporarysorting(void)
{

    return p_private->p_temporarysorting;
}


void hk_datasource::set_use_temporarysorting(bool use)
{
    p_private->p_use_temporarysorting=use;
    create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);

}


bool hk_datasource::use_temporarysorting(void)
{
    return p_private->p_use_temporarysorting;

}


hk_string hk_datasource::filter(void)
{

    return p_private->p_filter;
}


hk_string hk_datasource::sorting(void)
{
    return p_private->p_sorting;
}


void hk_datasource::clear_filter(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::clear_filter");
#endif
    if (p_presentation!=NULL&& (p_private->p_filter.size()>0)&&registerchange) p_presentation->set_has_changed();
    p_private->p_filter="";
    create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);
}


void    hk_datasource::clear_sorting(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::clear_sorting");
#endif
    if (p_presentation!=NULL&& (p_private->p_sorting.size()>0)&&registerchange) p_presentation->set_has_changed();
    p_private->p_sorting="";
    create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);

}


void hk_datasource::set_automatic_data_update(bool u)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::set_automatic_data_update");
#endif
    p_automatic_data_update=u;
}


bool hk_datasource::is_automatic_data_update(void)
{
    return p_automatic_data_update;
}


void hk_datasource::create_actual_row_where_statement(void)
{
    p_actual_row_where=create_row_where_statement_at(p_counter);
}


hk_string hk_datasource::create_row_where_statement_at(long unsigned int position,bool withwhere)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::create_row_where_statement_at");
#endif
    hk_string buffer;
    hk_string p_row_where="";
    list<hk_column*>::iterator col;
    if (p_primary_key_used)
    {                                             //table uses primary key
        for(col=p_columns->begin();col!=p_columns->end();col++)
        {
            hk_column* cl=*col;
            if (cl->is_primary())
            {
                if (p_row_where==""){ if (withwhere)p_row_where=" WHERE ";}
                else p_row_where+=" AND ";
                buffer=delimit_identifier(cl->name());buffer+=" = ";buffer+=cl->get_delimiter();
                buffer+=cl->transformed_asstring_at(position);
                buffer+=cl->get_delimiter();
                buffer+=" ";
                p_row_where+=buffer;
            }

        }
    }
    else
    {                                             //no primary key used
        for(col=p_columns->begin();col!=p_columns->end();col++)
        {
            if ((*col)->columntype()!=hk_column::binarycolumn)
            {
                if (p_row_where=="") {if (withwhere)p_row_where=" WHERE ";}
                else p_row_where+=" AND ";

                buffer=delimit_identifier((*col)->name());
                if ((*col)->is_nullvalue_at(position))
                {
                    buffer+=" IS NULL ";
                }
                else
                {
                    buffer+=" = ";
                    buffer+=(*col)->get_delimiter();
                    buffer+=(*col)->transformed_asstring_at(position);
                    buffer+=(*col)->get_delimiter();
                    buffer+=" ";
                }
                p_row_where+=buffer;
            }
        }
    }

#ifdef HK_DEBUG
    hkdebug("ROW WHERE ",p_row_where);
#endif
    return p_row_where;
}


void hk_datasource::reset_changed_data(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::reset_changed_data");
#endif
    if (p_columns!=NULL)
    {
        list<hk_column*>::iterator it;
        it=p_columns->begin();
        while (it!=p_columns->end())
        {
            (*it)->reset_changed_data();
            it++;
        }
        set_has_not_changed();
    }
}


void hk_datasource::inform_before_row_change()
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_before_row_change");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->before_row_change();
    }

    inform_depending_ds_store_data();
}


void hk_datasource::inform_visible_objects_before_store_changed_data()
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_visible_objects_before_store_changed_data");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;

    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->before_store_changed_data();
    }

}


void hk_datasource::inform_visible_objects_after_store_changed_data()
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_visible_objects_after_store_changed_data");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->after_store_changed_data();
    }

}

void hk_datasource::set_columns_tableorigin(void)
{
  if (!p_parsed_sql||type()==ds_table) return ;
  list <pair<hk_string,hk_string> >::iterator it=p_parsed_sql->tablepart.begin();

  hk_datasource* ds= p_database->new_table();
  if (!ds) return;
  while (it!=p_parsed_sql->tablepart.end())
  {
    ds->set_name((*it).first);
    list<hk_column*>* c=columns();
    list<hk_column*>::iterator colit=c->begin();
    while (colit!=c->end())
     {
     (*colit)->p_table_origin_already_set=true;

     if ((*colit)->tableorigin().size()==0)
      (*colit)->p_tableorigin= columntablepart(fieldorigin((*colit)->name()));


	if ((*colit)->tableorigin().size()==0)
	  {

	     hk_column* col=ds->column_by_name(fieldorigin((*colit)->name()));
	     if (col) (*colit)->p_tableorigin=(*it).second;
	  }
	++colit;
     }
    ++it;
  }
    delete ds;
}



hk_string hk_datasource::columntablepart(const hk_string& s)
{
  return "";
    hk_string::size_type offset=0;
    hk_string quotedtype;
    hk_string tststring=trim(s);
    enum
    {
	S_START,S_IN_IDENTIFIER,S_IN_IDENTIFIERQUOTED
    } state=S_START;


//parser begin
hk_string tag;
hk_string errormessages;
while (offset<=tststring.size())
{
	hk_string xs(1,tststring[offset]);
	switch (state)
	{
	case S_START:
			tag="";
			if (isspace(xs[0]))
				  break;
			if (xs==p_identifierdelimiter)
			 {
			   state=S_IN_IDENTIFIERQUOTED;
			   break;

			 }
			else
			{
			 state=S_IN_IDENTIFIER;
			}
 			tag=xs;
			break;




	case S_IN_IDENTIFIER:
	           {
			if (isspace(xs[0]))
			{

			 return "";
			}
			else
			if (xs==p_identifierdelimiter)
			  state=S_IN_IDENTIFIERQUOTED;
			else
			if (xs==",")
			{
			 return "";
			}
			else
			if (xs==".")
			{
			 return tag;
			}

    		        tag+=xs;
			break;
			}



	case S_IN_IDENTIFIERQUOTED:
					if (xs==p_identifierdelimiter)
					{
					   state=S_IN_IDENTIFIER;
					 }
					else
					{
					tag=tag+xs;
					}
					break;


	}
	++offset;
}

  return "";
}


void    hk_datasource::inform_visible_objects_new_columns_created(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_visible_objects_new_columns_created");
#endif
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it=p_visibles.begin() ;
    mark_visible_objects_as_not_handled();
    while(it!=p_visibles.end())
    {
        (*it)->columns_new_created();
        it++;
    }

}


void hk_datasource::inform_before_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_before_enable");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->before_datasource_enables();
    }

}


void hk_datasource::inform_before_disable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_before_disable");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->before_row_change();
    }

}

void hk_datasource::execute_visible_object_script_before_update(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_before_update");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->action_before_store_changed_data();
    }


}



void hk_datasource::execute_visible_object_script_after_update(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_after_update");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->action_after_store_changed_data();
    }


}



void hk_datasource::execute_visible_object_before_delete(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_before_delete");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->action_before_delete();
    }


}





void hk_datasource::execute_visible_object_after_delete(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_after_delete");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->action_after_delete();
    }


}




void hk_datasource::execute_visible_object_before_insert(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_before_insert");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->action_before_insert();
    }


}





void hk_datasource::execute_visible_object_after_insert(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_after_insert");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->action_after_insert();
    }


}










void hk_datasource::execute_visible_object_before_row_change(void)
{
if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->action_before_row_change();
    }

}

void hk_datasource::execute_visible_object_after_row_change(void)
{
if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->action_after_row_change();
    }

}


void hk_datasource::inform_depending_ds_disable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_depending_ds_disable");
#endif
    if (p_private->p_blockdatasourcesignals) return;
    list<hk_datasource*>::iterator it;
    it=p_dependinglist.begin();

    while ( it!=p_dependinglist.end() )
    {
        hk_datasource* dv=*it;
        ++it;

        if (dv!=NULL)
        {
            dv->disable();
        }
    }

}


void hk_datasource::inform_depending_ds_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_depending_ds_enable");
#endif
    if (p_private->p_blockdatasourcesignals) return;
    list<hk_datasource*>::iterator it;
    it=p_dependinglist.begin();

    while ( it!=p_dependinglist.end() )
    {
        hk_datasource* dv=*it;
        ++it;

        if (dv!=NULL)
        {
            dv->enable();
        }
    }

}


void hk_datasource::inform_visible_objects_before_columns_deleted(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_visible_objects_before_columns_deleted");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->before_columns_deleted();
    }

}


void hk_datasource::inform_visible_objects_row_add(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_visible_objects_row_add");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->row_added();
    }

}


void hk_datasource::inform_visible_objects_row_delete(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::inform_visible_objects_row_delete");
#endif
//inform visible objects
    if (p_private->p_blockvisiblesignals) return;
    list<hk_dsvisible*>::iterator it ;
    for(it=p_visibles.begin();it!=p_visibles.end();)
    {
        hk_dsvisible* vi=*it;
        it++;
        vi->row_deleted();
    }

}


void hk_datasource::set_sqldelimiter(const hk_string& d)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::set_sqldelimiter");
#endif
    p_sql_delimiter=d;

}


hk_database* hk_datasource::database(void)
{
    return p_database;
}


#ifdef HK_DEBUG
void hk_datasource::dump_data()
{
    list<hk_column*>::iterator it;
    cout<<endl<<"DUMP"<<endl;
    cout<<"===="<<endl;

    for (unsigned int k=0;k<max_rows();k++)
    {
        int hcol=0;
        for (it=columns()->begin(); it!=columns()->end();it++)
        {
            cout <<k<<" "<<hcol++<<" "<<(*it)->asstring_at(k,false)<<" ";
        }
        cout<<endl;
    }
    cout <<"================="<<endl<<endl;
}
#endif

void  hk_datasource::savedata(ostream& s,bool saveall)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::savedata");
#endif
    hk_string dtag="DATASOURCE";
    start_mastertag(s,dtag);

    hk_string dd;
    switch (type())
    {
       case ds_view: dd="VIEW";break;
       case ds_table: dd="TABLE";break;
       default: dd="QUERY";
    }
    set_tagvalue(s,"TYPE",dd);
    set_tagvalue(s,"DATASOURCENAME",name());
    set_tagvalue(s,"TEMPORARYFILTER",p_private->p_temporaryfilter);
    set_tagvalue(s,"X",p_private->p_x);
    set_tagvalue(s,"Y",p_private->p_y);
    set_tagvalue(s,"WIDTH",p_private->p_width);
    set_tagvalue(s,"HEIGHT",p_private->p_height);
    set_tagvalue(s,"DESIGNWIDTH",p_private->p_designwidth);
    set_tagvalue(s,"DESIGNHEIGHT",p_private->p_designheight);
    //if (p_presentation==NULL)
    set_tagvalue(s,"SQL",p_private->p_definedsql);
    if (saveall)
    {
        hk_string mtag="DATASOURCEEXTRA";
        start_mastertag(s,mtag);
        set_tagvalue(s,"AUTOMATIC_DATA_UPDATE",p_automatic_data_update);
        set_tagvalue(s,"FILTER",p_private->p_filter);
        set_tagvalue(s,"SORTING",p_private->p_sorting);
        set_tagvalue(s,"DATETIMEFORMAT",p_datetimeformat);
        set_tagvalue(s,"DATEFORMAT",p_dateformat);
        set_tagvalue(s,"TIMEFORMAT",p_timeformat);
        set_tagvalue(s,"READONLY",p_readonly);
        if (p_presentation!=NULL)
        {
            set_tagvalue(s,"UPN",p_dscounter);
            set_tagvalue(s,"DEPENDING_ON",p_private->p_depending_on_presentationdatasource);
            set_tagvalue(s,"IS_LEFT_JOIN",p_depending_on_is_left_join);
            set_tagvalue(s,"REACT_ON_CHANGED_DATA",p_depending_on_datasource_react_on_changed_data);
            hk_string dm;
            switch ( p_private->p_dependingmode)
            {
                case depending_nohandle   : dm="NOHANDLE";
                break;
                case depending_change     : dm="CHANGE";
                break;
                case depending_delete     : dm="DELETE";
                break;
                case depending_changedelete   : dm="CHANGEDELETE";
                break;
                default           : dm="STANDARD";

            }
            set_tagvalue(s,"DEPENDINGMODE",dm);
        }
        hk_string mtag1="DEPENDINGFIELDS";
        start_mastertag(s,mtag1);
        list<hk_string>::iterator thisit=depending_this_fields.begin();
        list<hk_string>::iterator masterit=depending_master_fields.begin();
        while (thisit!=depending_this_fields.end())
        {
            set_tagvalue(s,"THISFIELD",(*thisit));
            set_tagvalue(s,"MASTERFIELD",(*masterit));

            thisit++;
            masterit++;
        }
        end_mastertag(s,mtag1);
        end_mastertag(s,mtag);
    }

    end_mastertag(s,dtag);

}




void  hk_datasource::loaddata(xmlNodePtr definition,bool loadsql)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::loaddata");
#endif
    xmlNodePtr buffer;
    hk_string buffer2;
    hk_string value,tp;
    buffer=get_tagvalue(definition,"TYPE",tp);
    get_tagvalue(definition,"DATASOURCENAME",value);
    //set_name(value);
    get_tagvalue(definition,"X",p_private->p_x);
    get_tagvalue(definition,"Y",p_private->p_y);
    get_tagvalue(definition,"WIDTH",p_private->p_width);
    get_tagvalue(definition,"HEIGHT",p_private->p_height);
    get_tagvalue(definition,"DESIGNWIDTH",p_private->p_designwidth);
    get_tagvalue(definition,"DESIGNHEIGHT",p_private->p_designheight);
    if (loadsql && (!((type()==ds_table)&&(tp=="TABLE")) ||value.size()==0/*datasource name==0*/))
    {   bool qbe=false;
        get_tagvalue(definition,"USE_QBE",qbe);
        if (qbe
	   &&p_presentation
	   &&p_presentation->presentationtype()!=hk_presentation::qbe)
	{
	  hk_qbe* q=new hk_qbe();
	  q->set_database(p_database);
	  xmlNodePtr g=	  get_tagvalue(definition,"QBE");
	  if (g)q->loaddata(g);
	  set_query(q,false);
	  delete q;
	}
	else
        get_tagvalue(definition,"SQL",p_original_sql);
        p_private->p_definedsql=p_sql=p_original_sql;
        parse_sql();
    }
    if (type()==ds_view) driver_specific_load_view();
//     get_tagvalue(definition,"TEMPORARYFILTER",p_private->p_temporaryfilter);


     xmlNodePtr node=get_tagvalue(definition,"DATASOURCEEXTRA");
     if (node)
     {
       node=node->xmlChildrenNode;
    get_tagvalue(node,"AUTOMATIC_DATA_UPDATE",p_automatic_data_update);
    get_tagvalue(node,"FILTER",p_private->p_filter);
    get_tagvalue(node,"SORTING",p_private->p_sorting);
    get_tagvalue(node,"DATETIMEFORMAT",p_datetimeformat);
    get_tagvalue(node,"DATEFORMAT",p_dateformat);
    get_tagvalue(node,"TIMEFORMAT",p_timeformat);
    get_tagvalue(node,"READONLY",p_readonly);
    buffer=get_tagvalue(node,"DEPENDINGFIELDS",tp);
    if (p_presentation!=NULL)
    {
        get_tagvalue(node,"UPN",p_dscounter);
        get_tagvalue(node,"DEPENDING_ON",p_private->p_depending_on_presentationdatasource);
        get_tagvalue(node,"IS_LEFT_JOIN",p_depending_on_is_left_join);
        get_tagvalue(node,"REACT_ON_CHANGED_DATA",p_depending_on_datasource_react_on_changed_data);
        int i=1;
        clear_depending_fields();
        while (get_tagvalue(buffer->xmlChildrenNode,"THISFIELD",value,i))
        {
            get_tagvalue(buffer->xmlChildrenNode,"MASTERFIELD",buffer2,i);
            add_depending_fields(value,buffer2);
            i++;
        }
        if (!get_tagvalue(buffer->xmlChildrenNode,"THISFIELD",value,1))
            set_depending_on(NULL);
        hk_string dm;
        get_tagvalue(node,"DEPENDINGMODE",dm);
        if (dm=="NOHANDLE") p_private->p_dependingmode=depending_nohandle;
        else
        if (dm=="CHANGE") p_private->p_dependingmode=depending_change;
            else
            if (dm=="DELETE") p_private->p_dependingmode=depending_delete;
                else
                if (dm=="CHANGEDELETE") p_private->p_dependingmode=depending_changedelete;
                    else
                        p_private->p_dependingmode=depending_standard;
     
     }





    }
    create_new_sql_statement(p_depending_on_datasource_react_on_changed_data);

}







bool hk_datasource::ask_name(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::ask_name");
#endif
    hk_string infotext;
    switch(type())
    {
     case ds_query: infotext=hk_translate("Please enter the queryname:");break;
     case ds_view:  infotext=hk_translate("Please enter the viewname:");break;
     default:        infotext=hk_translate("Please enter the tablename:");
    }

    hk_string res=show_stringvaluedialog(infotext);
    if (res.size()==0) return false;

    set_name(res);

    return true;

}


bool hk_datasource::alter_column(const hk_string& col, const hk_string* colname,hk_column::enum_columntype* newtype,long* size,const hk_string* defaultvalue,const bool* primary,const bool* notnull)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::alter_column");
#endif
    if (p_mode!=mode_altertable||col.size()==0)return false;
    hk_column* acol=column_by_name(col);
    if (acol==NULL)return false;
    class_altercolumns s;
    s.name=col;
    if (colname!=NULL)s.newname=*colname;else s.newname=col;
    if(newtype!=NULL)s.type=*newtype; else s.type=hk_column::othercolumn;
    if(size!=NULL)s.size=*size; else s.size=-1;
    if (defaultvalue!=NULL) s.defaultvalue=*defaultvalue; else s.defaultvalue="HK_NULL";
    if (primary!=NULL) s.primary=*primary; else s.primary=acol->is_primary();
    if (notnull!=NULL) s.notnull=*notnull; else s.notnull=acol->is_notnull();
    p_altercolumns.insert(p_altercolumns.end(),s);
    return false;
}


bool hk_datasource::delete_column(const hk_string& col)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::delete_column");
#endif

    if (p_mode!=mode_altertable||col.size()==0)return false;
    p_deletecolumns.insert(p_deletecolumns.end(),col);
    return true;
}


bool hk_datasource::alter_table_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::alter_table_now");
#endif
    if (p_mode==mode_altertable)
    {
       list<hk_column*>* c=columns();
       list<hk_column*>::iterator it=c->begin();
       while(it!=c->end())
       {
        (*it)->before_alter_table();
	++it;
       }
      if (p_altercolumns.size()==0&&p_deletecolumns.size()==0&&p_newcolumns.size()==0)
         {
		show_warningmessage("Can't modify table structure. No changes set");
             p_mode=mode_disabled;
	  return true;
	 }
        bool res=driver_specific_alter_table_now();
        if (res)
        {
            clear_columnlist();
            inform_when_table_structure_changes();
        }
        p_mode=mode_disabled;
        return res;
    }
    else return false;
}


void hk_datasource::setmode_altertable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::setmode_altertable");
#endif
    if (!p_database->table_exists(name())||type()!=ds_table) return;
    if (is_enabled()) disable();
    clear_columnlist();
    list<hk_column*>* c=columns();
    list<hk_column*>::iterator it=c->begin();
    while (it!=c->end())
    {
      (*it)->p_originalcolumnname=(*it)->p_columnname;
      (*it)->p_originalsize=(*it)->p_size;
      (*it)->p_originalcolumntype=(*it)->p_columntype;
      (*it)->p_originalnotnull=(*it)->p_notnull;
      (*it)->p_originalprimary_index=(*it)->p_primary_index;
      ++it;
    }

    clear_modecolumnlists();
    p_mode=mode_altertable;
}


list<hk_datasource::indexclass>* hk_datasource::indices(void)
{

    return driver_specific_indices();
}

bool hk_datasource::index_exists(const hk_string& i)
{
list<indexclass>* p_indices =indices();
list<hk_datasource::indexclass>::iterator it=p_indices->begin();
while (it!=p_indices->end())
{
  if ((*it).name==i) return true;
  ++it;
}
 return false;
}


bool    hk_datasource::drop_index(const hk_string& i)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::drop_index");
#endif
    bool res= driver_specific_drop_index(i);
    if (res)inform_when_indexlist_changes();
    else show_warningmessage(hk_translate("Error while dropping the index!")+"\n"+hk_translate("Servermessage: ")+p_database->connection()->last_servermessage());
    return res;
}


bool    hk_datasource::create_index(const hk_string& newname,bool unique,list<hk_string>& fields)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::create_index");
#endif
    bool res= driver_specific_create_index(newname,unique,fields);
#ifdef HK_DEBUG
    if (res) hkdebug("index created");
    else hkdebug("index NOT created");
#endif
    if (res)inform_when_indexlist_changes();
    return res;
}


bool    hk_datasource::alter_index(const hk_string& newname,bool unique,list<hk_string>& fields)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::alter_index");
#endif

    if (index_exists(newname))drop_index(newname);
    return create_index(newname,unique,fields);

}


bool    hk_datasource::driver_specific_goto_row(unsigned long )
{
    return false;
}


bool    hk_datasource::driver_specific_drop_index(const hk_string& )
{
    return false;
}


bool    hk_datasource::driver_specific_create_index(const hk_string& ,bool ,list<hk_string>& )
{
    return false;
}


list<hk_string>* hk_datasource::depending_on_thisfields(void)
{
    return &depending_this_fields;
}


list<hk_string>* hk_datasource::depending_on_masterfields(void)
{
    return &depending_master_fields;
}


bool hk_datasource::datasource_used(void) const
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::datasource_used");
#endif
    bool res =false;
    if (p_visibles.size()>0||p_dependinglist.size()>0) res=true;
    return res;

}


void hk_datasource::set_has_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::set_has_changed");
#endif
    p_has_changed=true;
    inform_depending_ds_data_has_changed();
}


void hk_datasource::set_has_not_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::set_has_not_changed");
#endif
    p_has_changed=false;
}

bool hk_datasource::has_changed(void) const
{
return p_has_changed;
}


bool hk_datasource::depending_on_react_on_data_changes(void)
{
    return p_depending_on_datasource_react_on_changed_data;
}


hk_datasource::enum_dependingmodes hk_datasource::dependingmode(void)
{
    return p_private->p_dependingmode;
}


bool hk_datasource::depending_on_datasource_updaterow_ok(void)
{
    if(p_depending_on_datasource==NULL) return true;
    switch (p_private->p_dependingmode)
    {
        case depending_change:
        case depending_changedelete:
        case depending_nohandle:  return true;
        break;
        case depending_standard: if  (max_rows()>0)
        {
            list<hk_string>::iterator it=depending_master_fields.begin();
            while (it!=depending_master_fields.end())
            {
                hk_column* c =p_depending_on_datasource->column_by_name((*it));
                if (c!=NULL) if (c->has_changed()) return false;
                it++;
            }
        }else return true;
        break;
        default: return false;
    }
    return true;
}


void hk_datasource::depending_on_datasource_before_update_row(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::depending_on_datasource_before_update_row");
#endif
    if(p_depending_on_datasource==NULL) return;
    if (p_private->p_dependingmode!=depending_change
        &&p_private->p_dependingmode!=depending_changedelete)
        return;
    list<hk_string>::iterator it= depending_master_fields.begin();
    list<hk_string>::iterator thisit= depending_this_fields.begin();
    hk_string changelist;
    while (it!=depending_master_fields.end())
    {
        hk_column* col =p_depending_on_datasource->column_by_name((*it));
        if (col!=NULL)
        {
            if (col->has_changed())
            {
                if (changelist.size()==0) changelist=" SET ";
                else changelist+=", ";
                changelist += (*thisit);
                changelist+=" = ";
                changelist+=col->get_delimiter();
                changelist+= col->changed_data_asstring(false);
                changelist+=col->get_delimiter();
                changelist+=" ";

            }

        }
        it++;thisit++;
    }

    if (changelist.size()==0) return;
    hk_string s= "UPDATE ";
    s+=name()+changelist+whole_datasource_where_statement();
    if (p_private->p_blockserversignals) return;
    hk_actionquery* a = p_database->new_actionquery();
    if (a==NULL) return;
    a->set_sql(s.c_str(),s.size());
    a->execute();
    delete a;
// cout <<"beforeupdaterow SQL: "<<s<<endl;
}


void hk_datasource::depending_on_datasource_after_update_row(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::depending_on_datasource_after_update_row");
#endif
    if (!p_depending_on_datasource->while_disabling())reload_data();
}


bool hk_datasource::delete_rows(unsigned long from,unsigned long to,bool check_depending_datasources,enum_interaction c)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::delete_rows(unsigned long from,unsigned long to)");
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
        if (p_private->p_dependingmode!=depending_nohandle)
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
    hk_string w,b;
    f_orig=f;
    while (f<=t)
    {
        b=create_row_where_statement_at(f,false);
        if (b.size()>0)
        {
            if (w.size()!=0) w+=" ) OR ( ";
            w+=b;
        }
        f++;
    }
    if (w.size()==0) return true;
    bool res=false;
    if (!p_private->p_blockserversignals)
    {
        execute_visible_object_before_delete();
        hk_actionquery* a = p_database->new_actionquery();
        if (a==NULL) return false;

        hk_string query="DELETE FROM ";
        query+=p_identifierdelimiter+name()+p_identifierdelimiter+" WHERE (";
        query+=w;
        query+=") ";
        query+=p_sql_delimiter;
        a->set_sql(query.c_str(),query.size());
        transaction_begin();
        res=a->execute();
        if (res) transaction_commit();
        else transaction_rollback();
        delete a;
	execute_visible_object_after_delete();
    }
    unsigned long pp=p_counter;
    if (res)
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
    return res;
}


hk_string hk_datasource::whole_datasource_where_statement(bool take_changed_data)
{
#ifdef HK_DEBUG
    hkdebug("hk_datasource::whole_datasource_where_statement");
#endif
    hk_string newwhere="",delimiter;
    hk_string buffer,buffer2;
    bool nullvalue=false;

    if (p_parsed_sql && p_parsed_sql->where_part!="")
    {
        newwhere=" WHERE (";
        newwhere=newwhere+p_parsed_sql->where_part;
    }

    if (p_depending_on_datasource!=NULL)
    {
        list<hk_string>::iterator this_ds;
        list<hk_string>::iterator master_ds;
        master_ds=depending_master_fields.begin();

        for (this_ds=depending_this_fields.begin();this_ds!=depending_this_fields.end();this_ds++)
        {
            if (newwhere=="")
            {
                if(p_parsed_sql&& p_parsed_sql->where_part=="")newwhere=" WHERE (" ;
                else newwhere=") AND (";
            }
            else newwhere=newwhere+") AND (";
            bool m_useident=true;
            bool s_useident=true;
            hk_string m=trim(*master_ds);
            hk_string s=trim(*this_ds);
            if (m[0]=='%')
              {
	         m_useident=false;
	         m.replace(0,1,"");
	      }
            if (s[0]=='%')
              {
	         s_useident=false;
	         s.replace(0,1,"");
	      }



	    if (!m_useident)
	    {
	       buffer2=m;
	       delimiter=p_sqltextdelimiter;
	    }

	    else
	    {
	    hk_column* buffercolumn=p_depending_on_datasource->column_by_name((*master_ds));
            if (buffercolumn!=NULL)
            { nullvalue=buffercolumn->is_nullvalue();
                if (take_changed_data&&buffercolumn->has_changed()&&buffercolumn->transformed_changed_data()!=NULL)
		{
                    nullvalue=false;
		    buffer2.assign(buffercolumn->transformed_changed_data()->data,0,buffercolumn->transformed_changed_data()->length);
                }
		else
                {
		    if (take_changed_data&&buffercolumn->has_changed()&&buffercolumn->transformed_changed_data()==NULL) nullvalue=true;
		    buffer2=buffercolumn->transformed_asstring();
		}
		delimiter=buffercolumn->get_delimiter();
	    }
            else
            {
                buffer2="";
                delimiter=p_sqltextdelimiter;
            }
	    }



	    if (!s_useident)
	    {
	      buffer=s+" = "+delimiter+buffer2+delimiter;
	    }
	    else
	    {
             /*if (type()==ds_query)
	     {
	     hk_datasource* qds=database()->new_resultquery();
	     qds->set_sql(sql());
	     qds->set_filter("0=1");
	     qds->enable();
	      hk_column* c=qds->column_by_name(*this_ds);
	      if (c)
	       {
		 if (c->tableorigin().size()>0)
		  buffer=p_identifierdelimiter+c->tableorigin()+p_identifierdelimiter+"."
		    +p_identifierdelimiter+*this_ds+p_identifierdelimiter;
	       }
 	      else
	      {
	        buffer=delimit_identifier(*this_ds);
	      }
	      delete qds;
	     }
	     else
	      buffer=delimit_identifier(*this_ds);*/
	      buffer=totalfieldorigin(*this_ds);
	    if (nullvalue) buffer=buffer+" IS NULL ";
	    else
            buffer=buffer+" ="+delimiter+buffer2+delimiter;
	    }
            master_ds++;
            newwhere=newwhere+buffer;

        }

    }                                             //endif depending_on_datasource !=NULL

//add filter if defined
    if (p_private->p_filter.length()>0)
    {
        if(newwhere=="")newwhere=" WHERE (";
        else newwhere=newwhere+") AND (";
        newwhere=newwhere+p_private->sqlconvertdelimiter(p_private->p_filter);
    }

    if ((p_private->p_use_temporaryfilter)&&(p_private->p_temporaryfilter.size()>0))
    {
        if(newwhere=="")newwhere=" WHERE (";
        else newwhere=newwhere+") AND (";
        newwhere=newwhere+p_private->sqlconvertdelimiter(p_private->p_temporaryfilter);
    }

    if ((p_private->p_use_internalfilter)&&(p_private->p_internalfilter.size()>0))
    {
        if(newwhere=="")newwhere=" WHERE (";
        else newwhere=newwhere+") AND (";
        newwhere=newwhere+p_private->sqlconvertdelimiter(p_private->p_internalfilter);
    }

    if ((newwhere.length()>0))newwhere+=" ) ";

    return newwhere;
}


void hk_datasource::transaction_begin(hk_string tname)
{
    driver_specific_transaction_begin(tname);
}


void hk_datasource::transaction_commit(hk_string tname)
{
    driver_specific_transaction_commit(tname);
}


void hk_datasource::transaction_rollback(hk_string tname)
{
    driver_specific_transaction_rollback(tname);
}


void hk_datasource::driver_specific_transaction_begin(hk_string)
{
}


void hk_datasource::driver_specific_transaction_commit(hk_string)
{
}


void hk_datasource::driver_specific_transaction_rollback(hk_string)
{
}


void hk_datasource::set_blockvisiblesignals(bool v)
{
    p_private->p_blockvisiblesignals=v;
}


void hk_datasource::set_blockserversignals(bool s)
{
    p_private->p_blockserversignals=s;
}


void hk_datasource::set_blockdatasourcesignals(bool )
{
    p_private->p_blockdatasourcesignals=false;
}


bool hk_datasource::blockvisiblesignals(void)
{
    return p_private->p_blockvisiblesignals;
}


bool hk_datasource::blockserversignals(void)
{
    return p_private->p_blockserversignals;
}


bool hk_datasource::blockdatasourcesignals(void)
{
    return p_private->p_blockdatasourcesignals;
}


void hk_datasource::before_connection_disconnects(void)
{
    switch (p_mode)
    {
        case mode_altertable :alter_table_now();break;
        case mode_createtable: create_table_now();break;
        default: disable();
    }
}


void hk_datasource::save_datasourcedefinition(ostream& s)
{
    hk_string dtag="DATASOURCEDEFINITION";
    start_mastertag(s,dtag);
    set_tagvalue(s,"DATASOURCENAME",name());
    hk_string ctag="COLUMNS";
    start_mastertag(s,ctag);
    list<hk_column*>* cols=columns();
    bool tmpcols_used=false;
      hk_string tmpf=temporaryfilter();
      bool tmpusef=use_temporaryfilter();
    if (!cols && type()==ds_query &&!is_enabled())
    {
      set_temporaryfilter("0=1");
      set_use_temporaryfilter(true);
      tmpcols_used=true;
      enable();
      cols=columns();
    }
    if (cols)
    {
        list<hk_column*>::iterator it=cols->begin();
        while (it!=cols->end())
        {
            (*it)->save_columndefinition(s);
            it++;
        }

    }
    if (tmpcols_used)
    {
      disable();
      set_temporaryfilter(tmpf);
      set_use_temporaryfilter(tmpusef);
      cols=NULL;
    }
    end_mastertag(s,ctag);
    ctag="INDICES";
    start_mastertag(s,ctag);
    list<indexclass>* i = indices();
    if (i)
    {
        hk_string tg="INDEXDEFINITION";
        list<indexclass>::iterator it=i->begin();
        while (it!=i->end())
        {
            start_mastertag(s,tg);
            set_tagvalue(s,"INDEXNAME",(*it).name);
            set_tagvalue(s,"INDEXUNIQUE",(*it).unique);
            start_mastertag(s,"INDEXFIELDS");
            list<hk_string>::iterator fit=(*it).fields.begin();
            while (fit!=(*it).fields.end())
            {
                set_tagvalue(s,"INDEXFIELD",(*fit));
                fit++;
            }
            end_mastertag(s,"INDEXFIELDS");
            end_mastertag(s,tg);

            it++;
        }

    }
    end_mastertag(s,ctag);

    end_mastertag(s,dtag);

}


bool hk_datasource::load_datasourcedefinition(xmlNodePtr definition,bool use_xmltablename,bool ask)
{

#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_datasource::load_datasourcedefinition");
#endif
    if (p_enabled) disable();
    clear_columnlist();
    hk_string buffer;
    if (use_xmltablename)
    {
        get_tagvalue(definition,"DATASOURCENAME",buffer);
        set_name(buffer);
    }
    else
    if (name().size()==0)
    {
        if (ask_name()) buffer=name();
        else return false;
    }

    if (ask&&database()->table_exists(buffer))
    {
        if (use_xmltablename)
        {
//if (show_yesnodialog(hk_translate("Overwrite table?"),true)) database()->delete_table(buffer);
//else
            set_name("");
        }
        else if(!ask_name()) return false;

    }
    if (name().size()==0)if(!ask_name()) return false;
    if (database()->table_exists(name()))
    {
        show_warningmessage(hk_translate("Table already exists!"));
        return false;
    }
    setmode_createtable();
    xmlNodePtr cols=get_tagvalue(definition,"COLUMNS");
    int i=1;

    xmlNodePtr coldef;
    hk_string xxx;
    while ((coldef=get_tagvalue(cols,"COLUMNDEFINITION",xxx,i)))
    {
        hk_column* c=new_column();
        if (c) c->load_columndefinition(coldef);
        i++;
    }

    if(!create_table_now()) return false;

    xmlNodePtr in=get_tagvalue(definition,"INDICES");

    hk_string nam;
    bool      uni;
    i=1;
    while (get_tagvalue(in,"INDEXDEFINITION",xxx,i))
    {
        get_tagvalue(in,"INDEXNAME",nam);
        get_tagvalue(in,"INDEXUNIQUE",uni);
        list<hk_string> f;
        hk_string idxf;
        hk_string v;
        get_tagvalue(in,"INDEXFIELDS",idxf);
        int yc=1;
        while (get_tagvalue(in,"INDEXFIELD",v,yc))
        {
            f.insert(f.end(),v);
            yc++;
        }

        i++;
        create_index(nam,uni,f);
    }
    return true;
}


void    hk_datasource::mark_visible_objects_as_not_handled(void)
{
    list<hk_dsvisible*>::iterator it=p_visibles.begin();
    while (it!=p_visibles.end())
    {
        (*it)->p_already_handled=false;
        ++it;
    }

}


void    hk_datasource::set_ignore_changed_data(void)
{
p_ignore_changed_data=true;
reset_changed_data();
}
bool   hk_datasource::ignore_changed_data(void) const
{
  return p_ignore_changed_data;
}

hk_presentation* hk_datasource::presentation(void)
{
 return p_presentation;
}


void hk_datasource::print_sql(void)
{
   for (int i=0;i<10;++i)cerr<<"*";
   time_t timeval;
   time(&timeval);
   cerr<<" "<<ctime(&timeval);
   cerr <<"hk_datasource SQL: '";
  cerr <<p_sql<<"'"<<endl;

}



 void hk_datasource::set_position(int nx,int ny, bool registerchange)
 {
   
   p_private->p_x=nx<0?0:nx;
   p_private->p_y=ny<0?0:ny;
    if (p_presentation!=NULL&&registerchange) p_presentation->set_has_changed();
 }

 void hk_datasource::set_size(int nwidth,int nheight, bool registerchange)
 {
   p_private->p_width=nwidth;
   p_private->p_height=nheight;
    if (p_presentation!=NULL&&registerchange) p_presentation->set_has_changed();
 }

 int hk_datasource::x(void) const
 {
   return p_private->p_x;
 }

 int hk_datasource::y(void) const
 {
   return p_private->p_y;
 }

 int hk_datasource::width(void) const
 {
   return p_private->p_width;
 }

 int hk_datasource::height(void) const
{
   return p_private->p_height;
}

void hk_datasource::automatic_position_datasource(void)
{
 if (!p_presentation) return;
 int minspace=10;
 int nx=minspace;
 int ny=minspace;

 bool positionok=false;

     int col=minspace;
     int row=minspace;
     const int ystep=minspace*2;
 for (  row=minspace;
      (row<=p_private->p_designheight-p_private->p_height)&&!positionok;
     row+=ystep)
 {
     col=minspace;
    while ((col<p_private->p_designwidth-p_private->p_width)&&!positionok)
    {
      list<hk_datasource*>* l=p_presentation->datasources();
      list<hk_datasource*>::iterator it=l->begin();
      bool isfree=true;
      int thisleft=col;
      int thisright=thisleft+p_private->p_width;
      int thistop=row;
      int thisbottom=thistop+p_private->p_height;

      while (it!=l->end()&&isfree)
      {
        if (*it!=this)
	{
         int itleft=(*it)->x();
         int itright=(*it)->x()+(*it)->width();
         int ittop=(*it)->y();
         int itbottom=(*it)->y()+(*it)->height();

	 if (
	      ((col>=itleft && col<=itright)
	       ||(thisright>=itleft &&thisright<=itright)
	      )
	    &&((row>=ittop &&row<=itbottom)
	       ||(thisbottom<=itbottom&& thisbottom>=ittop )
	      )
	    )
	    {
	      isfree=false;

	      col=itright+minspace;
	      thisleft=col;
              thisright=thisleft+p_private->p_width;
	    }
	}
	++it;
      }
     positionok=isfree;
    }
 }
     if (positionok)
        {
	  nx=col;
	  ny=row-ystep;
	}
 set_position(nx,ny,false);
}

void hk_datasource::set_designsize(int w,int , bool registerchange)
{
  p_private->p_designwidth=w;
  p_private->p_designheight=w;
  if (p_presentation!=NULL&&registerchange) p_presentation->set_has_changed();
}

int hk_datasource::designwidth() const
{
  return  p_private->p_designwidth;

}
int hk_datasource::designheight() const
{
  return  p_private->p_designheight;

}

 bool hk_datasource::previous_enable_problems(void) const
 {
   return p_private->p_previous_enable_problems;
 }



 hk_string hk_datasource::delimit_identifier(const hk_string& s)
 {
  return p_identifierdelimiter+s+p_identifierdelimiter;
 }




bool    hk_datasource::driver_specific_create_view_now(void)
{
  hk_actionquery* a = p_database->new_actionquery();
  if (a==NULL) return false;
  hk_string query = "CREATE VIEW ";
  query+=p_identifierdelimiter+name()+p_identifierdelimiter+" AS "+p_viewsql;
  if (p_viewsql.size()==0) show_warningmessage(hk_translate("Bug: View-SQL is empty!"));
  a->set_sql(query.c_str(),query.size());
  bool result=a->execute();
  delete a;


return result;
}


bool    hk_datasource::driver_specific_alter_view_now(void)
{
if (p_database->view_exists(name()))
{
  hk_actionquery* a = p_database->new_actionquery();
  if (a==NULL) return false;
  hk_string query = "DROP VIEW ";
  query+=p_identifierdelimiter+name()+p_identifierdelimiter;

  a->set_sql(query.c_str(),query.size());
  bool result=a->execute();
  delete a;
  if (!result) return false;
}
return driver_specific_create_view_now();
}



list <referentialclass>* hk_datasource::referenceslist(void)
{
  p_references.clear();
  driver_specific_referenceslist();
  return &p_references;
}

bool  hk_datasource::add_reference(const referentialclass &c)
{
  if (!p_database->connection()->server_supports(hk_connection::SUPPORTS_REFERENTIALINTEGRITY))
    {
      show_warningmessage(hk_translate("Server does not support referential integrity!"));
      return false;
    }
  bool result= driver_specific_add_reference(c);
  if (!result) show_warningmessage(hk_translate("Error adding reference!\nServermessage: ")+database()->connection()->last_servermessage());
  return result;
}

bool  hk_datasource::drop_reference(const hk_string& n)
{
/*  if ( find(p_private->p_references.begin(),p_private->p_references.end(),n)
        ==p_private->p_references.end())
   {
     show_warningmessage(hk_translate("No such reference!"));
     return false;
   }
*/
  return driver_specific_drop_reference(n);
}

bool hk_datasource::driver_specific_add_reference(const referentialclass & ref)
{
bool result=true;
hk_actionquery*fk =database()->new_actionquery();
hk_string masterkeys,slavekeys;
hk_string c=",";
hk_string referencename=trim(ref.p_name);
if (referencename.size()==0)referencename=new_referencename(ref);

list<dependingclass>::const_iterator it=ref.p_fields.begin();
while (it!=ref.p_fields.end())
{
 if (masterkeys.size()>0)masterkeys+=c;
 if (slavekeys.size()>0)slavekeys+=c;
 masterkeys+="\""+(*it).masterfield+"\"";
 slavekeys+="\""+(*it).dependingfield+"\"";

++it;
}
hk_string s="ALTER TABLE \""+name()+"\" ADD CONSTRAINT \""+referencename+"\" FOREIGN KEY("+slavekeys+") REFERENCES \""
+ref.p_masterdatasource+"\"("+masterkeys+")";
if (ref.p_updatecascade) s+=" ON UPDATE CASCADE";
if (ref.p_deletecascade) s+=" ON DELETE CASCADE";
fk->set_sql(s.c_str(),s.size());
result=fk->execute();
delete fk;

return result;
}

bool hk_datasource::driver_specific_drop_reference(const hk_string& r)
{
bool result=true;
hk_actionquery*fk =database()->new_actionquery();
hk_string s="ALTER TABLE \""+name()+"\" DROP CONSTRAINT \""+r+"\"";
fk->set_sql(s.c_str(),s.size());
result=fk->execute();
delete fk;

return result;
}



hk_string hk_datasource::new_referencename(const referentialclass&ref)
{
hk_string newname="FK_"+name()+"_"+ref.p_masterdatasource;
driver_specific_referenceslist();
bool is_new=false;
int n=0;
hk_string checkname=newname;
while (!is_new)
{

 is_new=is_newreferencename(checkname);
 if (is_new) return checkname;
 else
 {
   checkname=newname+format_number(n,false,0);
 }
++n;
}

return newname;
}


bool hk_datasource::is_newreferencename(const hk_string&n)
{
list<referentialclass>::iterator it=p_references.begin();
while (it!=p_references.end())
{
  if ((*it).p_name==n) return false;
  ++it;
}
return true;
}










void hk_datasource::driver_specific_referenceslist(void)
{

}


hk_string hk_datasource::fieldorigin(const hk_string& f)
{
  if (!p_parsed_sql) return f;
  list<fieldoriginclass>::iterator it=p_parsed_sql->fieldpart.begin();
  while (it!=p_parsed_sql->fieldpart.end())
  {  
     if ((*it).alias==f)
        { 
	    return (*it).fieldname;
	}
    ++it;
  }

  return "\""+f+"\"";
}



hk_string hk_datasource::totalfieldorigin(const hk_string& f)
{
hk_string result;
// 	 cerr <<" totalfieldorigin: suche nach:"<<f<<endl;
if (type()==ds_query)
  { 
     hk_datasource* qds=database()->new_resultquery();
     qds->set_sql(sql());
     qds->set_filter("0=1");
     qds->enable();
      hk_column* c=qds->column_by_name(f);
      if (c)
       { //cerr<<"column gefunden"<<endl;
	 if (c->tableorigin().size()>0)
	 {
	  result=p_identifierdelimiter+c->tableorigin()+p_identifierdelimiter+"."
	    +p_identifierdelimiter+f+p_identifierdelimiter;
         }
	 else
        {
         result=qds->fieldorigin(f);
// 	 cerr <<" totalfieldorigin: suche nach:"<<f<<" result='"<<result<<"'"<<endl;
       }
       }

      if (result.size()==0)
      {
        result=delimit_identifier(f);
      }
      delete qds;
   }
     else
     { //  is table
     result=columntablepart(fieldorigin(f));
     if (result.size()>0)
      {
        result=p_identifierdelimiter+result+p_identifierdelimiter+"."+p_identifierdelimiter+f+p_identifierdelimiter;
      }
     else
     result=delimit_identifier(f);
     }

return result;
}


void hk_datasource::set_enablefunction(voidfunction* f, long counter)
{
  p_enablefunction=f;
  p_enablefunctioncounter=counter;
}


list<hk_string>* hk_datasource::columnnames(void)
{
  p_private->p_columnnames.clear();
  list<hk_column*>* col=NULL;
    hk_datasource* qds=NULL;
  if (type()==ds_query &&!is_enabled())
  {
    qds=database()->new_resultquery();
    qds->set_sql(sql());
    qds->set_filter("0=1");
    qds->enable();
    col=qds->columns();

  }
  else
    col=columns();

 if (col)
 {
  list<hk_column*>::iterator it=col->begin();
  while (it!=col->end())
  {
   p_private->p_columnnames.push_back((*it)->name());
   ++it;
  }
 }
 if (qds) delete qds;

return &p_private->p_columnnames;
}

hk_string hk_datasource::systemcolumnname(const hk_string& n)
{
  hk_string nm;
  for (unsigned int i=0;i<n.size();++i)
  {
    char c=n[i];
    if (
    (!p_database->connection()->server_supports(hk_connection::SUPPORTS_NONASCII_FIELDNAMES)
    && ((c>='A' && c<='Z') ||(c>='a' && c<='z')||(c>='0' && c<='9') ||(p_database->connection()->server_supports(hk_connection::SUPPORTS_SPACE_FIELDNAMES)&&c==' ') )
    )
    ||p_database->connection()->server_supports(hk_connection::SUPPORTS_NONASCII_FIELDNAMES)
    )
    {
     nm+=c;
    }


  }
  return nm;
}


void hk_datasource::set_depending_on_is_left_join(bool d,bool registerchange)
{
p_depending_on_is_left_join=d;
    if (p_presentation!=NULL&&registerchange) p_presentation->set_has_changed();
}

bool hk_datasource::depending_on_is_left_join(void)
{
  return p_depending_on_is_left_join;
}



void hk_datasource::set_progressdialog(progress_dialogtype* prdialog)
{
  p_private->p_progressdialog=prdialog;
}

progress_dialogtype* hk_datasource::progressdialog(void)const
{
  return p_private->p_progressdialog;
}

void hk_datasource::set_progressinterval(int i)
{
 p_private->p_progressinterval=i;
}

int hk_datasource::progressinterval(void) const
{
   return p_private->p_progressinterval;
}


