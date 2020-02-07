// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.3 $

#ifndef HK_POSTGRESQLVIEW
#define HK_POSTGRESQLVIEW
#include <hk_datasource.h>
#include <hk_postgresqldatasource.h>

class hk_postgresqldatabase;
class hk_presentation;
class hk_postgresqlview :  public hk_postgresqldatasource
{
    friend class hk_postgresqldatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_postgresqlview(hk_postgresqldatabase* db,hk_presentation* p);
	virtual bool driver_specific_load_view();

    private:
};
#endif
