// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_sqliteclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.7 $
#include "hk_sqliteview.h"
#include "hk_database.h"

hk_sqliteview::hk_sqliteview(hk_sqlitedatabase* db,hk_presentation* p)
:hk_sqlitedatasource(db,p)
 {
 
 }
 
  
hk_sqliteview::enum_datasourcetypes hk_sqliteview::type() const
{
  return ds_view;
}






bool hk_sqliteview::driver_specific_load_view()
{  cerr <<"driver_specific_load_view: "<<name()<<endl;
  hk_string s="SELECT sql as viewselect FROM sqlite_master where type='view' and name='"+name()+"'";
  hk_datasource* r=p_database->new_resultquery();
  if (!r) return false;
  r->set_sql(s);
  r->enable();
  hk_column* c=r->column_by_name("viewselect");
  if (!c)
    {
      delete r;
      show_warningmessage(hk_translate("View definition could not be loaded"));
      return false;
    }
    s=c->asstring();
  hk_string upper=string2upper(s);
  hk_string::size_type p=upper.find("CREATE VIEW"); 
  if (p!=hk_string::npos)
    {
      p=upper.find(" AS ");
      if (p!=hk_string::npos) s=s.substr(p+4);
    }
  p=s.find_last_not_of(" \\t\\n");
  if (p!=hk_string::npos && s[p]==';')
     s.replace(p,1,"");
  p_viewsql=s;
  cerr <<"setze sql="<<sql()<<endl;
  delete r;
  return true;
}

