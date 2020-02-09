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
//$Revision: 1.40 $
#include "hk_button.h"
#include "hk_form.h"
#include "hk_database.h"
#include "hk_datasource.h"
#include "hk_report.h"
#include "hk_actionquery.h"
#include "hk_interpreter.h"

#include <string.h>



class hk_buttonprivate
{
 public:
 hk_buttonprivate()
    {
    }
     hk_string p_value;
};

class hk_buttonmodeprivate
{
 public:
 hk_buttonmodeprivate()
    {
    p_is_togglebutton=false;
    p_is_pushed=false;
    }
	bool  operator=(const hk_buttonmodeprivate* v)
     {
	if (!v) return false;
	if (p_icon.data)
	{
	  delete[] p_icon.data;
	  p_icon.data=NULL;
	  p_icon.length=0;
	}
	p_icon.length=v->p_icon.length;
	p_icon.data=new char[p_icon.length];
	memcpy(p_icon.data,v->p_icon.data,p_icon.length);
	p_is_togglebutton=v->p_is_togglebutton;
	p_is_pushed=v->p_is_pushed;
        return true;
     }
  struct_raw_data p_icon;
  bool p_is_togglebutton;
  bool p_is_pushed;
};


















hk_button::hk_button(hk_form* frm):hk_dsvisible(frm)
{
#ifdef HK_DEBUG
    hkdebug("hk_button::hk_button");
#endif
    p_private=new hk_buttonprivate;
    p_viewdata=new hk_buttonmodeprivate;
    p_designdata=new hk_buttonmodeprivate;
    p_visibletype=button;
    p_database=NULL;
    p_action=-1;
    p_showmaximized=false;
}


hk_button::~hk_button(void)
{
  delete p_designdata;
  delete p_viewdata;
  delete p_private;

}


bool    hk_button::push_action(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_button::push_action");
#endif

//execute script
    action_on_click();
// now handle other actions
    if (p_presentation==NULL) return false;
    if (p_presentation->mode()==hk_presentation::designmode) return false;
    if (action()>5 &&!datasource() && action()!=14)
    {
        show_warningmessage(hk_translate("No datasource set!"));
        return false;
    }
    switch (action())
    {
        case 0   : return widget_specific_open_form();
        case 1   : return widget_specific_close_form();
        case 2   : return widget_specific_open_table();
        case 3   : return widget_specific_open_query();
        case 4   : return widget_specific_preview_report();
        case 5   : return widget_specific_print_report();

        case 6   : if (datasource()!=NULL)return datasource()->goto_first();else break;
        case 7   : if (datasource()!=NULL)return datasource()->goto_last();else break;
        case 8   : if (datasource()!=NULL)return datasource()->goto_next();else break;
        case 9   : if (datasource()!=NULL)return datasource()->goto_previous();else break;
        case 10  : if (datasource()!=NULL)
        {
            datasource()->setmode_insertrow();
            return true;
        }
        else break;
        case 11  : if (datasource()!=NULL)
        {
            datasource()->delete_actualrow();
            return true;
        }
        else break;
        case 12  : if (datasource()!=NULL)
        {
            datasource()->store_changed_data();
            return true;
        }
        else break;
        case 13  : if (datasource()!=NULL)
        {
            hk_actionquery* a= datasource()->database()->new_actionquery();
            if (a)
            {
                xmlNodePtr result=datasource()->database()->xmlload(p_object,ft_query);
                hk_string querystring;
                hk_string sql;
                result=get_tagvalue(result,"DATASOURCE",querystring);
                get_tagvalue(result,"SQL",sql);

                a->set_sql(sql.c_str(),sql.size());
                bool res= a->execute();
                delete a;
                if (res)return true;

            }
            show_warningmessage(hk_translate("Error while executing actionquery"));
            return false;
        }
        else break;
	case 14  :      return widget_specific_close_application();
			break;
	case 15  :      return widget_specific_open_view();
			break;

        default  :;

    }

    return false;

}


void hk_button::set_action(const hk_string& actionstring,const hk_string& objectname,bool showmaximized, bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_button::set_action ",actionstring);
    hkdebug("object: ",objectname);
#endif
    long int a=-1;
    if (actionstring=="open_form") a=0;
    else
    if (actionstring=="close_form") a=1;
        else
        if (actionstring=="open_table") a=2;
            else
            if (actionstring=="open_query") a=3;
                else
                if (actionstring=="preview_report") a=4;
                    else
                    if (actionstring=="print_report") a=5;
                        else
                        if (actionstring=="goto_firstrow") a=6;
                            else
                            if (actionstring=="goto_lastrow") a=7;
                                else
                                if (actionstring=="goto_nextrow") a=8;
                                    else
                                    if (actionstring=="goto_previousrow") a=9;
                                        else
                                        if (actionstring=="insert_row") a=10;
                                            else
                                            if (actionstring=="delete_row") a=11;
                                                else
                                                if (actionstring=="store_row") a=12;
                                                    else
                                                    if (actionstring=="action_query") a=13;
						    else
						    if (actionstring=="close_application") a=14;
						    else
						    if (actionstring=="open_view") a=15;

  set_action(a,objectname,showmaximized,registerchange);

}


void hk_button::set_action(long actionid,const hk_string& objectname,bool showmaximized,bool registerchange)
{
    p_action=actionid;
    p_object=objectname;
    p_showmaximized=showmaximized;
    has_changed(registerchange);

}


bool hk_button::widget_specific_open_form(void)
{
    return false;
}


bool hk_button::widget_specific_close_form(void)
{
    return false;
}


bool    hk_button::widget_specific_open_table(void)
{
    return false;
}


bool    hk_button::widget_specific_open_view(void)
{
    return false;
}


bool    hk_button::widget_specific_open_query(void)
{
    return false;
}


bool    hk_button::widget_specific_preview_report(void)
{
    return false;
}


bool    hk_button::widget_specific_close_application(void)
{
    exit(0);
    return true;
}






bool    hk_button::widget_specific_print_report(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_button::widget_specific_print_report");
#endif
    hk_report* w=new hk_report();
    if (w==NULL) return false;
    if (p_presentation!=NULL)w->set_database(p_presentation->database());
    else w->set_database(database());
    if (!w->load_report(object())) return false;
    hk_string c=parsed_condition(w);

    if (c.size()>0&&w->datasource())
    {
        w->datasource()->set_temporaryfilter(c);
        w->datasource()->set_use_temporaryfilter(true);
    }

    w->print_report();
    delete w;

    return true;
}


void  hk_button::savedata(ostream& s )
{
#ifdef HK_DEBUG
    hkdebug("hk_button::savedata");
#endif
    hk_string dtag="HK_BUTTON";
    start_mastertag(s,dtag);
    set_tagvalue(s,"ACTION",p_action);
    set_tagvalue(s,"OBJECT",p_object);
    set_tagvalue(s,"SHOWMAXIMIZED",p_showmaximized);
    hk_string ctag="REPORTCONDITIONS";
        hk_string d;
    if (data2hex(&p_designdata->p_icon,d))
    {
    start_mastertag(s,"ICON");
       set_tagvalue(s,"ICONDATA",d);
       set_tagvalue(s,"ICONSIZE",p_designdata->p_icon.length);
    end_mastertag(s,"ICON");
    }

    set_tagvalue(s,"IS_TOGGLEBUTTON",p_designdata->p_is_togglebutton);
    set_tagvalue(s,"IS_PUSHED",p_designdata->p_is_pushed);


    start_mastertag(s,ctag);

    hk_string itemtag="REPORTCONDITION";
    list<struct_condition>::iterator it=p_reportconditions.begin();
    while(it!=p_reportconditions.end())
    {
        start_mastertag(s,itemtag);
        set_tagvalue(s,"REPORTFIELD",(*it).reportfield);
        set_tagvalue(s,"CONDITION",(*it).condition);
        set_tagvalue(s,"FORMDATASOURCE",(*it).formdatasource);
        set_tagvalue(s,"FORMFIELD",(*it).formfield);

        end_mastertag(s,itemtag);
        ++it;
    }

    end_mastertag(s,ctag);
    hk_dsvisible::savedata(s);
    end_mastertag(s,dtag);
}




void  hk_button::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_button::loaddata");
#endif

    get_tagvalue(definition,"ACTION",p_action);
    get_tagvalue(definition,"OBJECT",p_object);
    get_tagvalue(definition,"SHOWMAXIMIZED",p_showmaximized);
    get_tagvalue(definition,"IS_TOGGLEBUTTON",p_designdata->p_is_togglebutton);
    get_tagvalue(definition,"IS_PUSHED",p_designdata->p_is_pushed);
    set_is_togglebutton(p_designdata->p_is_togglebutton,false,true);
    if (p_designdata->p_is_togglebutton) set_is_pushed(p_designdata->p_is_pushed,false,true);

    hk_string itemtag="REPORTCONDITION";
    clear_conditions(false);
    xmlNodePtr buffer,buffer2; hk_string b;
    buffer=buffer2=NULL;
    if ((buffer=get_tagvalue(definition,"REPORTCONDITIONS",b)))
    {

        int i=1;
        while((buffer2=get_tagvalue(buffer->xmlChildrenNode,"REPORTCONDITION",b,i)))
        {
            hk_button::struct_condition c;
            get_tagvalue(buffer2->xmlChildrenNode,"REPORTFIELD",c.reportfield);
            get_tagvalue(buffer2->xmlChildrenNode,"CONDITION",c.condition);
            get_tagvalue(buffer2->xmlChildrenNode,"FORMDATASOURCE",c.formdatasource);
            get_tagvalue(buffer2->xmlChildrenNode,"FORMFIELD",c.formfield);
            p_reportconditions.insert(p_reportconditions.end(),c);
            ++i;
        }                                         //endwhile
    }                                             //endif

    hk_dsvisible::loaddata(definition);
    hk_string d;
    if ((buffer=get_tagvalue(definition,"ICON",b)))
    {
      unsigned long size=0;
       if (get_tagvalue(buffer,"ICONDATA",d)
          &&get_tagvalue(buffer,"ICONSIZE",size))
        {
      	  hex2data(d,size,&p_designdata->p_icon);
      	  show_icon();
        }
    }
      *p_viewdata=*p_designdata;
    set_action(p_action,p_object,p_showmaximized,false);
}






hk_string hk_button::object(void)
{
    return p_object;
}


int hk_button::action(void)
{
    return p_action;
}


bool hk_button::show_maximized(void)
{
    return p_showmaximized;
}


void hk_button::set_database(hk_database* db)
{
    p_database=db;
}


hk_database* hk_button::database(void)
{
    return p_database;
}


void hk_button::add_condition(const hk_string& reportfield,const hk_string& condition,const hk_string&formdatasource,const hk_string& formfield,bool registerchange)
{
    hk_button::struct_condition c;
    c.reportfield=reportfield;
    c.condition=condition;
    c.formdatasource=formdatasource;
    c.formfield=formfield;
    p_reportconditions.insert(p_reportconditions.end(),c);
    has_changed(registerchange);

}


list<hk_button::struct_condition>* hk_button::conditions(void)
{
    return &p_reportconditions;

}


void hk_button::clear_conditions(bool registerchange)
{
    p_reportconditions.erase(p_reportconditions.begin(),p_reportconditions.end());
    has_changed(registerchange);

}


hk_string hk_button::parsed_condition(hk_presentation* pr)
{  //cerr<<endl<<endl<<"parsed_condition"<<endl<<endl;
    hk_string result;
    if (!p_presentation) return "";
    hk_datasource* prdatasource=NULL;
    if (pr && pr->datasource()&& pr->datasource()->type()!=hk_datasource::ds_table)
    {
      prdatasource=pr->datasource()->database()->new_resultquery();
      if (prdatasource)
      {
       prdatasource->set_sql(pr->datasource()->sql());
       prdatasource->set_filter("0=1");
       prdatasource->enable();
      }
    }

    list<struct_condition>::iterator it=p_reportconditions.begin();
    while(it!=p_reportconditions.end())
    {
      bool r_useident=true;
      bool f_useident=true;
      hk_string r=trim((*it).reportfield);
      hk_string f=trim((*it).formfield);
      if (r[0]=='%')
        {
	  r_useident=false;
	  r.replace(0,1,"");
	}
      if (f[0]=='%')
        {
	  f_useident=false;
	  f.replace(0,1,"");
	}

        if (result.size()>0) result+=" AND ";
        if (r_useident)
	{
	   hk_string v=defaultidentifierdelimiter+r+defaultidentifierdelimiter;
	   if (pr && pr->datasource()&& pr->datasource()->type()==hk_datasource::ds_table)
	   {
	    result+=defaultidentifierdelimiter+pr->datasource()->name()+defaultidentifierdelimiter+"."+v+(*it).condition;
	   }
	   else
	   if (prdatasource)
	 {

	   hk_column* col=prdatasource->column_by_name(r);
	   if (col)
	   {
	      /*hk_string t=col->tableorigin();
	      if (t.size()>0)
	        v=defaultidentifierdelimiter+t+defaultidentifierdelimiter+"."+defaultidentifierdelimiter+pr->datasource()->fieldorigin(r)+defaultidentifierdelimiter;
	      */
	      v=prdatasource->totalfieldorigin(r);
	   }
	   else
	    cerr<<"keine column gesetzt! fr:"<<r<<endl;
	   result+=v+(*it).condition;
	}
	else
	{
	if (pr && pr->datasource()&& pr->datasource()->type()!=hk_datasource::ds_table)
	 {
	   hk_column* col=pr->datasource()->column_by_name(r);
	   if (col)
	   {
	      /*hk_string t=col->tableorigin();
	      if (t.size()>0)
	        r=defaultidentifierdelimiter+t+defaultidentifierdelimiter+"."+defaultidentifierdelimiter+pr->datasource()->fieldorigin(r)+defaultidentifierdelimiter;
	      */
	       r=pr->datasource()->totalfieldorigin(r);
	   }else cerr <<"COL: "<<r<<" nicht gefunden !!!"<<endl;

	 }

	result+=r+(*it).condition;
	}
	}

        hk_datasource* ds=p_presentation->get_datasource((*it).formdatasource);
        if (ds)
        {
//set formdatasourcevalue
            hk_column* c=ds->column_by_name(f);
            if (c)
	    {
	      if (f_useident)
	      result+=defaulttextdelimiter+c->asstring()+defaulttextdelimiter;
	      else result+=f;
	    }
            else
            {
                hk_string warning=hk_translate("No such field %FIELD% in datasource %DATASOURCE%!");
                warning=replace_all("%FIELD%",warning,(*it).formfield);
                warning=replace_all("%DATASOURCE%",warning,(*it).formdatasource);
                show_warningmessage(warning);
            }
        }

        else
        {                                         //no formdatasource, so use it as a constant
              if (f_useident)
                result+=defaulttextdelimiter+f+defaulttextdelimiter;
	      else result+=f;

        }
        ++it;
    }
    if (prdatasource) delete prdatasource;
//     cout <<"parsed buttoncondition: "<<endl<<result<<endl;

    return result;

}


bool hk_button::load_icon(const hk_string& filename,bool registerchange)
{
    has_changed(registerchange);
    return load_file(filename,icon());
}

struct_raw_data* hk_button::icon()
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return &(p_viewdata->p_icon);
return &(p_designdata->p_icon);

}


void hk_button::set_is_togglebutton(bool t,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_is_togglebutton=t;
    }
    p_viewdata->p_is_togglebutton=t;
    has_changed(registerchange);

}

bool hk_button::is_togglebutton(void) const
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_is_togglebutton;
return p_designdata->p_is_togglebutton;

}

bool hk_button::is_pushed() const
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_is_pushed;
return p_designdata->p_is_pushed;

}

void hk_button::set_is_pushed(bool p,bool registerchange,bool force_setting)
{
if (!is_togglebutton()) return;
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_is_pushed=p;
    }
    p_viewdata->p_is_pushed=p;
    has_changed(registerchange);
widget_specific_is_pushed();
}


bool hk_button::widget_specific_is_pushed(void)
{
  return true;
}
