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
#include "hk_dsmodevisible.h"
#include "hk_datasource.h"
#include "hk_database.h"
#include "hk_dsgrid.h"
#include "hk_qbe.h"


class hk_dsmodevisibleprivate
{
public:
hk_dsmodevisibleprivate()
  {
   p_mode=hk_dsmodevisible::viewmode;
   p_has_changed=false;
   p_block_has_changed=false;
  }

        hk_dsmodevisible::enum_mode p_mode;
	bool p_has_changed;
	bool p_block_has_changed;
};



hk_dsmodevisible::hk_dsmodevisible(hk_presentation* p):hk_dsvisible(p)
{
   p_private=new hk_dsmodevisibleprivate;
    set_mode(hk_dsmodevisible::viewmode);
p_is_new=true;

}

hk_dsmodevisible::~hk_dsmodevisible()
{
delete p_private;
p_private=NULL;
}

void hk_dsmodevisible::set_has_changed(enum_has_changed f)
{
#ifdef HK_DEBUG
    hkdebug("hk_dsmodevisible::set_has_changed");
#endif
    if (p_private &&(mode()==designmode||f==forcesetting) && !p_private->p_block_has_changed)
        p_private->p_has_changed=true;
}

bool hk_dsmodevisible::has_changed(void) const
{
   if (p_private)return p_private->p_has_changed;
   else return false;
}


void    hk_dsmodevisible::has_changed(bool registerchange)
{
    if (registerchange) set_has_changed();
}


void hk_dsmodevisible::reset_has_changed(void)
{
   if (p_private) p_private->p_has_changed=false;
}





void hk_dsmodevisible::set_block_has_changed(bool b)
{
  p_private->p_block_has_changed=b;
}

bool hk_dsmodevisible::block_has_changed(void) const
{
  return p_private->p_block_has_changed;
}






bool hk_dsmodevisible::set_mode(enum_mode s)
{

    p_private->p_mode=s;
    if (s==hk_presentation::designmode) show_widget();
    return true;
}
hk_dsmodevisible::enum_mode hk_dsmodevisible::mode(void) const
{
   return p_private->p_mode;
}


bool hk_dsmodevisible::widget_specific_before_modechanges(enum_mode)
{
    return true;
}

void hk_dsmodevisible::set_viewmode(void)
{
set_mode(viewmode);
}

void hk_dsmodevisible::set_designmode(void)
{
set_mode(designmode);
}

