// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mdbclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_mdbdatasource.h"
#include "hk_mdbdatabase.h"

hk_mdbdatasource::hk_mdbdatasource(hk_mdbdatabase* d,hk_presentation* p):hk_storagedatasource(d,p)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_mdbdatasource::constructor");
#endif
}


hk_mdbdatasource::~hk_mdbdatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_mdbdatasource::destructor");
#endif
    if (accessmode()!=standard &&is_enabled()) driver_specific_batch_disable();
}





