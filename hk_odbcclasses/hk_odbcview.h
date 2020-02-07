// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_odbcclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.2 $

#ifndef HK_ODBCVIEW
#define HK_ODBCVIEW
#include "hk_datasource.h"
#include "hk_odbcdatasource.h"

class hk_odbcdatabase;
class hk_presentation;
class hk_odbcview :  public hk_odbcdatasource
{
    friend class hk_odbcdatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_odbcview(hk_odbcdatabase* db,hk_presentation* p);
	virtual bool driver_specific_load_view();

    private:
};
#endif
