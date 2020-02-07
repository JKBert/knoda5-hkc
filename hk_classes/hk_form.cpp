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
//$Revision: 1.82 $
#include "hk_form.h"
#include "hk_visible.h"
#include "hk_dsdatavisible.h"
#include "hk_dsgrid.h"
#include "hk_database.h"
#include "hk_dsvisible.h"
#include "hk_dsdate.h"
#include "hk_dsimage.h"
#include "hk_tabvisible.h"
#include "hk_dsdatavisible.h"
#include "hk_dslineedit.h"
#include "hk_dsrowselector.h"
#include "hk_dsboolean.h"
#include "hk_dscombobox.h"
#include "hk_dsmemo.h"
#include "hk_button.h"
#include "hk_label.h"
#include "hk_subform.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define VERSION UNKNOWN
#endif



class hk_formprivate
{
public:
hk_formprivate()
{
p_masterform=NULL;
p_subform=NULL;
p_while_loading=false;
}
        list<hk_visible*>   p_visibles;
	hk_form* p_masterform;
	hk_subform* p_subform;
	hk_string p_loadformname;
	bool p_while_loading;

};


class hk_formmodeprivate
{
public:

list<int> p_taborder;


bool operator=(const hk_formmodeprivate* v)
	{
		if (!v) return false;
		p_taborder=v->p_taborder;
	}

};



hk_form::hk_form(void) : hk_presentation()
{
#ifdef HK_DEBUG
    hkdebug("hk_form::hk_form");
#endif
p_designdata=new hk_formmodeprivate;
p_viewdata=new hk_formmodeprivate;
p_private=new hk_formprivate;
    p_visibletype=hk_visible::form;
    set_designsize(800,600);
    p_presentationtype=hk_presentation::form;
}


hk_form::~hk_form(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::~hk_form");
#endif
    if (mode()==viewmode) action_on_close();
    p_presentation=NULL;
    clear_visiblelist();
    delete p_designdata;
    delete p_viewdata;
    delete p_private;
#ifdef HK_DEBUG
    hkdebug("hk_form::~hk_form ENDE");
#endif
}




void hk_form::add_visible(hk_visible* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::add_visible");
#endif
    if (v==NULL) return;
    register_object(v);
    p_private->p_visibles.insert(p_private->p_visibles.end(),v);

    if (v->type()==hk_visible::textlabel) return;
    p_viewdata->p_taborder.push_back(v->p_vupn);

    if (mode()==designmode)
        p_designdata->p_taborder.push_back(v->p_vupn);

}


void hk_form::remove_visible(hk_visible* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::remove_visible");
#endif
    if (!v) return;
    p_private->p_visibles.remove(v);


    if (mode()==designmode)
    {
      p_designdata->p_taborder.remove(v->p_vupn);
    }
    p_viewdata->p_taborder.remove(v->p_vupn);

}


hk_dsgrid*  hk_form::new_grid(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_grid");
#endif
    if (mode()==viewmode)return NULL;
    hk_dsgrid* g = widget_specific_new_grid();
    if (g!=NULL)
    {
        add_visible(g);
        set_has_changed();
        g->set_presentationdatasource(presentationdatasource());
//g->set_foregroundcolour(foregroundcolour());
//g->set_backgroundcolour(backgroundcolour());
    }
    return g;
}


hk_dsrowselector*   hk_form::new_rowselector(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_rowselector");
#endif
    if (mode()==viewmode)return NULL;
    hk_dsrowselector* g = widget_specific_new_rowselector();
    if (g!=NULL)
    {
        add_visible(g);
        set_has_changed();
        g->set_presentationdatasource(presentationdatasource());
        g->set_foregroundcolour(foregroundcolour());
        g->set_backgroundcolour(backgroundcolour());
    }
    return g;
}


hk_dslineedit*  hk_form::new_lineedit(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_lineedit");
#endif
    if (mode()==viewmode)return NULL;
    hk_dslineedit* g = widget_specific_new_lineedit();
    if (g!=NULL)
    {
        add_visible(g);
        set_has_changed();
//g->set_foregroundcolour(foregroundcolour());
//g->set_backgroundcolour(backgroundcolour());
        g->set_presentationdatasource(presentationdatasource());
    }
    return g;
}


hk_subform*  hk_form::new_subform(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_subform");
#endif
    if (mode()==viewmode)return NULL;
    hk_subform* g = widget_specific_new_subform();
    if (g!=NULL)
    {
        add_visible(g);
        set_has_changed();
        g->set_presentationdatasource(presentationdatasource());
	if (g->subform())g->subform()->set_masterform(this);
    }
    return g;
}


hk_dscombobox*  hk_form::new_combobox(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_lineedit");
#endif
    if (mode()==viewmode)return NULL;
    hk_dscombobox* g = widget_specific_new_combobox();
    if (g!=NULL)
    {
        add_visible(g);
        set_has_changed();
//g->set_foregroundcolour(foregroundcolour());
//g->set_backgroundcolour(backgroundcolour());
        g->set_presentationdatasource(presentationdatasource());
    }
    return g;
}


hk_dsboolean*   hk_form::new_bool(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_bool");
#endif
    if (mode()==viewmode)return NULL;
    hk_dsboolean* g = widget_specific_new_bool();
    if (g!=NULL)
    {
        add_visible(g);
        set_has_changed();
        g->set_foregroundcolour(foregroundcolour());
        g->set_backgroundcolour(backgroundcolour());
        g->set_presentationdatasource(presentationdatasource());
    }
    return g;

}


hk_dsmemo*  hk_form::new_memo(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_memo");
#endif
    if (mode()==viewmode)return NULL;
    hk_dsmemo* g = widget_specific_new_memo();
    if (g!=NULL)
    {
        add_visible(g);
        set_has_changed();
        g->set_foregroundcolour(foregroundcolour());
        g->set_backgroundcolour(backgroundcolour());
        g->set_presentationdatasource(presentationdatasource());
    }
    return g;

}


hk_button*  hk_form::new_button(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_button");
#endif
    if (mode()==viewmode)return NULL;
    hk_button* g = widget_specific_new_button();
    if (g!=NULL)
    {
        add_visible(g);
//g->set_foregroundcolour(foregroundcolour());
//g->set_backgroundcolour(backgroundcolour());
        g->set_presentationdatasource(presentationdatasource());
        set_has_changed();
    }
    return g;

}


hk_label*   hk_form::new_label(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_label");
#endif
    if (mode()==viewmode)return NULL;
    hk_label* g = widget_specific_new_label();
    if (g!=NULL)
    {
        add_visible(g);
        set_has_changed();
        g->set_foregroundcolour(foregroundcolour());
        g->set_backgroundcolour(backgroundcolour());
    }
    return g;

}


hk_dsimage*   hk_form::new_image(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_image");
#endif
    if (mode()==viewmode)return NULL;
    hk_dsimage* g = widget_specific_new_image();
    if (g!=NULL)
    {
        add_visible(g);
        g->set_presentationdatasource(presentationdatasource());
        set_has_changed();
        g->set_foregroundcolour(foregroundcolour());
        //g->set_backgroundcolour(backgroundcolour());
    }
    return g;

}


hk_dsdate*   hk_form::new_date(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_date");
#endif
    if (mode()==viewmode)return NULL;
    hk_dsdate* g = widget_specific_new_date();
    if (g!=NULL)
    {
        add_visible(g);
        set_has_changed();
        g->set_foregroundcolour(foregroundcolour());
        g->set_backgroundcolour(backgroundcolour());
        g->set_presentationdatasource(presentationdatasource());
    }
    return g;

}


hk_tabvisible*   hk_form::new_tabvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::new_tabvisible");
#endif
    if (mode()==viewmode)return NULL;
    hk_tabvisible* g = widget_specific_new_tabvisible();
    if (g!=NULL)
    {
        add_visible(g);
        set_has_changed();
        g->set_foregroundcolour(foregroundcolour());
        g->set_backgroundcolour(backgroundcolour());
    }
    return g;

}


void        hk_form::set_designsize(unsigned int w, unsigned int h,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::set_designsize");
#endif
    hk_presentation::set_designsize(w,h,registerchange);
    list<hk_visible*>::iterator it=p_private->p_visibles.begin();
    while (it!=p_private->p_visibles.end())
    {
        (*it)->set_size((*it)->x(),(*it)->y(),(*it)->width(),(*it)->height(),false);

        it++;
    }

}


void        hk_form::widget_specific_fieldresize(hk_visible* )
{

}


bool  hk_form::save_form(const hk_string& n,bool ask)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::save_form");
#endif
    if (database()==NULL)
    {
        show_warningmessage(hk_translate("Form error: No database defined!"));
        return false;

    }
    if (n.size()>0) set_name(n);
    hk_database::enum_storage storage=hk_database::st_overwrite;
    reset_has_changed();
    if (name().size()==0) if (!ask_name())return false;
    if (p_private->p_loadformname==name()) storage=hk_database::st_storechanges;
    if (database()->storagemode(ft_form)==hk_database::local)
    {
      ofstream* p_save=database()->savestream(storage,name(),ft_form,ask,true,false);
      if (p_save==NULL) return false;
      savedata(*p_save);
      p_save->close();
      delete p_save;
    }
    else
    { // store central
       STRINGSTREAM*  p_save;
      p_save=database()->savestringstream(ft_form);
      if (p_save==NULL) return false;
      savedata(*p_save);
      hk_string s=p_save->str();
      database()->save(storage,s,name(),ft_form,ask);
      delete p_save;
    }
    p_private->p_loadformname=name();
    reset_has_changed();
    return true;
}


bool hk_form::load_form(const hk_string& n)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::load_form("+n+")");
#endif
    if (database()==NULL)
    {
        show_warningmessage(hk_translate("Form error: No database defined!"));
        return false;

    }

    if (has_changed())
    {
        save_form();
        reset_has_changed();
    }
    if (n.size()>0) set_name(n);
    enum_mode modebuffer=mode();
    hk_dsmodevisible::set_mode(designmode);
    p_private->p_while_loading=true;
    if (name().size()==0) if (!ask_name())
      {
       p_private->p_while_loading=false;
       return false;
      }
    xmlNodePtr cur =database()->xmlload(name(),ft_form);
    if (!cur)
    {
     p_private->p_while_loading=false;
     return false;
    }
    p_private->p_loadformname=n;
//     if (res.size()==0) show_warningmessage(hk_translate("Bug: form is empty!"));
//     if ( res.find("encoding=\"UTF-8\"")<res.size() ) res=u2l(res);
    loaddata(cur->xmlChildrenNode);
    p_private->p_while_loading=false;
    hk_dsmodevisible::set_mode(modebuffer);
    widget_specific_after_loadform();
    p_private->p_while_loading=false;
    reset_has_changed();

#ifdef HK_DEBUG
    hkdebug("hk_form::load_form ENDE");
#endif
return true;
}

bool hk_form::while_load_form(void) const
{
  return p_private->p_while_loading;

}

bool hk_form::while_loading(void)
{
  return while_load_form();
}

void    hk_form::savedata(ostream& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::savedata");
#endif
    hk_string ftag="FORM";
    hk_string vistag="FORMOBJECT";
    start_mastertag(s,ftag);
    set_tagvalue(s,"HK_FORMVERSION",(hk_string)VERSION);
//   hk_string datatag="FORMDATA";
//   start_mastertag(s,datatag);
//   hk_dsvisible::savedata(s);
//   end_mastertag(s,datatag);
    hk_presentation::savedata(s);

    list<hk_visible*>::iterator it=p_private->p_visibles.begin();
    while (it!=p_private->p_visibles.end())
    {
        start_mastertag(s,vistag);
        (*it)->savedata(s);
        end_mastertag(s,vistag);
        it++;
    }

    list<int>::iterator tit=p_designdata->p_taborder.begin();
    while (tit!=p_designdata->p_taborder.end())
    {
        set_tagvalue(s,"TABORDER",*tit);
      ++tit;
    }

    end_mastertag(s,ftag);
}


void    hk_form::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::loaddata");
#endif
    clear_visiblelist();
    hk_string xxx;
    hk_presentation::loaddata(definition);
    hk_string buffer;
    hk_string buffer2;
    hk_string value;
//     long unsigned int b;
    int i=1;
    xmlNodePtr node;
    while ((node=get_tagvalue(definition,"FORMOBJECT",value,i)))
    {   
        get_tagvalue(node,"VISIBLETYPE",buffer2);
        hk_visible* vis=new_object(buffer2);
        if (vis!=NULL)
        {
            vis->loaddata(node);
            register_object(vis);
        }
        i++;
    }
    i=1;
    int t;
    while (get_tagvalue(definition,"TABORDER",t,i))
    {
        if (i==1)p_designdata->p_taborder.clear();
        p_designdata->p_taborder.push_back(t);
	++i;
    }
    *p_viewdata=*p_designdata;
}



hk_visible* hk_form::new_object(const hk_string& newname)
{
    if (newname=="BUTTON") return new_button();
    else
    if (newname=="SELECTOR") return new_rowselector();
        else
        if (newname=="BOOLEAN") return new_bool();
            else
            if (newname=="LINEEDIT") return new_lineedit();
                else
                if (newname=="MEMO") return new_memo();
                    else
                    if (newname=="COMBOBOX") return new_combobox();
                        else
                        if (newname=="GRID") return new_grid();
                            else
                            if (newname=="TEXTLABEL") return new_label();
                            else
                            if (newname=="SUBFORM") return new_subform();
                            else
                            if (newname=="IMAGE") return new_image();
                            else
                            if (newname=="DATE") return new_date();
                            else
                            if (newname=="TABVISIBLE") return new_tabvisible();

                                return NULL;
}


void hk_form::clear_visiblelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::clear_visiblelist()");
#endif
    list<hk_visible*>::iterator it=p_private->p_visibles.begin();
    while (it!=p_private->p_visibles.end())
    {
        hk_visible* v=(*it);
        it++;
//    v->p_form =NULL;
        v->p_presentation = NULL;
        delete v;
    }
    p_private->p_visibles.clear();
#ifdef HK_DEBUG
    hkdebug("hk_form::clear_visiblelist() ENDE");
#endif

}


hk_visible* hk_form::get_visible(long nr)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::get_visible(long)");
#endif
    list<hk_visible*>::iterator it=p_private->p_visibles.begin();
    while (it!=p_private->p_visibles.end())
    {
        if ( (*it)->p_vupn==nr) return (*it);
        it++;
    }
    return NULL;
}

hk_visible* hk_form::get_visible(const hk_string &i)
{
#ifdef HK_DEBUG
    hkdebug("hk_form::get_visible(identifier)");
#endif
    list<hk_visible*>::iterator it=p_private->p_visibles.begin();
    while (it!=p_private->p_visibles.end())
    {
        if ( (*it)->identifier()==i) return (*it);
        it++;
    }
    return NULL;
}








void hk_form::bulk_operation(enum_bulkoperation bulkoperation)
{
    list<hk_visible*>::iterator it=p_private->p_visibles.begin();
    while (it!=p_private->p_visibles.end())
    {
        switch (bulkoperation)
        {
            case hk_presentation::bulkfont:  (*it)->set_font(font());break;
            case hk_presentation::bulkforeground:  (*it)->set_foregroundcolour(foregroundcolour());break;
            case hk_presentation::bulkbackground:
            {
                switch((*it)->type())
                {
                    case lineedit:;
                    case memo:;
                    case rowselector:;
                    case grid :break;
                    default:(*it)->set_backgroundcolour(backgroundcolour());break;
                }
            }
        }
        ++it;
    }

}


bool    hk_form::set_mode(enum_mode s)
{// cerr <<"hk_form::set_mode: "<<(s==viewmode?"viewmode":"designmode")<<endl;
    if (p_private->p_while_loading) return false;
    list<hk_visible*>::iterator it=p_private->p_visibles.begin();
	while (it!=p_private->p_visibles.end())
        {
	    if (s==viewmode&& (*it)->type()==subform)

	    {
	      hk_subform* sub=static_cast<hk_subform*>(*it);
	      sub->internal_set_datasourcevalues();
	    }
	    ++it;
        }
    if (s==viewmode)
          *p_viewdata=*p_designdata;
    
    
    if (s==viewmode)
     {
        it=p_private->p_visibles.begin();
	while (it!=p_private->p_visibles.end())
        {
	    (*it)->before_presentationmode_changed();
	    ++it;
        }
      }
    
    
    
    
    
    bool res=hk_presentation::set_mode(s);
    it=p_private->p_visibles.begin();
    if (s==viewmode)
     {

       if (!presentationmode_changed())
         {
         cerr <<"form action_on_open failed"<<endl;
	    if (!p_runtime_only)set_mode(designmode);
	   return false;
	 }
     }

	while (it!=p_private->p_visibles.end())
        {
	    if (!(*it)->presentationmode_changed())
	    {
         cerr <<"visible action_on_open failed"<<endl;
	    if (!p_runtime_only)set_mode(designmode);
	    return false;
	    }
	    ++it;
        }
    if (s==designmode)
    {
     if (!p_while_errorhandling)
       action_on_close();
     p_while_errorhandling=false;
    }
    return res;

}


void hk_form::raise_widget(hk_visible* v)
{
    p_private->p_visibles.remove(v);
    p_private->p_visibles.insert(p_private->p_visibles.end(),v);
}


void hk_form::lower_widget(hk_visible* v)
{
    p_private->p_visibles.remove(v);
    p_private->p_visibles.insert(p_private->p_visibles.begin(),v);

}


list<hk_visible*>*  hk_form::visibles(void)
{
return &(p_private->p_visibles);
}

void hk_form::sizetype_changed(void)
{
    list<hk_visible*>::iterator it=p_private->p_visibles.begin();
    while (it!=p_private->p_visibles.end())
    {
        (*it)->sizetype_changed();
        ++it;
    }
}

void hk_form::set_masterform(hk_form*f)
{
p_private->p_masterform=f;

}

hk_form* hk_form::masterform(void) const
{
  return p_private->p_masterform;
}


void hk_form::set_taborder(list<int> t,bool registerchange, bool force_setting)
{
     list<int> l;
     list<int>::iterator it=t.begin();
     while (it!=t.end())
     {
       hk_visible* v=get_visible(*it);
       if (v && v->type()!=textlabel) l.push_back(*it);
       ++it;
     }

     if (allow_datachanging(force_setting))
    {
     p_designdata->p_taborder=l;
    }
     p_viewdata->p_taborder=l;

    has_changed(registerchange);

}


list<int> hk_form::taborder() const
{
  if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return p_viewdata->p_taborder;
  else return p_designdata->p_taborder;

}

hk_visible* hk_form::taborder_previous(hk_visible* v)
{
  if (!v) return NULL;
  list<int> t= taborder();
  list<int>::reverse_iterator it=find(t.rbegin(),t.rend(),v->p_vupn);
  if (it==t.rend() && !p_private->p_visibles.empty())
     return *(p_private->p_visibles.rbegin());
 ++it;

  if (it==t.rend() && !p_private->p_visibles.empty())
  {
     cerr <<"taborder: am Ende angekommen "<<endl;
     if (is_subform())
      {
        return p_private->p_masterform->taborder_previous(this->subformobject());
      }
      else
     it=t.rbegin();
  }



  hk_visible* r=get_visible(*it);

  if (r&&r->type()==subform)
    {
      hk_subform*s=dynamic_cast<hk_subform*>(r);
      if (s && s->subform())
        r=s->subform()->last_tabobject();
    }


  if (!r && !p_private->p_visibles.empty())
     return *(p_private->p_visibles.rbegin());
  return r;
}



hk_visible* hk_form::taborder_next(hk_visible* v)
{
  if (!v) return NULL;
  list<int> t= taborder();
  list<int>::iterator it=find(t.begin(),t.end(),v->p_vupn);
  if (it==t.end() && !p_private->p_visibles.empty())
  {

     return first_tabobject();
  }
 ++it;

  if (it==t.end() && !p_private->p_visibles.empty())
  {

     if (is_subform())
      {
        return p_private->p_masterform->taborder_next(this->subformobject());
      }
      else
     it=t.begin();
  }

  hk_visible* r=get_visible(*it);
  if (r&&r->type()==subform)
    {
      hk_subform*s=dynamic_cast<hk_subform*>(r);
      if (s && s->subform())
        r=s->subform()->first_tabobject();
    }
  if (!r)return first_tabobject();
  return r;
}




hk_visible* hk_form::first_tabobject()
{
  hk_visible* result=NULL;
  list<int> t=taborder();
  if (!t.empty())
  {
    result=get_visible(*(t.begin()));
  }

  if (!result)
  {
  if ( !p_private->p_visibles.empty())
     result= *(p_private->p_visibles.begin());
  }

  return result;
}


hk_visible* hk_form::last_tabobject()
{
 hk_visible* result=NULL;
  list<int> t=taborder();
  if (!t.empty())
  {
    result=get_visible(*(t.rbegin()));
  }

  if (!result)
  {
  if (!p_private->p_visibles.empty())
     result= *(p_private->p_visibles.begin());
  }

  return result;
}



bool hk_form::is_subform() const
{
  return p_private->p_masterform!=NULL;
}

bool hk_form::is_subpresentation(void)
{
  return is_subform();
}


void hk_form::set_subformobject(hk_subform* s)
{
 p_private->p_subform=s;
}

hk_subform* hk_form::subformobject() const
{

  return p_private->p_subform;

}


int hk_form::screen_width()
{
return -1;
}
int hk_form::screen_height()
{
return -1;
}



bool hk_form::show_asdialog(hk_form*)
{
return false;
}

hk_string  hk_form::form2html(enum_include)
{
  return "";
}

bool hk_form::set_formbasedfilter(void)
{

list<hk_datasource*>::iterator it=datasources()->begin();
while (it!=datasources()->end())
{
  (*it)->set_temporaryfilter("");
  ++it;
}
bool result=false;
list<hk_visible*>::iterator vit=p_private->p_visibles.begin();
while (vit!=p_private->p_visibles.end())
{
  hk_dsdatavisible* ds=dynamic_cast<hk_dsdatavisible*>(*vit);
  if (ds)
  {  
    ds->update_filter();
    hk_string f=ds->filtervalue(true);
    if (f.size()>0 && ds->columnname().size()>0 && ds->datasource())
    {
      hk_string tmp=ds->datasource()->temporaryfilter();
      if (tmp.size()>0) tmp+=" AND ";
      ds->datasource()->set_temporaryfilter(tmp+"("+f+")");
      result=true;
    }
  }
  else
  {
   hk_dsgrid* g=dynamic_cast<hk_dsgrid*>(*vit);
   if (g)
    {
     for(unsigned int count=0;count<g->columnscount();count++)
      {
        hk_dsgridcolumn* col=g->gridcolumn(count);
        if (col)
        {
          
          col->update_filter();
          hk_string f=col->filtervalue(true);
          if (f.size()>0 && col->columnname().size()>0 && col->datasource())
          {
             hk_string tmp=col->datasource()->temporaryfilter();
             if (tmp.size()>0) tmp+=" AND ";
             col->datasource()->set_temporaryfilter(tmp+"("+f+")");
             result=true;
          }
        }
      }
    
    }
   }
  ++vit;
}
return result;

}

