// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_sqliteclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_SQLITERESULTQUERY
#define HK_SQLITERESULTQUERY
#include "hk_datasource.h"
#include "hk_sqlitedatasource.h"
class hk_sqlitedatabase;
class hk_presentation;

class hk_sqliteresultquery : public hk_sqlitedatasource
{
    friend class hk_sqlitedatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_sqliteresultquery(hk_sqlitedatabase* db,hk_presentation* p);

    private:

} ;
#endif
