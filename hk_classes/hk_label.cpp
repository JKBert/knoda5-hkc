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
//$Revision: 1.12 $
#include "hk_label.h"
class hk_labelmodeprivate
{
public:
        int p_top;
        int p_left;
        int p_right;
        int p_bottom;
        int p_loru;
        int p_luro;
 bool operator=(hk_labelmodeprivate* m)
 {
   p_top=m->p_top;
   p_left=m->p_left;
   p_right=m->p_right;
   p_bottom=m->p_bottom;
   p_loru=m->p_loru;
   p_luro=m->p_luro;
   return true;
 }
};

hk_label::hk_label(hk_form* f):hk_visible(f)
{
p_designdata=new hk_labelmodeprivate;
p_viewdata=new hk_labelmodeprivate;
p_visibletype=textlabel;
    p_designdata->p_top=p_designdata->p_bottom=p_designdata->p_left=p_designdata->p_right=p_designdata->p_luro=p_designdata->p_loru=false;
    *p_viewdata=*p_designdata;
}

hk_label::~hk_label()
{
delete p_designdata;
delete p_viewdata;
}



void hk_label::set_topline(int l,bool registerchange)
{
    if (p_presentation&& p_presentation->mode()==hk_presentation::designmode) p_designdata->p_top=l;
    p_viewdata->p_top=l;
    has_changed(registerchange);
    widget_specific_topline_changed();

}

void hk_label::set_bottomline(int l,bool registerchange)
{
    if (p_presentation&& p_presentation->mode()==hk_presentation::designmode) p_designdata->p_bottom=l;
    p_viewdata->p_bottom=l;
    has_changed(registerchange);
    widget_specific_bottomline_changed();
}

void hk_label::set_leftline(int l,bool registerchange)
{
    if (p_presentation&& p_presentation->mode()==hk_presentation::designmode) p_designdata->p_left=l;
    p_viewdata->p_left=l;
    has_changed(registerchange);
    widget_specific_leftline_changed();
}

void hk_label::set_rightline(int l,bool registerchange)
{
    if (p_presentation&& p_presentation->mode()==hk_presentation::designmode) p_designdata->p_right=l;
    p_viewdata->p_right=l;
    has_changed(registerchange);
    widget_specific_rightline_changed();

}

void hk_label::set_diagonalluro(int l,bool registerchange)
{
    if (p_presentation&& p_presentation->mode()==hk_presentation::designmode) p_designdata->p_luro=l;
    p_viewdata->p_luro=l;
    has_changed(registerchange);
    widget_specific_diagonalluro_changed();

}

void hk_label::set_diagonalloru(int l,bool registerchange)
{
    if (p_presentation&& p_presentation->mode()==hk_presentation::designmode) p_designdata->p_loru=l;
    p_viewdata->p_loru=l;
    has_changed(registerchange);
    widget_specific_diagonalloru_changed();

}


int hk_label::topline(void)
{
        if (p_presentation&&p_presentation->mode()==hk_presentation::designmode) return p_designdata->p_top;
	else return p_viewdata->p_top;

}


int hk_label::bottomline(void)
{
        if (p_presentation&&p_presentation->mode()==hk_presentation::designmode) return p_designdata->p_bottom;
	else return p_viewdata->p_bottom;

}


int hk_label::leftline(void)
{
        if (p_presentation&&p_presentation->mode()==hk_presentation::designmode) return p_designdata->p_left;
	else return p_viewdata->p_left;

}


int hk_label::rightline(void)
{
        if (p_presentation&&p_presentation->mode()==hk_presentation::designmode) return p_designdata->p_right;
	else return p_viewdata->p_right;

}


int hk_label::diagonalluro(void)
{
        if (p_presentation&&p_presentation->mode()==hk_presentation::designmode) return p_designdata->p_luro;
	else return p_viewdata->p_luro;

}


int hk_label::diagonalloru(void)
{
        if (p_presentation&&p_presentation->mode()==hk_presentation::designmode) return p_designdata->p_loru;
	else return p_viewdata->p_loru;

}


bool hk_label::presentationmode_changed(void)
{
    if (p_presentation&&p_presentation->mode()==hk_presentation::viewmode)
        *p_viewdata=*p_designdata;
  return hk_visible::presentationmode_changed();
}


void     hk_label::set_frame(int l,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_label::set_frame");
#endif
   set_topline(l,registerchange);
   set_bottomline(l,registerchange);
   set_leftline(l,registerchange);
   set_rightline(l,registerchange);
}


void hk_label::savedata(ostream& s )
{
   hk_visible::savedata(s);
    set_tagvalue(s,"TOPBORDER",p_designdata->p_top);
    set_tagvalue(s,"LEFTBORDER",p_designdata->p_left);
    set_tagvalue(s,"RIGHTBORDER",p_designdata->p_right);
    set_tagvalue(s,"BOTTOMBORDER",p_designdata->p_bottom);
    set_tagvalue(s,"DIAGONALLORU",p_designdata->p_loru);
    set_tagvalue(s,"DIAGONALLURO",p_designdata->p_luro);

}

void hk_label::loaddata(xmlNodePtr definition)
{
   hk_visible::loaddata(definition);
    get_tagvalue(definition,"TOPBORDER",p_designdata->p_top);
    get_tagvalue(definition,"LEFTBORDER",p_designdata->p_left);
    get_tagvalue(definition,"RIGHTBORDER",p_designdata->p_right);
    get_tagvalue(definition,"BOTTOMBORDER",p_designdata->p_bottom);
    get_tagvalue(definition,"DIAGONALLORU",p_designdata->p_loru);
    get_tagvalue(definition,"DIAGONALLURO",p_designdata->p_luro);
    *p_viewdata=*p_designdata;
    widget_specific_topline_changed();
    widget_specific_bottomline_changed();
    widget_specific_leftline_changed();
    widget_specific_rightline_changed();
    widget_specific_diagonalluro_changed();
    widget_specific_diagonalloru_changed();
}


