// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_dbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include <hk_dbaseresultquery.h>
#include <hk_dbasedatabase.h>

hk_dbaseresultquery::hk_dbaseresultquery(hk_dbasedatabase* db,hk_presentation* p):hk_dbasedatasource(db,p)
{
p_readonly=true;
}


hk_datasource::enum_datasourcetypes hk_dbaseresultquery::type() const
{
    return ds_query;
}
