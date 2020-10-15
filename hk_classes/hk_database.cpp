// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// copyright (c) 2020 Patrik Hanak <hanakp@users.sourceforge.net>
// This file is part of the hk_classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.126 $
#include "hk_database.h"
#include "hk_data.h"
#include "hk_dbvisible.h"
#include "hk_presentation.h"
#include "hk_connection.h"
#include "hk_datasource.h"
#include "hk_column.h"
#include "hk_actionquery.h"
#include "hk_form.h"
#include "hk_report.h"
#include "hk_dsmodevisible.h"
#include "hk_dsquery.h"
#include "hk_dstable.h"
#include "hk_datetime.h"
#include "hk_url.h"

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

newform_type *hk_database::p_newformfunction=NULL;
newform_type *hk_database::p_newdialogformfunction=NULL;
newreport_type *hk_database::p_newreportfunction=NULL;
newtable_type *hk_database::p_newtablefunction=NULL;
newquery_type *hk_database::p_newqueryfunction=NULL;


class hk_databaseprivate
{
public:
hk_databaseprivate(void)
			{
			  p_parent=NULL;
			  p_automatic_data_update=hk_class::default_automatic_data_update();
			  for (int f=ft_table;f<=ft_referentialintegrity;++f)
			  {
			    p_storagemode[f]=hk_database::local;
			    p_loadmode[f]=hk_database::local;
			  }
			}
        hk_string   p_dbname;
        hk_string p_sqldelimiter;
        list<hk_data*> p_hkdsourcelist;
        vector<hk_string> p_filelist;
        hk_string p_databasepath;
        list<hk_dbvisible*> p_visibles;
        list<hk_presentation*> p_presentations;
	hk_class* p_parent;// will be used as widget parent in e.g.  ::new_formvisible;
	hk_database::enum_storagemode p_storagemode[ft_referentialintegrity+1];
	hk_database::enum_storagemode p_loadmode[ft_referentialintegrity+1];
	hk_string p_charset;
	bool p_automatic_data_update;
};



hk_database::hk_database(hk_connection* c):hk_class()
{
#ifdef HK_DEBUG
    hkdebug("hk_database::hk_database");
#endif
    p_private=new hk_databaseprivate;
    p_connection=c;
}


hk_database::~hk_database()
{
#ifdef HK_DEBUG
    hkdebug("hk_database::~hk_database");
#endif
    clear_presentationlist();
    clear_visiblelist();
    inform_datasources_before_closing();
    p_connection->db_remove(this);
    list<hk_data*>::iterator it;
    for (it=p_private->p_hkdsourcelist.begin();it!=p_private->p_hkdsourcelist.end();)
    {
        hk_data* dt=*it ;
        it++;
        delete dt;
    }
    p_private->p_hkdsourcelist.erase(p_private->p_hkdsourcelist.begin(),p_private->p_hkdsourcelist.end());
delete p_private;
}


bool hk_database::set_name(const hk_string& n)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::set_name");
#endif
    hk_string nm=trim(n);
    if (!p_connection->server_supports(hk_connection::SUPPORTS_LOCAL_FILEFORMAT))
    {
      hk_url u=nm;
      if (u.directory().size()>0)
      nm=u.filename();
      if (u.extension().size()>0)
       nm="_"+u.extension();
    }
    if (nm==p_private->p_dbname)return true;
    hk_string oldname=p_private->p_dbname;
    p_private->p_dbname=nm;
    driver_specific_set_name();
    p_private->p_dbname=oldname;
    bool res=select_db(nm);
    if (res) p_private->p_dbname=nm;
    return res;
}


hk_string hk_database::name(void)
{

    return p_private->p_dbname;
}


vector<hk_string>* hk_database::tablelist(bool with_systemtable)
{
    driver_specific_tablelist();
    if (!with_systemtable)
    {
      vector<hk_string>::iterator it=find(p_tablelist.begin(),p_tablelist.end(),"HKCLASSES");
      if(it!=p_tablelist.end()) p_tablelist.erase(it);
    }
    return &p_tablelist;

}


vector<hk_string>* hk_database::viewlist()
{
    driver_specific_viewlist();
    return &p_viewlist;

}

vector<hk_string>* hk_database::tableviewlist(bool with_systemtable)
{
   p_tableviewlist.erase(p_tableviewlist.begin(),p_tableviewlist.end());
   tablelist(with_systemtable);
   viewlist();
   vector<hk_string>::iterator it=p_tablelist.begin();
   p_tableviewlist.insert(p_tableviewlist.end(),p_tablelist.begin(),p_tablelist.end());
   p_tableviewlist.insert(p_tableviewlist.end(),p_viewlist.begin(),p_viewlist.end());
   sort(p_tableviewlist.begin(),p_tableviewlist.end());

    return &p_tableviewlist;

}



bool hk_database::table_exists(const hk_string& tablename)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::table_exists");
#endif
    bool res=false;
    vector<hk_string>* liste=tablelist();
    if (!liste) return false;
    vector<hk_string>::iterator it= liste->begin();
    while (it!=liste->end())
    {
        if ((*it)==tablename)return true;

        it++;
    }
    return res;
}

bool hk_database::view_exists(const hk_string& viewname)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::view_exists");
#endif
    bool res=false;
    vector<hk_string>* liste=viewlist();
    if (!liste) return false;
    vector<hk_string>::iterator it= liste->begin();
    while (it!=liste->end())
    {
        if ((*it)==viewname)return true;

        it++;
    }

    return res;
}


bool hk_database::query_exists(const hk_string& queryname)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::query_exists");
#endif
    bool res=false;
    vector<hk_string>* liste=querylist();
    if (!liste) return false;
    vector<hk_string>::iterator it= liste->begin();
    while (it!=liste->end())
    {
        if ((*it)==queryname)return true;

        it++;
    }

    return res;
}

bool hk_database::form_exists(const hk_string& nm)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::form_exists");
#endif
    bool res=false;
    vector<hk_string>* liste=formlist();
    if (!liste) return false;
    vector<hk_string>::iterator it= liste->begin();
    while (it!=liste->end())
    {
        if ((*it)==nm)return true;

        it++;
    }

    return res;
}

bool hk_database::report_exists(const hk_string& nm)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::report_exists");
#endif
    bool res=false;
    vector<hk_string>* liste=reportlist();
    if (!liste) return false;
    vector<hk_string>::iterator it= liste->begin();
    while (it!=liste->end())
    {
        if ((*it)==nm)return true;

        it++;
    }

    return res;
}





hk_datasource* hk_database::new_table(const hk_string& nm,hk_presentation* p)
{
if (!p_connection->database_exists(p_private->p_dbname)) return NULL;
    hk_datasource* tb=driver_specific_new_table(p);
    if (tb!=NULL)
    {
        p_private->p_hkdsourcelist.insert(p_private->p_hkdsourcelist.end(),tb);
        if (p_connection->server_needs(hk_connection::NEEDS_SQLDELIMITER))
            tb->set_sqldelimiter(p_connection->sqldelimiter());

    }
    else
    {
     show_warningmessage(hk_translate("Bug: hk_database::driver_specific_new_table returned empty table!"));
     return NULL;
    }

    if (nm.size()>0)tb->set_name(nm,false);
    return tb;
}


hk_datasource* hk_database::new_view(const hk_string& nm,hk_presentation* p)
{
   hk_datasource* ds= driver_specific_new_view(p);
    if (ds!=NULL)
    {
        p_private->p_hkdsourcelist.insert(p_private->p_hkdsourcelist.end(),ds);
        if (p_connection->server_needs(hk_connection::NEEDS_SQLDELIMITER))
            ds->set_sqldelimiter(p_connection->sqldelimiter());

    }
    else
    {
     show_warningmessage(hk_translate("Bug: hk_database::driver_specific_new_view returned empty view!"));
     return NULL;
    }

    if (nm.size()>0)ds->set_name(nm,false);
    return ds;
}





hk_datasource* hk_database::new_resultquery(hk_presentation* p)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::new_resultquery");
#endif
if (!p_connection->database_exists(p_private->p_dbname)) return NULL;
    hk_datasource* tb=driver_specific_new_resultquery(p);
    if (tb!=NULL)
    {
        p_private->p_hkdsourcelist.insert(p_private->p_hkdsourcelist.end(),tb);
        if (p_connection->server_needs(hk_connection::NEEDS_SQLDELIMITER))
            tb->set_sqldelimiter(p_connection->sqldelimiter());
    }
    return tb;
}


hk_actionquery* hk_database::new_actionquery(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::new_actionquery");
#endif
//if (!p_connection->database_exists(p_private->p_dbname)) return NULL;
    hk_actionquery* tb=driver_specific_new_actionquery();
    if (tb!=NULL) p_private->p_hkdsourcelist.insert(p_private->p_hkdsourcelist.end(),tb);
    return tb;
}


void hk_database::ds_remove(hk_data* ds)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::ds_remove");
#endif

    p_private->p_hkdsourcelist.remove(ds);

}


bool hk_database::select_db(const hk_string& newname)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::select_db");
#endif
    if (in_presentationload())
      return false;
    hk_string oldname=name();
    clear_presentationlist();
    clear_visiblelist();
    inform_datasources_before_closing();
    if (p_connection->is_connected())
    {
        p_private->p_dbname=newname;
	
	    if (p_connection->server_needs(hk_connection::NEEDS_DIRECTORY_AS_DATABASE))
	        p_url=newname;//+"/";
	    else
	        p_url=newname;
        bool erg= driver_specific_select_db();
        if (erg)
        {
            if (p_connection->server_supports(hk_connection::SUPPORTS_LOCAL_FILEFORMAT)&&
	           p_url.is_valid() && p_url.directory().size() >0)
	        {
	            //cerr <<"directory=#"<<p_url.directory()<<"#"<<endl;
	            //cerr <<"local file name"<<endl;
                p_private->p_databasepath=p_connection->databasepath();
                p_private->p_databasepath+="/";
                p_private->p_databasepath+=replace_all("/",replace_all(".",p_url.url(),"_."),"_");
	            p_private->p_dbname=p_url.url();
	           //cerr <<"p_dbname="<<p_private->p_dbname<<endl;
	           //cerr <<"p_databasepath="<<p_private->p_databasepath<<endl;
	        }
	        else
	        {
	            p_private->p_databasepath=p_connection->databasepath();
                p_private->p_databasepath+="/";
                p_private->p_databasepath+=name();
	        }
            mkdir (p_private->p_databasepath.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
            hk_string b=p_private->p_databasepath+"/output";
            mkdir (b.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
	        if (has_centralstoragetable())
	        {
	            for (int f=ft_query;f<=ft_module;++f)
	            {
		            if (f!=ft_view)
		            {
		                p_private->p_storagemode[f]=hk_database::central;
		                p_private->p_loadmode[f]=hk_database::central;
		            }
	            }

	        }
	        else
	        {
	            for (int f=ft_query;f<=ft_referentialintegrity;++f)
	            {
		            p_private->p_storagemode[f]=hk_database::local;
		            p_private->p_loadmode[f]=hk_database::local;
	            }
	        }
	        load_configuration();

        }
        else
	    {
            p_private->p_dbname=oldname;
            show_warningmessage(replace_all("%1",hk_translate("No such Database '%1'!"),newname));
	    }
        return erg;
    }
    else
    {
        //p_private->p_dbname=oldname;
        show_warningmessage(hk_translate("Not connected to server!"));
        return false;
    }
}


hk_string hk_database::database_path(void)
{

    return p_private->p_databasepath;
}


void hk_database::connection_connected(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::connection_connected");
#endif
    select_db(name());

}


void hk_database::connection_disconnected(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::connection_disconnected");
#endif
    clear_presentationlist();
    list<hk_data*>::iterator it ;
    it=p_private->p_hkdsourcelist.begin();
    mark_datasources_as_not_handled();
    while (it!=p_private->p_hkdsourcelist.end())
    {
        if (!(*it)->p_already_handled)
        {
            (*it)->p_already_handled=true;
            (*it)->before_source_vanishes();
            (*it)->before_connection_disconnects();
            it=p_private->p_hkdsourcelist.begin();
        }
        else
            it++;
    }
}


bool hk_database::delete_table(const hk_string& table,enum_interaction x)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::delete_table");
#endif
    if (!p_connection->is_connected()) return false;
    hk_actionquery* q=new_actionquery();
    hk_string sql="DROP TABLE ";
    hk_string del;
    hk_datasource* d=new_table();
    if (d)
      {
       del=d->p_identifierdelimiter;
       delete d;
      }
    sql+=del+table+del+p_private->p_sqldelimiter;
    bool result=false;
    hk_string warning=hk_translate("Delete the table \"%TBLNAME%\"?");
    warning=replace_all("%TBLNAME%",warning,table);
    if(x==noninteractive||show_yesnodialog(warning,true))
    {

        if (q!=NULL)
        {
            d=new_table();
            d->set_name(table);
            d->driver_specific_before_drop_table();
            delete d;
            q->set_sql(sql.c_str(),sql.size());
            result=q->execute();
            delete  q;
        }
    } else return false;
    if (result)
    {
        list<hk_data*>::iterator it ;
        it=p_private->p_hkdsourcelist.begin();
        while (it!=p_private->p_hkdsourcelist.end())
        {
            hk_data* vi=*it;
            hk_string t=table;
            if (vi->type()==hk_data::ds_table && vi->name()==t)vi->before_source_vanishes();
            it++;
        }
     inform_datasources_filelist_changes(lt_table);
    }
    else
    {
        hk_string reason=hk_translate("Table could not be deleted!")+"\n"+hk_translate("Servermessage: ")+connection()->last_servermessage();
        show_warningmessage(reason);
    }
    return result;

}





bool hk_database::delete_view(const hk_string& view,enum_interaction x)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::delete_view");
#endif
    if (!p_connection->is_connected()) return false;
    hk_actionquery* q=new_actionquery();
    hk_string sql="DROP VIEW ";
    hk_string del;
    hk_datasource* d=new_view();
    if (d)
      {
       del=d->p_identifierdelimiter;
       delete d;
      }
    sql+=del+view+del+p_private->p_sqldelimiter;
    bool result=false;
    hk_string warning=hk_translate("Delete the view \"%TBLNAME%\"?");
    warning=replace_all("%TBLNAME%",warning,view);
    if(x==noninteractive||show_yesnodialog(warning,true))
    {

        if (q!=NULL)
        {
            /*hk_datasource* d=new_table();
            d->set_name(table);
            d->driver_specific_before_drop_table();
            delete d;*/
            q->set_sql(sql.c_str(),sql.size());
            result=q->execute();
            delete  q;
        }
    } else return false;
    if (result)
    {
        list<hk_data*>::iterator it ;
        it=p_private->p_hkdsourcelist.begin();
        while (it!=p_private->p_hkdsourcelist.end())
        {
            hk_data* vi=*it;
            hk_string t=view;
            if (vi->type()==hk_data::ds_view && vi->name()==t)vi->before_source_vanishes();
            it++;
        }
     inform_datasources_filelist_changes(lt_view);
    }
    else
    {
        hk_string reason=hk_translate("View could not be deleted!")+"\n"+hk_translate("Servermessage: ")+connection()->last_servermessage();
        show_warningmessage(reason);
    }
    return result;

}





















void hk_database::inform_datasources_before_closing(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::inform_datasources_before_closing");
#endif
    mark_datasources_as_not_handled();
    list<hk_data*>::iterator it ;
    it=p_private->p_hkdsourcelist.begin();
    while (it!=p_private->p_hkdsourcelist.end())
    {
        hk_data* vi=(*it);
        ++it;
        if (!vi->p_already_handled)
        {
            vi->p_already_handled=true;
            vi->before_source_vanishes();
            it=p_private->p_hkdsourcelist.begin();
        }

    }

}


void hk_database::save(const hk_string& statement,const hk_string& nm,filetype type,bool ask_before_overwrite,bool ask_for_new_name)
{
  save(st_overwrite,statement,nm,type,ask_before_overwrite,ask_for_new_name);
}
void hk_database::save(enum_storage storage,const hk_string& statement,const hk_string& nm,filetype type,bool ask_before_overwrite,bool ask_for_new_name)
{

#ifdef HK_DEBUG
    hkdebug("hk_database::save");
#endif

return (p_private->p_storagemode[type]==central?save_central(storage,statement,nm,type,ask_before_overwrite,ask_for_new_name):save_local(storage,statement,nm,type,ask_before_overwrite,ask_for_new_name));
}



void hk_database::save_local(const hk_string& statement,const hk_string& nm,filetype type,bool ask_before_overwrite,bool ask_for_new_name)
{
save_local( st_overwrite,statement,nm,type,ask_before_overwrite,ask_for_new_name);
}

void hk_database::save_local(enum_storage /*storage*/, const hk_string& statement,const hk_string& nm,filetype type,bool ask_before_overwrite,bool ask_for_new_name)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::save_local");
#endif
    ofstream* s=savestream(nm,type,ask_before_overwrite,false,ask_for_new_name);
    if (!s) return ;
    (*s)<<statement;
    s->close();
    delete s;
    return ;


}


void hk_database::save_central(const hk_string& statement,const hk_string& nm,filetype type,bool ask_before_overwrite,bool ask_for_new_name)
{
save_central(st_overwrite,statement,nm,type,ask_before_overwrite,ask_for_new_name);
}


void hk_database::save_central(enum_storage storage,const hk_string& statement,const hk_string& nm,filetype type,bool ask_before_overwrite,bool ask_for_new_name)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::save_central");
#endif
hk_datasource* ds=new_table("HKCLASSES");
 if (!ds)
   {
   show_warningmessage(hk_translate("Error: hk_database::save_central could not get a new table"));
   return;
   }
 hk_string filter="\"type\"="+longint2string(type);
 ds->set_filter(filter);
 ds->enable();
 hk_column* n=ds->column_by_name("name");
 hk_column* v=ds->column_by_name("value");
 hk_column* t=ds->column_by_name("type");
 hk_column* u=ds->column_by_name("user");
 hk_column* p=ds->column_by_name("update");
 if (!n ||!v||!t)
   {
    show_warningmessage(hk_translate("Error: hk_database::save_central could not find system columns!"));
    delete ds;
    return;
   }
  unsigned int row=n->find(nm,true,true);
  if (row > ds->max_rows())
  { // not existing => append object
    ds->setmode_insertrow();
    t->set_asinteger(type);
    n->set_asstring(nm);
  }
  else
  {  // replace existing object
    if (ask_before_overwrite )
    {

    hk_string dialog=savemessage(type,storage,nm);
     if(runtime_only() || !show_yesnodialog(dialog,true))
            {
              // no, so don't overwrite it
                if (ask_for_new_name)
                {
                    if (type==ft_query)dialog=hk_translate("Enter new query name:");
                    else if (type==ft_form)dialog=hk_translate("Enter new form name:");
                    else if (type==ft_report)dialog=hk_translate("Enter new report name:");
                    else if (type==ft_module)dialog=hk_translate("Enter new module name:");
                    else dialog=hk_translate("Enter new name:");
                    hk_string res=show_stringvaluedialog(dialog);
                    if (res.size()==0)
		       {
		         delete ds;
		         return ;
		       }
                    save_central(statement,res,type,ask_before_overwrite,ask_for_new_name);
		    delete ds;
		    return;

                }
                else
		{
		    delete ds;
                    return ;
		}
            }
        }




    ds->goto_row(row);
  }
  v->set_asstring(statement);
  if (u) u->set_asstring(p_connection->user());
  if (p)
    {
     hk_datetime date;
     date.set_now();
     p->set_asstring(date.datetime_asstring());
    }
  ds->store_changed_data();
  inform_datasources_filelist_changes((listtype)type);
  delete ds;
}





ofstream* hk_database::savestream(const hk_string&nm,filetype type,bool ask_before_overwrite)
{
    return savestream(nm,type,ask_before_overwrite,true,false);
}

hk_string hk_database::savemessage(filetype type,enum_storage storage,hk_string nm)
{
 hk_string dialog;
 if (storage==st_overwrite)
 {
    if (type==ft_query)dialog=replace_all("%1",hk_translate("Query '%1' already exists. Overwrite it?"),nm);
    else if (type==ft_form)dialog=replace_all("%1",hk_translate("Form '%1' already exists. Overwrite it?"),nm);
    else if (type==ft_report)dialog=replace_all("%1",hk_translate("Report '%1' already exists. Overwrite it?"),nm);
    else if (type==ft_module)dialog=replace_all("%1",hk_translate("Module '%1' already exists. Overwrite it?"),nm);
    else dialog=replace_all("%1",hk_translate("File '%1' already exists. Overwrite it?"),nm);
 }
 else
 {
    if (type==ft_query)dialog=replace_all("%1",hk_translate("Query '%1' has changed. Store changes?"),nm);
    else if (type==ft_form)dialog=replace_all("%1",hk_translate("Form '%1' has changed. Store changes?"),nm);
    else if (type==ft_report)dialog=replace_all("%1",hk_translate("Report '%1' has changed. Store changes?"),nm);
    else if (type==ft_module)dialog=replace_all("%1",hk_translate("Module '%1' has changed. Store changes?"),nm);
    else dialog=replace_all("%1",hk_translate("File '%1' has changed. Store changes?"),nm);
 }
 return dialog;
}

ofstream* hk_database::savestream(const hk_string&nm,filetype type,bool ask_before_overwrite,bool with_header,bool ask_for_new_name)
{
  return savestream(st_overwrite,nm,type,ask_before_overwrite,with_header,ask_for_new_name);
}


ofstream* hk_database::savestream(enum_storage storage,const hk_string&nm,filetype type,bool ask_before_overwrite,bool with_header,bool ask_for_new_name)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::savestream");
#endif
    hk_string filename=p_private->p_databasepath+"/";
    hk_url url(nm);
    filename+=url.filename()+(url.extension().size()>0?"."+url.extension():"")+fileendings(type);
    hk_string dialog=savemessage(type,storage,url.filename());
    
    if (ask_before_overwrite)
    {
        ifstream ifs(filename.c_str(),ios::in);
        if (ifs &&type!=ft_table)
        {
//file already exists, overwrite it?
            if(runtime_only() || !show_yesnodialog(dialog,true))
            {
// no, so don't overwrite it
                if (ask_for_new_name)
                {
                    if (type==ft_query)dialog=hk_translate("Enter new query name:");
                    else if (type==ft_form)dialog=hk_translate("Enter new form name:");
                    else if (type==ft_report)dialog=hk_translate("Enter new report name:");
                    else if (type==ft_module)dialog=hk_translate("Enter new module name:");
                    else dialog=hk_translate("Enter new name:");
                    hk_string res=show_stringvaluedialog(dialog);
                    if (res.size()==0) return NULL;
                    return savestream(res,type,ask_before_overwrite,with_header,ask_for_new_name);

                }
                else
                    return NULL;
            }
        }
    }

    ofstream* stream= new ofstream(filename.c_str());
    if (stream!=NULL &&!stream->fail())
    {
        inform_datasources_filelist_changes((listtype)type);
        if (with_header) (*stream) <<"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"<<endl;
    } else
       {
        show_warningmessage(replace_all("%1",hk_translate("Error: File '%1' could not be created"),filename));
	return NULL;
	}
    return stream;

}











#ifdef HAVE_SSTREAM
    stringstream*  hk_database::savestringstream(filetype type)

#else
    strstream*  hk_database::savestringstream(filetype type)

#endif
{
    return savestringstream(type,true);
}

    STRINGSTREAM*  hk_database::savestringstream(filetype /*type*/,bool with_header)


{
#ifdef HK_DEBUG
    hkdebug("hk_database::savestringstream");
#endif

    STRINGSTREAM* stream= new STRINGSTREAM();
    if (stream!=NULL &&!stream->fail())
    {
        if (with_header) (*stream) <<"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"<<endl;
    }

    return stream;

}















xmlNodePtr hk_database::xmlload(const hk_string& nm,filetype type)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::xmlload");
#endif
return (p_private->p_loadmode[type]==central?xmlload_central(nm,type):xmlload_local(nm,type));
}


hk_string hk_database::load(const hk_string& nm,filetype type)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::load");
#endif
return (p_private->p_loadmode[type]==central?load_central(nm,type):load_local(nm,type));
}


xmlNodePtr hk_database::xmlload_local(const hk_string& nm,filetype type)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::load_local");
#endif
    if (nm.size()==0) return NULL;
    hk_string n=p_private->p_databasepath+"/";
    n+=nm+fileendings(type);
    xmlDocPtr d= xmlParseFile(n.c_str());
    return xmlDocGetRootElement(d);
}


xmlNodePtr hk_database::xmlload_central(const hk_string& nm,filetype type)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::load_central");
#endif

hk_datasource* ds=new_table("HKCLASSES");
 if (!ds)
   {
   show_warningmessage(hk_translate("Error: hk_database::load_central could not get a new table"));
   return NULL;
   }
 hk_string result;
 hk_string filter="\"type\"="+longint2string(type);
 ds->set_filter(filter);
 ds->enable();
 hk_column* n=ds->column_by_name("name");
 hk_column* v=ds->column_by_name("value");
 hk_column* t=ds->column_by_name("type");
 if (!n ||!v||!t)
   {
    show_warningmessage(hk_translate("Error: hk_database::load_central could not find system columns!"));
    delete ds;
    return NULL;
   }
  unsigned int row=n->find(nm,true,true);
  if (row > ds->max_rows())
  { // not existing
     /*show_warningmessage(replace_all("%1",hk_translate("Error: hk_database::load_central object '%1' not found"),name));*/
     //cerr <<"no file: "<<n<<endl;
     delete ds;
     return NULL;
  }
  else
  {  // replace existing object
    ds->goto_row(row);
  }
  result=v->asstring();
  delete ds;
xmlDocPtr d= xmlParseMemory(result.c_str(),result.size());
return xmlDocGetRootElement(d);
}


















hk_string hk_database::load_local(const hk_string& nm,filetype type)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::load_local");
#endif
    if (nm.size()==0) return "";
    hk_string n=p_private->p_databasepath+"/";
    n+=nm+fileendings(type);
    hk_string result;
    ifstream ifs(n.c_str(),ios::in);
    if (ifs)
    {
        char c;
        while (ifs.get(c))
            result+=c;
    }
    //else cerr <<"no file: "<<n<<endl;
    return result;
}


hk_string hk_database::load_central(const hk_string& nm,filetype type)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::load_central");
#endif

hk_datasource* ds=new_table("HKCLASSES");
 if (!ds)
   {
   show_warningmessage(hk_translate("Error: hk_database::load_central could not get a new table"));
   return "";
   }
 hk_string result;
 hk_string filter="\"type\"="+longint2string(type);
 ds->set_filter(filter);
 ds->enable();
 hk_column* n=ds->column_by_name("name");
 hk_column* v=ds->column_by_name("value");
 hk_column* t=ds->column_by_name("type");
 if (!n ||!v||!t)
   {
    show_warningmessage(hk_translate("Error: hk_database::load_central could not find system columns!"));
    delete ds;
    return "";
   }
  unsigned int row=n->find(nm,true,true);
  if (row > ds->max_rows())
  { // not existing
     /*show_warningmessage(replace_all("%1",hk_translate("Error: hk_database::load_central object '%1' not found"),name));*/
     //cerr <<"no file: "<<n<<endl;
     delete ds;
     return "";
  }
  else
  {  // replace existing object
    ds->goto_row(row);
  }
  result=v->asstring();
  delete ds;
return result;

}



hk_string hk_database::fileendings(filetype e)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::fileendings");
#endif
    switch (e)
    {

        case ft_table: return ".hk_table";
        case ft_query: return ".hk_query";
        case ft_form: return ".hk_form";
        case ft_report: return ".hk_report";
        case ft_view: return ".hk_view";
        case ft_module: return ".hk_module";
        case ft_referentialintegrity: return ".hk_referentialintegrity";
    }
    return ".hk_unknown";
}


hk_string hk_database::fileendings(objecttype e)
{
   filetype f;
   switch (e)
   {
     case ot_table: f=ft_table;break;
     case ot_query: f=ft_query;break;
     case ot_form: f=ft_form;break;
     case ot_report: f=ft_report;break;
     case ot_module: f=ft_module;break;
     default: return ".hk_unknown";
   }
   return fileendings(f);
}


vector<hk_string>* hk_database::querylist(void)
{
    return filelist(ft_query);
}


vector<hk_string>* hk_database::formlist(void)
{
    return filelist(ft_form);
}


vector<hk_string>* hk_database::reportlist(void)
{
    return filelist(ft_report);
}

vector<hk_string>* hk_database::modulelist(void)
{
    return filelist(ft_module);
}




vector<hk_string>* hk_database::filelist(filetype type)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::filelist");
#endif

return (p_private->p_loadmode[type]==central?central_filelist(type):local_filelist(type));
}



vector<hk_string>* hk_database::central_filelist(filetype type)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::central_filelist");
#endif
 p_private->p_filelist.erase(p_private->p_filelist.begin(),p_private->p_filelist.end());
if (!p_connection->server_supports(hk_connection::SUPPORTS_SQL)) return &p_private->p_filelist;
 if (!has_centralstoragetable() || !p_connection->is_connected()) return &p_private->p_filelist;
 hk_datasource* ds=new_resultquery();
 if (!ds)
   {
   show_warningmessage(hk_translate("Error: hk_database::central_filelist could not get a new resultquery"));
   }
 else
 {
   hk_string t;
   switch (type)
   {
  /*
   type 1 = table
   type 2 = query
   type 3 = form
   type 4 = report
   type 7 = module
 */
        case ft_table:  t="1";break;
        case ft_query:  t="2";break;
        case ft_form:   t="3";break;
        case ft_report: t="4";break;
        case ft_module: t="7";break;
        default :
	   {
	     show_warningmessage(hk_translate("Warning: hk_database::central_filelist, unknown filetype"));
	     t="-1";
	   }
   }

   hk_string query="SELECT * FROM \"HKCLASSES\" WHERE \"type\"="+t+" ORDER BY \"name\"";
   ds->set_sql(query);
   ds->enable();
   unsigned k=0;
   hk_column* namecol=ds->column_by_name("name");
   if (namecol)
   {
     while (k<ds->max_rows())
     {
       p_private->p_filelist.insert(p_private->p_filelist.end(),namecol->asstring());
       ds->goto_next();++k;
     };
   }
   else show_warningmessage("hk_database::central_filelist ERROR could not find column!");

 }

 delete ds;
return &p_private->p_filelist;
}




vector<hk_string>* hk_database::local_filelist(filetype type)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::local_filelist");
#endif
    DIR *dp;
    struct dirent *entry;
    hk_string datei;
    hk_string fileend=fileendings(type);

    p_private->p_filelist.erase(p_private->p_filelist.begin(),p_private->p_filelist.end());
    if ((dp=opendir(p_private->p_databasepath.c_str()))!=NULL)
    {

        while ((entry = readdir(dp))!=NULL)
        {
            datei=entry->d_name;
            hk_url url(entry->d_name);
            if ("."+url.extension()==fileend)
             {
                p_private->p_filelist.insert(p_private->p_filelist.end(),url.filename());
             
             }
        }
        closedir(dp);

    }
    sort(p_private->p_filelist.begin(),p_private->p_filelist.end());
    return &p_private->p_filelist;
}


bool  hk_database::delete_file(const hk_string& nm,filetype type,enum_interaction x)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::delete_file");
#endif
    hk_string message;
    if (type==ft_query)
        message=hk_translate("Delete the query:%FNAME%?");
    else
    if (type==ft_report)
        message=hk_translate("Delete the report:%FNAME%?");
    else
    if (type==ft_form)
        message=hk_translate("Delete the form:%FNAME%?");
    else
    if (type==ft_module)
        message=hk_translate("Delete the module:%FNAME%?");
    else
        message=hk_translate("Delete the file:%FNAME%");
    message=replace_all("%FNAME%",message,nm);
    if(x==interactive&&!show_yesnodialog(message,true))
        return false;
return (p_private->p_loadmode[type]==central?delete_centralfile(nm,type,x):delete_localfile(nm,type,x));

}


bool  hk_database::delete_centralfile(const hk_string& nm,filetype type,enum_interaction /*x*/)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::delete_centralfile");
#endif

hk_datasource* ds=new_table("HKCLASSES");
 if (!ds)
   {
   show_warningmessage(hk_translate("Error: hk_database::delete_centralfile could not get a new table"));
   return false;
   }
 hk_string result;
 hk_string filter="\"type\"="+longint2string(type);
 ds->set_filter(filter);
 ds->enable();
 hk_column* n=ds->column_by_name("name");
 hk_column* v=ds->column_by_name("value");
 hk_column* t=ds->column_by_name("type");
 if (!n ||!v||!t)
   {
    show_warningmessage(hk_translate("Error: hk_database::delete_centralfile could not find system columns!"));
    delete ds;
    return false;
   }
  unsigned int row=n->find(nm,true,true);
  if (row > ds->max_rows())
  { // not existing
     show_warningmessage(replace_all("%1",hk_translate("Error: hk_database::delete_centralfile object '%1' not found"),nm));
     delete ds;
     return false;
  }
  else
  {  // delete existing object
    ds->goto_row(row);
  }
  ds->delete_actualrow(noninteractive);
  inform_datasources_filelist_changes((listtype)type);
  delete ds;
return true;

}



bool  hk_database::delete_localfile(const hk_string& nm,filetype type,enum_interaction /*x*/)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::delete_file");
#endif
    hk_string filename=p_private->p_databasepath+"/";
    filename+=nm+fileendings(type);
    int r=unlink(filename.c_str());
    if (r==0)
    {
        inform_datasources_filelist_changes((listtype)type);
        return true;
    }
    return false;
}


void hk_database::inform_datasources_filelist_changes(listtype type)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::inform_datasources_filelist_changes");
#endif
    list<hk_data*>::iterator it ;
    it=p_private->p_hkdsourcelist.begin();
    while (it!=p_private->p_hkdsourcelist.end())
    {
        hk_data* vi=*it;
        it++;
        vi->filelist_changes(type);
    }

    mark_visible_objects_as_not_handled();
    list<hk_dbvisible*>::iterator dbit=p_private->p_visibles.begin();
    while ( dbit!=p_private->p_visibles.end() )
    {
        hk_dbvisible* dv=*dbit;
        ++dbit;
        if (!dv->p_already_handled)
        {
            dv->p_already_handled=true;
            dv->list_changes(type);
            dbit=p_private->p_visibles.begin();
        }
    }


}


hk_datasource*  hk_database::load_datasource(const hk_string& nm,datasourcetype dt,hk_presentation* p)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::load_datasource");
#endif

    switch (dt)
    {
      case dt_query:
    {
        hk_datasource* d=NULL;
	if (nm.size()==0) return new_resultquery(p);
        xmlNodePtr r=xmlload(nm,ft_query);
        if (r)
        {
            d=new_resultquery(p);
        }
        if (d!=NULL) d->loaddata(r);
        return d;
    }
    case dt_view:
        {
        return new_view(nm,p);
        }
    default:
        return new_table(nm,p);

    }

}


hk_datasource* hk_database::driver_specific_new_table(hk_presentation* )
{
    return NULL;
}


hk_datasource* hk_database::driver_specific_new_view(hk_presentation* )
{
    return NULL;
}


hk_datasource* hk_database::driver_specific_new_resultquery(hk_presentation* )
{
    return NULL;
}


void hk_database::dbvisible_add(hk_dbvisible* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::visible_add");
#endif
    p_private->p_visibles.insert(p_private->p_visibles.end(),v);
}


void hk_database::dbvisible_remove(hk_dbvisible* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::visible_remove");
#endif
    p_private->p_visibles.remove(v);
}


void hk_database::clear_visiblelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::clear_visiblelist");
#endif
    mark_visible_objects_as_not_handled();
    list<hk_dbvisible*>::iterator it=p_private->p_visibles.begin();
    while ( it!=p_private->p_visibles.end() )
    {
        hk_dbvisible* dv=*it;
        ++it;
        if (!dv->p_already_handled)
        {
            dv->p_already_handled=true;
            dv->database_delete();
            p_private->p_visibles.remove(dv);
            it=p_private->p_visibles.begin();

        }
    }

}


void hk_database::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::before_source_vanishes");
#endif

    mark_visible_objects_as_not_handled();
    list<hk_dbvisible*>::iterator it=p_private->p_visibles.begin();
    while ( it!=p_private->p_visibles.end() )
    {
        hk_dbvisible* dv=*it;
        ++it;
        if (!dv->p_already_handled)
        {
            dv->p_already_handled=true;
            dv->before_source_vanishes();
            it=p_private->p_visibles.begin();
        }
    }



#ifdef HK_DEBUG
    hkdebug("ENDE database::before_source_vanishes");
#endif
}


void hk_database::presentation_add(hk_presentation* p)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::presentation_add");
#endif
    p_private->p_presentations.insert(p_private->p_presentations.end(),p);
}


void hk_database::presentation_remove(hk_presentation* p)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::presentation_remove");
#endif
    p_private->p_presentations.remove(p);
}

list <hk_presentation*>* hk_database::presentationlist() const
{
  return &p_private->p_presentations;
}


bool hk_database::in_presentationload(void)
{
    list<hk_presentation*>::iterator it;
    it=p_private->p_presentations.begin();
    while ( it!=p_private->p_presentations.end() )
    {
#ifdef HK_DEBUG
        hkdebug("dv->classname: ");
        hkdebug((*it)->hkclassname());
#endif
        if ((*it)->while_loading()) return true;
        ++it;
    }
return false;

}

void hk_database::clear_presentationlist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::clear_presentationlist");
#endif
    list<hk_presentation*>::iterator it;
    it=p_private->p_presentations.begin();
    while ( it!=p_private->p_presentations.end() )
    {
        hk_presentation* dv=*it;
#ifdef HK_DEBUG
        hkdebug("dv->classname: ");
        hkdebug(dv->hkclassname());
#endif
        ++it;
        dv->database_delete();
        p_private->p_presentations.remove(dv);
    }

}


hk_presentation* hk_database::existing_presentation(const hk_string& nm, hk_presentation::enum_presentationtype t)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::existing_presentation");
#endif
    list<hk_presentation*>::iterator it;
    it=p_private->p_presentations.begin();
    while ( it!=p_private->p_presentations.end() )
    {
        hk_presentation* dv=*it;
        ++it;
        if (dv->name()==nm &&dv->presentationtype()== t&&!dv->is_subpresentation())
        {
            return dv;
        }
    }
    return NULL;
}


hk_form* hk_database::existing_form(const hk_string nm)
{
  hk_form*f =dynamic_cast<hk_form*>(existing_presentation(nm,hk_presentation::form));
  return f;
}


hk_report* hk_database::existing_report(const hk_string nm)
{
  hk_report*r =dynamic_cast<hk_report*>(existing_presentation(nm,hk_presentation::report));
  return r;

}





bool hk_database::copy_table(hk_datasource* fromdatasource,bool schema_and_data,bool replacetable, bool ask,progress_dialogtype* progressdialog)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::copy_table");
#endif
    if (!connection()->server_supports(hk_connection::SUPPORTS_NEW_TABLE)) return false;
    if (!fromdatasource)return false;
    if (fromdatasource->columnnames()->size()==0)return false;
    bool tblexists=table_exists(fromdatasource->name());
    if ((tblexists&&runtime_only()) || (tblexists&&!ask&&!replacetable))return false;
    hk_datasource* newds=new_table();
    hk_string structure,buffer;
    STRINGSTREAM stream;
    fromdatasource->save_datasourcedefinition(stream);
//    while (stream >>buffer) structure+=buffer;
            char c;
        while (stream.get(c))
            structure+=c;
    xmlDocPtr d=xmlParseMemory(structure.c_str(),structure.size());
    xmlNodePtr node=xmlDocGetRootElement(d);           
    bool result=newds->load_datasourcedefinition(node,replacetable);
    bool cancel=false;
    if (result&&schema_and_data)
    {
        newds->set_accessmode(hk_datasource::batchwrite);
        newds->enable();
        bool source_was_enabled=fromdatasource->is_enabled();
        fromdatasource->set_blockvisiblesignals(true);
        unsigned long pos=fromdatasource->row_position();
        hk_datasource::enum_accessmode access=hk_datasource::batchread;
        if (!source_was_enabled)
        {
        access=fromdatasource->accessmode();
        fromdatasource->set_accessmode(hk_datasource::batchread);
        fromdatasource->enable();
        }
        else
        fromdatasource->goto_first();
        unsigned long r=0;
	if (!newds->columns())
	  {
	   if (ask) show_warningmessage(hk_translate("hk_database::copy_table: bug newds has no columns!"));
	   return false;
	   }
        list<hk_column*>::iterator colit =newds->columns()->begin();
        while (colit!=newds->columns()->end())
        {
            (*colit)->set_allow_autoincwrite(true);
            colit++;
        }

        while (r<fromdatasource->max_rows()&&!cancel)
        {
            newds->setmode_insertrow();
            list<hk_column*>::iterator it=fromdatasource->columns()->begin();
            while (it!=fromdatasource->columns()->end())
            {
                hk_column* cl=newds->column_by_name(newds->systemcolumnname((*it)->name()),fromdatasource->columnname_occurance(*it));
		//cerr <<"    add c:"<<c<<" name:"<<newds->systemcolumnname((*it)->name())<<endl;
                if (cl&&!(*it)->is_nullvalue())
                {

                    switch(cl->columntype())
                    {
                        case hk_column::binarycolumn: if (connection()->server_supports(hk_connection::SUPPORTS_BINARYCOLUMN))
							cl->set_asbinary((*it)->asbinary());
							break;
                        case hk_column::boolcolumn: cl->set_asbool((*it)->asbool());break;
                        default: if ((*it)->asstring().size()>0)
                        {

                            cl->set_asstring((*it)->asstring());
                        }
                    }
                }

                it++;
            }

            if (progressdialog) cancel=progressdialog(r,fromdatasource->max_rows(),replace_all("%TABLE%",hk_translate("Copying table %TABLE%"),name()));
            if (!newds->store_changed_data())
	    {
	    if(!show_yesnodialog(hk_translate("Continue loading data?"),true))
	      {
	       newds->set_ignore_changed_data();
	       cancel=true;
	      }
	    }
            fromdatasource->goto_next();
            r++;
        }
        newds->driver_specific_after_copy_table();
        newds->disable();
        fromdatasource->set_blockvisiblesignals(false);
        if (!source_was_enabled)
        {
         fromdatasource->disable();
         fromdatasource->set_accessmode(access);
        }
        else
        fromdatasource->goto_row(pos);
        delete newds;
    }
    if (result)inform_datasources_filelist_changes(lt_table);
    if (cancel) return false;
    return result;
}






bool hk_database::copy_view(hk_datasource* fromdatasource,progress_dialogtype* progressdialog, const hk_string& newname)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::copy_view");
#endif
cerr <<"copy_view"<<endl;
    if (!fromdatasource)return false;
    //if (fromdatasource->columns()->size()==0)return false;
    if (!connection()->server_supports(hk_connection::SUPPORTS_NEW_VIEW))
    {
     show_warningmessage(hk_translate("Error copy_view: target database does not support views!"));
     return false;
    }
cerr <<"vor viewexists"<<endl;
    bool viewexists=view_exists(newname.size()==0?fromdatasource->name():newname);
    if ((viewexists&&runtime_only()))return false;
    hk_string res;
    if (viewexists)
      {
       if(show_yesnodialog(replace_all("%1",hk_translate("View '%1' already exists. Replace it?"),newname.size()==0?fromdatasource->name():newname),true))
       {
         delete_view(fromdatasource->name(),noninteractive);
	 res=newname.size()==0?fromdatasource->name():newname;
       }
       else
       {
         /* while(viewexists)
	  {
	  res=show_stringvaluedialog(hk_translate("Please enter the viewname:"));
          if (res.size()==0) return false;
	  viewexists=view_exists(res);
	  }
	  */
	  res=show_stringvaluedialog(hk_translate("Please enter the viewname:"));
          if (res.size()==0) return false;
	  return copy_view(fromdatasource,progressdialog,res);
       }
      }
      else
        res=newname.size()==0?fromdatasource->name():newname;
cerr<<"vor new_view"<<endl;
    hk_datasource* newds=new_view();
    if (!newds) return false;
    newds->set_name(res);
    newds->set_sql(fromdatasource->sql());
cerr<<"vor create_view_now"<<endl;
    bool result=newds->create_view_now();
    if (result)inform_datasources_filelist_changes(lt_view);
cerr <<"vor delete"<<endl;
    delete newds;
    return result;
}















void    hk_database::mark_datasources_as_not_handled(void)
{
    list<hk_data*>::iterator it=p_private->p_hkdsourcelist.begin();
    while (it!=p_private->p_hkdsourcelist.end())
    {
        (*it)->p_already_handled=false;
        ++it;
    }

}


void    hk_database::mark_visible_objects_as_not_handled(void)
{
    list<hk_dbvisible*>::iterator it=p_private->p_visibles.begin();
    while (it!=p_private->p_visibles.end())
    {
        (*it)->p_already_handled=false;
        ++it;
    }

}

hk_form* hk_database::new_formvisible(void)
{
   hk_form* result=NULL;
    if (p_newformfunction)
    	result=p_newformfunction(this,p_private->p_parent);
    else
    {
      result=new hk_form();
      if (result) result->set_database(this);
    }
   return result;
}


hk_form* hk_database::new_dialogformvisible(void)
{
   hk_form* result=NULL;
    if (p_newformfunction)
    	result=p_newdialogformfunction(this,p_private->p_parent);
    else
    {
      result=new hk_form();
      if (result) result->set_database(this);
    }
   return result;
}



hk_report* hk_database::new_reportvisible(void)
{
   hk_report* result=NULL;
    if (p_newreportfunction)
    	result=p_newreportfunction(this,p_private->p_parent);
    else
    {
     result=new hk_report();
     if (result) result->set_database(this);
    }
   return result;
}

void	hk_database::set_new_formvisiblefunction(newform_type* f)
{
p_newformfunction=f;
}

void	hk_database::set_new_dialogformvisiblefunction(newform_type* f)
{
p_newdialogformfunction=f;
}

void	hk_database::set_new_reportvisiblefunction(newreport_type* f)
{
p_newreportfunction=f;

}


void	hk_database::set_new_tablevisiblefunction(newtable_type* f)
{
p_newtablefunction=f;

}


void	hk_database::set_new_queryvisiblefunction(newquery_type* f)
{
p_newqueryfunction=f;

}


hk_dstable* hk_database::new_tablevisible(void)
{
    if (p_newtablefunction)
    	 return p_newtablefunction(p_private->p_parent);
    else return new hk_dstable();
}

hk_dsquery* hk_database::new_queryvisible(void)
{
    if (p_newqueryfunction)
    	 return p_newqueryfunction(p_private->p_parent);
    else return new hk_dsquery();
}

void hk_database::set_usewidgetparent(hk_class* parent)
{
 p_private->p_parent=parent;
}

bool hk_database::rename_table(const hk_string& originalname, const hk_string& new_name,enum_interaction )
{
   if ( driver_specific_rename_table(originalname,new_name))
   {
     list<hk_data*>::iterator it=p_private->p_hkdsourcelist.begin();
     while (it!=p_private->p_hkdsourcelist.end())
     {
       if ((*it)->type()==hk_data::ds_table && (*it)->name()==originalname)
           {
		bool enabled=((hk_datasource*)(*it))->is_enabled();
		if (enabled) ((hk_datasource*)(*it))->disable();
		(*it)->set_name(new_name);
		if (enabled) ((hk_datasource*)(*it))->enable();
	   }
       ++it;
       inform_datasources_filelist_changes(lt_table);
     }


   return true;
   }
   else return false;
}


bool hk_database::rename_file(const hk_string& originalname, const hk_string& new_name,filetype ftype,enum_interaction x)
{
    if (ftype==ft_table) return rename_table(originalname,new_name,x);

    if (storagemode(ftype)==central)
    {
      hk_datasource* ds=new_table("HKCLASSES");
         if (!ds)
          {
           show_warningmessage(hk_translate("Error: hk_database::rename_file could not get a new table"));
           return false;
          }
     hk_string filter="\"type\"="+longint2string(ftype);
     ds->set_filter(filter);
     ds->enable();
     hk_column* n=ds->column_by_name("name");
     hk_column* v=ds->column_by_name("value");
     hk_column* t=ds->column_by_name("type");
     if (!n ||!v||!t)
       {
        show_warningmessage(hk_translate("Error: hk_database::rename_file could not find system columns!"));
        delete ds;
        return false;
       }
     unsigned int row=n->find(originalname,true,true);
     if (row > ds->max_rows())
     {
        show_warningmessage(hk_translate("Error: hk_database::rename file: No such file!"));
        delete ds;
        return false;
     }

   // replace existing object
    ds->goto_row(row);
    n->set_asstring(new_name);
    ds->store_changed_data();
    inform_datasources_filelist_changes((listtype)ftype);
    delete ds;
    return true;
    } // Central storage END

    hk_string n=p_private->p_databasepath+"/"+originalname+fileendings(ftype);

    ifstream ifs(n.c_str(),ios::in);
    ofstream* ofs = NULL;
    if (ifs)ofs=savestream(new_name,ftype,true,false,true);

    if (ifs && ofs &&(*ofs))
    {                                             //copy file
        char c;
        while (ifs.get(c))
            ofs->put(c);
    }
    else
    {
        if (!ifs && x==interactive) show_warningmessage("no file: "+n);
        if (ofs) delete ofs;
        return false;
    }
    ofs->close();
    ifs.close();
    delete ofs;
    delete_file(originalname,ftype,noninteractive);
    return true;



}


void hk_database::set_storagemode(filetype type,enum_storagemode loadmd,enum_storagemode storemd)
{
   if (type==ft_referentialintegrity || type==ft_view) return;
   bool hascentraltable =has_centralstoragetable();
      p_private->p_loadmode[type]=((hascentraltable && loadmd==hk_database::central)?
                             hk_database::central:hk_database::local
                            );
      p_private->p_storagemode[type]=((hascentraltable && storemd==hk_database::central)?
                             hk_database::central:hk_database::local
                            );
}

hk_database::enum_storagemode hk_database::storagemode(filetype f)
{
  if (f==ft_view||f==ft_referentialintegrity) return hk_database::local;
  //if (f==ft_referentialintegrity) return p_private->p_storagemode[ft_form];
  return p_private->p_storagemode[f];
}

hk_database::enum_storagemode hk_database::loadmode(filetype f)
{
  if (f==ft_view||f==ft_referentialintegrity) return hk_database::local;
  //if (f==ft_referentialintegrity) return p_private->p_storagemode[ft_form];
  return p_private->p_loadmode[f];

}

bool hk_database::create_centralstoragetable(void)
{
  if (has_centralstoragetable()) return false;
  hk_datasource *ds=new_table();
  if (!ds)
    {
       show_warningmessage(hk_translate("Error: hk_database::create_centralstoragetable could not get a new table"));
       return false;
    }
  ds->setmode_createtable();
  hk_column* col=ds->new_column();
  if (!col) return false;
  col->set_name("type");
  col->set_primary(true);
  col->set_columntype(hk_column::smallintegercolumn);
  col->set_notnull(true);

  col=ds->new_column();
  if (!col) return false;
  col->set_name("name");
  col->set_primary(true);
  col->set_columntype(hk_column::textcolumn);
  col->set_size(190);// reduced for Firebird which Primary key is not allowed to be >253 chars (primary key is: 'name'+'type'
  col->set_notnull(true);

  col=ds->new_column();
  if (!col) return false;
  col->set_name("value");
  col->set_primary(false);
  col->set_columntype(hk_column::memocolumn);
  col->set_notnull(true);

  col=ds->new_column();
  if (!col) return false;
  col->set_name("user");
  col->set_size(50);
  col->set_primary(false);
  col->set_columntype(hk_column::textcolumn);

  col=ds->new_column();
  if (!col) return false;
  col->set_name("update");
  col->set_primary(false);
  col->set_columntype(hk_column::datetimecolumn);


  ds->set_name("HKCLASSES");
  bool result = ds->create_table_now();
  delete ds;
  return result;
}



bool hk_database::has_centralstoragetable(void)
{
 driver_specific_tablelist();

  return (find(p_tablelist.begin(),p_tablelist.end(),"HKCLASSES")!=p_tablelist.end());
}


void hk_database::load_configuration(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::load_configuration");
#endif
    hk_string n=database_path()+"/";
    n+="database.conf";
    if (!file_exists(n)) return;
    xmlDocPtr doc=xmlParseFile(n.c_str());
    xmlNodePtr result=NULL;
    if (doc) result=xmlDocGetRootElement(doc);

    if (doc && result)
    {
        loaddata(result);

    }


}

void hk_database::save_configuration(void)
{
    mkdir (database_path().c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
    hk_string p_filename =database_path()+"/database.conf";
    ofstream* stream= new ofstream(p_filename.c_str());
    if (stream==NULL)
      return;
    savedata(*stream);

    stream->close();
    delete stream;
    chmod(p_filename.c_str(),S_IRUSR|S_IWUSR);
}

void hk_database::savedata(ostream& s)
{
    hk_string mt="DATABASE";
    s<<"<?xml version=\"1.0\" ?>"<<endl;
    start_mastertag(s,mt);
        save_storage(s,ft_query);
        save_storage(s,ft_form);
        save_storage(s,ft_report);
        save_storage(s,ft_module);
    set_tagvalue(s,"DATABASECHARSET",p_private->p_charset);
    set_tagvalue(s,"AUTOMATIC_DATA_UPDATE",is_automatic_data_update());
    
    end_mastertag(s,mt);
}

void hk_database::save_storage(ostream& s,filetype f)
{
    hk_string savetxt,loadtxt;
    switch (f)
    {
     case ft_table:
     			savetxt="STORETABLE";
			loadtxt="LOADTABLE";
     			break;
     case ft_query:
     			savetxt="STOREQUERY";
			loadtxt="LOADQUERY";
     			break;
     case ft_form:
     			savetxt="STOREFORM";
			loadtxt="LOADFORM";
     			break;
     case ft_report:
     			savetxt="STOREREPORT";
			loadtxt="LOADREPORT";
     			break;
     case ft_view:
     			savetxt="STOREVIEW";
			loadtxt="LOADVIEW";
     			break;
     case ft_module:
     			savetxt="STOREMODULE";
			loadtxt="LOADMODULE";
     			break;
     case ft_referentialintegrity:
     			savetxt="STOREREFERENTIALINTEGRITY";
			loadtxt="LOADREFERENTIALINTEGRITY";
     			break;

    };
//     cerr <<"f:"<<f<<" savetxt:"<<savetxt<<" loadtxt:"<<loadtxt<<endl;
    hk_string c=(p_private->p_storagemode[f]==hk_database::local?"LOCAL":"CENTRAL");
    set_tagvalue(s,savetxt,c);
    c=(p_private->p_loadmode[f]==hk_database::local?"LOCAL":"CENTRAL");
    set_tagvalue(s,loadtxt,c);

}


void hk_database::loaddata(xmlNodePtr definition)
{
    get_tagvalue(definition,"DATABASECHARSET",p_private->p_charset);
    get_tagvalue(definition,"AUTOMATIC_DATA_UPDATE",p_private->p_automatic_data_update);
   if (!has_centralstoragetable())
   {
    for (int f=ft_table;f<=ft_referentialintegrity;++f)
     {
       p_private->p_storagemode[f]=hk_database::local;
       p_private->p_loadmode[f]=hk_database::local;
     }
    return;
   }
    for (int f=ft_query;f<=ft_module;++f)
    {
    load_storage(definition,(filetype)f);
    }

}

void hk_database::load_storage(xmlNodePtr definition,filetype f)
{

    hk_string savetxt,loadtxt;
    switch (f)
    {
     case ft_table:
     			savetxt="STORETABLE";
			loadtxt="LOADTABLE";
     			break;
     case ft_query:
     			savetxt="STOREQUERY";
			loadtxt="LOADQUERY";
     			break;
     case ft_form:
     			savetxt="STOREFORM";
			loadtxt="LOADFORM";
     			break;
     case ft_report:
     			savetxt="STOREREPORT";
			loadtxt="LOADREPORT";
     			break;
     case ft_view:
     			savetxt="STOREVIEW";
			loadtxt="LOADVIEW";
     			break;
     case ft_module:
     			savetxt="STOREMODULE";
			loadtxt="LOADMODULE";
     			break;
     case ft_referentialintegrity:
     			savetxt="STOREREFERENTIALINTEGRITY";
			loadtxt="LOADREFERENTIALINTEGRITY";
     			break;

    };
    hk_string savebuffer,loadbuffer;
    get_tagvalue(definition,savetxt,savebuffer);
    get_tagvalue(definition,loadtxt,loadbuffer);

    set_storagemode(f,(loadbuffer=="CENTRAL"?central:local),(savebuffer=="CENTRAL"?central:local));

}


void hk_database::disable()
{
 clear_presentationlist();
 clear_visiblelist();
 inform_datasources_before_closing();
 p_private->p_dbname="";
}

bool hk_database::store_connectionfile(const hk_url& url, bool store_password)
{
        ifstream ifs(url.url().c_str(),ios::in);
        if (ifs )
        {
//file already exists, overwrite it?
            if(runtime_only() || !show_yesnodialog(replace_all("%1",hk_translate("Overwrite '%1'?"),url.url()),true))
            {
              return false;
            }
        }

    ofstream* stream= new ofstream(url.url().c_str());
    if (stream!=NULL &&!stream->fail())
	{
        (*stream) <<"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"<<endl;
 hk_string mtag="STOREDCONNECTION";
 start_mastertag(*stream,mtag);
 set_tagvalue(*stream,"CONNECTION",p_connection->drivername());
 set_tagvalue(*stream,"DATABASE",name());
 set_tagvalue(*stream,"USER",p_connection->user());
 set_tagvalue(*stream,"PASSWORD",(store_password?p_connection->password():""));
 set_tagvalue(*stream,"HOST",p_connection->host());
 set_tagvalue(*stream,"TCPPORT",p_connection->tcp_port());
 set_tagvalue(*stream,"BOOLEANEMULATION",p_connection->booleanemulation());
 end_mastertag(*stream,mtag);

	}
    delete stream;
   return true;
}


void hk_database::set_databasecharset(const hk_string& charset)
{
 p_private->p_charset=charset;
}

hk_string hk_database::databasecharset(void) const
{
return p_private->p_charset;
}


void hk_database::set_automatic_data_update(bool u)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::set_automatic_data_update");
#endif
    p_private->p_automatic_data_update=u;
}


bool hk_database::is_automatic_data_update(void)
{
    return p_private->p_automatic_data_update;
}



