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
#include "hk_subform.h"
#include "hk_form.h"
#include "hk_datasource.h"

class hk_subformprivate
{
public:
hk_form* p_form;
};

class hk_subformmodeprivate
{
public:
list<dependingclass> p_fields;
hk_string p_name;

bool operator=(const hk_subformmodeprivate* s)
  {
    if (!s) return false;
    
    p_name=s->p_name;
    p_fields.clear();
    list<dependingclass>::const_iterator it=s->p_fields.begin();
    while (it!=s->p_fields.end())
    {  
      dependingclass c;
      c.masterfield=(*it).masterfield;
      c.dependingfield=(*it).dependingfield;
      p_fields.push_back(c);
      ++it;
    }
    
    return true;
  }
};


hk_subform::hk_subform(hk_presentation* p):hk_dsvisible(p)
{
#ifdef HK_DEBUG
//    wanna_debug(true);
    hkdebug("hk_subform::hk_subform");
#endif
  p_visibletype=hk_visible::subform;
  p_private=new hk_subformprivate;
  p_private->p_form=NULL;
  p_designdata=new hk_subformmodeprivate;
  p_viewdata=new hk_subformmodeprivate;
  
}



hk_subform::~hk_subform()
{
#ifdef HK_DEBUG
    hkdebug("hk_subform::~hk_subform()");
#endif
   delete p_designdata;
   delete p_viewdata;
   delete p_private;
}


void hk_subform::set_subform(hk_form* f)
{
#ifdef HK_DEBUG
    hkdebug("hk_subform::set_subform");
#endif
  p_private->p_form=f;
  if (p_private->p_form && p_presentation)
  {
     p_private->p_form->set_database(p_presentation->database());
     p_private->p_form->set_subformobject(this);
#ifdef HK_DEBUG
     p_private->p_form->hkclassname("hk_form as subform");
#endif
  }
}

void hk_subform::set_name(const hk_string& n, bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_subform::set_name("+n+")");
#endif
if (allow_datachanging(false))
 {
 p_designdata->p_name=n;
 }

 p_viewdata->p_name=n;
 has_changed(registerchange);
}


hk_string hk_subform::name(void) const
{
  
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_name;
   else
    return p_designdata->p_name;

}


hk_form* hk_subform::subform()
{
#ifdef HK_DEBUG
    hkdebug("hk_subform::subform()");
#endif
  return p_private->p_form;
}


void        hk_subform::add_depending_fields(const hk_string& subfield,const hk_string& masterfield, bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_subform::add_depending_fields");
#endif
   has_changed(registerchange);
   dependingclass c;
   c.masterfield=masterfield;
   c.dependingfield=subfield;
   list<dependingclass>* ld =depending_on_fields();
   ld->push_back(c);
}


void        hk_subform::clear_depending_fields(bool registerchange)
{
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
     p_viewdata->p_fields.clear();
   else
     p_designdata->p_fields.clear();
   
   has_changed(registerchange);

}


list<dependingclass>* hk_subform::depending_on_fields(void)
{
      if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return &p_viewdata->p_fields;
   else
    return &p_designdata->p_fields;

}


void    hk_subform::before_datasource_enables(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_subform::before_datasource_enables");
#endif
    hk_dsvisible::before_datasource_enables();


}


void hk_subform::savedata(ostream& s, bool savedatasource,bool saveall)
{
   start_mastertag( s,"HK_SUBFORM");
    set_tagvalue(s,"SUBFORM",p_designdata->p_name);
    hk_dsvisible::savedata(s,savedatasource,saveall);
   hk_string mtag1="DEPENDINGFIELDS";
   start_mastertag(s,mtag1);
        list<dependingclass>::iterator it=p_designdata->p_fields.begin();
        while (it!=p_designdata->p_fields.end())
        {
            set_tagvalue(s,"DEPENDINGFIELD",(*it).dependingfield);
            set_tagvalue(s,"MASTERFIELD",(*it).masterfield);

            it++;
        }
    end_mastertag(s,mtag1);

    end_mastertag( s,"HK_SUBFORM");

}


void hk_subform::savedata(ostream& s  )
{
#ifdef HK_DEBUG
    hkdebug("hk_subform::savedata");
#endif

savedata(s,false,false);

}

void hk_subform::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_subform::loaddata");
#endif

    get_tagvalue(definition,"SUBFORM",p_designdata->p_name);
    set_name(p_designdata->p_name,false);
    int i=1;
    clear_depending_fields();
    hk_string depvalue,mastervalue;
    while (get_tagvalue(definition,"DEPENDINGFIELD",depvalue,i))
        {
            get_tagvalue(definition,"MASTERFIELD",mastervalue,i);
            add_depending_fields(depvalue,mastervalue);
            i++;
        }

    hk_dsvisible::loaddata(definition);
    *p_viewdata=*p_designdata;
}


bool	hk_subform::presentationmode_changed(void)
{
// cerr <<"hk_subform::presentationmode_changed()"<<endl;
 if (!p_private->p_form ||!p_presentation ) return true;
  bool r=hk_dsvisible::presentationmode_changed();
  switch (p_presentation->mode())
  {
   case hk_presentation::viewmode:
   	{
// 	cerr <<"hk_subform::presentationmode_changed() viewmode"<<endl;
    		p_private->p_form->set_mode(hk_presentation::viewmode);
		break;

    	}
    case hk_presentation::designmode:
    case hk_presentation::filtermode:
    		{
// 	cerr <<"hk_subform::presentationmode_changed() designmode"<<endl;
		    	p_private->p_form->clear_visiblelist();
			p_private->p_form->clear_datasourcelist();
			p_private->p_form->reset_has_changed();
			break;
		}
   }// switch end
  return r;
}


void   hk_subform::activate_subform(void)
{
internal_set_datasourcevalues();
p_private->p_form->set_viewmode();
}

void hk_subform::internal_set_datasourcevalues()
{
  		//p_private->p_form->set_mode(hk_presentation::designmode);
		p_private->p_form->reset_has_changed();
    		if(name().size()>0)p_private->p_form->load_form(name());
    		list<dependingclass>* dl=depending_on_fields();
		if (dl->size()>0
		   &&datasource()
		   &&p_private->p_form->datasource())
		   {
		     p_private->p_form->datasource()->set_depending_on(datasource());
		     list<dependingclass>::iterator it=dl->begin();
		     while (it!=dl->end())
		     {
	     	        p_private->p_form->datasource()->add_depending_fields((*it).dependingfield,(*it).masterfield,true);
		        ++it;
		     }
		   }
		p_private->p_form->reset_has_changed();

}
