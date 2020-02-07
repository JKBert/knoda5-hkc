// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_dbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_dbaseactionquery.h"
#include "hk_dbasedatabase.h"
#include "hk_dbaseconnection.h"

hk_dbaseactionquery::hk_dbaseactionquery(hk_dbasedatabase* db) :hk_actionquery(db)
{
    p_dbasedatabase=db;
   // p_identifierdelimiter="`";
}


bool hk_dbaseactionquery::driver_specific_execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbaseactionquery::driver_specific_execute");
#endif
return false;
}


bool hk_dbaseactionquery::driver_specific_sql(const char* )
{
    return true;
}
