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
//$Revision: 1.9 $
#include "hk_dslineedit.h"
#include "hk_form.h"

hk_dslineedit::hk_dslineedit(hk_form* f):hk_dsdatavisible(f)
{
#ifdef HK_DEBUG
    hkclassname("lineedit");
    hkdebug("hk_dslineedit::hk_dslineedit");
#endif
    p_visibletype=lineedit;
    p_gridcolumn=NULL;
}


hk_dslineedit::~hk_dslineedit(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dslineedit::~hk_dslineedit");
#endif

}


void hk_dslineedit::set_gridcolumn(hk_dsgridcolumn* c)
{
  p_gridcolumn= c;
}

hk_dsgridcolumn* hk_dslineedit::gridcolumn (void) const
{
return p_gridcolumn;
}

