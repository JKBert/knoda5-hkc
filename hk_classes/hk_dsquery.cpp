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
#include "hk_dsquery.h"
#include "hk_datasource.h"
#include "hk_database.h"
#include "hk_dsgrid.h"
#include "hk_qbe.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define VERSION UNKNOWN
#endif



class hk_dsqueryprivate
{
public:
hk_dsqueryprivate()
	{
	  p_use_qbe=true;
	}
bool p_use_qbe;
hk_string p_loadqueryname;

};


hk_dsquery::hk_dsquery(hk_presentation* p):hk_dsmodevisible(p)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsquery::hk_dsquery");
#endif
  p_private= new hk_dsqueryprivate;
}


hk_dsquery::~hk_dsquery()
{
#ifdef HK_DEBUG
    hkdebug("hk_dsquery::~hk_dsquery");
#endif
 delete p_private;
}


void hk_dsquery::set_use_qbe(bool u, bool registerchange)
{
  p_private->p_use_qbe=u;
  has_changed(registerchange);
}

bool hk_dsquery::use_qbe(void) const
{
  return p_private->p_use_qbe;
}




bool hk_dsquery::save_query(const hk_string& n,bool ask)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsquery::save_query");
#endif

    if (datasource()==NULL)
    {
    cerr <<"hk_dsquery::save_query datasource==0"<<endl;
    return false;
    }
    
    hk_database::enum_storage storage=hk_database::st_overwrite;


    if (n.size()>0) datasource()->set_name(n);
    if (datasource()->name().size()==0) if (!(datasource()->ask_name()))return false;
    cerr <<"nach name setzen(askname) "<<datasource()->name()<<endl;
    if (p_private->p_loadqueryname==datasource()->name()) storage=hk_database::st_storechanges;
    bool result=false;
    if (datasource()->type()==hk_datasource::ds_view)
    {
      cerr <<"save_query ->view"<<endl;
      if (datasource()->database()->view_exists(datasource()->name()))
      {
          if ((ask &&show_yesnodialog(replace_all("%1",storage==hk_database::st_storechanges?hk_translate("View '%1' has changed. Store changes?"):hk_translate("View '%1' already exists. Overwrite it?"),datasource()->name()),true))||!ask)
	  	result=datasource()->alter_view_now();
	  else  result=false;
      }
         else
	  result=datasource()->create_view_now();
    reset_has_changed();
    return result;

    }


     reset_has_changed();
    if (datasource()->database()->storagemode(ft_query)==hk_database::local)
    {
    ofstream* p_save=datasource()->database()->savestream(storage,datasource()->name(),ft_query,ask,true,false);
    if (p_save==NULL)
    {
     return false;
    }
    savedata(*p_save);
    p_save->close();
    delete p_save;
    }
    else
    { // store central
       STRINGSTREAM*  p_save;
      p_save=datasource()->database()->savestringstream(ft_query);
      if (p_save==NULL)
        {
	  cerr <<"savestringstream=0!"<<endl;
	return false;
	}
       savedata(*p_save);
       hk_string s=p_save->str();
       datasource()->database()->save(storage,s,datasource()->name(),ft_query,ask);
       delete p_save;
    }
    p_private->p_loadqueryname=datasource()->name();
    reset_has_changed();
    return true;

}




void hk_dsquery::savedata(ostream& s)
{
    start_mastertag(s,"HK_QUERY");
    set_tagvalue(s,"HK_QUERYVERSION",(hk_string)VERSION);
    if (grid())
    	    grid()->savedata(s);

    set_tagvalue(s,"USE_QBE",p_private->p_use_qbe);
    if (p_private->p_use_qbe && qbe())
      {
         qbe()->savedata(s);
      }

    end_mastertag(s,"HK_QUERY");
p_is_new=false;

}










bool hk_dsquery::load_query(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsquery::load_query");
#endif
    if (datasource()==NULL) return false;
    widget_specific_before_load_query();

    xmlNodePtr res=datasource()->database()->xmlload(datasource()->name(),ft_query);

    if (!res)return false;
     loaddata(res);
    reset_has_changed();
    p_private->p_loadqueryname=datasource()->name();
    widget_specific_after_load_query();
    return true;

}

void hk_dsquery::loaddata(xmlNodePtr definition)
{
    if (grid())
    {
    xmlNodePtr g;
    hk_string xxx;
    if ((g=get_tagvalue(definition,"HK_DSGRID",xxx,1,mastertag)))
        grid()->loaddata(g);
// DIE N�HSTEN ZEILEN L�CHEN UM EINDEUTIGES QUERYFORMAT ZU HABEN
    else
        grid()->loaddata(definition);
    grid()->set_font(hk_font::defaultfontname(),hk_font::defaultfontsize());
   }
    p_private->p_use_qbe=false;
    get_tagvalue(definition,"USE_QBE",p_private->p_use_qbe);

    if (p_private->p_use_qbe && qbe())
      {
	 xmlNodePtr g=get_tagvalue(definition,"QBE");
	 if (g)qbe()->loaddata(g);
      }
p_is_new=false;

}





void hk_dsquery::widget_specific_before_load_query(void)
{

}

void hk_dsquery::widget_specific_after_load_query(void)
{

}

void hk_dsquery::widget_specific_before_save_query(void)
{

}

void hk_dsquery::widget_specific_after_save_query(void)
{

}
