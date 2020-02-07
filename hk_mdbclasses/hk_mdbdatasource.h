// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mdbclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_MDBDATASOURCE
#define HK_MDBDATASOURCE
#include "hk_storagedatasource.h"
#include "hk_mdbcolumn.h"
#include <list>
#include <vector>
#include <algorithm>
#ifdef __cplusplus
  extern "C" {
#endif
#include "mdbtools/mdbtools.h"
#ifdef __cplusplus
  }
#endif

class hk_mdbdatabase;
class hk_mdbconnection;
class hk_presentation;

class hk_mdbdatasource:public hk_storagedatasource
{
    friend class hk_mdbdatabase;

    public:

    protected:
        hk_mdbdatasource(hk_mdbdatabase* d,hk_presentation* p);
        virtual ~hk_mdbdatasource();
}


;
#endif
