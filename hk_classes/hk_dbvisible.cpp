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
//$Revision: 1.13 $
#include "hk_dbvisible.h"
#include "hk_database.h"
hk_dbvisible::hk_dbvisible(hk_presentation* p):hk_visible(p)
{
    p_database=NULL;
    p_already_handled=false;
}


hk_dbvisible::~hk_dbvisible(void)
{
    if (p_database!=NULL)p_database->dbvisible_remove(this);

}


void hk_dbvisible::set_database(hk_database* db)
{
    if (p_database!=NULL)
    {
        p_database->dbvisible_remove(this);
        p_database=NULL;
    }
    if (db!=NULL)
    {
        p_database=db;
        p_database->dbvisible_add(this);
    }

}


hk_database* hk_dbvisible::database(void) const
{
    return p_database;

}


void hk_dbvisible::database_delete(void)
{
    p_database=NULL;
}


void hk_dbvisible::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_dbvisible::before_source_vanishes");
#endif
    set_database(NULL);

}


void    hk_dbvisible::list_changes(listtype )
{
}

