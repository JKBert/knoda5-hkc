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
#include "hk_mdbcolumn.h"
#include "hk_mdbdatasource.h"


hk_mdbcolumn::hk_mdbcolumn(hk_mdbdatasource* ds, const hk_string& tTRUE,const hk_string& tFALSE)
:hk_storagecolumn(ds,tTRUE,tFALSE)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_mdbcolumn::constructor");
#endif
//     p_driverspecific_timestampformat="YMDhms";
    p_driverspecific_datetimeformat="D.M.Y h:m:s";
    
}


hk_mdbcolumn::~hk_mdbcolumn(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mdbcolumn::destructor");
#endif

}
