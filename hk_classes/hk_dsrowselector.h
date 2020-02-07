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
#ifndef HK_DSROWSELECTOR
#define HK_DSROWSELECTOR
#include "hk_dsvisible.h"
#include "hk_form.h"
using namespace std;
/**
 *
 *@short  base class for row selectors
 *@version $Revision: 1.10 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *  base class for row selectors
 */
class hk_dsrowselector:public hk_dsvisible
{
    public:
        hk_dsrowselector(hk_form* f=NULL);
};
#endif
