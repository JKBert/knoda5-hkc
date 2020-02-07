// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_xbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_XBASERESULTQUERY
#define HK_XBASERESULTQUERY
#include "hk_xbasedatasource.h"
class hk_xbasedatabase;
class hk_presentation;

class hk_xbaseresultquery : public hk_xbasedatasource
{
    friend class hk_xbasedatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_xbaseresultquery(hk_xbasedatabase* db,hk_presentation* p);

    private:

} ;
#endif
