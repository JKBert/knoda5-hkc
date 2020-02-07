// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_firebirdclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.1 $

#ifndef HK_FIREBIRDVIEW
#define HK_FIREBIRDVIEW
#include <hk_datasource.h>
#include "hk_firebirddatasource.h"

class hk_firebirddatabase;
class hk_presentation;
class hk_firebirdview :  public hk_firebirddatasource
{
    friend class hk_firebirddatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_firebirdview(hk_firebirddatabase* db,hk_presentation* p);
	virtual bool driver_specific_load_view();

    private:
};
#endif
