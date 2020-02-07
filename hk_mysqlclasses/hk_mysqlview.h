// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mysqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.3 $

#ifndef HK_MYSQLVIEW
#define HK_MYSQLVIEW
#include "hk_mysqldatasource.h"

class hk_mysqldatabase;
class hk_presentation;
class hk_mysqlview :  public hk_mysqldatasource
{
    friend class hk_mysqldatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_mysqlview(hk_mysqldatabase* db,hk_presentation* p);
	virtual bool driver_specific_load_view();
	virtual bool driver_specific_create_view_now(void);

    private:
};
#endif
