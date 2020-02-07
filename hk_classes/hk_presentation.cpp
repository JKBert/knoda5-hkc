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
//$Revision: 1.81 $

#include "hk_presentation.h"
#include "hk_database.h"
#include "hk_datasource.h"
#include "hk_definitions.h"
#include "hk_dsmodevisible.h"
#include "hk_form.h"
#include "hk_report.h"
#include "hk_interpreter.h"
#include <cassert>

class hk_presentationprivate
{

public:
hk_presentationprivate()
	{
	  p_counter=0;
	}
~hk_presentationprivate()
{
   if (p_interpreter) delete p_interpreter;

}

        hk_string       p_name;

        unsigned int        p_designwidth;
        unsigned int        p_designheight;

        long    p_counter;                        //unique number of datasource variable
        long int p_vupncentral;                   //unique number of visible objects variable
        bool p_automatic_enable_datasources;
	hk_presentation::enum_sizetype  p_sizetype;
        list<hk_datasource*>    p_datasources;
        hk_database*        p_database;
	hk_interpreter* p_interpreter;
	hk_string 		p_interpretername;

};

hk_presentation::enum_sizetype hk_presentation::p_defaultsizetype=hk_presentation::absolute;
int hk_presentation::p_snap_to_gridx=10;
int hk_presentation::p_snap_to_gridy=10;

const double teiler=10000.0;

hk_presentation::hk_presentation(void):hk_dsmodevisible()
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::hk_presentation");
#endif
   p_private=new hk_presentationprivate;
    p_private->p_interpreter=NULL;
    p_private->p_database=NULL;
    p_private->p_automatic_enable_datasources=true;
    p_private->p_counter=0;
    p_presentationtype=hk_presentation::general;
    p_private->p_vupncentral=0;
    p_presentation=this;
    p_private->p_sizetype=p_defaultsizetype;
    set_designsize(100,100);
    p_private->p_interpretername="python";
    p_while_errorhandling=false;
}


hk_presentation::~hk_presentation(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::~hk_presentation");
#endif
    if (p_datasource!=NULL)
    {
        p_datasource->visible_remove(this);
        p_datasource->p_presentation=NULL;
    }
    list<hk_datasource*>::iterator it=p_private->p_datasources.begin();

    while (it!=p_private->p_datasources.end())
    {
        hk_datasource* d=(*it);
        it++;
        if (d!=NULL)
        {
            d->p_presentation=NULL;
        }

    }
    it=p_private->p_datasources.begin();
    while (it!=p_private->p_datasources.end())
    {
        hk_datasource* d=(*it);
        it++;
        if (d!=NULL)
        {
            d->store_changed_data();
            d->disable();
	    if (d->p_depending_on_datasource)
	     d->p_depending_on_datasource->depending_ds_remove(d);
	    d->p_depending_on_datasource=NULL;
        }

//     if (d!=nodelete)delete d;
        delete d;
    }

    p_private->p_datasources.clear();

    p_datasource=NULL;
    if (p_private->p_database!=NULL) p_private->p_database->presentation_remove(this);
   delete p_private;
}


void hk_presentation::set_database(hk_database* db)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::set_database");
#endif

    if (p_private->p_database!=NULL)
    {
        p_private->p_database->presentation_remove(this);
        p_private->p_database=NULL;
    }
    if (db!=NULL)
    {
        db->presentation_add(this);
    }

    p_private->p_database=db;

}


long hk_presentation::add_datasource(hk_datasource* d)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::add_datasource");
#endif
    if (d==NULL)
     {
       show_warningmessage(hk_translate("Bug: tried to add empty datasource!"));
       return -1;
     }
    d->p_dscounter=p_private->p_counter;
    p_private->p_counter++;
    p_private->p_datasources.insert(p_private->p_datasources.end(),d);
    return d->p_dscounter;
}


long    hk_presentation::new_datasource(const hk_string& newname,datasourcetype dt)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::new_datasource");
#endif
    if (p_private->p_database==NULL) return -1;
    hk_datasource* d=p_private->p_database->load_datasource(newname,dt,this);
    if (d) d->set_name(newname,false);
    long i=add_datasource(d);
#ifdef HK_DEBUG
    if (d)d->hkclassname(longint2string(i));
#endif
    set_has_changed();
    return i;
}


hk_datasource*  hk_presentation::get_datasource(long nr)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::get_datasource(long)");
#endif
    list<hk_datasource*>::iterator it=p_private->p_datasources.begin();
    while (it!=p_private->p_datasources.end())
    {
        if ( (*it)->p_dscounter==nr) return (*it);
        it++;
    }
    return NULL;
}


hk_datasource*  hk_presentation::get_datasource(const hk_string& description)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::get_datasource(const hk_string&)");
#endif
    list<hk_datasource*>::iterator it=p_private->p_datasources.begin();
    while (it!=p_private->p_datasources.end())
    {
        if ( unique_datasourcename((*it)->p_dscounter)==description)
	  return (*it);
        it++;
    }
    return NULL;
}

hk_datasource*  hk_presentation::get_datasource_by_shortname(const hk_string& description)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::get_datasource_by_shortname(const hk_string&)");
#endif
    list<hk_datasource*>::iterator it=p_private->p_datasources.begin();
    while (it!=p_private->p_datasources.end())
    {
        if ( unique_shortdatasourcename((*it)->p_dscounter)==description)
	  return (*it);
        it++;
    }
    return NULL;
}


void    hk_presentation::remove_datasource(long nr,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::remove_datasource");
#endif
    remove_datasource(get_datasource(nr),registerchange);
}


void hk_presentation::remove_datasource( hk_datasource* d,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::remove_datasource");
#endif
    if (d!=NULL)
    {
        p_private->p_datasources.remove(d);
        if (registerchange) set_has_changed();
    }
}


void hk_presentation::clear_datasourcelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::clear_datasourcelist");
#endif
    list<hk_datasource*>::iterator it=p_private->p_datasources.begin();
    while (it!=p_private->p_datasources.end())
    {
        hk_datasource* d=(*it);
        it++;
        if (d!=NULL)
        {
            d->store_changed_data();
            d->disable();
        }

//     if (d!=nodelete)delete d;
        delete d;
    }

    p_private->p_datasources.clear();
#ifdef HK_DEBUG
    hkdebug("hk_presentation::clear_datasourcelist ENDE");
#endif
}


void    hk_presentation::savedata(ostream& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::savedata");
#endif
    hk_string tag="PRESENTATION";
    start_mastertag(s,tag);
    hk_dsmodevisible::savedata(s);
    set_tagvalue(s,"DESIGNWIDTH",(long unsigned int)p_private->p_designwidth);
    set_tagvalue(s,"DESIGNHEIGHT",(long unsigned int)p_private->p_designheight);
    hk_string stype=(p_private->p_sizetype==absolute?"ABSOLUTE":"RELATIVE");
    set_tagvalue(s,"INTERPRETERNAME",p_private->p_interpretername);
    set_tagvalue(s,"SIZETYPE",stype);
    list<hk_datasource*>::iterator it=p_private->p_datasources.begin();
    start_mastertag(s,"DATASOURCES");
    if (p_private->p_datasources.size()==0)
    	cerr <<hk_translate("hk_presentation::savedata No datasources defined!")<<endl ;
    while (it!=p_private->p_datasources.end())
    {
        (*it)->savedata(s);
        it++;
    }
    end_mastertag(s,"DATASOURCES");

    end_mastertag(s,tag);
p_is_new=false;
}


void    hk_presentation::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::loaddata");
#endif
    get_tagvalue(definition,"DESIGNWIDTH", p_private->p_designwidth);
    get_tagvalue(definition,"DESIGNHEIGHT",p_private->p_designheight);
    get_tagvalue(definition,"INTERPRETERNAME",p_private->p_interpretername);
    set_designsize(p_private->p_designwidth,p_private->p_designheight,false);
    hk_string buffer2;
    p_private->p_sizetype=relative;
    if (get_tagvalue(definition,"SIZETYPE",buffer2))
    {
      if (buffer2=="ABSOLUTE")p_private->p_sizetype=absolute;
    }


    hk_string value;
    int i=1;
    clear_datasourcelist();
    xmlNodePtr v=NULL;
    while ((v=get_tagvalue(definition,"DATASOURCE",value,i,mastertag)))
    {
        v=v->xmlChildrenNode;
        hk_datasource* ds=NULL;
        get_tagvalue(v,"TYPE",buffer2);
        hk_string dsname;
        get_tagvalue(v,"DATASOURCENAME",dsname);
        datasourcetype dt=dt_table;
        if (buffer2=="QUERY")  dt=dt_query;
	else
        if (buffer2=="VIEW")  dt=dt_view;

        long int dint=new_datasource(dsname,dt);
        ds=get_datasource(dint);
        if (ds!=NULL)
        {
            ds->loaddata(v, ( dsname.size()==0|| (dt==dt_table ||dt==dt_view) ));
	    ds->set_name(dsname,false);
            if (ds->p_dscounter>=p_private->p_counter) p_private->p_counter=ds->p_dscounter+1;
        } else show_warningmessage(hk_translate("hk_presentation::loaddata: Error creating/loading datasource! This is probably a bug"));
        i++;
    }
    //if (p_private->p_datasources.size()==0) show_warningmessage(hk_translate("No datasources defined!")) ;
    v=get_tagvalue(definition,"HK_DSVISIBLE");
    if (v)hk_dsmodevisible::loaddata(v->xmlChildrenNode);
p_is_new=false;
}


hk_string hk_presentation::unique_datasourcename(long nr)
{
    hk_string result;
    hk_datasource* rd =get_datasource(nr);
    if (!rd) return "";
        result=rd->name()+" (";
        result+=longint2string(rd->p_dscounter);
    result+=") ";
    hk_datasource* depds=get_datasource(rd->depending_on_presentationdatasource());
    if (depds!=NULL)
    {
        result+=" [";
        result+=depds->name()+"]";

    }
    if (rd->filter().size()>0)
    {
        result+=" | ";
        result+=rd->filter();
    }
    return result;
}



hk_string hk_presentation::unique_shortdatasourcename(long nr)
{

   hk_string result;
   hk_datasource* rd =get_datasource(nr);
   if (!rd) return "";
   result=rd->name()+longint2string(rd->p_dscounter);
   return result;
}


bool hk_presentation::set_mode(enum_mode s)
{

    if (s==hk_presentation::designmode && runtime_only()) return false;
    if (!widget_specific_before_modechanges(s)) return false;
    hk_dsmodevisible::set_mode(s);
    if (p_private->p_automatic_enable_datasources)
    {
        bool enable=(s==hk_presentation::viewmode);
        enable_datasources(enable,hk_datasource::standard);
    }
    widget_specific_modechanges(s);
    return true;
}


void hk_presentation::enable_datasources(bool enable,hk_datasource::enum_accessmode m)
{
    list<hk_datasource*>* mydatasources=datasources();
    list<hk_datasource*>::iterator it = mydatasources->begin();
    while (it!=mydatasources->end())
    {
        if (enable)
        {
            if ((*it)->datasource_used())
            {
                if ((*it)->is_enabled()) (*it)->disable();
                (*it)->set_accessmode(m);
                //if (!((*it)->previous_enable_problems()))
		   (*it)->enable();
            }
        }
        else
        {
            (*it)->disable();
            (*it)->set_accessmode(m);
        }
        it++;
    }

}






void hk_presentation::database_delete(void)
{
    database_has_vanished();
    p_private->p_database =NULL;
}


void hk_presentation::database_has_vanished(void)
{

}


void hk_presentation::before_source_vanishes(void)
{

    //hk_dsmodevisible::before_source_vanishes();
    //p_datasources.remove(datasource());
}


bool hk_presentation::ask_name(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::ask_name");
#endif
    hk_string text;
    if (p_visibletype==hk_visible::form) text=hk_translate("Please enter the name of the form:");
    else text=hk_translate("Please enter the name of the report");
    hk_string res=show_stringvaluedialog(text);
    if (res.size()==0) return false;

    set_name(res);

    return true;

}




void    hk_presentation::set_designsize(unsigned int w, unsigned int h,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::set_designsize");
#endif
    p_private->p_designwidth=w;
    p_private->p_designheight=h;
    set_presentationsize(w,h);
    if (registerchange) set_has_changed();
}


void    hk_presentation::set_presentationsize(unsigned int w, unsigned int h)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::set_presentationsize");
#endif
    p_actualwidth= (w>0?w:1);
    p_actualheight=(h>0?h:1);

    widget_specific_presentationresize();

}


unsigned int    hk_presentation::horizontal2relativ(unsigned int h)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::horizontal2relativ");
#endif
    assert(p_private->p_designwidth!=0);
    return (unsigned int)( h*teiler / p_private->p_designwidth+0.5) ;

}


unsigned int    hk_presentation::vertical2relativ(unsigned int v)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::vertical2relativ");
#endif
    assert(p_private->p_designheight!=0);
    return (unsigned int)(v*teiler / p_private->p_designheight+0.5);

}


unsigned int    hk_presentation::relativ2horizontal(unsigned int r)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::relativ2horizontal");
#endif
    return (unsigned int)(r*p_private->p_designwidth /teiler+0.5);

}


unsigned int    hk_presentation::relativ2vertical(unsigned int r)
{
#ifdef HK_DEBUG
    hkdebug("hk_presentation::relativ2vertical");
#endif
    return (unsigned int)(r*p_private->p_designheight/teiler+0.5);

}


void hk_presentation::register_object(hk_visible* v)
{
    if (v==NULL) return;
    if (v->p_vupn==-1)
    {
        v->p_vupn=p_private->p_vupncentral;
        p_private->p_vupncentral++;
    }
    else
    {
        if (v->p_vupn>=p_private->p_vupncentral)
            p_private->p_vupncentral=v->p_vupn+1;
    }

 if (v->identifier().size()==0)
    {
       int i=v->p_vupn;
	while (!v->set_identifier(hk_translate("field_")+longint2string(v->p_vupn+i),false,noninteractive))
	{
	++i;
	}
    }
}


unsigned int    hk_presentation::designwidth(void)
{
    if (type()==hk_visible::report)
    {
        hk_report* r =dynamic_cast <hk_report*>(this);
        if (r!=NULL)
        {
            if (r->masterreport()!=NULL) return r->masterreport()->designwidth();
        }

    }

    return p_private->p_designwidth;
}


unsigned int    hk_presentation::designheight(void)
{
    if (type()==hk_visible::report)
    {
        hk_report* r =dynamic_cast <hk_report*>(this);
        if (r!=NULL)
        {
            if (r->masterreport()!=NULL) return r->masterreport()->designheight();
        }

    }
    return p_private->p_designheight;
}


void hk_presentation::set_automatic_enable_datasources(bool b)
{
    p_private->p_automatic_enable_datasources=b;
}


bool hk_presentation::automatic_enable_datasources(void)
{
    return p_private->p_automatic_enable_datasources;
}


void hk_presentation::bulk_operation(enum_bulkoperation )
{

}



void hk_presentation::set_sizetype(enum_sizetype t,bool registerchange)
{
  if (t==p_private->p_sizetype) return;
  p_private->p_sizetype=t;
  has_changed(registerchange);
  sizetype_changed();

}



hk_presentation::enum_sizetype hk_presentation::sizetype(void)
{
   return p_private->p_sizetype;
}


void hk_presentation::sizetype_changed(void)
{


}


 void hk_presentation::set_defaultsizetype(enum_sizetype type)
{
 p_defaultsizetype=type;

}


hk_presentation::enum_sizetype hk_presentation::defaultsizetype(void)
 {

 return p_defaultsizetype;
 }

hk_interpreter* hk_presentation::interpreter()
{
   if (p_private->p_interpreter)
      {
        if (p_private->p_interpreter->interpretername()!=p_private->p_interpretername)
	   {
		delete p_private->p_interpreter;
		p_private->p_interpreter=NULL;
	   }
      }
  if (!p_private->p_interpreter) p_private->p_interpreter=new_interpreter(p_private->p_interpretername,this);
  return p_private->p_interpreter;

}

void hk_presentation::set_interpretername(const hk_string& n,bool /*registerchange*/)
{
 p_private-> p_interpretername=n;
}

hk_string hk_presentation::interpretername(void) const
{
return p_private->p_interpretername;
}

void            hk_presentation::set_name(const hk_string& n)
{
 hk_string nm=replace_all("/",replace_all(".",n,"_"),"_");
 if (nm!=p_private->p_name)p_is_new=true;
 p_private->p_name=nm;
 widget_specific_set_name();
}
hk_string       hk_presentation::name(void) const
{
return p_private->p_name;
}


hk_database*   hk_presentation::database(void) const
{
return p_private->p_database;
}



list<hk_datasource*>*   hk_presentation::datasources(void) const
{
return &p_private->p_datasources;
}



void hk_presentation::set_snap2gridx(int v)
{
  p_snap_to_gridx=v;
}

int hk_presentation::snap2gridx(void)
{
  return p_snap_to_gridx;
}

void hk_presentation::set_snap2gridy(int v)
{
  p_snap_to_gridy=v;
}

int hk_presentation::snap2gridy(void)
{
  return p_snap_to_gridy;
}


bool hk_presentation::is_subpresentation(void)
{
  return false;
}


bool hk_presentation::load_module(const hk_string& modulename)
{
return interpreter()->load_module(modulename);

}



