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
//$Revision: 1.77 $
#include "hk_dsdatavisible.h"
#include "hk_form.h"
#include "hk_datasource.h"
#include "hk_database.h"
#include "hk_connection.h"
#include "hk_presentation.h"
#include "hk_label.h"

        enum enum_token {t_number,t_string,t_date,t_tag,t_identifier,t_and,t_or,t_lt,t_gt,t_eq,t_like,t_is,t_not,t_null};
        typedef class
        {
         public:
         enum_token token;
         hk_string value;
        }tokenclass;

bool parse_filter(vector<tokenclass> &p_tokens,hk_string& v);
bool tokenize_filter(vector<tokenclass> &p_tokens,hk_connection* con,const hk_string& v);

int hk_dsdatavisible::p_defaultprecision=2;
bool hk_dsdatavisible::p_defaultthousandsseparator=false;

class hk_dsdatavisiblemodeprivate
{
public:
        hk_string      p_columnname;
        hk_string      p_defaultvalue;
        hk_string      p_valuechangedaction;
        bool           p_usedefault;
        int  p_precision;
        bool p_use_numberseparator;
        int p_columnoccurance;
hk_dsdatavisiblemodeprivate()
  {
    p_use_numberseparator=false;
    p_columnoccurance=-1;
    p_usedefault=false;
  }
bool operator=(const hk_dsdatavisiblemodeprivate& v)
   {
     p_columnname=v.p_columnname;
     p_defaultvalue=v.p_defaultvalue;
     p_precision=v.p_precision;
     p_columnoccurance=v.p_columnoccurance;
     p_usedefault=v.p_usedefault;
     p_valuechangedaction=v.p_valuechangedaction;
     return true;
   }
};

class hk_dsdatavisibleprivate
{
public:
hk_string p_value;
int tag_colname;
hk_string p_filtervalue;
};

hk_dsdatavisible::hk_dsdatavisible(hk_presentation* p) :hk_dsvisible(p)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::hk_dsdatavisible");
#endif
p_private=new hk_dsdatavisibleprivate;
p_designdata=new hk_dsdatavisiblemodeprivate;
p_viewdata=new hk_dsdatavisiblemodeprivate;
    p_column=NULL;
    set_numberformat(defaultuse_numberseparator(),defaultprecision(),false,true);
    set_alignment(aligndefault,false);
    p_virginname=true;
   p_private->tag_colname=register_tag("COLUMNNAME");

}


hk_dsdatavisible::~hk_dsdatavisible()
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::destructor");
#endif
    if (p_column!=NULL) p_column->datavisible_remove(this);
delete p_designdata;
delete p_viewdata;
delete p_private;
}


void hk_dsdatavisible::set_datasource(hk_datasource* d)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::set_datasource");
#endif

    hk_dsvisible::set_datasource(d);
    if(d==NULL)
    {
        if (p_column!=NULL) p_column->datavisible_remove(this);
        p_column=NULL;
        return;
    }
    if ((d->is_enabled())&&(p_designdata->p_columnname!="")) column();
}


hk_datasource* hk_dsdatavisible::datasource(void)
{
    return hk_dsvisible::datasource();
}


void hk_dsdatavisible::set_columnname(const hk_string& c,bool registerchange)
{
    set_columnname(c,registerchange,1);
}


void hk_dsdatavisible::set_columnname(const hk_string& c, bool registerchange, int coloccurance)
{
#ifdef HK_DEBUG
    hkclassname(c);
    hkdebug("hk_dsdatavisible::set_columnname(hk_string&)");
    debug_p_columnname=c;
#endif
    p_designdata->p_columnname=c;
    p_designdata->p_columnoccurance=(coloccurance<1?1:coloccurance);
    if (label().size()==0) set_label(c,registerchange);
    has_changed(registerchange);

    hk_form* f=dynamic_cast<hk_form*>(p_presentation);
    if (f!=NULL && buddylabel()>-1)
    {
       hk_label* l=dynamic_cast<hk_label*>(f->get_visible(buddylabel()));
       if (l && l->label().size()==0&&c.size()>0)
       {
        l->set_label(c+":");
       }
    }

    if (datasource()!=NULL)
    {
        if ((datasource()->is_enabled())&&(p_designdata->p_columnname!=""))
        {
            set_column();
            row_change();
        }
#ifdef HK_DEBUG
        else
        {
//cout <<"columnname: "<<p_columnname<<endl<<"datasource-enabled() "<<(datasource()->is_enabled()?"ja":"nein")<<endl;
        }
    }else
    {
        hkdebug("hk_dsdatavisible::set_columnname(hk_string&) datasource==NULL");
#endif
    }
    p_filterreplacevalue="\""+columnname()+"\" ";
    widget_specific_enable_disable();
    widget_specific_set_column();

}


hk_column* hk_dsdatavisible::column()
{
#ifdef HK_DEBUG
  //  hkdebug("hk_dsdatavisible::column");
  //  hkdebug("columname= ",p_designdata->p_columnname);
#endif
/*        if (datasource()!=NULL)
            if (datasource()->depending_on()!=NULL) p_column=NULL;
    */
    if (p_column!=NULL)
    {
#ifdef HK_DEBUG
//        hkdebug("p_colum!=NULL=>return");
#endif
        return p_column;
    };
    if (p_designdata->p_columnname!="")
    {

        if (p_datasource!=NULL)
        {
#ifdef HK_DEBUG
            hkdebug("p_colum=NULL");
#endif
            if (p_datasource->is_enabled())
                set_column();
//	     cout <<"p_column: "<<p_column<<endl;
        }
    }
    return p_column;
}


bool hk_dsdatavisible::datasource_disable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::datasource_disable");
#endif
    if (p_column!=NULL) p_column->datavisible_remove(this);
    p_column=NULL;
    hk_dsvisible::datasource_disable();
//widget_specific_enable_disable();
    return true;
}


bool hk_dsdatavisible::datasource_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::datasource_enable");
#endif

p_private->p_value="";
    if (p_column!=NULL) p_column->datavisible_remove(this);
    p_column=NULL;
    column();
    hk_dsvisible::datasource_enable();
//widget_specific_enable_disable();
    return true;
}


void hk_dsdatavisible::datasource_delete(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::datasource_delete");
#endif

    if (p_column!=NULL) p_column->datavisible_remove(this);
    p_column=NULL;
    p_datasource=NULL;
    widget_specific_enable_disable();
}


bool hk_dsdatavisible::before_columns_deleted(void)
{
    if (p_column!=NULL) p_column->datavisible_remove(this);
    p_column=NULL;
    return true;
}


void hk_dsdatavisible::savedata(ostream& s )
{
    hk_string tag="HK_DSDATAVISIBLE";
    start_mastertag(s,tag);
    hk_dsvisible::savedata(s);
    set_tagvalue(s,"COLUMN",p_designdata->p_columnname);
    set_tagvalue(s,"COLUMNOCCURANCE",p_designdata->p_columnoccurance);
    set_tagvalue(s,"DEFAULTVALUE",p_designdata->p_defaultvalue);
    set_tagvalue(s,"USE_DEFAULTVALUE",p_designdata->p_usedefault);
    set_tagvalue(s,"NUMBERSEPARATOR",p_designdata->p_use_numberseparator);
    set_tagvalue(s,"COMMADIGITS",(long int)p_designdata->p_precision);
    set_tagvalue(s,"ONVALUECHANGED_ACTION",p_designdata->p_valuechangedaction);

    end_mastertag(s,tag);
}


void hk_dsdatavisible::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::loaddata");
#endif
    xmlNodePtr buffer=get_tagvalue(definition,"HK_DSVISIBLE");

    if (buffer)
    {
    buffer=buffer->xmlChildrenNode;
    hk_dsvisible::loaddata(buffer);
    }
    p_virginname=false;
    get_tagvalue(definition,"COLUMN",p_designdata->p_columnname);
    p_filterreplacevalue="\""+columnname()+"\" ";
    get_tagvalue(definition,"COLUMNOCCURANCE",p_designdata->p_columnoccurance);
    get_tagvalue(definition,"ONVALUECHANGED_ACTION",p_designdata->p_valuechangedaction);

#ifdef HK_DEBUG
    hkclassname(p_designdata->p_columnname);
#endif
    hk_string b;
    if (get_tagvalue(definition,"DEFAULTVALUE",b))p_designdata->p_defaultvalue=b;
    bool bb;
    if (get_tagvalue(definition,"USE_DEFAULTVALUE",bb))p_designdata->p_usedefault=bb;
    get_tagvalue(definition,"NUMBERSEPARATOR",p_designdata->p_use_numberseparator);
    get_tagvalue(definition,"COMMADIGITS",p_designdata->p_precision);
    set_numberformat(p_designdata->p_use_numberseparator,p_designdata->p_precision,false,true);
    *p_viewdata=*p_designdata;

}


hk_string hk_dsdatavisible::columnname(void) const
{
    return p_designdata->p_columnname;
}


void hk_dsdatavisible::widget_specific_set_column(void)
{

}


void hk_dsdatavisible::set_column(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::set_column");
#endif
    if (p_column!=NULL)
    {
        p_column->datavisible_remove(this);
        p_column=NULL;
    }
    if(datasource()!=NULL) p_column=datasource()->column_by_name(p_designdata->p_columnname,p_designdata->p_columnoccurance);
    if (p_column!=NULL)
    {
        p_column->datavisible_add(this);
        p_column->set_dateformat(dateformat());
        p_column->set_timeformat(timeformat());
        p_column->set_datetimeformat(datetimeformat());
        if (!use_defaultvalue()&&p_virginname)
        {

            switch (p_column->columntype())
            {
                case hk_column::smallintegercolumn : ;
                case hk_column::integercolumn : set_defaultvalue(format_number(0,use_numberseparator(),precision(),locale()),false);break;
                case hk_column::smallfloatingcolumn :;
                case hk_column::floatingcolumn : ; set_defaultvalue(format_number(0.0,use_numberseparator(),precision(),locale()),false);break;
                case hk_column::datecolumn : ;
                case hk_column::timecolumn :;
                case hk_column::timestampcolumn:;
                case hk_column::datetimecolumn  :; set_defaultvalue("%NOW%",false);break;
                case hk_column::boolcolumn : ; set_defaultvalue("",false);break;
                default:;
            }
        }
//	    cout <<"col: "<<p_column->name()<<" type: "<<p_column->columntype_name()<<" is_numeric: "<<(is_numerictype(p_column)?"ja":"nein")<<endl;
        if (alignment()==hk_visible::aligndefault)
        {
            if (is_numerictype(p_column))set_alignment(defaultnumberalignment(),false);
            else set_alignment(defaulttextalignment(),false);
        }
        new_column_pointer_created();
	p_virginname=false;
    }

}


bool    hk_dsdatavisible::use_defaultvalue(void)
{
    if (p_column!=NULL)
        if (p_column->columntype()==hk_column::auto_inccolumn)
            return true;
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_usedefault;
return p_designdata->p_usedefault;

}


void    hk_dsdatavisible::set_defaultvalue(const hk_string& def,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::set_defaultvalue");
#endif
    if (allow_datachanging(false))
    {
    p_designdata->p_defaultvalue=def;
    p_designdata->p_usedefault=true;
    }
    p_viewdata->p_usedefault=true;
    p_viewdata->p_defaultvalue=def;
    has_changed(registerchange);
    widget_specific_set_defaultvalue();
}


hk_string  hk_dsdatavisible::raw_defaultvalue(void)
{

    return p_designdata->p_defaultvalue;
}


hk_string  hk_dsdatavisible::defaultvalue(void)
{
    if (!p_column)
    { 
    if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_defaultvalue;
    else
    return p_designdata->p_defaultvalue;
    }
    hk_string dt;

    switch  (p_column->columntype())
    {
        case hk_column::auto_inccolumn :  { 
        					return hk_translate("[Auto]");
        				  }
        case hk_column::datecolumn :  dt=p_date.date_asstring();break;
        case hk_column::timecolumn :  dt=p_date.time_asstring();break;
        case hk_column::datetimecolumn :;
        case hk_column::timestampcolumn:;
        default:dt=p_date.datetime_asstring();
    }
    hk_string p_res=(p_presentation && p_presentation->mode()==hk_presentation::viewmode?p_viewdata->p_defaultvalue:p_designdata->p_defaultvalue);
    if (!p_presentation &&p_viewdata->p_defaultvalue.size()>0)p_res=p_viewdata->p_defaultvalue;
    p_res=replace_all("%NOW%",p_res,dt);
    p_res=replace_all("%NOWTIME%",p_res,p_date.time_asstring());
    p_res=replace_all("%NOWDATE%",p_res,p_date.date_asstring());
    p_res=replace_all("%TRUE%",p_res,"TRUE");
    p_res=replace_all("%FALSE%",p_res,"FALSE");
    return p_res;
}


void    hk_dsdatavisible::reset_default(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::reset_default");
#endif

    p_viewdata->p_usedefault=false;
    p_viewdata->p_defaultvalue="";
   if (allow_datachanging(false))
   {
    p_designdata->p_usedefault=false;
    p_designdata->p_defaultvalue="";

   }
    has_changed(registerchange);
}


void hk_dsdatavisible::before_insert_row(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::before_insert_row");
#endif
    if (datasource()==NULL||p_column==NULL||!use_defaultvalue()) return;
    if (!p_column->has_changed()&&use_defaultvalue()&&defaultvalue().size()>0)
    {
        p_column->set_asstring(defaultvalue(),true,true);
    }

}


void    hk_dsdatavisible::set_numberformat(bool numsep, int dig,bool registerchange)
{
   set_numberformat(numsep,dig,registerchange,false);
}


void    hk_dsdatavisible::set_numberformat(bool numsep, int dig,bool registerchange,bool force_setting)
{

    if (allow_datachanging(force_setting))
    {
    p_designdata->p_use_numberseparator=numsep;
    p_designdata->p_precision=(dig<0?-1:dig);

    }
    p_viewdata->p_use_numberseparator=numsep;
    p_viewdata->p_precision=(dig<0?-1:dig);
    widget_specific_numberformat();
    has_changed(registerchange);
}


bool    hk_dsdatavisible::use_numberseparator(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_use_numberseparator;
    return p_designdata->p_use_numberseparator;

}


int     hk_dsdatavisible::commadigits(void)
{
    return precision();
}


int     hk_dsdatavisible::precision(void)
{

if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_precision;
    return p_designdata->p_precision;

}


void hk_dsdatavisible::set_defaultnumberformat(bool use_thousandsseparator,int precision)
{
    p_defaultthousandsseparator=use_thousandsseparator;
    p_defaultprecision=precision;

}


bool hk_dsdatavisible::defaultuse_numberseparator(void)
{
    return p_defaultthousandsseparator;

}


int hk_dsdatavisible::defaultprecision(void)
{
    return p_defaultprecision;
}


void hk_dsdatavisible::widget_specific_numberformat(void)
{

}


hk_string hk_dsdatavisible::value_at(unsigned long at)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::value_at");
#endif
    if (!column()) return p_private->p_value;
    hk_string valuestring;
    bool is_null=false;
    if (p_column->has_changed() &&
        ((datasource()->row_position()==at&&datasource()->mode()!=hk_datasource::mode_insertrow)
        ||(datasource()->row_position()==datasource()->max_rows()&&datasource()->mode()==hk_datasource::mode_insertrow))

        )
	{
        valuestring=p_column->changed_data_asstring();
	is_null=p_column->changed_data_is_nullvalue();
	}
    else
    if (datasource()->mode()==hk_datasource::mode_insertrow&& at>= datasource()->max_rows()&& use_defaultvalue())
    {
        valuestring=defaultvalue();
	is_null=(valuestring.size()==0);
    }
    else
    {
        valuestring=p_column->asstring_at(at,true);
	is_null=p_column->is_nullvalue_at(at);
     }
    if (!is_null&&is_numerictype(p_column)&&p_column->columntype()!=hk_column::auto_inccolumn&&valuestring.length()>0
        || (p_column->columntype()==hk_column::auto_inccolumn
        && (datasource()->mode()!=hk_datasource::mode_insertrow||at<datasource()->max_rows())))
    {
        valuestring=format_number(valuestring,true,use_numberseparator(),commadigits(),locale());
    }

    return valuestring;

}

hk_string hk_dsdatavisible::value(void)
{
     if (!column()) return p_private->p_value;
hk_datasource* ds=datasource();
if (ds)
   {
     return value_at(ds->row_position());
   }
   else return p_private->p_value;
}




void hk_dsdatavisible::new_column_pointer_created(void)
{

}


void hk_dsdatavisible::set_value(const hk_string& v)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::set_value");
#endif

 p_private->p_value=v;
 hk_column* c=column();
 if (c)c->set_asstring(v);
 action_on_valuechanged();
}

unsigned int hk_dsdatavisible::find(unsigned int from,unsigned int to,const hk_string& searchtext,bool wholephrase,bool casesensitive,bool backwards)
{
 hk_column* c=column();
 if (c)return c->find(from,to,searchtext,wholephrase,casesensitive,backwards);
 else return (unsigned int) -1;
}


unsigned int hk_dsdatavisible::find(const hk_string& searchtext,bool wholephrase,bool casesensitive, bool backwards)
{
 hk_column* c=column();
 if (c)return c->find(searchtext,wholephrase,casesensitive,backwards);
 else return (unsigned int) -1;
}


bool     hk_dsdatavisible::presentationmode_changed(void)
{

    if (p_column!=NULL) p_column->datavisible_remove(this);
    p_column=NULL;
    if (p_presentation->mode()==hk_presentation::viewmode )
      *p_viewdata=*p_designdata;
    return hk_dsvisible::presentationmode_changed();
}

void hk_dsdatavisible::set_on_valuechanged_action(const hk_string& action,bool registerchange,bool force_setting)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsdatavisible::set_on_valuechanged_action",action);
#endif
    if (allow_datachanging(force_setting))
    {
      p_designdata->p_valuechangedaction=action;
    }
p_viewdata->p_valuechangedaction=action;
has_changed(registerchange);
}

hk_string hk_dsdatavisible::on_valuechanged_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return p_viewdata->p_valuechangedaction;
  return p_designdata->p_valuechangedaction;
}


bool hk_dsdatavisible::action_on_valuechanged(void)
{
if (on_valuechanged_action().size()==0) return true;
 if (!p_presentation)return false;
	return p_presentation->interpreter()->on_valuechanged(this);
 }


void hk_dsdatavisible::set_data_has_changed(void)
{
   if (p_column) p_column->set_has_changed();

}

hk_string hk_dsdatavisible::tag_value(int tagnumber, bool& ok)
{
hk_string result;
  if (tagnumber==p_private->tag_colname)
    {
     if (column())result=column()->name();
     ok=true;
     return result;
    }
    
ok=false;
return hk_dsvisible::tag_value(tagnumber,ok);

}


bool hk_dsdatavisible::set_filtervalue(const hk_string& v)
{
  if (trim(v).size()==0)
  {
  p_private->p_filtervalue="";
  return true;
  }
  vector<tokenclass> p_tokens;
  hk_connection* c=NULL;
  if (datasource())c=datasource()->database()->connection();
  hk_string result=v;
  if (!parsed_value(result,c))
  {
  cerr <<"error parsing value"<<endl;
   return false;
   }
  p_private->p_filtervalue=result;
  return true;
}

hk_string hk_dsdatavisible::filtervalue(bool withcolumnnames) const
{
   hk_string v;
   hk_string e;
   if (withcolumnnames)
     { 
//      cerr <<"withcolumnnames: ";
      v=p_filterreplacevalue;
      e=p_filtervalueend;
     }
//    cerr <<"filtervalue:"<<p_private->p_filtervalue<<" p_filterreplacevalue:"<<v<<" p_filtervalueend:"<<e<<endl;
   return replace_all("%VALEND%",replace_all("%COLNAME%",p_private->p_filtervalue,v),e);

}

bool hk_dsdatavisible::parsed_value( hk_string& v,hk_connection* c)
{
  vector<tokenclass> p_tokens;
  if (!tokenize_filter(p_tokens,c,v))
  {
   cerr <<"error tokenize_filter"<<endl;
   return false;
  }
  hk_string result;
  if (!parse_filter(p_tokens,result))
  {
   cerr <<"error parse_filter"<<endl;
   return false;
  }
   v=result;
   return true;
}


bool tokenize_filter(vector<tokenclass> &p_tokens,hk_connection* con,const hk_string& v)
{
      enum
    {
        S_START,S_QUOTED_TEXT,S_MAYBE_END_OF_QUOTED_TEXT,
        S_NORMAL_TEXT, S_NUMBER,S_DATE,
        S_QUOTED_IDENTIFIER,S_END_OF_QUOTED_IDENTIFIER,S_FIELDNAME,
        S_QUOTED_FIELDNAME,S_IDENTIFIER_EXPECTED
    } state=S_START;
    bool real_num=false;
    hk_string endtag=")+-*/><=";
    hk_string::size_type offset=0;
    hk_string field;
    hk_string quoted="\'";
    hk_string fieldquoted="\"";
    hk_string lt="<";
    hk_string gt=">";
    hk_string eq="=";
    p_tokens.clear();
//     cerr <<"tokenize:"<<v<<endl;
    bool is_endtag=false;
    while (offset<v.size())
    {
        hk_string xc(1,v[offset]);               //row[offset];
//         cerr <<offset<<": #"<<xc<<"# ";
        ++offset; 
        is_endtag=(endtag.find(xc)!=hk_string::npos);
        switch (state)
        {
            case S_START :
            	field=""; //cerr <<"S_START ";
                if (isspace(xc[0]))
                {
                	break;
                }
                
                if(isdigit(xc[0]))
                {
                 state = S_NUMBER;
                 field = xc; //cerr <<" field:#"<<field<<"#";
                 real_num=false;
                }
                else
                if ( lt==xc)
                {
                tokenclass t;
                t.token=t_lt;
                p_tokens.push_back(t);
                break;
                }
                else
                if ( gt==xc)
                {
                tokenclass t;
                t.token=t_gt;
                p_tokens.push_back(t);
                break;
                }
                else
                if ( eq==xc)
                {
                tokenclass t;
                t.token=t_eq;
                p_tokens.push_back(t);
                break;
                }
                else
                if ( "("==xc||is_endtag)
                {
                tokenclass t;
                t.token=t_tag;
                t.value=xc;
                p_tokens.push_back(t);
                break;
                }
                else
                if ( quoted==xc)
                {
                    state = S_QUOTED_TEXT;      
                }
                else
                if ( fieldquoted==xc)
                {
                    state = S_QUOTED_IDENTIFIER;      
                }
                else
                if (xc=="#")
                {
                    state = S_DATE;
                }
                 else
                {
                    field = xc; //cerr <<" field:#"<<field<<"#";
                    state = S_NORMAL_TEXT; //cout <<"S_STARTED MAYBE_NORMAL";
                }
                
                break;

            case S_QUOTED_TEXT :
            	//cerr <<"S_QUOTED_TEXT ";
                if (xc == quoted)
                {
                                                //  cout <<"S_QUOTED_FIELD set S_MAYBE_END_OF_QUOTED_FIELD"<<endl;
                    state = S_MAYBE_END_OF_QUOTED_TEXT;
                }

                else
                {
                    field += xc; //cerr <<" field:#"<<field<<"#";
                }
                break;

            case S_MAYBE_END_OF_QUOTED_TEXT :
            //cerr <<"S_MAYBE_END_OF_QUOTED_TEXT ";
                if (xc == quoted)
                {
                    field += xc; //cerr <<" field:#"<<field<<"#";
                    state = S_QUOTED_TEXT;
                }
                else
                {   //cerr <<"von S_MAYBE_END_OF_QUOTED_FIELD nach S_END_OF_QUOTED_FIELD"<<endl;
                tokenclass t;
                t.token=t_string;
                t.value=field;
                p_tokens.push_back(t);
                state=S_START;
                --offset; // to handle character again


                }
                break;


	    case S_NORMAL_TEXT :
                if (isspace(xc[0]) ||xc=="("||is_endtag)
                {
                //cerr <<"S_NORMAL_TEXT ";
                bool is_tag=false;
                if (con)
                {
                  is_tag=con->is_sqlstatement(field);
//                   cerr <<"is_tag?"<<is_tag<<endl;
                }
                tokenclass t;
                enum_token token=t_tag;
                if (is_tag)
                {
                if (xc=="(") field+="(";
                hk_string vgl=string2upper(field);
                if (vgl=="LIKE") token=t_like;
                else
                if (vgl=="AND") token=t_and;
                else
                if (vgl=="OR") token=t_or;
                else
                if (vgl=="IS") token=t_is;
                else
                if (vgl=="NOT") token=t_not;
                else
                if (vgl=="NULL") token=t_null;
                }
                else
                {
                token=t_string;
                }
                t.token=token;
                t.value=field;
                p_tokens.push_back(t);
                state=S_START;
                
                if (is_endtag||(!is_tag&&xc=="("))
                {
                t.token=t_tag;
                t.value=xc;
                p_tokens.push_back(t);
                
                }
                }
                else
                {
                    field += xc; //cerr <<" field:#"<<field<<"#";
                }
                break;
            case S_NUMBER :
//             cerr <<"S_NUMBER ";
                if (isdigit(xc[0]))
                   field+=xc;
                else
                if (isspace(xc[0])|| is_endtag)
                  {
                  tokenclass t;
                  t.token=t_number;
                  t.value=field;
                  p_tokens.push_back(t);
                  state=S_START;
                  if (is_endtag)
                  {
                  t.token=t_tag;
                  t.value=xc;
                  p_tokens.push_back(t);
                  }
                  }
                else
                 if ((xc[0]==','||xc[0]=='.')&&!real_num)
                  {
                    real_num=true;
                    field+=xc; //cerr <<" field:#"<<field<<"#";
                  }
                  else
                  { //cerr <<"tokenize S_NUMBER ERROR field:#"<<xc<<"#"<<endl;
//                     return false;
                    field+=xc; //cerr <<" field:#"<<field<<"#";
                  }
            	break;
            case S_QUOTED_IDENTIFIER:
                if (xc == fieldquoted)
                {
                    state = S_END_OF_QUOTED_IDENTIFIER;
                }

                else
                {
                    field += xc; //cerr <<" field:#"<<field<<"#";
                }
                break;
            case S_END_OF_QUOTED_IDENTIFIER:
                if (xc==".")
                {
                  state=S_IDENTIFIER_EXPECTED;
                  field+="\""+xc;//cerr <<" field:#"<<field<<"#";
                }
                else
                {
                  tokenclass t;
                  t.token=t_identifier;
                  t.value=field;
                  p_tokens.push_back(t);
                  state=S_START;
                  --offset; // to handle character again

                }
            	break;
            case S_IDENTIFIER_EXPECTED:
                if (isspace(xc[0])|| is_endtag)
                    return false;
                if (xc==fieldquoted)
                  {
                    state=S_QUOTED_FIELDNAME;
                  }
                 else
                 {
                    field+="\"";
                    state=S_FIELDNAME;
                 }
                 field+=xc;
                break;
            case S_FIELDNAME:
                if (isspace(xc[0]) ||xc=="("||is_endtag)
                {
//                 cerr <<"S_FIELDNAME ";
                tokenclass t;
                t.token=t_identifier;
                t.value=field;
                p_tokens.push_back(t);
                state=S_START;
                
                if (is_endtag)
                {
                t.token=t_tag;
                t.value=xc;
                p_tokens.push_back(t);
                
                }
                }
                else
                {
                    field += xc; //cerr <<" field:#"<<field<<"#";
                }
                break;
            case S_QUOTED_FIELDNAME:
                if (xc == fieldquoted)
                {
                  tokenclass t;
                  t.token=t_identifier;
                  t.value=field;
                  p_tokens.push_back(t);
                  state=S_START;
                }

                else
                {
                    field += xc; //cerr <<" field:#"<<field<<"#";
                }
            	break;
            case S_DATE:
               if (xc=="#")
                {
                  tokenclass t;
                  t.token=t_date;
                  t.value=field;
                  p_tokens.push_back(t);
                  state=S_START;
                }

                else
                {
                    field += xc; //cerr <<" field:#"<<field<<"#";
                }
                 
                 
            
        }
//     cerr <<endl;
    }
if (state!=S_START)
{
  tokenclass t;
  switch (state)
  {
  case S_NUMBER:t.token=t_number;break;
  case S_NORMAL_TEXT:
                {
                bool is_tag=false;
                if (con)
                {
                  is_tag=con->is_sqlstatement(field);
                  //cerr <<"is_tag?"<<is_tag<<endl;
                }
                t.token=is_tag?t_tag:t_string;
                if (is_tag)
                {
                  hk_string vgl=string2upper(field);
                if (vgl=="LIKE") t.token=t_like;
                else
                if (vgl=="AND") t.token=t_and;
                else
                if (vgl=="OR") t.token=t_or;
                else
                if (vgl=="IS") t.token=t_is;
                else
                if (vgl=="NOT") t.token=t_not;
                else
                if (vgl=="NULL") t.token=t_null;
  	        }
  	        break;
  	        }
  case S_QUOTED_IDENTIFIER:
  case S_END_OF_QUOTED_IDENTIFIER:
  case S_FIELDNAME:
  case S_QUOTED_FIELDNAME:
  		{
  		t.token=t_identifier;
  		 break;
  		}
  case S_IDENTIFIER_EXPECTED:
  	return false;
  
  default :
                t.token=t_string;

  }
  t.value=field; 
  p_tokens.push_back(t);

}

return true;
}





bool parse_filter(vector<tokenclass> &p_tokens,hk_string& v)
{
// cerr<< "parse_filter"<<endl;
  if (p_tokens.size()==0) return false;

  v="";
      enum
    {
        S_START,S_IS,S_NOT,S_CONDITION,S_VALUE,S_IDENTIFIER
    } state=S_START;
    
/*  for (unsigned int i=0;i<p_tokens.size();++i)
  {
    hk_string token;
    switch (p_tokens[i].token)
    {
      case t_number : token="t_number";break;
      case t_string : token="t_string";break;
      case t_tag : token="t_tag";break;
      case t_and : token="t_and";break;
      case t_or : token="t_or";break;
      case t_lt : token="t_lt";break;
      case t_gt : token="t_gt";break;
      case t_eq : token="t_eq";break;
      case t_like : token="t_like";break;
      case t_is : token="t_is";break;
      case t_not : token="t_not";break;
      case t_null : token="t_null";break;
      case t_identifier: token="t_identifier";break;
      case t_date: token="t_date";break;
    }
    cerr <<"TOKEN: "<<token<<" value #"<<p_tokens[i].value<<"#"<<endl;
  }*/
  
  
  while (p_tokens.size())
  {
   tokenclass t=*p_tokens.begin();
   switch (state)
   {
     case S_START:
     		{
     		switch(t.token)
     		{
     		 case t_number:
     		 		{
     		 		v+="%COLNAME%="+t.value+"%VALEND% ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_date:
     		 		{
     		 		hk_datetime dt;
     		 		v+="%COLNAME%=#"+t.value+"#%VALEND% ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_string:
     		 		{
     		 		hk_string tst=replace_all("\\%",t.value,"");
     		 		tst=replace_all("\\_",tst,"");
     		 		hk_string::size_type percent=tst.find("%");
     		 		bool isbool=false;
     		 		if (tst.find("%TRUE%")!=hk_string::npos||tst.find("%FALSE%")!=hk_string::npos)
     		 		{
     		 		    percent=hk_string::npos;
     		 		    isbool=true;
     		 		}
     		 		
     		 		hk_string::size_type underscore=tst.find("_");
     		 		bool is_like=percent!=hk_string::npos||underscore!=hk_string::npos;
     		 		
     		 		if (is_like )
     		 		v+="%COLNAME%LIKE '"+t.value+"'%VALEND% ";
     		 		else
     		 		
     		 		if (isbool)
     		 		v+="%COLNAME%="+t.value+"%VALEND% ";
     		 		else
     		 		v+="%COLNAME%='"+t.value+"'%VALEND% ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_tag:
     		 		{
     		 		v+="%COLNAME%="+t.value+"%VALEND% ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_and:
     		 case t_or:
     		 		{
     		 		return false;
     		 		}
     		 case t_lt:
     		 		{
     		 		v+="%COLNAME%<";
     		 		if (p_tokens.size()>1 && p_tokens[1].token==t_eq)
     		 		   {
     		 		    v+="=";
     		 		    p_tokens.erase(p_tokens.begin());
     		 		   }
     		 		  
     		 		state=S_CONDITION;
     		 		break;
     		 		}
     		 case t_gt:
     		 		{
     		 		v+="%COLNAME%>";
     		 		if (p_tokens.size()>1 && p_tokens[1].token==t_eq)
     		 		   {
     		 		    v+="=";
     		 		    p_tokens.erase(p_tokens.begin());
     		 		   }
     		 		state=S_CONDITION;
     		 		break;
     		 		}
     		 case t_eq:
     		 		{
     		 		v+="%COLNAME%=";
     		 		state=S_CONDITION;
     		 		break;
     		 		}
     		 case t_like:
     		 		{
     		 		v+="%COLNAME%LIKE ";
     		 		state=S_CONDITION;
     		 		break;
     		 		}
     		 case t_is:
     		 		{
     		 		state=S_IS;
      		 		v+="%COLNAME%IS ";
    		 		break;
     		 		}
     		 case t_not:
     		 		{
     		 		v+="%COLNAME%NOT ";
     		 		state=S_NOT;
     		 		break;
     		 		}
     		 
     		 case t_null:
     		 		{
     		 		v+="%COLNAME%IS NULL ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_identifier:
     		 		{
     		 		  v+="\""+t.value+"\"%VALEND% ";
     		 		  state=S_IDENTIFIER;
     		 		  break;
     		 		}
     		}
     		
     		break;
     		}
     
     
     case S_IS:
     		{
     		switch(t.token)
     		{
     		 case t_number:
     		 case t_string:
     		 case t_date:
     		 case t_tag:
     		 case t_and:
     		 case t_or:
     		 case t_lt:
     		 case t_gt:
     		 case t_eq:
     		 case t_like:
     		 case t_is:
     		 		{
     		 		return false;
     		 		}
     		 case t_not:
     		 		{
     		 		v+="NOT ";
     		 		state=S_NOT;
     		 		break;
     		 		}
     		 case t_null:
     		 		{
     		 		v+="NULL ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_identifier:
     		 		{
     		 		  v+="\""+t.value+"\"%VALEND% ";
     		 		  state=S_START;
     		 		}
     		}
     		break;
     		}
     case S_NOT:
     		{
     		switch(t.token)
     		{
     		 case t_number:
     		 case t_tag:
     		 case t_date:
     		 case t_and:
     		 case t_or:
     		 case t_lt:
     		 case t_gt:
     		 case t_eq:
     		 case t_is:
     		 case t_not:
     		 		{
     		 		return false;
     		 		}
     		 case t_string:
     		 		{
     		 		hk_string tst=replace_all("\\%",t.value,"");
     		 		tst=replace_all("\\_",tst,"");
     		 		hk_string::size_type percent=tst.find("%");
     		 		hk_string::size_type underscore=tst.find("_");
     		 		bool is_like=percent!=hk_string::npos||underscore!=hk_string::npos;
     		 		
     		 		if (is_like )
     		 		{
     		 		v+="LIKE '"+t.value+"'%VALEND% ";
     		 		state=S_VALUE;
     		 		}
     		 		else
     		 		return false;
     		 		break;
     		 		}
     		 case t_like:
     		 		{
     		 		v+="LIKE ";
     		 		state=S_CONDITION;
     		 		break;
     		 		}
     		 case t_null:
     		 		{
     		 		v+="NULL ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_identifier:
     		 		{
     		 		  v+="\""+t.value+"\"%VALEND% ";
     		 		  state=S_START;
     		 		}
     		}
     		break;
     		}
     case S_CONDITION:
     		{
     		switch(t.token)
     		{
     		 case t_number:
     		 		{
     		 		v+=t.value+"%VALEND%";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_date:
     		 		{
     		 		v+="#"+t.value+"#%VALEND%";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_string:
     		 		{
     		 		v+="'"+t.value+"'%VALEND% ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_tag:
     		 		{
     		 		v+=t.value+"%VALEND% ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_lt:
     		 case t_gt:
     		 case t_eq:
     		 case t_and:
     		 case t_or:
      		 case t_null:
     		 case t_like:
     		 case t_is:
     		 case t_not:
     		 		{
     		 		return false;
     		 		}
     		 case t_identifier:
     		 		{
     		 		  v+="\""+t.value+"\"%VALEND% ";
     		 		  state=S_START;
     		 		}
    		}
     		break;
     		}
     
     case S_IDENTIFIER:
     		{
     		switch(t.token)
     		{
     		 
     		 
     		 
     		 case t_lt:
     		 		{
     		 		v+="<";
     		 		if (p_tokens.size()>1 && p_tokens[1].token==t_eq)
     		 		   {
     		 		    v+="=";
     		 		    p_tokens.erase(p_tokens.begin());
     		 		   }
     		 		  
     		 		state=S_CONDITION;
     		 		break;
     		 		}
     		 case t_gt:
     		 		{
     		 		v+=">";
     		 		if (p_tokens.size()>1 && p_tokens[1].token==t_eq)
     		 		   {
     		 		    v+="=";
     		 		    p_tokens.erase(p_tokens.begin());
     		 		   }
     		 		state=S_CONDITION;
     		 		break;
     		 		}
     		 case t_eq:
     		 		{
     		 		v+="=";
     		 		state=S_CONDITION;
     		 		break;
     		 		}
     		 case t_like:
     		 		{
     		 		v+="LIKE ";
     		 		state=S_CONDITION;
     		 		break;
     		 		}
     		 case t_is:
     		 		{
     		 		state=S_IS;
      		 		v+="IS ";
    		 		break;
     		 		}
     		 case t_not:
     		 		{
     		 		v+="NOT ";
     		 		state=S_NOT;
     		 		break;
     		 		}
     		 
     		 case t_null:
     		 		{
     		 		v+="IS NULL ";
     		 		state=S_VALUE;
     		 		break;
     		 		}

     		 
     		 case t_number: 
     		 		{
     		 		 v+=t.value+"%VALEND% ";
     		 		 break;
     		 		}
     		 case t_date: 
     		 		{
     		 		 v+="#"+t.value+"#%VALEND% ";
     		 		 break;
     		 		}
     		 case t_string:
     		 		{
     		 		 v+="'"+t.value+"'%VALEND% ";
     		 		 break;
     		 		}
     		 case t_tag:
     		 		{
     		 		v+=t.value+"%VALEND% ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_and:
     		 		{
     		 		 v+=" AND ";
     		 		 state=S_START;
     		 		 break;
     		 		}
     		 case t_or:
     		 		{
     		 		 v+=" OR ";
     		 		 state=S_START;
     		 		 break;
     		 		}
     		 case t_identifier:
     		 		{
     		 		  v+="\""+t.value+"\"%VALEND% ";
     		 		  state=S_START;
     		 		}
     		}
     		break;
     		}
     
     
     
     case S_VALUE:
     		{
     		switch(t.token)
     		{
     		 case t_lt:
     		 case t_gt:
     		 case t_eq:
     		 case t_like:
     		 case t_is:
     		 case t_not:
     		 case t_null:
     		 		{
     		 		return false;
     		 		}
     		 case t_number: 
     		 		{
     		 		 v+=t.value+"%VALEND% ";
     		 		 break;
     		 		}
     		 case t_date: 
     		 		{
     		 		 v+="#"+t.value+"#%VALEND% ";
     		 		 break;
     		 		}
     		 case t_string:
     		 		{
     		 		 v+="'"+t.value+"'%VALEND% ";
     		 		 break;
     		 		}
     		 case t_tag:
     		 		{
     		 		v+=t.value+"%VALEND% ";
     		 		state=S_VALUE;
     		 		break;
     		 		}
     		 case t_and:
     		 		{
     		 		 v+=" AND ";
     		 		 state=S_START;
     		 		 break;
     		 		}
     		 case t_or:
     		 		{
     		 		 v+=" OR ";
     		 		 state=S_START;
     		 		 break;
     		 		}
     		 case t_identifier:
     		 		{
     		 		  v+="\""+t.value+"\"%VALEND% ";
     		 		  state=S_START;
     		 		}
     		}
     		break;
     		}
   }
   p_tokens.erase(p_tokens.begin());
  }
  
  return true;
}

