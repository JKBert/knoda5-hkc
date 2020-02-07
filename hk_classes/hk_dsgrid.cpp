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
//$Revision: 1.62 $
#include "hk_dsgrid.h"
#include "hk_form.h"
#include "hk_column.h"
#include "hk_datasource.h"
#include "hk_database.h"
#include "hk_dsgridcolumn.h"
hk_dsgrid::hk_dsgrid(hk_form* f):hk_dsvisible(f)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkclassname("gridvisible");
    hkdebug("hk_dsgrid::constructor");
#endif
    p_automatic_columns=true;
    p_delete_nonexisting_columns=false;
    p_add_new_columns=false;
    p_visibletype=grid;
    p_holdrowdefinition=true;
    p_rowheight=20;
}


hk_dsgrid::~hk_dsgrid(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::destructor");
#endif
    clear_cols();
}


bool hk_dsgrid::columns_new_created(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::columns_new_created");
#endif
    if (p_automatic_columns )
    {
        clear_cols();

    }
    if (datasource()==NULL) return false;

    if (p_automatic_columns)
    {
#ifdef HK_DEBUG
        hkdebug("hk_dsgrid::columns_new_created   p_automatic_columns==true");
#endif
        list<hk_column*>* dscols=datasource()->columns();
        if (dscols!=NULL)
        {
            resize_cols(dscols->size());
            vector<hk_dsgridcolumn*>::iterator cols_it=p_cols.begin();
            for (list<hk_column*>::iterator it=dscols->begin();it!=dscols->end();it++)
            {
//cout <<"col: "<<(*it)->name()<<" occurance: "<<datasource()->columnname_occurance((*it))<<endl;
                (*cols_it)->set_grid(this);
                (*cols_it)->set_datasource(datasource());
                (*cols_it)->set_columnname((*it)->name(),false,datasource()->columnname_occurance((*it)));
                if (is_numerictype((*it)))
                    (*cols_it)->set_numberformat((*cols_it)->defaultuse_numberseparator(),is_realtype((*it))?(*cols_it)->defaultprecision():0,false);

                (*cols_it)->set_columntype(hk_dsgridcolumn::columnedit,false);
                if ((*cols_it)->column()!=NULL)
                    if ((*cols_it)->column()->columntype()==hk_column::boolcolumn)
                        (*cols_it)->set_columntype(hk_dsgridcolumn::columnbool,false) ;
                (*cols_it)->set_columnwidth(default_cellwidth,false);
                cols_it++;
            };
        }
    }
    else
    {                                             //p_automatic_columns==false
#ifdef HK_DEBUG
        hkdebug("hk_dsgrid::columns_new_created   p_automatic_columns==false");
#endif
        if (p_delete_nonexisting_columns) delete_nonexisting_columns();
        if (p_add_new_columns) add_new_columns();

    }
    widget_specific_columns_created();
    return true;
}


class column_exists
{
    public:
        bool operator()(hk_column* c)
        {
            if (searchvalue.size()==0||c->name().size()==0) return false;
            return searchvalue==c->name();
        }
        static hk_string searchvalue;
};

hk_string column_exists::searchvalue="";

void hk_dsgrid::delete_nonexisting_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::delete_nonexisting_columns");
#endif
    if (datasource()==NULL) return;
    list <hk_column*>* dscols=datasource()->columns();
    if (dscols==NULL) return;

    vector<hk_dsgridcolumn*>::iterator colit = p_cols.begin();
    while (colit!=p_cols.end())
    {

        column_exists::searchvalue=(*colit)->columnname();
        list<hk_column*>::iterator where=find_if(dscols->begin(),dscols->end(),column_exists());
        hk_dsgridcolumn* co =(*colit);
        if (where==dscols->end())
        {
            colit=p_cols.erase(colit);
            delete (co);
        }
        else colit++;
    }

}


class gridcolumn_exists
{
    public:
        bool operator()(hk_dsgridcolumn* c)
        {
            if (searchvalue.size()==0||c->columnname().size()==0) return false;
            return searchvalue==c->columnname();
        }
        static hk_string searchvalue;
};

hk_string gridcolumn_exists::searchvalue="";

void hk_dsgrid::add_new_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::add_new_columns");
#endif

    if (datasource()==NULL) return;
    list <hk_column*>* dscols=datasource()->columns();
    if (dscols==NULL) return;
    list<hk_column*>::iterator colit = dscols->begin();
    while (colit!=dscols->end())
    {
        gridcolumn_exists::searchvalue=(*colit)->name();

        vector<hk_dsgridcolumn*>::iterator where=find_if(p_cols.begin(),p_cols.end(),gridcolumn_exists());
        if (where==p_cols.end())
        {                                         // add new gridcolumn
            resize_cols(p_cols.size()+1);
            p_cols[p_cols.size()-1]->set_grid(this);
            p_cols[p_cols.size()-1]->set_datasource(datasource());
                                                  //cout <<"neue automatische Spalte: "<<(*colit)->name()<<endl;
            p_cols[p_cols.size()-1]->set_columnname((*colit)->name());
            if ((*colit)->columntype()==hk_column::boolcolumn)
                p_cols[p_cols.size()-1]->set_columntype(hk_dsgridcolumn::columnbool);

        }

        colit++;
    }
}


void  hk_dsgrid::savedata(ostream& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::savedata");
#endif
    savedata(s,true);
}


void  hk_dsgrid::savedata(ostream& s,bool saveall)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::savedata(ostream& s,bool saveall)");
#endif
    hk_string ftag="HK_DSGRID";
    start_mastertag(s,ftag);
    bool dstype=false;
    if (datasource()!=NULL &&saveall) dstype=(datasource()->type()!=hk_datasource::ds_query);
    hk_dsvisible::savedata(s,p_presentation==NULL,dstype);
    set_tagvalue(s,"ROWHEIGHT",p_rowheight);
    set_tagvalue(s,"AUTOMATIC_COLUMNS",p_automatic_columns);
    if (!p_automatic_columns)
    {
        hk_string ctag="COLUMNDEFINITIONS";
        start_mastertag(s,ctag);
        set_tagvalue(s,"COLUMNSCOUNT",(long unsigned int)p_cols.size());
        vector<hk_dsgridcolumn*>::iterator it=p_cols.begin();
        while (it!=p_cols.end())
        {
            (*it)->savedata(s);
            it++;
        }

        end_mastertag(s,ctag);

    }
    end_mastertag(s,ftag);
}


void hk_dsgrid::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::loaddata");
#endif
    xmlNodePtr d=get_tagvalue(definition,"HK_DSVISIBLE");
    if (d)d=d->xmlChildrenNode;

    hk_dsvisible::loaddata(d);
    if (get_tagvalue(definition,"ROWHEIGHT",p_rowheight))
       {
          set_rowheight(p_rowheight,false);
       }
    get_tagvalue(definition,"AUTOMATIC_COLUMNS",p_automatic_columns);
    if (!p_automatic_columns)
    {
#ifdef HK_DEBUG
        hkdebug("hk_dsgrid::loaddata p_automatic_columns==FALSE");
#endif
        long unsigned int c=0;
        xmlNodePtr cols=get_tagvalue(definition,"COLUMNDEFINITIONS");
        if (cols) cols=cols->xmlChildrenNode;
	get_tagvalue(cols,"COLUMNSCOUNT",c);
        resize_cols(c);
        xmlNodePtr st=NULL;
        hk_string xxx;
        unsigned int i=0;  int v=1;
        while ((st=(get_tagvalue(cols,"HK_DSGRIDCOLUMN",xxx,v,mastertag)))&&i<c)
        {  
#ifdef HK_DEBUG
            hkdebug("hk_dsgrid::loaddata another column");
#endif
            p_cols[i]->set_grid(this);
            p_cols[i]->set_datasource(datasource());
            p_cols[i]->loaddata(st->xmlChildrenNode);
            i++;v++;
        }
    }
}


hk_dsgridcolumn* hk_dsgrid::gridcolumn(unsigned int  c)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::gridcolumn: ",(int)c);
    hkdebug("p_numcols=",(int)p_cols.size());
#endif

    if (c<p_cols.size())
    {
        return (p_cols[c]);
    }
    else
    {
        return NULL;
    }
}


hk_dsgridcolumn* hk_dsgrid::gridcolumn(const hk_string&  c)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::gridcolumn: ",c);
    hkdebug("p_numcols=",(int)p_cols.size());
#endif

    for (unsigned int a=0;a<p_cols.size();++a)
    {
        if (p_cols[a]&&p_cols[a]->identifier()==c)
        return (p_cols[a]);
    }
        return NULL;
}


bool hk_dsgrid::datasource_disable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::datasource_disable");
#endif

    if (p_automatic_columns )
    {
        clear_cols();
    }
    hk_dsvisible::datasource_disable();

    return true;
}


void hk_dsgrid::clear_cols(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::clear_cols");
#endif
    vector<hk_dsgridcolumn*>::iterator it=p_cols.begin();
    while (it!=p_cols.end())
    {
        hk_dsgridcolumn* c=(*it);
        ++it;
        if (c) delete c;
    }
    p_cols.erase(p_cols.begin(),p_cols.end());
}


bool hk_dsgrid::datasource_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::datasource_enable");
#endif
    hk_dsvisible::datasource_enable();
    //columns_new_created();
    return true;
}


void hk_dsgrid::set_gridcolumnwidth(unsigned int col,int newwidth)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::set_gridcolumnwidth");
#endif

    if (((unsigned)col <p_cols.size()))
    {
        p_cols[col]->set_columnwidth(newwidth);
        if (p_holdrowdefinition)p_automatic_columns=false;
    }

}


void hk_dsgrid::change_columnposition(unsigned int fromp, unsigned int top)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::change_columnposition");
#endif

    hk_dsgridcolumn* buffer;

    if ((fromp==top))return;
    if ((fromp<p_cols.size())&&(top<=p_cols.size()))
    {

        if (fromp<top)
        {                                         //from<to von links nach rechts
            buffer=p_cols[fromp];
            for(unsigned int k=fromp;k<top-1;k++)
                p_cols[k]=p_cols[k+1];

            p_cols[top-1]=buffer;
        }
        else
        {                                         //from>to  = von rechts nach links

            buffer=p_cols[fromp];
            for(unsigned int k=fromp;k>top;k--)
                p_cols[k]=p_cols[k-1];

            p_cols[top]=buffer;

        }

    }
    if (p_holdrowdefinition) p_automatic_columns=false;

}


void hk_dsgrid::clear_gridcolumn(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid:clear_gridcolumn");
#endif
    clear_cols();
    p_automatic_columns=true;
}


bool hk_dsgrid::table_structure_changes(void)
{

    if (!p_holdrowdefinition)return true;

    columns_new_created();
    return true;
}


void hk_dsgrid::set_gridcolumns(vector<hk_dsgridcolumn>& c,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::set_gridcolumns");
#endif
    clear_gridcolumn();
//  p_cols=c;
    resize_cols(c.size());
    for (unsigned int k=0;k<c.size();k++)
    {
        p_cols[k]->set_columnname(c[k].columnname());
        p_cols[k]->set_datasource(datasource());
        p_cols[k]->set_grid(this);
        p_cols[k]->set_columntype(c[k].columntype());
        p_cols[k]->set_displayname(c[k].displayname());
    }
    if (p_holdrowdefinition) p_automatic_columns=false;
    columns_new_created();
    has_changed(registerchange);
}


void hk_dsgrid::before_store_changed_data(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::before_store_changed_data");
#endif
    before_row_change();
}


void hk_dsgrid::set_hold_rowdefinition(bool h)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::set_hold_rowdefinition");
#endif
    p_holdrowdefinition=h;
}


void hk_dsgrid::set_enablingbehaviour(bool add_col,bool del_col)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::set_enablingbehaviour");
#endif
    p_add_new_columns=add_col;
    p_delete_nonexisting_columns=del_col;

}


unsigned int hk_dsgrid::columnscount()
{
    return p_cols.size();
}


void hk_dsgrid::set_datasource(hk_datasource* d)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::set_datasource");
#endif
    hk_dsvisible::set_datasource(d);
    vector<hk_dsgridcolumn*>::iterator it= p_cols.begin();
    while (it!=p_cols.end())
    {
        (*it)->set_datasource(d);
        it++;
    }
    if (d && d->is_enabled())
    {
     columns_new_created();
    }

}


void hk_dsgrid::resize_cols(int new_size)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::resize_cols");
#endif
    int old_size =p_cols.size();
    if (new_size < old_size)
    {
        for (int k=old_size-1;k>new_size;k--)
        {
            delete p_cols[k];
        }
        p_cols.resize(new_size);
    }
    else
    if (new_size > old_size)
    {
        p_cols.resize(new_size);
        for (int k=old_size;k<new_size;k++)
        {
            p_cols[k]=new hk_dsgridcolumn();
        }

    }

}


void hk_dsgrid::set_gridcolumns(list<hk_string>&sl)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::set_gridcolumns(list<hk_string>&");
#endif
    vector<hk_dsgridcolumn*> newcols;
    newcols.resize(sl.size());
    for (unsigned int k=0;k<newcols.size();k++)
    {
        newcols[k]=new hk_dsgridcolumn();
        newcols[k]->set_grid(this);
        newcols[k]->set_datasource(datasource());
    }

    list<hk_string>::iterator it=sl.begin();
    int k=0;
    while(it!=sl.end())
    {
//set columnname
        newcols[k]->set_columnname((*it));

//search for already existing definition
        gridcolumn_exists::searchvalue=(*it);
        vector<hk_dsgridcolumn*>::iterator where=find_if(p_cols.begin(),p_cols.end(),gridcolumn_exists());
        if (where!=p_cols.end())
        {
            newcols[k]->set_displayname((*where)->displayname());
            newcols[k]->set_columntype((*where)->columntype());
            newcols[k]->set_columnwidth((*where)->columnwidth());
	    newcols[k]->set_on_click_action((*where)->on_click_action(),true,true);
	    newcols[k]->set_on_doubleclick_action((*where)->on_doubleclick_action(),true,true);
	    newcols[k]->set_on_open_action((*where)->on_open_action(),true,true);
	    newcols[k]->set_on_close_action((*where)->on_close_action(),true,true);
	    newcols[k]->set_before_row_change_action((*where)->before_row_change_action(),true,true);
	    newcols[k]->set_after_row_change_action((*where)->after_row_change_action(),true,true);
	    newcols[k]->set_before_delete_action((*where)->before_delete_action(),true,true);
	    newcols[k]->set_after_delete_action((*where)->after_delete_action(),true,true);
	    newcols[k]->set_before_update_action((*where)->before_update_action(),true,true);
	    newcols[k]->set_after_update_action((*where)->after_update_action(),true,true);
	    newcols[k]->set_before_insert_action((*where)->before_insert_action(),true,true);
	    newcols[k]->set_after_insert_action((*where)->after_insert_action(),true,true);

    	    newcols[k]->set_on_getfocus_action((*where)->on_getfocus_action(),true,true);
	    newcols[k]->set_on_loosefocus_action((*where)->on_loosefocus_action(),true,true);

	    if ((*where)->use_defaultvalue())
                newcols[k]->set_defaultvalue((*where)->defaultvalue());
        }
        it++;k++;
    }
//now store the whole chose
    clear_cols();
    p_cols.resize(newcols.size());
    for (unsigned int kl=0;kl<newcols.size();kl++)
    {
        delete p_cols[kl];
	p_cols[kl]=newcols[kl];
    }

    if (p_holdrowdefinition)p_automatic_columns=false;
columns_new_created();
}


list <hk_string>* hk_dsgrid::visible_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::visible_columns");
#endif
    p_visiblecols.erase(p_visiblecols.begin(),p_visiblecols.end());
    for (unsigned int k=0;k<p_cols.size();k++)
    {
        p_visiblecols.insert(p_visiblecols.end(),p_cols[k]->columnname());

    }

    return &p_visiblecols;
}


list <hk_string>* hk_dsgrid::nonvisible_columns(void)
{
    p_nonvisiblecols.erase(p_nonvisiblecols.begin(),p_nonvisiblecols.end());
    if (datasource()!=NULL)
    {
        list <hk_column*>* dscols=datasource()->columns();
        if (dscols!=NULL)
        {
            list<hk_column*>::iterator it=dscols->begin();
            while (it!=dscols->end())
            {
                gridcolumn_exists::searchvalue=(*it)->name();
                vector<hk_dsgridcolumn*>::iterator where=find_if(p_cols.begin(),p_cols.end(),gridcolumn_exists());
                if (where==p_cols.end())
                {
                    p_nonvisiblecols.insert(p_nonvisiblecols.end(),(*it)->name());
                }
                it++;
            }
        }
        else
        {
           list <hk_string>* dscolnm=datasource()->columnnames();
            list<hk_string>::iterator it=dscolnm->begin();
            while (it!=dscolnm->end())
            {
                gridcolumn_exists::searchvalue=(*it);
                vector<hk_dsgridcolumn*>::iterator where=find_if(p_cols.begin(),p_cols.end(),gridcolumn_exists());
                if (where==p_cols.end())
                {
                    p_nonvisiblecols.insert(p_nonvisiblecols.end(),(*it));
                }
                it++;
            }
        }
    }
    return &p_nonvisiblecols;
}




bool hk_dsgrid::save_table(const hk_string& n,bool ask)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::save_table");
#endif

    if (datasource()==NULL) return false;
    if (n.size()>0) datasource()->set_name(n);
    if (datasource()->name().size()==0) if (!(datasource()->ask_name()))return false;
    ofstream* p_save=datasource()->database()->savestream(datasource()->name(),ft_table,ask);
    if (p_save==NULL) return false;
    start_mastertag(*p_save,"HK_TABLE");
    savedata(*p_save,false);
    end_mastertag(*p_save,"HK_TABLE");
    p_save->close();
    delete p_save;
    return true;

}


bool hk_dsgrid::load_table(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::load_table");
#endif
    if (datasource()==NULL) return false;
    xmlNodePtr res=datasource()->database()->xmlload(datasource()->name(),ft_table);
    res=get_tagvalue(res,"HK_TABLE");
    if (!res) return false;
    res=res->xmlChildrenNode;
    if (!p_presentation)datasource()->set_automatic_data_update(datasource()->database()->is_automatic_data_update());
    if (!res)return false;

    xmlNodePtr g;
    if ((g=get_tagvalue(res,"HK_DSGRID")))
      {
        g=g->xmlChildrenNode;
        loaddata(g);
      }
    set_font(hk_font::defaultfontname(),hk_font::defaultfontsize());

    return true;

}


bool hk_dsgrid::save_view(const hk_string& n,bool ask)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::save_view");
#endif

    if (datasource()==NULL) return false;
    if (n.size()>0) datasource()->set_name(n);
    if (datasource()->name().size()==0) if (!(datasource()->ask_name()))return false;
    ofstream* p_save=datasource()->database()->savestream(datasource()->name(),ft_view,ask);
    if (p_save==NULL) return false;
    start_mastertag(*p_save,"HK_VIEW");
    savedata(*p_save,false);
    end_mastertag(*p_save,"HK_VIEW");
    p_save->close();
    delete p_save;
    return true;

}


bool hk_dsgrid::load_view(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgrid::load_VIEW");
#endif
    if (datasource()==NULL) return false;
    xmlNodePtr res=datasource()->database()->xmlload(datasource()->name(),ft_table);
    if (!res)return false;

    xmlNodePtr g;

    if ((g=get_tagvalue(res,"HK_DSGRID")))
        loaddata(g);
    set_font(hk_font::defaultfontname(),hk_font::defaultfontsize());

    return true;

}


void hk_dsgrid::set_rowheight(int newheight,bool registerchange)
{
p_rowheight=newheight; //cerr<<"hk_dsgrid::set_rowheight: "<<newheight<<endl;
widget_specific_rowheight_changes();
has_changed(registerchange);
}

int hk_dsgrid::rowheight(void)
{
return p_rowheight;
}


