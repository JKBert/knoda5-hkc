// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_DSTABLE
#define HK_DSTABLE
#include "hk_dsmodevisible.h"

class hk_dstable:public hk_dsmodevisible
{
public:
hk_dstable(hk_presentation* presentation=NULL);
virtual hk_dsgrid *grid(void){return NULL;}
};
#endif // HK_DSTABLE


