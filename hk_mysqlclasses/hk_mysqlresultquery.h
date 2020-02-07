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
#ifndef HK_MYSQLRESULTQUERY
#define HK_MYSQLRESULTQUERY
#include <hk_datasource.h>
#include <hk_mysqldatasource.h>
class hk_mysqldatabase;
class hk_presentation;

class hk_mysqlresultquery : public hk_mysqldatasource
{
    friend class hk_mysqldatabase;
    public:
        enum_datasourcetypes type() const;

    protected:
        hk_mysqlresultquery(hk_mysqldatabase* db,hk_presentation* p);

    private:

} ;
#endif
