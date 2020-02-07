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
#ifndef HK_MODULE
#define HK_MODULE
#include "hk_dbvisible.h"
using namespace std;

/**
 *@short translates date and/or time values in different formats
 *@author Horst Knorr <hk_classes@knoda.org>
 *@version $Revision: 1.2 $
 *@internal
 *
 *This class transforms date and/or time formats to any other format
 *
 */
 class hk_moduleprivate;
 
class hk_module: public hk_dbvisible
{
    public:
        hk_module(void);
        ~hk_module();
       void set_script(const hk_string& s,bool registerchange=true);
       hk_string script();
       void set_name(const hk_string&);
       hk_string name(void);
       bool load_module(const hk_string& n="");
       bool save_module(const hk_string& n="",bool ask=true);
       virtual void savedata(ostream& s );
       virtual void loaddata(xmlNodePtr definition);
               bool has_changed();
       void set_block_has_changed(bool);
       bool block_has_changed();
    protected:
        virtual void widget_specific_has_changed(){}
                bool ask_name(void);
    void reset_has_changed();
    private:
hk_moduleprivate* p_private;
};
#endif //HK_MODULE
