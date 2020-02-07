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
//$Revision: 1.41 $
#include "hk_dsvisible.h"
#include "hk_database.h"
#include "hk_presentation.h"
//#include "hk_form.h"
#include "hk_datasource.h"
#include "hk_interpreter.h"

class hk_dsvisiblemodeprivate
{
public:
    hk_dsvisiblemodeprivate(){p_presentationdatasource=-1;}
        long        p_presentationdatasource;
	hk_string p_before_row_change_action;
	hk_string p_after_row_change_action;
	hk_string p_after_delete_action,p_before_delete_action;
	hk_string p_after_update_action,p_before_update_action;
	hk_string p_after_insert_action,p_before_insert_action;
bool operator=(const hk_dsvisiblemodeprivate* v)
	{
		if (!v) return false;
		p_presentationdatasource=v->p_presentationdatasource;
	p_before_row_change_action=v->p_before_row_change_action;
	p_after_row_change_action=v->p_after_row_change_action;
	p_after_delete_action=v->p_after_delete_action;
	p_before_delete_action=v->p_before_delete_action;
	p_after_update_action=v->p_after_update_action;
	p_before_update_action=v->p_before_update_action;
	p_after_insert_action=v->p_after_insert_action;
	p_before_insert_action=v->p_before_insert_action;
	}
};

class hk_dsvisibleprivate
{
public:
   hk_dsvisibleprivate(){ }
        bool        internal_dsrefresh;
        mutable bool        p_readonly;
        int tag_dsname;

};

hk_dsvisible::hk_dsvisible(hk_presentation* p):hk_visible(p)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsvisible::constructor");
#endif
    p_datasource=NULL;

#ifdef HK_DEBUG
    hkdebug("hk_dsvisible::constructor",p_presentation==NULL?"presentation==NULL":"presentation!=NULL");
#endif
p_designdata=new hk_dsvisiblemodeprivate;
p_viewdata=new hk_dsvisiblemodeprivate;
p_private=new hk_dsvisibleprivate;
    p_private->p_readonly=false;
    widget_specific_enable_disable();
    p_private->internal_dsrefresh=false;
    p_already_handled=false;
   p_private->tag_dsname=register_tag("DATASOURCENAME");

}


hk_dsvisible::~hk_dsvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsvisible::destructor");
#endif
    if (p_datasource!=NULL)p_datasource->visible_remove(this);
    p_datasource=NULL;
//    else  cout <<"datasource==NULL"<<endl;
delete p_designdata;
delete p_viewdata;
delete p_private;
}


void hk_dsvisible::set_datasource(hk_datasource* d)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsvisible::set_datasource");
#endif
    if (p_presentation!=NULL)if (!p_private->internal_dsrefresh)return;
#ifdef HK_DEBUG
    hkdebug("hk_dsvisible::datasource Datenquelle anfgen");
#endif
    if (p_datasource!=NULL)
    {
        p_datasource->visible_remove(this);
        p_datasource=NULL;
    }
    p_datasource=d;
    if (d!=NULL)
    {
        p_datasource->visible_add(this);
        if (p_datasource->is_enabled())datasource_enable();
#ifdef HK_DEBUG
        hkdebug("datasource add");
#endif
    }
    widget_specific_enable_disable();
}


hk_datasource* hk_dsvisible::datasource(void)
{
    return p_datasource;
}


bool hk_dsvisible::set_presentationdatasource(long n,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsvisible::set_presentationdatasource");
#endif
    if (p_presentation==NULL)
    {
#ifdef HK_DEBUG
        hkdebug("hk_dsvisible::presentationdatasource presentation==NULL");
#endif

        return false;
    }
    widget_specific_presentationdatasource(n);
    p_private->internal_dsrefresh=true;
    p_designdata->p_presentationdatasource=n;
    hk_datasource* d=p_presentation->get_datasource(n);
    set_datasource(d);
    p_private->internal_dsrefresh=false;
    has_changed(registerchange);

    return true;

}


long    hk_dsvisible::presentationdatasource(void)
{

    return p_designdata->p_presentationdatasource;
}


void hk_dsvisible::row_change()
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::row_change");
#endif

    widget_specific_row_change();

}

void hk_dsvisible::batch_next()
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::batch_next");
#endif

    widget_specific_batch_next();

}

void hk_dsvisible::batch_previous()
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::batch_previous");
#endif

    widget_specific_batch_previous();

}

void hk_dsvisible::action_before_row_change(void)
{
if (!p_presentation ||before_row_change_action().size()==0) return;
	if(!p_presentation->interpreter()->scripterror())p_presentation->interpreter()->before_row_change(this);
}



void hk_dsvisible::action_after_row_change(void)
{
if (	p_presentation
	&& after_row_change_action().size()>0
	&&!p_presentation->interpreter()->scripterror()
   ) p_presentation->interpreter()->after_row_change(this);

}

void hk_dsvisible::action_before_insert(void)
{
if (	p_presentation
	&& before_insert_action().size()>0
	&&!p_presentation->interpreter()->scripterror()
   ) p_presentation->interpreter()->before_insert(this);

}

void hk_dsvisible::action_after_insert(void)
{
if (	p_presentation
	&& after_insert_action().size()>0
	&&!p_presentation->interpreter()->scripterror()
   ) p_presentation->interpreter()->after_insert(this);

}
void hk_dsvisible::action_before_delete(void)
{
if (	p_presentation
	&& before_delete_action().size()>0
	&&!p_presentation->interpreter()->scripterror()
   ) p_presentation->interpreter()->before_delete(this);

}

void hk_dsvisible::action_after_delete(void)
{
if (	p_presentation
	&& after_delete_action().size()>0
	&&!p_presentation->interpreter()->scripterror()
   ) p_presentation->interpreter()->after_delete(this);

}

void        hk_dsvisible::before_store_changed_data(void)
{

}


void hk_dsvisible::action_before_store_changed_data(void)
{
if (	p_presentation
	&& before_update_action().size()>0
	&&!p_presentation->interpreter()->scripterror()
   ) p_presentation->interpreter()->before_update(this);

}

void    hk_dsvisible::after_store_changed_data(void)
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::after_store_changed_data");
#endif
    widget_specific_row_change();
}

void hk_dsvisible::action_after_store_changed_data(void)
{
if (	p_presentation
	&& after_update_action().size()>0
	&&!p_presentation->interpreter()->scripterror()
   ) p_presentation->interpreter()->after_update(this);

}


void hk_dsvisible::datasource_delete()
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::datasource_delete");
#endif
    p_datasource=NULL;
}


bool hk_dsvisible::datasource_disable()
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::datasource_disable");
#endif
    widget_specific_disable();
    widget_specific_enable_disable();
    return true;
}


bool hk_dsvisible::datasource_enable()
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::datasource_enable");
#endif
    widget_specific_enable();
    widget_specific_enable_disable();
    return true;
}


void hk_dsvisible::insert_mode(void)
{
    p_date.set_now();
    widget_specific_insert_mode();
}


void hk_dsvisible::row_added()
{
    widget_specific_row_added();
}


void hk_dsvisible::row_deleted()
{
    widget_specific_row_deleted();

}


void hk_dsvisible::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::before_source_vanishes");
#endif
    hk_datasource*d =datasource();
    if (d==NULL) return;
    //if (d->is_enabled()) d->disable();
    set_datasource(NULL);

}


void    hk_dsvisible::list_changes(listtype)
{
}


void hk_dsvisible::savedata(ostream& s)
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::savedata");
#endif
    savedata(s,false,false);
}


void hk_dsvisible::savedata(ostream& s, bool savedatasource,bool saveall )
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::savedata(ostream& s, bool savedatasource,bool saveall )");
#endif
    hk_string tag="HK_DSVISIBLE";
    start_mastertag(s,tag);
    set_tagvalue(s,"PRESENTATIONDATASOURCE",p_designdata->p_presentationdatasource);
    set_tagvalue(s,"READONLY",p_private->p_readonly);
    set_tagvalue(s,"BEFORE_ROWCHANGEACTION",p_designdata->p_before_row_change_action);
    set_tagvalue(s,"AFTER_ROWCHANGEACTION",p_designdata->p_after_row_change_action);
    set_tagvalue(s,"BEFORE_UPDATEACTION",p_designdata->p_before_update_action);
    set_tagvalue(s,"AFTER_UPDATEACTION",p_designdata->p_after_update_action);
    set_tagvalue(s,"BEFORE_DELETEACTION",p_designdata->p_before_delete_action);
    set_tagvalue(s,"AFTER_DELETEACTION",p_designdata->p_after_delete_action);
    set_tagvalue(s,"BEFORE_INSERTACTION",p_designdata->p_before_insert_action);
    set_tagvalue(s,"AFTER_INSERTACTION",p_designdata->p_after_insert_action);
    if (savedatasource&& datasource()!=NULL)
        datasource()->savedata(s,saveall);

    hk_visible::savedata(s);
    end_mastertag(s,tag);

}


void hk_dsvisible::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("dsvisible::loaddata");
#endif
    get_tagvalue(definition,"PRESENTATIONDATASOURCE",p_designdata->p_presentationdatasource);
    set_presentationdatasource(p_designdata->p_presentationdatasource);
    get_tagvalue(definition,"READONLY",p_private->p_readonly);
    hk_string b;
    xmlNodePtr buffer;
    buffer=get_tagvalue(definition,"HK_VISIBLE",b);
    get_tagvalue(definition,"BEFORE_ROWCHANGEACTION",p_designdata->p_before_row_change_action);
    get_tagvalue(definition,"AFTER_ROWCHANGEACTION",p_designdata->p_after_row_change_action);
    get_tagvalue(definition,"BEFORE_UPDATEACTION",p_designdata->p_before_update_action);
    get_tagvalue(definition,"AFTER_UPDATEACTION",p_designdata->p_after_update_action);
    get_tagvalue(definition,"BEFORE_DELETEACTION",p_designdata->p_before_delete_action);
    get_tagvalue(definition,"AFTER_DELETEACTION",p_designdata->p_after_delete_action);
    get_tagvalue(definition,"BEFORE_INSERTACTION",p_designdata->p_before_insert_action);
    get_tagvalue(definition,"AFTER_INSERTACTION",p_designdata->p_after_insert_action);
    if (buffer) buffer=buffer->xmlChildrenNode;

    hk_visible::loaddata(buffer);
    *p_viewdata=*p_designdata;
    if (p_designdata->p_presentationdatasource!=-1) return;
    xmlNodePtr g;
    hk_string xxx;

    if ((g=get_tagvalue(definition,"DATASOURCE",xxx,1,mastertag)))
    {
        if (datasource()!=NULL && g)
        {
          g=g->xmlChildrenNode;
          datasource()->loaddata(g);
        }
    }

}


void    hk_dsvisible::set_readonly(bool readonly)
{
    p_private->p_readonly=readonly;

}


bool    hk_dsvisible::is_readonly(void) const
{
    return p_private->p_readonly;

}



bool     hk_dsvisible::presentationmode_changed(void)
{
     if (!p_presentation) return true;
   switch (p_presentation->mode())
   {
     	case hk_presentation::designmode :
     	case hk_presentation::filtermode :
						widget_specific_presentationdatasource(p_designdata->p_presentationdatasource);
     	case hk_presentation::viewmode :
						*p_viewdata=*p_designdata;
   }
  return hk_visible::presentationmode_changed();

}

void hk_dsvisible::before_row_change(void)
{
}


void hk_dsvisible::set_before_row_change_action(const hk_string& action, bool registerchange,bool force_setting)
{
      if (allow_datachanging(force_setting))
    {
      p_designdata->p_before_row_change_action=action;
     }
p_viewdata->p_before_row_change_action=action;
has_changed(registerchange);
}

hk_string hk_dsvisible::before_row_change_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return  p_viewdata->p_before_row_change_action;
    return p_designdata->p_before_row_change_action;
}

void hk_dsvisible::set_after_row_change_action(const hk_string& action, bool registerchange,bool force_setting)
{
      if (allow_datachanging(force_setting))
    {
      p_designdata->p_after_row_change_action=action;
     }

p_viewdata->p_after_row_change_action=action;
has_changed(registerchange);
}

hk_string hk_dsvisible::after_row_change_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return p_viewdata->p_after_row_change_action;
    return p_designdata->p_after_row_change_action;
}

void hk_dsvisible::set_before_update_action(const hk_string& action, bool registerchange,bool force_setting)
{
      if (allow_datachanging(force_setting))
    {
      p_designdata->p_before_update_action=action;
     }

p_viewdata->p_before_update_action=action;
has_changed(registerchange);

}

hk_string hk_dsvisible::before_update_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return p_viewdata->p_before_update_action;
   return p_designdata->p_before_update_action;
}

void hk_dsvisible::set_after_update_action(const hk_string& action, bool registerchange,bool force_setting)
{
      if (allow_datachanging(force_setting))
    {
      p_designdata->p_after_update_action=action;
     }
p_viewdata->p_after_update_action=action;
has_changed(registerchange);

}

hk_string hk_dsvisible::after_update_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return  p_viewdata->p_after_update_action;
  return p_designdata->p_after_update_action;
}

void hk_dsvisible::set_before_delete_action(const hk_string& action, bool registerchange,bool force_setting)
{
      if (allow_datachanging(force_setting))
    {
      p_designdata->p_before_delete_action=action;
     }
p_viewdata->p_before_delete_action=action;
has_changed(registerchange);

}

hk_string hk_dsvisible::before_delete_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return p_viewdata->p_before_delete_action;
  return p_designdata->p_before_delete_action;
}

void hk_dsvisible::set_after_delete_action(const hk_string& action, bool registerchange,bool force_setting)
{
      if (allow_datachanging(force_setting))
    {
      p_designdata->p_after_delete_action=action;
     }
p_viewdata->p_after_delete_action=action;
has_changed(registerchange);

}

hk_string hk_dsvisible::after_delete_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return  p_viewdata->p_after_delete_action;
  return p_designdata->p_after_delete_action;
}

void hk_dsvisible::set_before_insert_action(const hk_string& action, bool registerchange,bool force_setting)
{
      if (allow_datachanging(force_setting))
    {
      p_designdata->p_before_insert_action=action;
     }
p_viewdata->p_before_insert_action=action;
has_changed(registerchange);

}

hk_string hk_dsvisible::before_insert_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return p_viewdata->p_before_insert_action;
  return p_designdata->p_before_insert_action;
}

void hk_dsvisible::set_after_insert_action(const hk_string& action, bool registerchange,bool force_setting)
{
      if (allow_datachanging(force_setting))
    {
      p_designdata->p_after_insert_action=action;
     }
p_viewdata->p_after_insert_action=action;
has_changed(registerchange);

}

hk_string hk_dsvisible::after_insert_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return p_viewdata->p_after_insert_action;
  return p_designdata->p_after_insert_action;
}


hk_string hk_dsvisible::tag_value(int tagnumber, bool& ok)
{
hk_string result;
  if (tagnumber==p_private->tag_dsname)
    {
     if (datasource())result=datasource()->name();
     ok=true;
     return result;

    }
ok=false;
return hk_visible::tag_value(tagnumber,ok);
}
