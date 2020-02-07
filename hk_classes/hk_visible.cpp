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
//$Revision: 1.92 $
#include "hk_visible.h"
#include "hk_presentation.h"
#include "hk_form.h"
#include "hk_report.h"
#include "hk_datetime.h"
#include "hk_interpreter.h"
#include "hk_tabvisible.h"
#include "hk_label.h"

class hk_visiblemodeprivate
{
  public:
hk_visiblemodeprivate()
{
p_buddylabel=-1;
p_enabled=true;
}
        unsigned int    p_x,p_y,p_width,p_height;
        hk_string p_datetimeformat;
        hk_string p_dateformat;
        hk_string p_timeformat;
        unsigned long int p_counts_as;
        bool p_load_presentationnumber;
        bool p_enabled;
        hk_visible::alignmenttype    p_align;
        hk_colour   p_foregroundcolour;
        hk_colour   p_backgroundcolour;
	hk_font     p_font;
        hk_string       p_label;
        hk_string p_clickaction,p_doubleclickaction, p_onopenaction,p_oncloseaction;
	hk_string p_ongetfocusaction,p_onloosefocusaction,p_onkeyaction;
	hk_string p_tooltip;
	long int p_buddylabel;
	bool  operator=(const hk_visiblemodeprivate* v)
     {
		if (!v) return false;
		p_x=v->p_x;
		p_y=v->p_y;
		p_width=v->p_width;
		p_height=v->p_height;
		p_datetimeformat=v->p_datetimeformat;
		p_dateformat=v->p_dateformat;
		p_timeformat=v->p_timeformat;
		p_counts_as=v->p_counts_as;
		p_load_presentationnumber=v->p_load_presentationnumber;
        	p_align=v->p_align;
        	p_foregroundcolour=v->p_foregroundcolour;
        	p_backgroundcolour=v->p_backgroundcolour;
		p_font=v->p_font;
        	p_label=v->p_label;
		p_clickaction=v->p_clickaction;
		p_doubleclickaction=v->p_doubleclickaction;
		p_onopenaction=v->p_onopenaction;
		p_oncloseaction=v->p_oncloseaction;
		p_ongetfocusaction=v->p_ongetfocusaction;
		p_onloosefocusaction=v->p_onloosefocusaction;
		p_onkeyaction=v->p_onkeyaction;
		p_tooltip=v->p_tooltip;
		p_buddylabel=v->p_buddylabel;
		p_enabled=v->p_enabled;
        return true;
     }

};


typedef map<hk_string,int,less<hk_string> > tagtype;

class hk_visibleprivate
{
public:
    hk_visibleprivate()
    {
      p_tabvisible=NULL;
      p_movebuddylabel=true;
    }
	hk_string p_identifier;
        bool        p_neverregisterchange;
	hk_key p_key;
	hk_tabvisible* p_tabvisible;
	bool p_movebuddylabel;
	tagtype p_tags;
	int tag_counts_as,tag_time,tag_date,tag_fgred,tag_fggreen,tag_fgblue,tag_bgred,tag_bggreen,tag_bgblue;

};

hk_visible::alignmenttype hk_visible::p_defaulttextalign=hk_visible::alignleft;
hk_visible::alignmenttype hk_visible::p_defaultnumberalign=hk_visible::alignright;
bool hk_visible::p_open_maximized_windows=false;

hk_visible::hk_visible(hk_presentation* p):hk_class()
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::hk_visible");
#endif
p_designdata=new hk_visiblemodeprivate;
p_viewdata=new hk_visiblemodeprivate;
p_private=new hk_visibleprivate;
    p_visibletype=other;
    p_presentation=p;
    p_setcoordinates=p_setwidgetcoordinates=false;
    set_size(0,0,1000,300,false);
    p_private->p_neverregisterchange=false;
    p_vupn=-1;
    p_designdata->p_counts_as=0;
    p_designdata->p_dateformat=p_defaultdateformat;
    p_designdata->p_timeformat=p_defaulttimeformat;
    p_designdata->p_datetimeformat=p_defaultdatetimeformat;
    p_designdata->p_load_presentationnumber=true;
    p_designdata->p_align=alignleft;
    set_alignment(alignleft,false);
    p_designdata->p_foregroundcolour=hk_black;
    p_designdata->p_backgroundcolour=hk_grey;
   *p_viewdata=*p_designdata;
   p_private->tag_counts_as=register_tag("COUNTS_AS");
   p_private->tag_time=register_tag("TIME");
   p_private->tag_date=register_tag("DATE");
   p_private->tag_fgred=register_tag("FGRED");
   p_private->tag_fggreen=register_tag("FGGREEN");
   p_private->tag_fgblue=register_tag("FGBLUE");
   p_private->tag_bgred=register_tag("BGRED");
   p_private->tag_bggreen=register_tag("BGGREEN");
   p_private->tag_bgblue=register_tag("BGBLUE");

}


hk_visible::~hk_visible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::destructor");
#endif
 //   if (p_presentation==NULL) return;

    hk_form* f=dynamic_cast<hk_form*>(p_presentation);
    if (f!=NULL)
    {
        f->remove_visible(this);
    }

    delete p_designdata;
    delete p_viewdata;
    delete p_private;

}


void hk_visible::set_size(unsigned int xv, unsigned int yv,unsigned int wv, unsigned int hv, bool registerchange)
{
   set_size(xv,yv,wv,hv,registerchange,false);
}

void hk_visible::set_size(unsigned int xv, unsigned int yv,unsigned int wv, unsigned int hv, bool registerchange,bool force_setting)
{
#ifdef HK_DEBUG
    hkdebug("hk_visible:set_size");
#endif
    if (p_setcoordinates==true)
    {
#ifdef HK_DEBUG
        hkdebug("hk_visible:set_size psetcoordinates== true !!!");
#endif
        return;
    }

    p_setcoordinates=true;
    unsigned int ppx=xv;
    unsigned int ppy=yv;
    unsigned int ppw=wv;
    unsigned int pph=hv;
    unsigned int usex=xv;
    unsigned int usey=yv;
    unsigned int usew=wv;
    unsigned int useh=hv;
    if (p_presentation!=NULL&&p_presentation->sizetype()==hk_presentation::relative)
    {
        if (ppw<100)
        {
            ppw=100;                              //cout <<"ppwminanpassung"<<endl;
        }
        if (pph<100)
        {
            pph=100;                              //cout <<"pphminanpassung"<<endl;
        }
        if (ppx>9950)
        {
            ppx=9950;                             //cout <<"ppxmaxanpassung"<<endl;
        }
        if (ppx+ppw>10000)
        {
            ppw=10000-ppx;                        //cout <<"ppwmaxanpassung"<<endl;
        }
        if (ppy>9950)
        {
            ppy=9950;                             //cout <<"ppymaxanpassung"<<endl;
        }
        if (ppy+pph>10000)
        {
            pph=10000-ppy;                        //cout <<"pphmaxanpassung"<<endl;
        }
        usex=ppx;
        usey=ppy;
        usew=ppw;
        useh=pph;
        ppx=p_presentation->relativ2horizontal(ppx);
        ppy=p_presentation->relativ2vertical(ppy);
        pph=p_presentation->relativ2vertical(pph);
        ppw=p_presentation->relativ2horizontal(ppw);

    }

  hk_label* l=NULL;
   hk_form* f=dynamic_cast<hk_form*>(p_presentation);
    if (f!=NULL && buddylabel()>-1)
    {
       l=dynamic_cast<hk_label*>(f->get_visible(buddylabel()));
    }

    if (p_setwidgetcoordinates)
    {
#ifdef HK_DEBUG
        hkdebug("hk_visible:set_size p_setwidgetcoordinates ==true");
#endif
    if (allow_datachanging(force_setting))
    {
        p_designdata->p_x=usex;
        p_designdata->p_y=usey;
        p_designdata->p_width=usew;
        p_designdata->p_height=useh;
    }
     if (l && p_private->p_movebuddylabel)
     {
       int dx=usex-p_viewdata->p_x;
       int dy=usey-p_viewdata->p_y;
       int nx=l->x()+dx;
       if (nx<0)nx=0;
       int ny=l->y()+dy;
       if (ny<0)ny=0;
       if (p_presentation)
       {
          if (nx+l->width()>p_presentation->designwidth()) nx=(p_presentation->designwidth()-l->width());
          if (ny+l->height()>p_presentation->designheight()) ny=(p_presentation->designheight()-l->height());
       }
       l->set_position(nx,ny);

     }

	p_viewdata->p_x=usex;
        p_viewdata->p_y=usey;
        p_viewdata->p_width=usew;
        p_viewdata->p_height=useh;
    }
    else
    if(widget_specific_coordinates(ppx,ppy,ppw,pph))
    {
    if (allow_datachanging(force_setting))
    {
        p_designdata->p_x=usex;
        p_designdata->p_y=usey;
        p_designdata->p_width=usew;
        p_designdata->p_height=useh;
    }
	p_viewdata->p_x=usex;
        p_viewdata->p_y=usey;
        p_viewdata->p_width=usew;
        p_viewdata->p_height=useh;
        if (p_presentation!=NULL) p_presentation->widget_specific_fieldresize(this);
    }
    has_changed(registerchange);
    p_setcoordinates=false;
}

hk_font hk_visible::font(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_font;
return p_designdata->p_font;

}

 bool hk_visible::neverregisterchange(void)
 {
      return p_private->p_neverregisterchange;
 }


long  hk_visible::presentationnumber(void) const
{
  return p_vupn;
 }

void hk_visible::set_x(unsigned int px, bool registerchange)
{
    set_size(px,y(),width(),height(),registerchange);
}


void hk_visible::set_y(unsigned int py, bool registerchange)
{
    set_size(x(),py,width(),height(),registerchange);
}


void hk_visible::set_width(unsigned int pwidth, bool registerchange)
{
    set_size(x(),y(),pwidth,height(),registerchange);
}


void hk_visible::set_size(unsigned int w, unsigned int h, bool registerchange)
{
    set_size(x(),y(),w,h,registerchange);
}


void hk_visible::set_position(unsigned int xv, unsigned int yv, bool registerchange)
{
    set_size(xv,yv,width(),height(),registerchange);
}


void hk_visible::set_height(unsigned int pheight, bool registerchange)
{
    set_size(x(),y(),width(),pheight,registerchange);
}


unsigned int hk_visible::x() const
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_x;
	return p_designdata->p_x;
}


unsigned int hk_visible::y() const
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_y;

    return p_designdata->p_y;
}


unsigned int hk_visible::width() const
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_width;

    return p_designdata->p_width;
}


unsigned int hk_visible::height() 
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_height;

    return p_designdata->p_height;
}


bool    hk_visible::widget_specific_coordinates(unsigned int /*px*/,unsigned int /*py*/,unsigned int /*pwidth*/,unsigned int /*pheight*/)
{
    return true;
}


void    hk_visible::widget_specific_font_changed(void)
{

}


void  hk_visible::widget_specific_form_resizes(void)
{
    set_size(p_designdata->p_x,p_designdata->p_y,p_designdata->p_width,p_designdata->p_height);
}




void hk_visible::set_label(const hk_string& l, bool registerchange)
{
  set_label(l,registerchange,false);
}


void hk_visible::set_label(const hk_string& l, bool registerchange,bool force_setting)
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::set_label(hk_string)");
#endif
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_label=l;
    }
    p_viewdata->p_label=l;
    has_changed(registerchange);
    widget_specific_label_changed();
}


hk_string hk_visible::label(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_label;


    return p_designdata->p_label;
}


void hk_visible::set_enabled(bool e, bool registerchange,bool force_setting)
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::set_label(hk_string)");
#endif
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_enabled=e;
    }
    p_viewdata->p_enabled=e;
    has_changed(registerchange);
    widget_specific_enabled_changed();

}

bool hk_visible::is_enabled(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_enabled;
return p_designdata->p_enabled;

}

bool hk_visible::set_identifier(const hk_string& i,bool registerchange,enum_interaction a)
{
if (i==p_private->p_identifier)return true;
if(i.size()==0) return false;
hk_form* f=dynamic_cast<hk_form*> (p_presentation);
if (f &&f->get_visible(i))
  {
    if (a==interactive)show_warningmessage(replace_all("%1",hk_translate("Identifier '%1' already in use"),i));
    return false;
  }
  else
  {
  hk_report* r=dynamic_cast<hk_report*> (p_presentation);
  if (r &&r->get_reportdatavisible(i))
  {
    if (a==interactive)show_warningmessage(replace_all("%1",hk_translate("Identifier '%1' already in use"),i));
    return false;
  }


  }

p_private->p_identifier=i;
has_changed(registerchange);
return true;
}

hk_string hk_visible::identifier(void) const
{
return p_private->p_identifier;
}

void hk_visible::savedata(ostream& s )
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::savedata");
#endif
    hk_string tag="HK_VISIBLE";
    start_mastertag(s,tag);
    hk_string v="";
    switch (p_visibletype)
    {
        case   textlabel      :  v="TEXTLABEL";break;
        case button       :  v="BUTTON";break;
        case rowselector      :  v="SELECTOR";break;
        case boolean      :  v="BOOLEAN";break;
        case lineedit         :  v="LINEEDIT";break;
        case memo         :  v="MEMO";break;
        case combobox         :  v="COMBOBOX";break;
        case grid         :  v="GRID";break;
        case form         :  v="FORM";break;
        case subform         :  v="SUBFORM";break;
        case report       :  v="REPORT";break;
        case reportsection    :  v="REPORTSECTION";break;
        case reportdata   :  v="REPORTDATA";break;
        case image   :  v="IMAGE";break;
        case date   :  v="DATE";break;
        case tabvisible   :  v="TABVISIBLE";break;
        default               : v="UNKNOWN";
    } ;
    set_tagvalue(s,"VISIBLETYPE",v);
    set_tagvalue(s,"LABEL",p_designdata->p_label);
    set_tagvalue(s,"IDENTIFIER",p_private->p_identifier);
    set_tagvalue(s,"X",(unsigned long ) p_designdata->p_x);
    set_tagvalue(s,"Y",(unsigned long ) p_designdata->p_y);
    set_tagvalue(s,"WIDTH",(unsigned long ) p_designdata->p_width);
    set_tagvalue(s,"HEIGHT",(unsigned long ) p_designdata->p_height);
    p_designdata->p_font.savedata(s);
    set_tagvalue(s,"VUPN",p_vupn);
    set_tagvalue(s,"COUNTS_AS",p_designdata->p_counts_as);
    set_tagvalue(s,"TOOLTIP",p_designdata->p_tooltip);
    set_tagvalue(s,"BUDDYLABEL",p_designdata->p_buddylabel);
    set_tagvalue(s,"ISENABLED",p_designdata->p_enabled);

    hk_string coltag="FOREGROUNDCOLOUR";
    start_mastertag(s,coltag);
    p_designdata->p_foregroundcolour.savedata(s);
    end_mastertag(s,coltag);
    coltag="BACKGROUNDCOLOUR";
    start_mastertag(s,coltag);
    p_designdata->p_backgroundcolour.savedata(s);
    end_mastertag(s,coltag);

    hk_string align;
    switch (p_designdata->p_align)
    {
        case alignright:align="RIGHT";
        break;
        case aligncenter:align="CENTER";
        break;
        default    :align="LEFT";
    }
    set_tagvalue(s,"ALIGN",align);
    set_tagvalue(s,"CLICK_ACTION",p_designdata->p_clickaction);
    set_tagvalue(s,"DOUBLECLICK_ACTION",p_designdata->p_doubleclickaction);
    set_tagvalue(s,"ONCLOSE_ACTION",p_designdata->p_oncloseaction);
    set_tagvalue(s,"ONOPEN_ACTION",p_designdata->p_onopenaction);

    set_tagvalue(s,"ONGETFOCUS_ACTION",p_designdata->p_ongetfocusaction);
    set_tagvalue(s,"ONLOOSEFOCUS_ACTION",p_designdata->p_onloosefocusaction);
    set_tagvalue(s,"ONKEY_ACTION",p_designdata->p_onkeyaction);

    end_mastertag(s,tag);

}


void hk_visible::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::loaddata");
#endif
    xmlNodePtr v;hk_string b;
    v=get_tagvalue(definition,"VISIBLETYPE",b);
    get_tagvalue(definition,"LABEL",p_designdata->p_label);
    set_label(p_designdata->p_label);
    if (p_designdata->p_load_presentationnumber)
       get_tagvalue(definition,"IDENTIFIER",p_private->p_identifier);
    get_tagvalue(definition,"X",p_designdata-> p_x);
    get_tagvalue(definition,"Y", p_designdata->p_y);
    get_tagvalue(definition,"WIDTH", p_designdata->p_width);
    get_tagvalue(definition,"HEIGHT", p_designdata->p_height);
    get_tagvalue(definition,"BUDDYLABEL",p_designdata->p_buddylabel);
    get_tagvalue(definition,"ISENABLED",p_designdata->p_enabled);
    if ((v=get_tagvalue(definition,"HK_FONT",b)))
    {
     p_designdata->p_font.loaddata(v->xmlChildrenNode);
     set_font(p_designdata->p_font);
    }
    else
    {
      get_tagvalue(definition,"FONT",b);
    long int pfs;
    get_tagvalue(definition,"FONTSIZE",pfs);
    set_font(b,pfs);

    }

    get_tagvalue(definition,"COUNTS_AS",p_designdata->p_counts_as);
    long int p=-1;
    if (p_designdata->p_load_presentationnumber) if (get_tagvalue(definition,"VUPN",p))p_vupn=p;
    p_designdata->p_load_presentationnumber=true;
    set_size(p_designdata->p_x,p_designdata->p_y,p_designdata->p_width,p_designdata->p_height,false);
    hk_string align;
    get_tagvalue(definition,"ALIGN",align);
    if (align=="RIGHT") p_designdata->p_align=alignright;
    else if (align=="CENTER")p_designdata->p_align=aligncenter; else p_designdata->p_align=alignleft;
    set_alignment(p_designdata->p_align,false);

    if (get_tagvalue(definition,"TOOLTIP",p_designdata->p_tooltip))
       set_tooltip(p_designdata->p_tooltip,false);

    xmlNodePtr col;
    if ((col=get_tagvalue(definition,"FOREGROUNDCOLOUR",b)))
    {
        col=get_tagvalue(col->xmlChildrenNode,"HK_COLOUR");
        if (col)p_designdata->p_foregroundcolour.loaddata(col->xmlChildrenNode);
        set_foregroundcolour(p_designdata->p_foregroundcolour,false);
    }
    if ((col=get_tagvalue(definition,"BACKGROUNDCOLOUR",b)))
    {
        col=get_tagvalue(col->xmlChildrenNode,"HK_COLOUR");
        if (col)p_designdata->p_backgroundcolour.loaddata(col->xmlChildrenNode);
        set_backgroundcolour(p_designdata->p_backgroundcolour,false);
    }
    get_tagvalue(definition,"CLICK_ACTION",p_designdata->p_clickaction);
    get_tagvalue(definition,"DOUBLECLICK_ACTION",p_designdata->p_doubleclickaction);
    get_tagvalue(definition,"ONCLOSE_ACTION",p_designdata->p_oncloseaction);
    get_tagvalue(definition,"ONOPEN_ACTION",p_designdata->p_onopenaction);
    get_tagvalue(definition,"ONGETFOCUS_ACTION",p_designdata->p_ongetfocusaction);
    get_tagvalue(definition,"ONLOOSEFOCUS_ACTION",p_designdata->p_onloosefocusaction);
    get_tagvalue(definition,"ONKEY_ACTION",p_designdata->p_onkeyaction);
    *p_viewdata=*p_designdata;
}



void hk_visible::has_changed(bool registerchange,enum_has_changed f)
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::has_changed(bool registerchange)");
#endif
    if ( (!registerchange)||(p_presentation==NULL)||p_private->p_neverregisterchange) return;

    p_presentation->set_has_changed(f);
}


void hk_visible::set_font(const hk_string& f,int s,bool registerchange)
{
   set_font(f,s,registerchange,false);
}


void hk_visible::set_font(const hk_string& f,int s,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_font.set_font(f,s);
    }
    p_viewdata->p_font.set_font(f,s);
    has_changed(registerchange);
    widget_specific_font_changed();
}


void hk_visible::set_font(const hk_font& f,bool registerchange)
{
  set_font(f,registerchange,false);
}



void hk_visible::set_font(const hk_font& f,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_font=f;
    }
    p_viewdata->p_font=f;
    has_changed(registerchange);
    widget_specific_font_changed();
}


void    hk_visible::widget_specific_label_changed(void)
{

}

void    hk_visible::widget_specific_enabled_changed(void)
{

}


void hk_visible::set_neverregisterchange(bool n)
{
   set_neverregisterchange(n,false);
}

void hk_visible::set_neverregisterchange(bool n,bool /*force_setting*/)
{

    p_private->p_neverregisterchange=n;
}


void hk_visible::set_datetimeformat(const hk_string& dt,const hk_string& d, const hk_string&  t)
{
   set_datetimeformat(dt,d,t,false);
}



void hk_visible::set_datetimeformat(const hk_string& dt,const hk_string& d, const hk_string&  t,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_datetimeformat=dt;
    p_designdata->p_dateformat=d;
    p_designdata->p_timeformat=t;
    }
    p_viewdata->p_datetimeformat=dt;
    p_viewdata->p_dateformat=d;
    p_viewdata->p_timeformat=t;
}


hk_string hk_visible::datetimeformat(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_datetimeformat;

    return p_designdata->p_datetimeformat;
}


hk_string hk_visible::dateformat(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_dateformat;

    return p_designdata->p_dateformat;
}


hk_string hk_visible::timeformat(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_timeformat;

    return p_designdata->p_timeformat;
}


unsigned long int hk_visible::counts_as(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_counts_as;

    return p_designdata->p_counts_as;
}


void hk_visible::set_counts_as(unsigned long int c)
{
set_counts_as(c,false);
}


void hk_visible::set_counts_as(unsigned long int c,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_counts_as=c;
    }
    p_viewdata->p_counts_as=c;

}


hk_string hk_visible::replace(const hk_string& where)
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::replace");
#endif

    enum
    {
	S_START,S_IN_TAG,S_MAYBE_IN_FIELDNAME
    } state=S_START;

hk_string::size_type offset=0;
hk_string result;
hk_string tag;
while (offset<where.size())
{
	hk_string xc(1,where[offset]);
	switch (state)
	{
	case S_START:   if (xc=="%")
			{
			  state=S_IN_TAG;
			  tag="";
			}
			else
			if (xc=="#")
			{
			  state=S_MAYBE_IN_FIELDNAME;
			  tag="";
			}
			else result+=xc;
			break;
	case S_IN_TAG: {
	                 if (xc=="%")
	                 {
	                   state=S_START;
	                   if (tag.size()>0)
	                   {
	                   bool ok;
	                   hk_string v=tag_value(tag,ok);
	                   if (ok) result+=v;
	                   else result+="%"+tag+"%";
	                   }
	                   else result+="%%";
	                 }
	                 else
	                 if (isspace(xc[0]))
	                 {
	                 tag+=xc;
	                 state=S_START;
	                 result+="%"+tag;
	                 }
	                 else
	                 tag+=xc;
			break;
			}
	case S_MAYBE_IN_FIELDNAME: {
	                 if (xc=="#")
	                 {
	                   state=S_START;
	                   if (tag.size()>0)
	                   {
	                   bool ok;
	                   hk_string v=column_value(tag,ok);
	                   if (ok) result+=v;
	                   else result+="#"+tag+"#";
	                   }
	                   else result+="##";
	                 }
	                 else
	                 if (xc=="\n")
	                 {
	                 tag+=xc;
	                 state=S_START;
	                 result+="#"+tag;
	                 }
	                 else
	                 if (xc=="%")
	                 {
	                 --offset;
	                 state=S_START;
	                 result+="#"+tag;
	                 }
	                 else
	                 tag+=xc;
	                 break;
			}
	default:;
	}
++offset;
}
if (state==S_MAYBE_IN_FIELDNAME)
   result+="#"+tag;
else
if (state==S_IN_TAG)
   result+="%"+tag;
return result;

}


hk_presentation* hk_visible::presentation(void)
{
    return p_presentation;

}


void hk_visible::set_load_presentationnumber(bool l)
{
  set_load_presentationnumber(l,false);
}



void hk_visible::set_load_presentationnumber(bool l,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_load_presentationnumber=l;
    }
    p_viewdata->p_load_presentationnumber=l;

}


bool hk_visible::load_presentationnumber(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_load_presentationnumber;

    return p_designdata->p_load_presentationnumber;

}



void hk_visible::set_alignment(alignmenttype a,bool registerchange)
{
   set_alignment(a,registerchange,false);
}

void hk_visible::set_alignment(alignmenttype a,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {

    p_designdata->p_align=a;
    }
    p_viewdata->p_align=a;
    widget_specific_alignment();
    has_changed(registerchange);
}


hk_visible::alignmenttype hk_visible::alignment(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_align;

    return p_designdata->p_align;
}


void hk_visible::widget_specific_alignment(void)
{

}


void hk_visible::set_defaulttextalignment(alignmenttype a)
{
    if (a==aligndefault)return;
    p_defaulttextalign=a;
}


hk_visible::alignmenttype hk_visible::defaulttextalignment(void)
{
    return p_defaulttextalign;

}


void hk_visible::set_defaultnumberalignment(alignmenttype a)
{
    if (a==aligndefault)return;
    p_defaultnumberalign=a;

}


hk_visible::alignmenttype hk_visible::defaultnumberalignment(void)
{

    return p_defaultnumberalign;
}


void hk_visible::set_open_maximized_windows(bool m)
{
    p_open_maximized_windows=m;
}


bool hk_visible::open_maximized_windows(void)
{
    return p_open_maximized_windows;
}


void   hk_visible::set_foregroundcolour(const hk_colour& colour,bool registerchange)
{

    set_foregroundcolour(colour,registerchange,false);
}


void   hk_visible::set_foregroundcolour(const hk_colour& colour,bool registerchange,bool force_setting)
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::set_foregroundcolour");
#endif
    hk_colour oldcolour(p_designdata->p_foregroundcolour);
    p_viewdata->p_foregroundcolour=colour;
    if (allow_datachanging(force_setting))
    {
        p_designdata->p_foregroundcolour=colour;
        has_changed(registerchange);
    }

    widget_specific_foregroundcolour_changed(oldcolour);

}


hk_colour hk_visible::foregroundcolour(void)
{
    if (p_presentation && p_presentation->mode()==hk_presentation::viewmode) return p_viewdata->p_foregroundcolour;
    return p_designdata->p_foregroundcolour;

}


void   hk_visible::set_backgroundcolour(const hk_colour& colour, bool registerchange)
{
    set_backgroundcolour(colour,registerchange,false);

}


void   hk_visible::set_backgroundcolour(const hk_colour& colour, bool registerchange,bool force_setting)
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::set_backgroundcolour");
#endif
    hk_colour oldcolour(p_designdata->p_backgroundcolour);
    p_viewdata->p_backgroundcolour=colour;
    if (allow_datachanging(force_setting))
    {
        p_designdata->p_backgroundcolour=colour;
        has_changed(registerchange);
    }
    widget_specific_backgroundcolour_changed(oldcolour);
}


hk_colour hk_visible::backgroundcolour(void)
{
    if (p_presentation && p_presentation->mode()==hk_presentation::viewmode) return p_viewdata->p_backgroundcolour;
    return p_designdata->p_backgroundcolour;
}


void    hk_visible::widget_specific_foregroundcolour_changed(const hk_colour&)
{

}


void    hk_visible::widget_specific_backgroundcolour_changed(const hk_colour& )
{

}


void hk_visible::lower_widget(bool registerchange)
{
    if (p_presentation) p_presentation->lower_widget(this);
    has_changed(registerchange);
    widget_specific_lower_widget();
}


void hk_visible::raise_widget(bool registerchange)
{
    if (p_presentation) p_presentation->raise_widget(this);
    has_changed(registerchange);
    widget_specific_raise_widget();

}


void hk_visible::widget_specific_raise_widget(void)
{

}


void hk_visible::widget_specific_lower_widget(void)
{

}


void hk_visible::sizetype_changed(void)
{
 if (!p_presentation) return;

 if (p_presentation->sizetype()==hk_presentation::relative)
  {
// this means previous sizetype was hk_presentation::absolute
// and changed now to hk_presentation::relative
   p_designdata->p_x=p_presentation->horizontal2relativ(p_designdata->p_x);
   p_designdata->p_y=p_presentation->vertical2relativ(p_designdata->p_y);
   p_designdata->p_width=p_presentation->horizontal2relativ(p_designdata->p_width);
   p_designdata->p_height=p_presentation->vertical2relativ(p_designdata->p_height);
  }
else
  {
// this means previous sizetype was hk_presentation::relative
// and changed now to hk_presentation::absolute
   p_designdata->p_x=p_presentation->relativ2horizontal(p_designdata->p_x);
   p_designdata->p_y=p_presentation->relativ2vertical(p_designdata->p_y);
   p_designdata->p_width=p_presentation->relativ2horizontal(p_designdata->p_width);
   p_designdata->p_height=p_presentation->relativ2vertical(p_designdata->p_height);

  }


widget_specific_sizetype_changed();
}


void        hk_visible::widget_specific_sizetype_changed(void)
{


}


bool        hk_visible::allow_datachanging(bool force_setting)
{
    return ((p_presentation && p_presentation->mode()==hk_presentation::designmode)||force_setting||!p_presentation);
}

void hk_visible::before_presentationmode_changed(void)
{

}

bool hk_visible::presentationmode_changed(void)
{
   if (!p_presentation) return true;
   bool result=true;
   switch (p_presentation->mode())
   {
     	case hk_presentation::designmode : ;
     	case hk_presentation::filtermode : ;
							widget_specific_foregroundcolour_changed(foregroundcolour());
    							widget_specific_backgroundcolour_changed(backgroundcolour());
    							widget_specific_label_changed();
							widget_specific_font_changed();
							widget_specific_alignment();
    							set_size(p_designdata->p_x,p_designdata->p_y,p_designdata->p_width,p_designdata->p_height,false);
							if (!action_on_close()) result=false;
							show_widget();
     							break;
	case hk_presentation::viewmode:	;
							*p_viewdata=*p_designdata;
							if (!action_on_open()) result=false;
							break;
   }

return result;
}



void hk_visible::set_on_click_action(const hk_string& action,bool registerchange,bool force_setting)
{
#ifdef HK_DEBUG
    hkdebug("hk_visible::set_on_click_action",action);
#endif
    if (allow_datachanging(force_setting))
    {
      p_designdata->p_clickaction=action;
    }
p_viewdata->p_clickaction=action;
has_changed(registerchange);
}

hk_string hk_visible::on_click_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return p_viewdata->p_clickaction;
  return p_designdata->p_clickaction;
}

bool hk_visible::action_on_click(void)
{
if (on_click_action().size()==0) return true;
 if (!p_presentation)return false;
	return p_presentation->interpreter()->on_click(this);
 }




void hk_visible::set_on_doubleclick_action(const hk_string& action,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
     p_designdata->p_doubleclickaction=action;
    }
p_viewdata->p_doubleclickaction=action;
has_changed(registerchange);
}

hk_string hk_visible::on_doubleclick_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return p_viewdata->p_doubleclickaction;
  return p_designdata->p_doubleclickaction;
}


bool hk_visible::action_on_doubleclick(void)
{
if (on_doubleclick_action().size()==0) return true;
 if (!p_presentation)return false;
	return p_presentation->interpreter()->on_doubleclick(this);
 }


void hk_visible::set_on_open_action(const hk_string& action, bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
     p_designdata->p_onopenaction=action;
    }
  p_viewdata->p_onopenaction=action;
  has_changed(registerchange);
}



hk_string hk_visible::on_open_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
return p_viewdata->p_onopenaction;
return p_designdata->p_onopenaction;
}



bool hk_visible::action_on_open(void)
{
if (on_open_action().size()==0) return true;
 if (!p_presentation)return false;
	return p_presentation->interpreter()->on_open(this);
 }




void hk_visible::set_on_close_action(const hk_string& action, bool registerchange,bool force_setting)
{
      if (allow_datachanging(force_setting))
    {
      p_designdata->p_oncloseaction=action;
    }
  p_viewdata->p_oncloseaction=action;
  has_changed(registerchange);
}




hk_string hk_visible::on_close_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
  return p_viewdata->p_oncloseaction;
return p_designdata->p_oncloseaction;
}




bool hk_visible::action_on_close(void)
{
if (p_viewdata->p_oncloseaction.size()==0) return true;
 if (!p_presentation)return false;
	return p_presentation->interpreter()->on_close(this);
 }

void hk_visible::set_tooltip(const hk_string& t,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_tooltip=t;
    }
    p_viewdata->p_tooltip=t;
    has_changed(registerchange);
    widget_specific_tooltip_changed();

}


hk_string hk_visible::tooltip()
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_tooltip;
return p_designdata->p_tooltip;

}
void hk_visible::widget_specific_tooltip_changed(void)
{

}





void hk_visible::set_on_getfocus_action(const hk_string& f,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_ongetfocusaction=f;
    }
    p_viewdata->p_ongetfocusaction=f;
    has_changed(registerchange);

}

hk_string hk_visible::on_getfocus_action(void)
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_ongetfocusaction;
return p_designdata->p_ongetfocusaction;

}

void hk_visible::set_on_loosefocus_action(const hk_string&f,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_onloosefocusaction=f;
    }
    p_viewdata->p_onloosefocusaction=f;
    has_changed(registerchange);

}

hk_string hk_visible::on_loosefocus_action(void)
{

if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_onloosefocusaction;
return p_designdata->p_onloosefocusaction;

}


bool hk_visible::action_on_getfocus(void)
{
if (on_getfocus_action().size()==0) return true;
 if (!p_presentation)return false;
	return p_presentation->interpreter()->on_getfocus(this);
}


bool hk_visible::action_on_loosefocus(void)
{
if (on_loosefocus_action().size()==0) return true;
 if (!p_presentation)return false;
	return p_presentation->interpreter()->on_loosefocus(this);
}





void hk_visible::set_on_key_action(const hk_string&f,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_onkeyaction=f;
    }
    p_viewdata->p_onkeyaction=f;
    has_changed(registerchange);

}

hk_string hk_visible::on_key_action(void)
{

if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_onkeyaction;
return p_designdata->p_onkeyaction;

}

bool hk_visible::action_on_key(void)
{
if (on_key_action().size()==0) return true;
 if (!p_presentation)return false;
	return p_presentation->interpreter()->on_key(this);
}


void hk_visible::set_key(const hk_key& k)
{
 p_private->p_key=k;
}


hk_key* hk_visible::key(void) const
{
//cerr <<"hk_key hk_visible::key"<<endl;
  return &p_private->p_key;
}


void hk_visible::show_widget(void)
{

}
void hk_visible::hide_widget(void)
{

}

bool hk_visible::close_widget(void)
{
return true;
}


 bool hk_visible::in_tabvisible(void) const
 {
   return (p_private->p_tabvisible!=NULL);
 }

void hk_visible::set_tabvisible(hk_tabvisible* t)
{
  p_private->p_tabvisible=t;
}


void hk_visible::set_buddylabel(long int labelid,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_buddylabel=labelid;
    }
    p_viewdata->p_buddylabel=labelid;
    has_changed(registerchange);

}

long int hk_visible::buddylabel(void) const
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_buddylabel;

    return p_designdata->p_buddylabel;

}


void hk_visible::set_movebuddylabel(bool m)
{
  p_private->p_movebuddylabel=m;
}

bool hk_visible::move_buddylabel(void) const
{
  return p_private->p_movebuddylabel;
}



int hk_visible::tag_number(const hk_string& tag)
{
  tagtype::iterator it=p_private->p_tags.find(tag);
  if (it!=p_private->p_tags.end())
    { 
   int result =(*it).second;
     return result;
    }
  else
 {
 return -1;
 }
}

hk_string hk_visible::column_value(const hk_string& ,bool& ok)
{
ok=false;
return "";
}



hk_string hk_visible::tag_value(int tagnumber, bool& ok)
{
  ok=true;
    hk_datetime dt;
    dt.set_now();
    hk_string result;
  if (tagnumber==p_private->tag_counts_as)
    {
     result=longint2string(counts_as());
     return result;
    }
  if (tagnumber==p_private->tag_time) 
    {
     result= dt.time_asstring();
     return result;
    }
  if (tagnumber==p_private->tag_date)
  {
   result=dt.date_asstring();
   return result;
  }
  if (tagnumber==p_private->tag_fgred)
  {
   result=format_standard_number((p_presentation && p_presentation->mode()==hk_presentation::viewmode)?p_viewdata->p_foregroundcolour.red():p_designdata->p_foregroundcolour.red(),0,0) ;

  return result;
  }
  if (tagnumber==p_private->tag_fggreen)
  {
   result=format_standard_number((p_presentation && p_presentation->mode()==hk_presentation::viewmode)?p_viewdata->p_foregroundcolour.green():p_designdata->p_foregroundcolour.green(),0,0) ;

   return result;
  }
  if (tagnumber==p_private->tag_fgblue)
  {
   result=format_standard_number((p_presentation && p_presentation->mode()==hk_presentation::viewmode)?p_viewdata->p_foregroundcolour.blue():p_designdata->p_foregroundcolour.blue(),0,0);

  return result;
  }
  if (tagnumber==p_private->tag_bgred)
  {
   result=format_standard_number((p_presentation && p_presentation->mode()==hk_presentation::viewmode)?p_viewdata->p_backgroundcolour.red():p_designdata->p_backgroundcolour.red(),0,0);

   return result;
  }
  if (tagnumber==p_private->tag_bggreen)
  {
   result=format_standard_number((p_presentation && p_presentation->mode()==hk_presentation::viewmode)?p_viewdata->p_backgroundcolour.green():p_designdata->p_backgroundcolour.green(),0,0);

   return result;
  }
  if (tagnumber==p_private->tag_bgblue)
  {
   result= format_standard_number((p_presentation && p_presentation->mode()==hk_presentation::viewmode)?p_viewdata->p_backgroundcolour.blue():p_designdata->p_backgroundcolour.blue(),0,0);

   return result;
  }
  ok=false;
  return "";
}

hk_string hk_visible::tag_value(const hk_string& tag, bool& ok)
{

  return tag_value(tag_number(tag),ok);
}

int hk_visible::register_tag(const hk_string& tag)
{
  tagtype::iterator it=p_private->p_tags.find(tag);
  if (it!=p_private->p_tags.end())
    {
     return (*it).second;
    }
  int pos=p_private->p_tags.size(); 
  p_private->p_tags.insert(tagtype::value_type(tag,pos));
  return pos;
}


