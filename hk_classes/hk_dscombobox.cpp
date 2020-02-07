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
//$Revision: 1.39 $
#include "hk_dscombobox.h"
#include "hk_datasource.h"
#include "hk_dsgridcolumn.h"
#include "hk_presentation.h"
#include "hk_form.h"
#include "hk_dsdatavisible.h"

class hk_dscomboboxprivate
{
public:
};


class hk_dscomboboxmodeprivate
{
 public:
 hk_dscomboboxmodeprivate()
 {
 p_use_textlist=false;
 };

list <hk_string> p_textlist;
bool p_use_textlist;
hk_string p_onselectaction;
bool operator=(const hk_dscomboboxmodeprivate& v)
   {
     p_textlist.clear();
     list<hk_string>::const_iterator it=v.p_textlist.begin();
     while (it!=v.p_textlist.end())
      {
        p_textlist.push_back(*it);
        ++it;
      }
     p_use_textlist=v.p_use_textlist;
     p_onselectaction=v.p_onselectaction;
     return true;
   }
};


class hk_listvisible :public hk_dsdatavisible
{
    public:
        hk_listvisible(hk_dscombobox* combo,hk_form* ):hk_dsdatavisible()
        {
#ifdef HK_DEBUG
//wanna_debug(true);
            hkdebug("hk_listvisible::hk_listvisible");
#endif
            p_viewcolumn=NULL;
            p_combo=combo;
        }
        ~hk_listvisible()
        {
#ifdef HK_DEBUG

            hkdebug("hk_listvisible::~hk_listvisible");
#endif

        }
        void set_viewcolumnname(const hk_string& n)
        {
#ifdef HK_DEBUG

            hkdebug("hk_listvisible::set_viewcolumnname");
#endif
            p_viewcolumnname=n;
            hk_datasource*d =datasource();
            if (d!=NULL) p_viewcolumn=d->column_by_name(n);
        }

        hk_string viewcolumnname(void){return p_viewcolumnname;}

        hk_column* viewcolumn(void){return p_viewcolumn;}

        virtual void set_datasource(hk_datasource* d)
        {
#ifdef HK_DEBUG

            hkdebug("hk_listvisible::set_datasource");
#endif
            hk_dsdatavisible::set_datasource(d);
            if (d==NULL) p_viewcolumn=NULL;
        }

        virtual bool        before_columns_deleted(void)
        {
#ifdef HK_DEBUG
            hkdebug("hk_listvisible::before_columns_deleted");
#endif
            hk_dsdatavisible::before_columns_deleted();
            p_viewcolumn=NULL;
            return true;
        }

        virtual void        before_source_vanishes(void)
        {
#ifdef HK_DEBUG
            hkdebug("hk_listvisible::before_source_vanishes");
#endif
            hk_dsdatavisible::before_source_vanishes();
            p_viewcolumn=NULL;
        }

        virtual bool datasource_enable(void)
        {
#ifdef HK_DEBUG
            hkdebug("hk_listvisible::datasource_enable");
#endif
            if (datasource()==NULL) return false;
            bool res=hk_dsdatavisible::datasource_enable();
            p_viewcolumn=datasource()->column_by_name(p_viewcolumnname);
            if (p_viewcolumn==NULL)p_viewcolumn=column();
            if (p_column==NULL)p_column=p_viewcolumn;
            if ((p_column==NULL)||(p_viewcolumn==NULL))return res;
            if (p_combo!=NULL) p_combo->load_listitems();
            return res;
        }
	virtual bool datasource_disable(void)
	{
	  p_viewcolumn=NULL;
	  p_column=NULL;
	  return true;
	}

        virtual void after_store_changed_data(void)
        {
            if (p_combo &&p_combo->mode()==hk_dscombobox::combo ||
	        p_combo &&p_combo->mode()==hk_dscombobox::combo_noedit
	       )
                hk_dsdatavisible::after_store_changed_data();
        }

    private:

        hk_column* p_viewcolumn;
        hk_string p_viewcolumnname;
        hk_dscombobox* p_combo;

};

//************************************************************************
//************************************************************************

hk_dscombobox::hk_dscombobox(hk_form* f):hk_dsdatavisible(f)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_dscombobox::hk_dscombobox");
#endif
    p_private=new hk_dscomboboxprivate;
    p_designdata=new hk_dscomboboxmodeprivate;
    p_viewdata=new hk_dscomboboxmodeprivate;
    p_visibletype=combobox;
    p_listvisible= new hk_listvisible(this,f);
    p_listdatasource=-1;
    p_mode=combo;
    p_gridcolumn=NULL;
}


hk_dscombobox::~hk_dscombobox(void)
{
    delete p_listvisible;
    delete p_viewdata;
    delete p_designdata;
    delete p_private;

}


void hk_dscombobox::set_listdatasource(hk_datasource* list)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::set_listdatasource");
#endif
    if (p_mode==selector) return;
    if (list==datasource()&& datasource()!=NULL&&(p_mode==combo||p_mode==combo_noedit))
    {

        show_warningmessage(hk_translate("Error: listdatasource and datasource in the combobox are identical!"));
        return ;
    }
    p_listvisible->set_datasource(list);
    if (datasource()&&list)
    {
        list->set_enabled(datasource()->is_enabled());
    }
    create_filternames();
}


void hk_dscombobox::set_datasource(hk_datasource* d)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::set_datasource");
#endif
    if ((p_mode==combo||p_mode==combo_noedit)&&d==p_listvisible->datasource()&& p_listvisible->datasource()!=NULL)
    {

        show_warningmessage(hk_translate("Error: listdatasource and datasource in the combobox are identical!"));
        return ;
    }
    hk_dsdatavisible::set_datasource(d);
    if (p_mode==selector)
    {
        p_listvisible->set_datasource(d);
    }

    if (d && d->is_enabled()) datasource_enable();
    else datasource_disable();

}


hk_datasource* hk_dscombobox::listdatasource(void)
{
    return p_listvisible->datasource();

}


void hk_dscombobox::set_listcolumnname(const hk_string& listcolumnnm,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::set_listcolumnname");
#endif

    p_listvisible->set_columnname(listcolumnnm);

    has_changed(registerchange);
}


hk_string hk_dscombobox::listcolumnname(void)
{
    return p_listvisible->columnname();
}


void hk_dscombobox::set_viewcolumnname(const hk_string& viewcolumnnm,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::set_viewcolumnname");
#endif
    p_listvisible->set_viewcolumnname(viewcolumnnm);
    has_changed(registerchange);
    create_filternames();
}


void hk_dscombobox::create_filternames()
{
    if (viewcolumnname().size()>0 && listdatasource())
    {
    p_filterreplacevalue="\""+columnname()+"\" IN (SELECT \""+listcolumnname()+"\" FROM \""+listdatasource()->name()+"\" WHERE \""+viewcolumnname()+"\" ";
    p_filtervalueend=") ";
    }
    else
    {
    p_filterreplacevalue="\""+columnname()+"\" ";
    p_filtervalueend="";
    }

// cerr <<"create_filternames= repl#"<<p_filterreplacevalue<<"# end:"<<p_filtervalueend<<endl;
}

hk_string hk_dscombobox::viewcolumnname(void)
{
    return p_listvisible->viewcolumnname();
}


void hk_dscombobox::savedata(ostream& s )
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::savedata");
#endif
    hk_string tag="HK_COMBOBOX";
    start_mastertag(s,tag);
    hk_dsdatavisible::savedata(s);
    set_tagvalue(s,"VIEWCOLUMNNAME",p_listvisible->viewcolumnname());
    set_tagvalue(s,"LISTCOLUMNNAME",p_listvisible->columnname());
    set_tagvalue(s,"LISTPRESENTATIONDATASOURCE",p_listdatasource);
    hk_string m=p_mode==selector?"SELECTOR":(p_mode==combo?"COMBO":"COMBO_NOEDIT");
    set_tagvalue(s,"COMBOBOXMODE",m);
    hk_string txttag="TEXTLIST";
    start_mastertag(s,txttag);
    set_tagvalue(s,"USE_TEXTLIST",p_designdata->p_use_textlist);

    list<hk_string>::iterator it=p_designdata->p_textlist.begin();

    while (it!=p_designdata->p_textlist.end())
    {
    set_tagvalue(s,"LISTELEMENT",(*it));
      ++it;
    }
    end_mastertag(s,txttag);
    set_tagvalue(s,"ONSELECT_ACTION",p_designdata->p_onselectaction);
    end_mastertag(s,tag);
}


void hk_dscombobox::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::loaddata");
#endif

    hk_string name;
    long int number;
    hk_dsdatavisible::loaddata(definition);
    if (get_tagvalue(definition,"VIEWCOLUMNNAME",name)) set_viewcolumnname(name);
    if (get_tagvalue(definition,"LISTCOLUMNNAME",name)) set_listcolumnname(name);
    if (get_tagvalue(definition,"COMBOBOXMODE",name))
         set_mode( (name=="SELECTOR")?selector:
	            (name=="COMBO"?combo:combo_noedit));
    if (get_tagvalue(definition,"LISTPRESENTATIONDATASOURCE",number)) set_listpresentationdatasource(number);

    get_tagvalue(definition,"USE_TEXTLIST",p_designdata->p_use_textlist);


        int i=1;
        p_designdata->p_textlist.clear();
	hk_string valuestring;
        while (get_tagvalue(definition,"LISTELEMENT",valuestring,i))
        {
            p_designdata->p_textlist.push_back(valuestring);
            ++i;
        }
    get_tagvalue(definition,"ONSELECT_ACTION",p_designdata->p_onselectaction);

    *p_viewdata=*p_designdata;

}


bool hk_dscombobox::set_listpresentationdatasource(long n, bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::set_listpresentationdatasource");
#endif
    if (p_presentation==NULL)
    {
#ifdef HK_DEBUG
        hkdebug("hk_dscombobox::listpresentationdatasource presentation==NULL");
#endif

        return false;
    }
    if (n==presentationdatasource()&& presentationdatasource()!=-1 &&(p_mode==combo||p_mode==combo_noedit))
    {

        show_warningmessage(hk_translate("Error: listdatasource and datasource in the combobox are identical!"));
        return false;
    }
    if (p_mode==combo||p_mode==combo_noedit)
        p_listvisible->set_datasource(p_presentation->get_datasource(n));
    p_listdatasource=n;
    has_changed(registerchange);

    return true;

}


long hk_dscombobox::listpresentationdatasource(void)
{
    return p_listvisible->presentationdatasource();
}


hk_column* hk_dscombobox::viewcolumn(void)
{
    return p_listvisible->viewcolumn();
}


hk_column* hk_dscombobox::listcolumn(void)
{
    return p_listvisible->column();
}


bool hk_dscombobox::datasource_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::datasource_enable");
#endif
    bool res=hk_dsdatavisible::datasource_enable();
    create_filternames();
    if (p_listvisible->datasource()!=NULL &&!p_listvisible->datasource()->is_enabled() )p_listvisible->datasource()->enable();
    else  load_listitems();
    return res;
}


bool hk_dscombobox::datasource_disable(void)
{
    bool res=hk_dsdatavisible::datasource_disable();
    if (p_listvisible->datasource()!=NULL&& (p_mode==combo||p_mode==combo_noedit))p_listvisible->datasource()->disable();
    return res;
}


bool hk_dscombobox::widget_specific_row_change(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::widget_specific_row_change");
#endif
    hk_dsdatavisible::widget_specific_row_change();
    if (p_mode==combo||p_mode==combo_noedit) load_listitems();
    return true;
}


void hk_dscombobox::columndata_has_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::columndata_has_changed");
#endif
//	load_listitems();

}


void hk_dscombobox::after_store_changed_data(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::after_store_changed_data");
#endif

    load_listitems();
}


hk_string hk_dscombobox::value_at(unsigned long at)
{
#ifdef HK_DEBUG
    hkdebug("hk_dscombobox::value_at");
#endif
    if (datasource()==NULL|| column()==NULL) return "";
     //bool is_null=false;
    if((listcolumn()==NULL)||(listdatasource()==NULL)||(viewcolumn()==NULL))
    {
        if (column()!=NULL)
        {
            if (column()->has_changed() &&datasource()->row_position()==at)
                return (!column()->changed_data_is_nullvalue() && is_numerictype(column())?
                    format_number(column()->changed_data_asstring(),true,use_numberseparator(),commadigits(),locale())
                    :column()->changed_data_asstring());
            return (!column()->is_nullvalue_at(at)&&is_numerictype(column())?
                format_number(column()->asstring_at(at,true),true,use_numberseparator(),commadigits(),locale())
                :column()->asstring_at(at,true));
        }
        else
            return "";

    }
    unsigned int r=0;
    if ((column()->has_changed() &&datasource()->row_position()==at)
        || (column()->has_changed() &&at>=datasource()->max_rows()))
        r=listcolumn()->find(column()->changed_data_asstring(true),true);
    else r=listcolumn()->find(column()->asstring_at(at,true),true);
    if (r<=listdatasource()->max_rows())
    {
        if (is_numerictype(viewcolumn()))
            return format_number(viewcolumn()->asstring_at(r,true),true,use_numberseparator(),commadigits(),locale());
        else
            return viewcolumn()->asstring_at(r,true);
    }
    else
        return "";

}


void hk_dscombobox::set_mode(enum_mode m)
{
    if (p_mode==m) return;
    p_mode=m;
    if (p_mode==selector)
    {
        set_listpresentationdatasource(presentationdatasource());
        p_listvisible->set_datasource(datasource());

    }
    else
    {
        p_listvisible->set_datasource(NULL);
        p_listdatasource=-1;
    }

    widget_specific_mode_change();

}


hk_dscombobox::enum_mode hk_dscombobox::mode(void)
{
    return p_mode;

}



void hk_dscombobox::set_gridcolumn(hk_dsgridcolumn* c)
{
  p_gridcolumn= c;
}

hk_dsgridcolumn* hk_dscombobox::gridcolumn (void) const
{
return p_gridcolumn;
}


void hk_dscombobox::set_use_textlist(bool u, bool registerchange)
{
if (p_presentation && p_presentation->mode()==hk_presentation::designmode)
  p_designdata->p_use_textlist=u;

p_viewdata->p_use_textlist=u;
has_changed(registerchange);
}

bool hk_dscombobox::use_textlist(void) const
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return  p_viewdata->p_use_textlist;
    return p_designdata->p_use_textlist;

}


void hk_dscombobox::set_textlist(list<hk_string>& l, bool registerchange)
{
p_viewdata->p_textlist.clear();

list<hk_string>::iterator it=l.begin();
if (p_presentation && p_presentation->mode()==hk_presentation::designmode)
{
p_designdata->p_textlist.clear();
}
while (it!=l.end())
{
  p_viewdata->p_textlist.push_back(*it);
  if (p_presentation && p_presentation->mode()==hk_presentation::designmode)
  {
     p_designdata->p_textlist.push_back(*it);
  }
++it;
}

has_changed(registerchange);
}



list<hk_string>* hk_dscombobox::textlist(void) const
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
{
  return  &p_viewdata->p_textlist;
}
    return &p_designdata->p_textlist;

}



void hk_dscombobox::add_text(const hk_string& t, bool registerchange)
{
if (p_presentation && p_presentation->mode()==hk_presentation::designmode)
  {
    p_designdata->p_textlist.push_back(t);
  }
 p_viewdata->p_textlist.push_back(t);
 has_changed(registerchange);
}




bool     hk_dscombobox::presentationmode_changed(void)
{
  if (p_presentation->mode()==hk_presentation::viewmode )
      *p_viewdata=*p_designdata;
  return hk_dsdatavisible::presentationmode_changed();

}


void hk_dscombobox::set_on_select_action(const hk_string&f,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_onselectaction=f;
    }
    p_viewdata->p_onselectaction=f;
    has_changed(registerchange);

}

hk_string hk_dscombobox::on_select_action(void)
{

if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_onselectaction;
return p_designdata->p_onselectaction;

}

bool hk_dscombobox::action_on_select(void)
{
if (p_gridcolumn?p_gridcolumn->on_select_action().size()==0:on_select_action().size()==0) return true;
 if (!p_presentation)return false;

if (p_gridcolumn)
  return p_presentation->interpreter()->on_select(p_gridcolumn);
else
return p_presentation->interpreter()->on_select(this);
}



