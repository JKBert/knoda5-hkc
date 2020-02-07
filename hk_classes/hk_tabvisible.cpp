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
#include "hk_tabvisible.h"
#include "hk_form.h"

class hk_tabvisiblepageprivate
{
public:
hk_tabvisiblepageprivate()
  {
  p_tabvisible=NULL;
  }
hk_tabvisible* p_tabvisible;
};

hk_tabvisiblepage::hk_tabvisiblepage(hk_tabvisible* t)
{
 p_private=new hk_tabvisiblepageprivate;
 p_private->p_tabvisible=t;
}

hk_tabvisiblepage::~hk_tabvisiblepage()
{
if (p_private->p_tabvisible)
   p_private->p_tabvisible->remove_page(this);
delete p_private;
}

// END of TABVISIBLEPAGE



class hk_tabvisibleprivate
{
public:
hk_tabvisibleprivate()
 {
 }
~hk_tabvisibleprivate()
 {
   vector<hk_tabvisiblepage*>::iterator it=p_pagelist.begin();
   while (it!=p_pagelist.end())
   {
   hk_tabvisiblepage* p=(*it);
   p->p_private->p_tabvisible=NULL;
   ++it;
   delete p;
   }
   p_pagelist.clear();
 }


vector<hk_tabvisiblepage*> p_pagelist;

};

class hk_tabvisiblemodeprivate
{
public:
     hk_tabvisiblemodeprivate()
     {
     
     }
     
     bool  operator=(const hk_tabvisiblemodeprivate* v)
     {
       return true;
     }

};




hk_tabvisible::hk_tabvisible(hk_form*f):hk_visible(f)
{
    p_visibletype=hk_visible::tabvisible;
    p_private=new hk_tabvisibleprivate;
p_designdata=new hk_tabvisiblemodeprivate;
p_viewdata=new hk_tabvisiblemodeprivate;
}


hk_tabvisible::~hk_tabvisible()
{
 delete p_designdata;
 delete p_viewdata;
 delete p_private;
}

void hk_tabvisible::remove_page(hk_tabvisiblepage* p)
{
  vector<hk_tabvisiblepage*>::iterator it=find(p_private->p_pagelist.begin(),p_private->p_pagelist.end(),p);
  if (it!=p_private->p_pagelist.end())
    p_private->p_pagelist.erase(it);
}

void hk_tabvisible::insert_tab(hk_tabvisiblepage* t,int index, bool registerchange)
{
  if (!t) return;
  if (index==-1 || index >=(int)p_private->p_pagelist.size())
     p_private->p_pagelist.push_back(t);
  else
  {
  p_private->p_pagelist.insert(p_private->p_pagelist.begin()+index,t);
  }
  has_changed(registerchange);
}

hk_tabvisiblepage* hk_tabvisible::current_page()
{
  return NULL;
}

hk_tabvisiblepage* hk_tabvisible::tab_at(int index)
{
  if (index <0||index >=(int)p_private->p_pagelist.size()) return NULL;
 return  p_private->p_pagelist[index];
}

vector<hk_tabvisiblepage*>* hk_tabvisible::tabpages() const
{
return  &p_private->p_pagelist;
}

void hk_tabvisible::savedata(ostream& s )
{
  hk_visible::savedata(s);
}

void hk_tabvisible::loaddata(xmlNodePtr definition)
{
  hk_visible::loaddata(definition);
}

