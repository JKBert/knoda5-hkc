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
#ifndef HK_DBASECOLUMN
#define HK_DBASECOLUMN
#include "hk_storagecolumn.h"
class hk_dbasedatasource;

class hk_dbasecolumn:public hk_storagecolumn

{
   friend class hk_dbasedatasource;
   friend class hk_dbasetable;
    public:
        bool    driver_specific_asstring(const hk_string& s);

    protected:
        hk_dbasecolumn(hk_dbasedatasource* ds, const hk_string& TRUESTRING,const hk_string& FALSESTRING);
        virtual ~hk_dbasecolumn(void);

} ;
#endif
