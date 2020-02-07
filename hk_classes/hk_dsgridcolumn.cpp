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
//$Revision: 1.29 $
#include "hk_dsgridcolumn.h"
#include "hk_dsgrid.h"
#include "hk_dscombobox.h"
#include "hk_presentation.h"
#include "hk_datasource.h"
#include "hk_database.h"

class hk_dsgridcolumnprivate
{

};

class hk_dsgridcolumnmodeprivate
{
 public:
 hk_string p_onselectaction;
 bool operator=(const hk_dsgridcolumnmodeprivate* v)
 {
  if (!v) return false;
  p_onselectaction=v->p_onselectaction;
  return true;
 }
};


hk_dsgridcolumn::hk_dsgridcolumn():hk_dsdatavisible()
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_dsgridcolumn::hk_dsgridcolumn");
#endif
    p_private=new hk_dsgridcolumnprivate;
    p_designdata=new hk_dsgridcolumnmodeprivate;
    p_viewdata=new hk_dsgridcolumnmodeprivate;


    p_columnwidth=default_cellwidth;
    p_columntype=columnedit;
    p_grid=NULL;
    p_combopresentationdatasource=-1;
    p_combolistdatasourcetype=dt_table;
    p_combobox=NULL;

}

hk_dsgridcolumn::~hk_dsgridcolumn(void)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_dsgridcolumn::~hk_dsgridcolumn");
#endif
if (p_combobox) delete p_combobox;
delete p_viewdata;
delete p_designdata;
delete p_private;
}
void hk_dsgridcolumn::set_displayname(hk_string d,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgridcolumn::set_displayname");
#endif
    p_displayname=d;
    if (p_grid!=NULL) p_grid->has_changed(registerchange);

}


hk_string hk_dsgridcolumn::displayname(void)
{
    return p_displayname;
}


void hk_dsgridcolumn::set_columntype(hk_dsgridcolumn::enum_columntype t,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgridcolumn::set_columntype");
#endif
    p_columntype=t;
    if (p_columntype==columncombo)
    {
        set_combovalues();
    }
    else
    {
        delete p_combobox;
        p_combobox=NULL;
    }

    if (p_grid!=NULL) p_grid->has_changed(registerchange);
}


void hk_dsgridcolumn::set_combovalues(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgridcolumn::set_combovalues");
#endif
    if (!datasource())return;
    if (!p_combobox){
     p_combobox= new hk_dscombobox();
    }
    p_combobox->set_viewcolumnname(p_comboviewcolumnname);
    p_combobox->set_listcolumnname(p_combolistcolumnname);
    p_combobox->set_columnname(columnname());
    p_combobox->set_numberformat(use_numberseparator(),commadigits(),false);
    p_combobox->set_alignment(alignment(),false);
    if (use_defaultvalue())p_combobox->set_defaultvalue(raw_defaultvalue());
    hk_datasource* l=NULL;
    if (p_combopresentationdatasource>-1&& p_grid)
    {
        if (p_grid->presentation())
        {                                         // a presenation
            l=p_grid->presentation()->get_datasource(p_combopresentationdatasource);

        }
    }
    else
    {
// not a presentation => try to load the datasource
        if (p_grid && p_grid->presentation())
        {

            l=p_grid->presentation()->get_datasource(p_combolistdatasourcename);
        }
        else

        if (p_combolistdatasourcename.size()>0)
	{
            l=datasource()->database()->load_datasource(p_combolistdatasourcename,p_combolistdatasourcetype);
        }
    }
    p_combobox->set_listdatasource(l);
    p_combobox->set_datasource(datasource());
}


hk_dsgridcolumn::enum_columntype hk_dsgridcolumn::columntype(void)
{

    return p_columntype;
}


void hk_dsgridcolumn::set_columnwidth(int w,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgridcolumn::set_columnwidth");
#endif
    p_columnwidth=w;
    if (p_grid!=NULL) p_grid->has_changed(registerchange);
}


int hk_dsgridcolumn::columnwidth(void)
{
    return p_columnwidth;

}


void hk_dsgridcolumn::savedata(ostream& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgridcolumn::savedata");
#endif
    hk_string ftag="HK_DSGRIDCOLUMN";
    start_mastertag(s,ftag);
    hk_dsdatavisible::savedata(s);
    set_tagvalue(s,"DISPLAYNAME",p_displayname);
    set_tagvalue(s,"COLUMNWIDTH",(long int)p_columnwidth);
    hk_string c;
    switch (p_columntype)
    {
        case columnbool : c="COLUMNBOOL";
        break;
        case columncombo : c="COLUMNCOMBO";
        break;

        default : c="COLUMNEDIT";
    }
    set_tagvalue(s,"COLUMNTYPE",c);
    set_tagvalue(s,"COMBOVIEWCOLUMN",p_comboviewcolumnname);
    set_tagvalue(s,"COMBOLISTCOLUMN",p_combolistcolumnname);
    set_tagvalue(s,"COMBOPRESENTATIONDATASOURCE",p_combopresentationdatasource);
    set_tagvalue(s,"COMBOLISTDATASOURCE",p_combolistdatasourcename);
    switch (p_combolistdatasourcetype)
    {
      case dt_table : c="TABLE";break;
      case dt_view : c="VIEW";break;
      case dt_query : c="QUERY";break;
    }
    set_tagvalue(s,"COMBODATASOURCETYPE",c);
    set_tagvalue(s,"ONSELECT_ACTION",p_designdata->p_onselectaction);
    end_mastertag(s,ftag);

}


void hk_dsgridcolumn::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgridcolumn::loaddata");
#endif

        xmlNodePtr d=get_tagvalue(definition,"HK_DSDATAVISIBLE");
    if (d)d=d->xmlChildrenNode;

    hk_dsdatavisible::loaddata(d);
    get_tagvalue(definition,"DISPLAYNAME",p_displayname);
    get_tagvalue(definition,"COLUMNWIDTH",p_columnwidth);
    hk_string c;
    get_tagvalue(definition,"COLUMNTYPE",c);
    if (c=="COLUMNBOOL")p_columntype=columnbool;
    else    if (c=="COLUMNCOMBO")p_columntype=columncombo;
    else p_columntype=columnedit;
    set_columnwidth(p_columnwidth,false);
    set_displayname(p_displayname,false);

    get_tagvalue(definition,"COMBOVIEWCOLUMN",p_comboviewcolumnname);
    get_tagvalue(definition,"COMBOLISTCOLUMN",p_combolistcolumnname);
    get_tagvalue(definition,"COMBOPRESENTATIONDATASOURCE",p_combopresentationdatasource);
    get_tagvalue(definition,"COMBOLISTDATASOURCE",p_combolistdatasourcename);
    bool b=true;
    if (get_tagvalue(definition,"COMBO_IS_TABLE",b))
    {
     p_combolistdatasourcetype=(b?dt_table:dt_query);
    }
    else
    {
     c="TABLE";
     get_tagvalue(definition,"COMBODATASOURCETYPE",c);
     if (c=="QUERY") p_combolistdatasourcetype=dt_query;
     else
     if (c=="VIEW") p_combolistdatasourcetype=dt_view;
     else
      p_combolistdatasourcetype=dt_table;
    }

    set_columntype(p_columntype,false);
    get_tagvalue(definition,"ONSELECT_ACTION",p_designdata->p_onselectaction);
    *p_viewdata=*p_designdata;
}


void hk_dsgridcolumn::set_grid(hk_dsgrid* g)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgridcolumn::set_grid");
#endif
    p_grid=g;
    if (g!=NULL) set_datasource(g->datasource());
}


void     hk_dsgridcolumn::set_listdatasource(const hk_string& datasourcename,datasourcetype dt, bool registerchange)
{
    p_combolistdatasourcetype=dt;
    p_combolistdatasourcename=datasourcename;
    if (p_grid!=NULL) p_grid->has_changed(registerchange);
    set_combovalues();
}


datasourcetype hk_dsgridcolumn::listdatasourcetype() const
{
  return p_combolistdatasourcetype;
}


hk_datasource*  hk_dsgridcolumn::listdatasource(void)
{
    return NULL;
}


hk_string       hk_dsgridcolumn::listdatasourcename(void)
{
    return p_combolistdatasourcename;
}




void     hk_dsgridcolumn::set_viewcolumnname(const hk_string& viewcolumn,bool registerchange)
{
    p_comboviewcolumnname=viewcolumn;
    if (p_grid!=NULL) p_grid->has_changed(registerchange);
    if (p_combobox)p_combobox->set_viewcolumnname(p_comboviewcolumnname);
    set_combovalues();
}


hk_string   hk_dsgridcolumn::viewcolumnname(void)
{
    return p_comboviewcolumnname;
}


void     hk_dsgridcolumn::set_listcolumnname(const hk_string& listcolumn,bool registerchange)
{
    p_combolistcolumnname=listcolumn;
    if (p_grid!=NULL) p_grid->has_changed(registerchange);
    if (p_combobox)p_combobox->set_listcolumnname(p_combolistcolumnname);
    set_combovalues();
}


hk_string  hk_dsgridcolumn::listcolumnname(void)
{
    return p_combolistcolumnname;
}


bool     hk_dsgridcolumn::set_listpresentationdatasource(long n, bool registerchange)
{
    if (p_grid!=NULL) p_grid->has_changed(registerchange);
    p_combopresentationdatasource=n;
    set_combovalues();
    return (presentationdatasource()!=-1);
}


long     hk_dsgridcolumn::listpresentationdatasource(void)
{
    return p_combopresentationdatasource;
}


hk_string hk_dsgridcolumn::value_at(unsigned long row)
{
    if (p_combobox)
        return p_combobox->value_at(row);
    else
    {
                                                  //column()->asstring_at(row);
        if (column()) return hk_dsdatavisible::value_at(row);
        else
            return "novalue";
    }
}


void hk_dsgridcolumn::set_datasource(hk_datasource* d)
{
    hk_dsdatavisible::set_datasource(d);
    set_combovalues();

}


hk_dscombobox* hk_dsgridcolumn::combobox(void)
{

    return p_combobox;

}


bool hk_dsgridcolumn::is_findstring(unsigned int pos,const hk_string& searchtext,bool wholephrase,bool casesensitive)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgridcolumn::is_findstring");
#endif
    hk_string search=searchtext;
    bool p_isnumeric=false;
    bool p_isinteger=false;
    if (p_combobox )
    {
       p_isnumeric=is_numerictype(p_combobox->viewcolumn());
       p_isinteger=is_integertype(p_combobox->viewcolumn());
    }
    else
    {
     p_isnumeric=is_numerictype(column());
     p_isinteger=is_integertype(column());

    }
    hk_string where=p_isnumeric?format_number(value_at(pos),true,false,(p_isinteger?0:8),locale()):value_at(pos);
    if (!casesensitive)
    {
        for (unsigned int tt=0;tt<where.size();tt++)
        {
            where[tt]=toupper(where[tt]);
        }
        for (unsigned int tt=0;tt<search.size();tt++)
        {
            search[tt]=toupper(search[tt]);
        }
    }

    if (wholephrase)
    {
        if (where==search)return true;
    }
    else
    {
        hk_string::size_type a=where.find(search);
        if (a!=hk_string::npos) return true;
    }

    return false;

}

unsigned int hk_dsgridcolumn::find(unsigned int from,unsigned int to,const hk_string& searchtext,bool wholephrase,bool casesensitive,bool backwards)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsgridcolumn::find(unsigned int from,unsigned int to,const hk_string& searchtext");
#endif
    unsigned int p_from, p_to, p_buffer;
    if (!datasource())return 1;
    if (datasource()->max_rows()==0)return 1;
    if (from <datasource()->max_rows())p_from=from; else p_from=0;
    if (to <datasource()->max_rows()) p_to=to;else p_to=datasource()->max_rows()-1;
    if (p_from>p_to)
    {
        p_buffer=p_from;
        p_from=p_to;
        p_to=p_buffer;
    }
    if (backwards)
    {
        p_buffer=p_from;
        p_from=p_to;
        p_to=p_buffer;
    }
    for (unsigned int k=p_from;(backwards?((k>=p_to)&&(k<=p_from)):k<=p_to);(backwards?k--:k++))
    {
        if (is_findstring(k,searchtext,wholephrase,casesensitive))return k;
    }
    return datasource()->max_rows()+1;
}


bool hk_dsgridcolumn::action_on_click(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 bool result=hk_dsdatavisible::action_on_click();
 p_presentation=p;
 return result;
}

bool hk_dsgridcolumn::action_on_doubleclick(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 bool result=hk_dsdatavisible::action_on_doubleclick();
 p_presentation=p;
 return result;

}

bool hk_dsgridcolumn::action_on_select(hk_dscombobox* ds)
{
if (!ds) return false;
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 bool result=ds->action_on_select();
 p_presentation=p;
 return result;

 if (on_select_action().size()==0) return true;
 if (!p_grid->presentation())return false;
	return p_grid->presentation()->interpreter()->on_select(this);

}



bool  hk_dsgridcolumn::action_on_close(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 bool result=hk_dsdatavisible::action_on_close();
 p_presentation=p;
 return result;

}

bool  hk_dsgridcolumn::action_on_open(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 bool result=hk_dsdatavisible::action_on_open();
 p_presentation=p;
 return result;

}




void hk_dsgridcolumn::action_before_store_changed_data(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 hk_dsdatavisible::action_before_store_changed_data();
 p_presentation=p;

}
void hk_dsgridcolumn::action_after_store_changed_data(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 hk_dsdatavisible::action_after_store_changed_data();
 p_presentation=p;

}
void hk_dsgridcolumn::action_before_row_change(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 hk_dsdatavisible::action_before_row_change();
 p_presentation=p;

}
void hk_dsgridcolumn::action_after_row_change(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 hk_dsdatavisible::action_after_row_change();
 p_presentation=p;

}
void hk_dsgridcolumn::action_before_delete(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 hk_dsdatavisible::action_before_delete();
 p_presentation=p;

}
void hk_dsgridcolumn::action_after_delete(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 hk_dsdatavisible::action_after_delete();
 p_presentation=p;


}
void hk_dsgridcolumn::action_before_insert(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 hk_dsdatavisible::action_before_insert();
 p_presentation=p;

}
void hk_dsgridcolumn::action_after_insert(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 hk_dsdatavisible::action_after_insert();
 p_presentation=p;

}

bool hk_dsgridcolumn::action_on_getfocus(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 bool r=hk_dsdatavisible::action_on_getfocus();
 p_presentation=p;
 return r;

}
bool hk_dsgridcolumn::action_on_loosefocus(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 bool r=hk_dsdatavisible::action_on_loosefocus();
 p_presentation=p;
 return r;
}

bool hk_dsgridcolumn::action_on_key(void)
{
 hk_presentation* p=p_presentation;
 p_presentation=p_grid->presentation();
 bool r=hk_dsdatavisible::action_on_key();
 p_presentation=p;
 return r;
}

void hk_dsgridcolumn::set_on_select_action(const hk_string&f,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_onselectaction=f;
    }
    p_viewdata->p_onselectaction=f;
    has_changed(registerchange);

}

hk_string hk_dsgridcolumn::on_select_action(void)
{

if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_onselectaction;
return p_designdata->p_onselectaction;

}

bool     hk_dsgridcolumn::presentationmode_changed(void)
{
  if (p_presentation->mode()==hk_presentation::viewmode )
      *p_viewdata=*p_designdata;
  return hk_dsdatavisible::presentationmode_changed();

}
