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
#ifndef HK_DBVISIBLE
#define HK_DBVISIBLE
#include "hk_visible.h"
using namespace std;

class hk_database;
/**
 *
 *@short base class for database depending only widgets
 *@version $Revision: 1.15 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *base class for database depending only widgets. Just set the database with @ref set_database
 */
class hk_dbvisible :public hk_visible
{
    friend class hk_database;
    public:
        hk_dbvisible(hk_presentation* p=NULL);
        virtual     ~hk_dbvisible(void);
/**
 * the database which is base of this object
 */
        void        set_database(hk_database* db);
        hk_database*    database(void) const;
    protected:
        virtual void    database_delete(void);
/**
 *FOR WIDGET DEVELOPERS ONLY:
 *will be automatically called before the database will be closed.(i.e when a new database will
 *be created and the old one will be destroyed by hk_connection
 */

        virtual void        before_source_vanishes(void);
        virtual void        list_changes(listtype type);

        hk_database* p_database;

    private:
//p_already_handled  is used from hk_database in bulk operations to find out if this object has been already handled.
        bool    p_already_handled;

};
#endif
