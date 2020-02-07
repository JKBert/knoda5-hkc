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
#ifndef HK_TABVISIBLE
#define HK_TABVISIBLE
#include "hk_visible.h"
#include <vector>

class hk_tabvisiblepageprivate;

class hk_tabvisiblepage:public hk_visible
{
friend class hk_tabvisible;
friend class hk_tabvisibleprivate;

public:
 hk_tabvisiblepage(hk_tabvisible* );
 virtual ~hk_tabvisiblepage();

private:
 hk_tabvisiblepageprivate* p_private;
};

class hk_tabvisibleprivate;
class hk_tabvisiblemodeprivate;


class hk_tabvisible : public hk_visible
{
friend class hk_tabvisiblepage;

public:
 hk_tabvisible(hk_form* form=NULL);
 virtual ~hk_tabvisible();
 void insert_tab(hk_tabvisiblepage* t,int index=-1, bool registerchange=true);
 virtual hk_tabvisiblepage* current_page();
 hk_tabvisiblepage* tab_at(int);
 vector<hk_tabvisiblepage*>* tabpages() const;
 virtual void savedata(ostream& s );
 virtual void loaddata(xmlNodePtr definition);

protected:
 
private:
 void remove_page(hk_tabvisiblepage*);
 hk_tabvisiblemodeprivate* p_designdata;
 hk_tabvisiblemodeprivate* p_viewdata;
 hk_tabvisibleprivate* p_private;

};
#endif //hk_tabvisible
