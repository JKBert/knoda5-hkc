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
#include "hk_reportdata.h"
#include "hk_reportsection.h"
#include "hk_report.h"
#include <math.h>
#include <locale.h>
#include <nl_types.h>
#include <langinfo.h>
#include "hk_reportutils.h"
#include "hk_column.h"
#include "hk_datasource.h"
#include <langinfo.h>

class hk_reportdatamodeprivate
{
public:
        hk_string p_data;
        hk_string p_beforedata;
        hk_string p_afterdata;
        hk_string p_displayname;
        bool p_top;
        bool p_left;
        bool p_right;
        bool p_bottom;
        bool p_loru;
        bool p_luro;
        bool p_linebreak;
        bool p_dynamic_height;
        bool p_is_image;
        
	bool operator=(const hk_reportdatamodeprivate* d)
	{
	if (!d)return false;
	p_data=d->p_data;
	p_beforedata=d->p_beforedata;
	p_afterdata=d->p_afterdata;
	p_displayname=d->p_displayname;
	p_top=d->p_top;
	p_left=d->p_left;
	p_right=d->p_right;
	p_bottom=d->p_bottom;
	p_loru=d->p_loru;
	p_luro=d->p_luro;
	p_linebreak=d->p_linebreak;
	p_dynamic_height=d->p_dynamic_height;
	p_is_image=d->p_is_image;
	return true;
	}
hk_reportdatamodeprivate()
  {
    p_top=p_bottom=p_left=p_right=p_luro=p_loru=false;
    p_linebreak=false;
    p_dynamic_height=false;
    p_is_image=false;
  }

};

class hk_reportdataprivate
{
public:
       unsigned long p_count;
        bool p_runningcount;
        bool p_minmax_alreadyset;
        int number_displayrows;
        number sum;
        number min;
        number max;
        longnumber squaresum;
        data_replacefunctiontype* p_replacefunction;
        hk_string p_replacefunctionstring;
        reportdatacounttype* p_datacountfunction;
        hk_string p_datacountfunctionstring;
        data_configurefunctiontype* p_dataconfigurefunction;
        hk_string p_dataconfigurefunctionstring;
	hk_string p_on_print_action;
	list<hk_string> p_valuelines;
	hk_string  p_value;
	bool p_fully_printed;
	int p_offset;
	int p_pages;
	bool p_first_print;
	int tag_VALUE,tag_ROWNUMBER,tag_ALIGN,tag_ABSOLUTEPAGENUMBER,tag_PAGENUMBER,tag_RELHEIGHT,tag_RELWIDTH,
	tag_RELYPOS,tag_RELXPOS,tag_WIDTH,tag_YPOS,tag_XPOS,tag_COUNT,tag_SUM,tag_MIN,tag_MAX,tag_AVERAGE,
	tag_STDDEV,tag_STDDEVSAMPLE, tag_FIELDNAME,tag_DISPLAYNAME,
	tag_HEIGHT,tag_FONT,tag_PSFONT,tag_FONTSIZE
	;
hk_reportdataprivate()
  {
  p_fully_printed=false;
  p_offset=0;
  p_pages=0;
  p_first_print=true;
  number_displayrows=-1;
  }
};



hk_reportdata::reportdataconfigurelisttype  hk_reportdata::p_reportdataconfigurefunctions;
list<hk_string> hk_reportdata::p_reportdataconfigurelist;

hk_reportdata::datacountlisttype  hk_reportdata::p_datacountfunctions;
list<hk_string> hk_reportdata::p_datacountfunctionlist;

hk_reportdata::datareplacelisttype  hk_reportdata::p_datareplacefunctions;
list<hk_string> hk_reportdata::p_datareplacefunctionlist;

//********************************************
//**   hk_reportdata                        **
//********************************************

hk_reportdata::hk_reportdata(hk_reportsection* s):hk_dsdatavisible(s->report())
{
#ifdef HK_DEBUG
// wanna_debug(true);
    hkdebug("hk_reportdata::hk_reportdata");
#endif
    p_viewdata=new hk_reportdatamodeprivate;
    p_designdata=new hk_reportdatamodeprivate;
    p_private=new hk_reportdataprivate;
    p_visibletype=hk_visible::reportdata;
    p_report=s!=NULL?s->report():NULL;
    //p_presentation=p_report;
    set_backgroundcolour(p_report?p_report->backgroundcolour():hk_white,false);
    set_foregroundcolour(p_report?p_report->foregroundcolour():hk_black,false);
    p_section=s;
    p_private->p_count=0;
//  commadigits()=-1;
    p_private->p_runningcount=false;
    p_private->p_minmax_alreadyset=false;
    p_private->p_replacefunction=NULL;
    p_private->p_datacountfunction=NULL;
    set_height(40,false);
    set_width(300,false);
//  wanna_debug(true);
    if (p_reportdataconfigurefunctions.size()==0)
    {
        add_configurefunctiontype("Postscript",&configure_postscriptdata);
        add_configurefunctiontype("ExcelXML",&configure_excelxmldata);
        add_configurefunctiontype("None",NULL);
    }

    set_configurefunction("None",false);

    if (p_datacountfunctions.size()==0)
    {
//         add_datacountfunctiontype("Postscript",&recount_postscriptdata);
        add_datacountfunctiontype("None",NULL);
    }
    set_datacountfunction("None",false);

    if (p_datareplacefunctions.size()==0)
    {
        add_datareplacefunctiontype("FILECHARACTERSET",&encodefilecharsetfunction);
        add_datareplacefunctiontype("Postscript",&replacepsdatafunction);
        add_datareplacefunctiontype("ExcelXML",&replaceexcelxmldatafunction);
        add_datareplacefunctiontype("None",NULL);
    }
    set_replacefunction("None",false);
    set_data("%VALUE%",false);
  p_private->tag_VALUE=register_tag("VALUE");
  p_private->tag_ROWNUMBER=register_tag("ROWNUMBER");
  p_private->tag_ALIGN=register_tag("ALIGN");
  p_private->tag_ABSOLUTEPAGENUMBER=register_tag("ABSOLUTEPAGENUMBER");
  p_private->tag_PAGENUMBER=register_tag("PAGENUMBER");
  p_private->tag_RELHEIGHT=register_tag("RELHEIGHT");
  p_private->tag_RELWIDTH=register_tag("RELWIDTH");
  p_private->tag_RELYPOS=register_tag("RELYPOS");
  p_private->tag_RELXPOS=register_tag("RELXPOS");
  p_private->tag_WIDTH=register_tag("WIDTH");
  p_private->tag_YPOS=register_tag("YPOS");
  p_private->tag_XPOS=register_tag("XPOS");
  p_private->tag_COUNT=register_tag("COUNT");
  p_private->tag_SUM=register_tag("SUM");
  p_private->tag_MIN=register_tag("MIN");
  p_private->tag_MAX=register_tag("MAX");
  p_private->tag_AVERAGE=register_tag("AVERAGE");
  p_private->tag_STDDEV=register_tag("STDDEV");
  p_private->tag_STDDEVSAMPLE=register_tag("STDDEVSAMPLE");
  p_private->tag_FIELDNAME=register_tag("FIELDNAME");
  p_private->tag_DISPLAYNAME=register_tag("DISPLAYNAME");
  p_private->tag_HEIGHT=register_tag("HEIGHT");
  p_private->tag_FONT=register_tag("FONT");
  p_private->tag_PSFONT=register_tag("PSFONT");
  p_private->tag_FONTSIZE=register_tag("FONTSIZE");

}


hk_reportdata::~hk_reportdata()
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::~hk_reportdata");
#endif
    if (p_section!=NULL) p_section->remove_data(this);
    delete p_private;
    delete p_designdata;
    delete p_viewdata;
}


void hk_reportdata::set_data(const hk_string& d,bool registerchange)
{
    p_viewdata->p_data=d;
    if (p_report->mode()==hk_presentation::designmode) p_designdata->p_data=d;
    has_changed(registerchange);
}


hk_string hk_reportdata::data(void)
{
     if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_data;
     else return p_viewdata->p_data;
}


void hk_reportdata::set_beforedata(const hk_string& b,bool registerchange)
{
     if (p_report->mode()==hk_presentation::designmode) p_designdata->p_beforedata=b;
     p_viewdata->p_beforedata=b;
    has_changed(registerchange);
}


void hk_reportdata::set_afterdata(const hk_string& a,bool registerchange)
{
     if (p_report->mode()==hk_presentation::designmode) p_designdata->p_afterdata=a;
     p_viewdata->p_afterdata=a;
    has_changed(registerchange);
}



hk_string hk_reportdata::actual_string(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::actual_string");
#endif
    hk_string displayvalue=p_viewdata->p_data;

    if (!linebreak()) 
      {
        displayvalue=p_private->p_value;
        p_private->p_fully_printed=true;
      }
    else
    {
//        int h=(p_private->p_first_print?y():0)+height();
//        if (p_report->sizetype()==hk_presentation::relative)
//            h=p_section->relativ2vertical(h);
       int restsize=p_report->periodic_when()-p_report->current_rowcount();
       p_private->number_displayrows=-1;
       unsigned int max_rows= (zmm2ps(restsize-(p_private->p_first_print?y():0))-font().char_descender_size()-1)/font().fontsize();
        if (max_rows<p_private->p_valuelines.size())
         {
           p_private->number_displayrows=max_rows;
           p_private->p_fully_printed=false;
         }
       else
       {
            p_private->number_displayrows=p_private->p_valuelines.size();
           p_private->p_fully_printed=true;
       }
       list<hk_string>::iterator it=p_private->p_valuelines.begin();
       displayvalue="";
       int c=0;
       while (it!=p_private->p_valuelines.end() && c<p_private->number_displayrows)
       {
          if (c>0)displayvalue+="\n";
          displayvalue+=*it;
         ++c;
         ++it;
       }
       p_private->p_value=displayvalue;
       unsigned int oc=p_private->number_displayrows;
       while (oc>0 && p_private->p_valuelines.size()>oc)
        {
         p_private->p_valuelines.erase(p_private->p_valuelines.begin());
         --oc;
        }


    
    }
    hk_string r=replace(p_viewdata->p_beforedata);
   recodefunctiontype* rf=(p_report?p_report->recodefunction():NULL);
   

    r+=rf?rf(displayvalue,p_report):displayvalue;
    r+=replace(p_viewdata->p_afterdata);
   if (p_private->p_replacefunction!=NULL) r=p_private->p_replacefunction(this,r);
    if (p_private->p_datacountfunction!=NULL)set_counts_as(p_private->p_datacountfunction(this));
   if (p_report->page_header()!=p_section &&p_report->page_footer()!=p_section  )
       p_report->rowcount(counts_as());
    p_private->p_first_print=false;
    ++p_private->p_pages;
    return  r;

}

bool hk_reportdata::data_fully_printed(void)
{
  return p_private->p_fully_printed;
}

int hk_reportdata::pagescount()
{
   return p_private->p_pages;
}

void hk_reportdata::reset_data_fully_printed(void)
{
  p_private->p_fully_printed=false;
  p_private->p_offset=0;
  p_private->p_pages=0;
    
    
    hk_string r;
    hk_string b;
    if (!action_on_print())
    {
      report()->stop_execution();
      return ;
    }
    if (column())p_private->p_value=column()->asstring(true);
    b=replace(p_viewdata->p_data);
    hk_string displayvalue=p_viewdata->p_data;
    p_private->p_valuelines.clear();
    if (!linebreak()) 
      {
        p_private->p_valuelines.push_back(b); 
        displayvalue=b;
        if (report()->useencodingtab()) 
           font().register_string(b);// just needed to register the characters in encodingtab()
       p_private->p_value=displayvalue;
        p_private->number_displayrows=1;
      }
    else
    if (!font().text2lines((p_report->sizetype()==hk_presentation::relative?p_section->relativ2horizontal(width()):width())-ps2zmm(4), &p_private->p_valuelines, b))
        cerr <<" TEXT2LINES hat nicht funktioniert!"<<endl;
    else
    { //linebreak
       displayvalue="";
       int h=(p_private->p_first_print?y():0)+height();
       if (p_report->sizetype()==hk_presentation::relative)
           h=p_section->relativ2vertical(h);
       int restsize=p_report->periodic_when()-p_report->current_rowcount();
           p_private->number_displayrows=-1;
       if (h>=restsize)
         {
           p_private->number_displayrows=(zmm2ps(restsize-(p_private->p_first_print?y():0))-font().char_descender_size()-1)/font().fontsize();

         }
    
    }
  
  
  p_private->p_first_print=true;


}

void hk_reportdata::count(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::count");
#endif
    p_private->p_count++;
    if(column()==0) return;

    switch(column()->columntype())
    {
        case hk_column::integercolumn  :;
        case hk_column::auto_inccolumn :;
        case hk_column::smallintegercolumn:
        {
            hk_string s=column()->asstring(true);
            int r=atoi(s.c_str());
            if (datasource()->row_position()==0)
            {
                p_private->sum.integer=r;
                p_private->squaresum.integer=r*r;
            }
            else
            {
                p_private->sum.integer+=r;
                p_private->squaresum.integer+=r*r;
            }
            if (s.size()>0)
            {
                if (p_private->p_minmax_alreadyset)
                {
                    if (r<p_private->min.integer)p_private->min.integer=r;
                    if (r>p_private->max.integer)p_private->max.integer=r;
                }
                else
                {
                    p_private->min.integer=r;
                    p_private->max.integer=r;
                    p_private->p_minmax_alreadyset=true;
                }
            }

        };break;
        case  hk_column::floatingcolumn:;
        case  hk_column::smallfloatingcolumn:;
        {
             hk_string s=column()->asstring();
           double r=localestring2double(s);
            if (datasource()->row_position()==0)
            {
                p_private->sum.real=r;
                p_private->squaresum.real=r*r;
            }
            else
            {
                p_private->sum.real+=r;
                p_private->squaresum.real+=r*r;
            }
            if (s.size()>0)
            {
                if (p_private->p_minmax_alreadyset)
                {
                    if (r<p_private->min.real)p_private->min.real=r;
                    if (r>p_private->max.real)p_private->max.real=r;
                }
                else
                {
                    p_private->min.real=r;
                    p_private->max.real=r;
                    p_private->p_minmax_alreadyset=true;
                }
            }
        }
        default:;
    }
}


void hk_reportdata::set_runningcount(bool c,bool registerchange)
{
    p_private->p_runningcount=c;
    has_changed(registerchange);
}


bool hk_reportdata::runningcount(void)
{

    return p_private->p_runningcount;
}


void hk_reportdata::set_replacefunction(const hk_string& f,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::set_replacefunction:"+f);
#endif
    if (f==p_private->p_replacefunctionstring)return;
    datareplacelisttype::iterator it=p_datareplacefunctions.find(f);
    if (it==p_datareplacefunctions.end())
    {

        show_warningmessage(replace_all("%1",hk_translate("replacefunction '%1' not found"),f));
        p_private->p_replacefunction=NULL;
        p_private->p_replacefunctionstring="None";
        return;
    }
    data_replacefunctiontype* c=*it->second;

    p_private->p_replacefunction=c;
    p_private->p_replacefunctionstring=f;
    has_changed(registerchange);
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::set_replacefunction:"+f+" found and set");
#endif
}


void hk_reportdata::set_datacountfunction(const hk_string& f,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::set_datacountfunction");
#endif
    if (f==p_private->p_datacountfunctionstring)return;

    datacountlisttype::iterator it=p_datacountfunctions.find(f);
    if (it==p_datacountfunctions.end())
    {

        show_warningmessage(hk_translate("Data Countfunction not found"));
        p_private->p_datacountfunction=NULL;
        p_private->p_datacountfunctionstring="None";
        return;
    }
    reportdatacounttype* c=*it->second;

    p_private->p_datacountfunction=c;
    p_private->p_datacountfunctionstring=f;
    has_changed(registerchange);
}


hk_string hk_reportdata::datacountfunctionstring(void)
{
    return p_private->p_datacountfunctionstring;

}


reportdatacounttype* hk_reportdata::datacountfunction(void)
{

    return p_private->p_datacountfunction;
}


bool    hk_reportdata::diagonalloru(void)
{
        if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_loru;
	else return p_viewdata->p_loru;
}


bool    hk_reportdata::topline(void)
{
        if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_top;
	else return p_viewdata->p_top;
;
}


bool    hk_reportdata::bottomline(void)
{
        if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_bottom;
	else return p_viewdata->p_bottom;

}


bool    hk_reportdata::leftline(void)
{
            if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_left;
	else return p_viewdata->p_left;
}


bool    hk_reportdata::rightline(void)
{
            if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_right;
	else return p_viewdata->p_right;
}


bool    hk_reportdata::diagonalluro(void)
{
            if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_luro;
	else return p_viewdata->p_luro;
}


bool    hk_reportdata::linebreak(void)
{
        if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_linebreak;
	else return p_viewdata->p_linebreak;
}


void hk_reportdata::set_dynamic_height(bool b,bool registerchange)
{
        if (p_report->mode()==hk_presentation::designmode) p_designdata->p_dynamic_height=b;
    p_viewdata->p_dynamic_height=b;
    if(p_private->p_dataconfigurefunction!=NULL)p_private->p_dataconfigurefunction(this);
    has_changed(registerchange);
    
}

bool    hk_reportdata::dynamic_height(void) const
{
        if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_dynamic_height;
	else return p_viewdata->p_dynamic_height;
}


void hk_reportdata::reset_count()
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::reset_count");
#endif
    if (!p_private->p_runningcount)
    {
      clear_counting();
    }
}

void hk_reportdata::clear_counting(void)
{
        p_private->p_count=0;
        if (column()!=NULL)
            if (column()->columntype()==hk_column::integercolumn||
            column()->columntype()==hk_column::smallintegercolumn||
            column()->columntype()==hk_column::auto_inccolumn)
        {
            p_private->sum.integer=0;
            p_private->squaresum.integer=0;
            p_private->min.integer=0;
            p_private->max.integer=0;
            p_private->p_minmax_alreadyset=false;
        }
        else
        if (column()->columntype()==hk_column::floatingcolumn||
            column()->columntype()==hk_column::smallfloatingcolumn)
        {
            p_private->sum.real=0;
            p_private->squaresum.real=0;
            p_private->min.real=0;
            p_private->max.real=0;
            p_private->p_minmax_alreadyset=false;
        }


}


hk_font hk_reportdata::font(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::font");
#endif
    hk_font f;
    if (hk_dsdatavisible::font().fontname().size()==0) f= p_section->font();
    else f= hk_dsdatavisible::font();
    f.set_encodingtab(p_report->masterreport()?p_report->masterreport()->encodingtab():p_report->encodingtab());
    return f;
}




void    hk_reportdata::set_topline(bool l,bool registerchange)
{
    if (p_report->mode()==hk_presentation::designmode) p_designdata->p_top=l;

    p_viewdata->p_top=l;
    if(p_private->p_dataconfigurefunction!=NULL)p_private->p_dataconfigurefunction(this);
    has_changed(registerchange);
}


void    hk_reportdata::set_bottomline(bool l,bool registerchange)
{
    if (p_report->mode()==hk_presentation::designmode) p_designdata->p_bottom=l;
    p_viewdata->p_bottom=l;

    if(p_private->p_dataconfigurefunction!=NULL)p_private->p_dataconfigurefunction(this);
    has_changed(registerchange);

}


void    hk_reportdata::set_leftline(bool l,bool registerchange)
{
    if (p_report->mode()==hk_presentation::designmode) p_designdata->p_left=l;
    p_viewdata->p_left=l;

    if(p_private->p_dataconfigurefunction!=NULL)p_private->p_dataconfigurefunction(this);
    has_changed(registerchange);
}


void    hk_reportdata::set_rightline(bool l,bool registerchange)
{
    if (p_report->mode()==hk_presentation::designmode) p_designdata->p_right=l;
    p_viewdata->p_right=l;
    has_changed(registerchange);
    if(p_private->p_dataconfigurefunction!=NULL)p_private->p_dataconfigurefunction(this);

}


void    hk_reportdata::set_diagonalluro(bool l,bool registerchange)
{
    if (p_report->mode()==hk_presentation::designmode) p_designdata->p_luro=l;
    p_viewdata->p_luro=l;
    if(p_private->p_dataconfigurefunction!=NULL)p_private->p_dataconfigurefunction(this);
    has_changed(registerchange);
}


void    hk_reportdata::set_diagonalloru(bool l,bool registerchange)
{
    if (p_report->mode()==hk_presentation::designmode) p_designdata->p_loru=l;
    p_viewdata->p_loru=l;
    if(p_private->p_dataconfigurefunction!=NULL)p_private->p_dataconfigurefunction(this);
    has_changed(registerchange);
}


void     hk_reportdata::set_frame(bool l,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::set_frame");
#endif
    if (p_report->mode()==hk_presentation::designmode)
    {
      p_designdata->p_top=l;
      p_designdata->p_left=l;
      p_designdata->p_right=l;
      p_designdata->p_bottom=l;
    }
    p_viewdata->p_top=l;
    p_viewdata->p_bottom=l;
    p_viewdata->p_left=l;
    p_viewdata->p_right=l;
    if(p_private->p_dataconfigurefunction!=NULL)p_private->p_dataconfigurefunction(this);
    has_changed(registerchange);
}


void hk_reportdata::set_linebreak(bool b,bool registerchange)
{
        if (p_report->mode()==hk_presentation::designmode) p_designdata->p_linebreak=b;
    p_viewdata->p_linebreak=b;
    if(p_private->p_dataconfigurefunction!=NULL)p_private->p_dataconfigurefunction(this);
    has_changed(registerchange);

    
    
}




void  hk_reportdata::savedata(ostream& s,bool userdefined)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::savedata");
#endif
    start_mastertag(s,"HK_REPORTDATA");
    hk_dsdatavisible::savedata(s);
    set_tagvalue(s,"DATAVALUE",p_designdata->p_data);
    set_tagvalue(s,"DISPLAYNAME",p_designdata->p_displayname);
    set_tagvalue(s,"TOPBORDER",p_designdata->p_top);
    set_tagvalue(s,"LEFTBORDER",p_designdata->p_left);
    set_tagvalue(s,"RIGHTBORDER",p_designdata->p_right);
    set_tagvalue(s,"BOTTOMBORDER",p_designdata->p_bottom);
    set_tagvalue(s,"DIAGONALLORU",p_designdata->p_loru);
    set_tagvalue(s,"DIAGONALLURO",p_designdata->p_luro);
    set_tagvalue(s,"WORDBREAK",p_designdata->p_linebreak);
    set_tagvalue(s,"IS_IMAGE",p_designdata->p_is_image);
    set_tagvalue(s,"DYNAMIC_HEIGHT",p_designdata->p_dynamic_height);
    set_tagvalue(s,"RUNNINGCOUNT",p_private->p_runningcount);
    set_tagvalue(s,"ONPRINT_ACTION",p_private->p_on_print_action);

    if (userdefined)
    {
        set_tagvalue(s,"BEFOREDATA",p_designdata->p_beforedata);
        set_tagvalue(s,"AFTERDATA",p_designdata->p_afterdata);
        set_tagvalue(s,"DATACONFIGUREFUNCTION",p_private->p_dataconfigurefunctionstring);
        set_tagvalue(s,"DATACOUNTFUNCTION",p_private->p_datacountfunctionstring);
        set_tagvalue(s,"DATAREPLACEFUNCTION",p_private->p_replacefunctionstring);
    }

    end_mastertag(s,"HK_REPORTDATA");

}


void  hk_reportdata::loaddata(xmlNodePtr definition,bool userdefined)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::loaddata");
#endif
    hk_dsdatavisible::loaddata(definition);
    get_tagvalue(definition,"DATAVALUE",p_designdata->p_data);
    p_viewdata->p_data=p_designdata->p_data;
    hk_string sbuffer;
    if (userdefined)
    {
        get_tagvalue(definition,"BEFOREDATA",p_designdata->p_beforedata);
	p_viewdata->p_beforedata=p_designdata->p_beforedata;
        get_tagvalue(definition,"AFTERDATA",p_designdata->p_afterdata);
	p_viewdata->p_afterdata=p_designdata->p_afterdata;
        if (get_tagvalue(definition,"DATACONFIGUREFUNCTION",sbuffer))
            set_configurefunction(sbuffer);
        if (get_tagvalue(definition,"DATACOUNTFUNCTION",sbuffer))
            set_datacountfunction(sbuffer,false);
        if (get_tagvalue(definition,"DATAREPLACEFUNCTION",sbuffer))
            set_replacefunction(sbuffer);

    }
    get_tagvalue(definition,"DISPLAYNAME",p_designdata->p_displayname);
    get_tagvalue(definition,"TOPBORDER",p_designdata->p_top);
    get_tagvalue(definition,"LEFTBORDER",p_designdata->p_left);
    get_tagvalue(definition,"RIGHTBORDER",p_designdata->p_right);
    get_tagvalue(definition,"BOTTOMBORDER",p_designdata->p_bottom);
    get_tagvalue(definition,"DIAGONALLORU",p_designdata->p_loru);
    get_tagvalue(definition,"DIAGONALLURO",p_designdata->p_luro);
    get_tagvalue(definition,"WORDBREAK",p_designdata->p_linebreak);
    get_tagvalue(definition,"RUNNINGCOUNT",p_private->p_runningcount);
    get_tagvalue(definition,"DYNAMIC_HEIGHT",p_designdata->p_dynamic_height);
    get_tagvalue(definition,"ONPRINT_ACTION",p_private->p_on_print_action);
    get_tagvalue(definition,"IS_IMAGE",p_designdata->p_is_image);
    *p_viewdata=*p_designdata;

    hk_string configurefunc;
    get_tagvalue(definition,"DATACONFIGUREFUNCTION",configurefunc);
    if  (configurefunc=="POSTSCRIPT") p_private->p_dataconfigurefunction=&configure_postscriptdata;
    if(p_private->p_dataconfigurefunction!=NULL)p_private->p_dataconfigurefunction(this);
    

}


void    hk_reportdata::set_configurefunction(const hk_string& f,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::set_configurefunction:"+f);
#endif

    if (f==p_private->p_dataconfigurefunctionstring)return;

    reportdataconfigurelisttype::iterator it=p_reportdataconfigurefunctions.find(f);
    if (it==p_reportdataconfigurefunctions.end())
    {

        show_warningmessage(hk_translate("Dataconfigurefunction not found"));
        p_private->p_dataconfigurefunction=NULL;
        p_private->p_dataconfigurefunctionstring="None";
        return;
    }

    data_configurefunctiontype* rf=*it->second;
    p_private->p_dataconfigurefunction=rf;
    p_private->p_dataconfigurefunctionstring=f;
    if (p_private->p_dataconfigurefunction!=NULL) p_private->p_dataconfigurefunction(this);
    has_changed(registerchange);
}


data_configurefunctiontype* hk_reportdata::configurefunction(void)
{
    return p_private->p_dataconfigurefunction;
}


hk_string hk_reportdata::configurefunctionstring(void)
{
    return p_private->p_dataconfigurefunctionstring;
}


void    hk_reportdata::neutralize_definition(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportdata::neutralize_definition");
#endif
    set_beforedata("",registerchange);
    set_afterdata("",registerchange);
    set_replacefunction("None",registerchange);
    set_datacountfunction("None",registerchange);
    set_configurefunction("None",registerchange);

}


void hk_reportdata::add_configurefunctiontype(const hk_string& name,data_configurefunctiontype* f)
{
    if (name.size()==0) return;
    typedef reportdataconfigurelisttype::value_type recodevalue;
    p_reportdataconfigurefunctions.insert(recodevalue(name,f));
    p_reportdataconfigurelist.insert(p_reportdataconfigurelist.end(),name);

}


list<hk_string>* hk_reportdata::configurefunctionlist(void)
{
    return &p_reportdataconfigurelist;
}


void hk_reportdata::add_datacountfunctiontype(const hk_string& name,reportdatacounttype* f)
{

    if (name.size()==0) return;
    typedef datacountlisttype::value_type datacountvalue;
    p_datacountfunctions.insert(datacountvalue(name,f));
    p_datacountfunctionlist.insert(p_datacountfunctionlist.end(),name);
}


list<hk_string>* hk_reportdata::datacountfunctionlist(void)
{
    return &p_datacountfunctionlist;
}


void hk_reportdata::add_datareplacefunctiontype(const hk_string& name,data_replacefunctiontype* f)
{
    if (name.size()==0) return;
    typedef datareplacelisttype::value_type datareplacevalue;
    p_datareplacefunctions.insert(datareplacevalue(name,f));
    p_datareplacefunctionlist.insert(p_datareplacefunctionlist.end(),name);
}


list<hk_string>* hk_reportdata::datareplacefunctionlist(void)
{
    return &p_datareplacefunctionlist;
}


hk_report* hk_reportdata::report(void)
{
    return p_report;
}


hk_reportsection* hk_reportdata::section(void)
{
    return p_section;
}


void hk_reportdata::new_column_pointer_created(void)
{
    hk_column* col=column();
    if (col)
    {
        if (is_integertype(col)) {p_private->sum.integer=0;p_private->min.integer=0;p_private->max.integer=0;p_private->squaresum.integer=0;}
        else if (is_realtype(col)){p_private->sum.real=0;p_private->min.real=0;p_private->max.real=0;p_private->squaresum.real=0;}
    }

}


void hk_reportdata::lower_widget(bool registerchange)
{
    if (p_section) p_section->lower_widget(this);
    has_changed(registerchange);
    widget_specific_lower_widget();
}


void hk_reportdata::raise_widget(bool registerchange)
{
    if (p_section) p_section->raise_widget(this);
    has_changed(registerchange);
    widget_specific_raise_widget();

}


void hk_reportdata::sizetype_changed(void)
{
 if (!p_report) return;

p_setwidgetcoordinates=true;
 if (p_report->sizetype()==hk_presentation::relative)
  {
// this means previous sizetype was hk_presentation::absolute
// and changed now to hk_presentation::relative

   set_size(p_section->horizontal2relativ(x()),
   		p_section->vertical2relativ(y()),
		p_section->horizontal2relativ(width()),
		p_section->vertical2relativ(height()),
		false
		);


  }
else
  {
// this means previous sizetype was hk_presentation::relative
// and changed now to hk_presentation::absolute

   set_size(p_section->relativ2horizontal(x()),
   		p_section->relativ2vertical(y()),
		p_section->relativ2horizontal(width()),
		p_section->relativ2vertical(height()),
		false
		);

  }
p_setwidgetcoordinates=false;
}


bool hk_reportdata::presentationmode_changed(void)
{
bool r=hk_dsdatavisible::presentationmode_changed();
    if (p_report->mode()==hk_presentation::viewmode)
    {
        p_viewdata->p_data=p_designdata->p_data;
        p_viewdata->p_beforedata=p_designdata->p_beforedata;
        p_viewdata->p_afterdata=p_designdata->p_afterdata;
        p_viewdata->p_displayname=p_designdata->p_displayname;
	p_viewdata->p_top=p_designdata->p_top;
	p_viewdata->p_bottom=p_designdata->p_bottom;
	p_viewdata->p_left=p_designdata->p_left;
	p_viewdata->p_right=p_designdata->p_right;
	p_viewdata->p_luro=p_designdata->p_luro;
	p_viewdata->p_loru=p_designdata->p_loru;
   }
 return r;
}


void hk_reportdata::set_on_print_action(const hk_string& a,bool registerchange)
{
 p_private->p_on_print_action=a;
 has_changed(registerchange);
}

hk_string hk_reportdata::on_print_action(void)
{
return p_private->p_on_print_action;
}

bool hk_reportdata::action_on_print(void)
{
if (p_private->p_on_print_action.size()==0) return true;
 if (!p_report)return false;
	return p_report->interpreter()->on_print_data(this);
}


hk_string hk_reportdata::beforedata()
{
     if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_beforedata;
     else return p_viewdata->p_beforedata;
}

hk_string hk_reportdata::afterdata()
{
     if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_afterdata;
     else return p_viewdata->p_afterdata;
}


list<hk_string>*  hk_reportdata::value_in_lines()
{
  return &p_private->p_valuelines;
}


unsigned int    hk_reportdata::height(void) 
{
 unsigned int h=hk_dsdatavisible::height();
 if (dynamic_height() && p_report->mode()==hk_presentation::viewmode )
 {  
   unsigned int n=ps2zmm(font().fontsize()*p_private->number_displayrows+font().char_descender_size()+1);
   if (n>h)h=n;
 }
//  hk_font f=font();
//  cerr <<"font:"<<f.fontname()<<" descender:"<<f.char_descender_size()<<endl;
 return h;
}



hk_string hk_reportdata::tag_value(int tagnumber, bool& ok)
{
  ok=false;
    if (!p_report->datasource()) return "";
    ok=true;


   int w=0;

    hk_string result;

      if (tagnumber==p_private->tag_XPOS)
    {
       return (p_report->sizetype()==hk_presentation::relative?longint2string(p_section->relativ2horizontal(x())):longint2string(x()));
     }
      if (tagnumber==p_private->tag_YPOS)
    {
     if (!p_private->p_first_print)
      return longint2string(0);
     else
      return (p_report->sizetype()==hk_presentation::relative?longint2string(p_section->relativ2vertical(y())):longint2string(y()));
    }
    
    
      if (tagnumber==p_private->tag_WIDTH)
    {
    return (p_report->sizetype()==hk_presentation::relative?longint2string(p_section->relativ2horizontal(width())):longint2string(width()));
    }
    w=p_report->sizetype()==hk_presentation::relative?p_section->relativ2horizontal(width()):width();
    
    if (tagnumber==p_private->tag_RELXPOS)
    {
     return (p_report->sizetype()==hk_presentation::relative?longint2string(x()):longint2string(p_section->horizontal2relativ(x())));
    }
    
    
    if (tagnumber==p_private->tag_RELYPOS)
    {
    return (p_report->sizetype()==hk_presentation::relative?longint2string(y()):longint2string(p_section->vertical2relativ(y())));
    }
    
        if (tagnumber==p_private->tag_RELWIDTH)
    {
      return (p_report->sizetype()==hk_presentation::relative?longint2string(width()):longint2string(p_section->horizontal2relativ(width())));
    }
    
        if (tagnumber==p_private->tag_RELHEIGHT)
    {

    return (p_report->sizetype()==hk_presentation::relative?longint2string(height()):longint2string(p_section->vertical2relativ(height())));
    }
    
    
    if (tagnumber==p_private->tag_PAGENUMBER)
    {
    return (format_number(p_report->pagenumber(),use_numberseparator(),(commadigits()<0?0:commadigits())));
    }
    
    if (tagnumber==p_private->tag_ABSOLUTEPAGENUMBER)
    {
    return (format_number(p_report->absolutepagenumber(),use_numberseparator(),(commadigits()<0?0:commadigits())));
    }
    if (tagnumber==p_private->tag_ROWNUMBER)
    {
    
    return (format_number(p_report->rownumber(),use_numberseparator(),(commadigits()<0?0:commadigits())));
    }
    if (tagnumber==p_private->tag_ALIGN)
    {
    hk_string align;
    switch (alignment())
    {
        case alignright : align="RIGHT";break;
        case aligncenter : align="CENTER";break;
        case alignleft         : align="LEFT";break;
        default:  if (is_numerictype(column())) align="RIGHT";
        	  else align="LEFT";
    };

    return align;
    }
    
    if (column()!=NULL)
    {
      if (tagnumber==p_private->tag_COUNT)
      {
        return (format_number(p_private->p_count,use_numberseparator(),(commadigits()<0?0:commadigits())));
      }
        hk_string format="%.3f";
        switch  (column()->columntype())
        {
            case  hk_column::integercolumn: ;
            case hk_column::auto_inccolumn: ;
            case hk_column::smallintegercolumn :
            {
                   if (tagnumber==p_private->tag_SUM)
    			{
 			  return (format_number(p_private->sum.integer,use_numberseparator(),(commadigits()<0?0:commadigits())));
                	}
                   if (tagnumber==p_private->tag_MIN)
    			{
 			 return (format_number(p_private->min.integer,use_numberseparator(),(commadigits()<0?0:commadigits())));
                	}
   		   if (tagnumber==p_private->tag_MAX)
                    {
                        return (format_number(p_private->max.integer,use_numberseparator(),(commadigits()<0?0:commadigits())));
                    }
                if (p_private->p_count>0)
                {
                    double average=(double)p_private->sum.integer/(double)p_private->p_count;
   			if (tagnumber==p_private->tag_AVERAGE)
    			{
                         return (format_number(average,use_numberseparator(),commadigits()));
                        }
                    double stddev=sqrt((p_private->squaresum.integer-2.0*average*p_private->sum.integer+p_private->p_count*average*average)/p_private->p_count);
                       if (tagnumber==p_private->tag_STDDEV)
    			{
 
                          return (format_number(stddev,use_numberseparator(),commadigits()));
                        }
                       if (tagnumber==p_private->tag_STDDEVSAMPLE)
    			{
 
                    stddev=sqrt((p_private->squaresum.integer-2.0*average*p_private->sum.integer+p_private->p_count*average*average)/(p_private->p_count-1));
                    return (format_number(stddev,use_numberseparator(),commadigits()));
                        }
                }
            };break;

            case hk_column::floatingcolumn:  ;
            case hk_column::smallfloatingcolumn:
            {
                   if (tagnumber==p_private->tag_SUM)
    		{
 			return (format_number(p_private->sum.real,use_numberseparator(),commadigits())); //cerr <<"sum="<<p_private->sum.real<<endl;
 		}
                   if (tagnumber==p_private->tag_MIN)
    		{
 			return (format_number(p_private->min.real,use_numberseparator(),commadigits()));
                }
                   if (tagnumber==p_private->tag_MAX)
    		{
 
                return (format_number(p_private->max.real,use_numberseparator(),commadigits()));
                }
                if (p_private->p_count>0)
                {
 			double average=p_private->sum.real/(double)p_private->p_count;
                       if (tagnumber==p_private->tag_AVERAGE)
    			{
                        return (format_number(average,use_numberseparator(),commadigits()));
                        }
                    
                       if (tagnumber==p_private->tag_STDDEV)
    			{
 			double stddev=sqrt((p_private->squaresum.real-2*average*p_private->sum.real+p_private->p_count*average*average)/p_private->p_count);
                        return (format_number(stddev,use_numberseparator(),commadigits()));
                        }
                    
                       if (tagnumber==p_private->tag_STDDEVSAMPLE)
    			{
 			  double stddev=sqrt((p_private->squaresum.real-2*average*p_private->sum.real+p_private->p_count*average*average)/(p_private->p_count-1));
                    	  return (format_number(stddev,use_numberseparator(),commadigits()));
			}
                }
                hk_string i="?";
                   if (tagnumber==p_private->tag_AVERAGE)
                   return i;
                   if (tagnumber==p_private->tag_STDDEV)
                   return i;
                   if (tagnumber==p_private->tag_STDDEVSAMPLE)
                   return i;
            } break;
            default:;

        }

        if (p_report!=NULL)
        {
            hk_string val, tstvalue;
            hk_string name;
            hk_string display;

            if (!column()->is_nullvalue())
            {
             if (is_numerictype(column()))
             {
                double n=localestring2double(column()->asstring(true));
                val=format_number(n,use_numberseparator(),commadigits()<0?(is_integertype(column())?0:-1):commadigits(),p_report->filelocale());
             }
             else   val=p_private->p_value;
	    }
             name=columnname();
             display=p_viewdata->p_displayname;
             tstvalue=val;
   	   if (tagnumber==p_private->tag_VALUE)
   	   {
             
             return val;
           } 
             if (tagnumber==p_private->tag_FIELDNAME)
            return name;
            
   	   if (tagnumber==p_private->tag_DISPLAYNAME)
             return display;

        
        }

    }
    
       if (tagnumber==p_private->tag_HEIGHT)
       return (p_report->sizetype()==hk_presentation::relative?longint2string(p_section->relativ2vertical(height())):longint2string(height()));

       if (tagnumber==p_private->tag_FONT)
        return font().fontname();
       if (tagnumber==p_private->tag_PSFONT)
       return font().psfontname();
       if (tagnumber==p_private->tag_FONTSIZE)
       return longint2string(font().fontsize());
    ok=false;
    return hk_dsdatavisible::tag_value(tagnumber,ok);


}



hk_string hk_reportdata::column_value(const hk_string& colname,bool& ok)
{
ok=false;

 if (datasource())
 {
  hk_column* c=datasource()->column_by_name(colname);
  if (c)
  {
    ok=true;
    return c->asstring();
  }
 }
return "";
}


void    hk_reportdata::set_is_image(bool  img,bool registerchange)
{
   p_viewdata->p_is_image=img;
    if (p_report->mode()==hk_presentation::designmode) p_designdata->p_is_image=img;
    has_changed(registerchange);
}

bool    hk_reportdata::is_image(void)
{
     if (p_report->mode()==hk_presentation::designmode) return p_designdata->p_is_image;
     else return p_viewdata->p_is_image;
}


