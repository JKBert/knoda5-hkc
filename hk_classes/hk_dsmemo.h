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
#ifndef HK_DSMEMO
#define HK_DSMEMO
#include "hk_dsdatavisible.h"
class hk_dsgridcolumn;
using namespace std;
/**
 *
 *@short  base class for memofields
 *@version $Revision: 1.11 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *base class for memofields
 */
class hk_dsmemo:public hk_dsdatavisible
{
    public:
        hk_dsmemo(hk_form* form=NULL);
	void set_gridcolumn(hk_dsgridcolumn* );
	hk_dsgridcolumn*gridcolumn (void) const;
    //needed for gridcolumn actions where a line editor takes the input (eg. action_on_key)
    protected:
    hk_dsgridcolumn* p_gridcolumn;

};
#endif
