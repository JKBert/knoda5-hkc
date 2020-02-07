// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_paradoxclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_PARADOXRESULTQUERY
#define HK_PARADOXRESULTQUERY
#include "hk_paradoxdatasource.h"
class hk_paradoxdatabase;
class hk_presentation;

class hk_paradoxresultquery : public hk_paradoxdatasource
{
    friend class hk_paradoxdatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_paradoxresultquery(hk_paradoxdatabase* db,hk_presentation* p);

    private:

} ;
#endif
