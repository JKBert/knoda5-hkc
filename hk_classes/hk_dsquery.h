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
#ifndef HK_QUERY
#define HK_QUERY
#include "hk_dsmodevisible.h"
class hk_qbe;
class hk_dsqueryprivate;

class hk_dsquery:public hk_dsmodevisible
{
public:
hk_dsquery(hk_presentation* presentation=NULL);
virtual ~hk_dsquery(void);
virtual hk_dsgrid *grid(void){return NULL;}
virtual hk_qbe*  qbe(void) {return NULL;}
bool use_qbe(void) const;
virtual void set_use_qbe(bool use, bool registerchange=true );


bool save_query(const hk_string& s="",bool ask=true);
bool load_query(void);
   virtual void savedata(ostream& s);
   virtual void loaddata(xmlNodePtr definition);

protected:
   virtual void widget_specific_before_load_query(void);
   virtual void widget_specific_after_load_query(void);
   virtual void widget_specific_before_save_query(void);
   virtual void widget_specific_after_save_query(void);
private:
  hk_dsqueryprivate* p_private;
};
#endif //HK_QUERY
