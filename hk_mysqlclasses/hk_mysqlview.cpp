// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mysqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.2 $
#include "hk_mysqlview.h"
#include "hk_database.h"
#include "hk_actionquery.h"

hk_mysqlview::hk_mysqlview(hk_mysqldatabase* db,hk_presentation* p)
:hk_mysqldatasource(db,p)
 {

 }


hk_mysqlview::enum_datasourcetypes hk_mysqlview::type() const
{
  return ds_view;
}






bool hk_mysqlview::driver_specific_load_view()
{
  hk_string s="select VIEW_DEFINITION  FROM information_schema.views where TABLE_NAME='"+name()+"'";
  hk_datasource* r=p_database->new_resultquery();
  if (!r) return false;
  r->set_sql(s);
  r->enable();
  hk_column* c=r->column_by_name("VIEW_DEFINITION");
  if (!c)
    {
      delete r;
      show_warningmessage(hk_translate("View definition could not be loaded"));
      return false;
    }
  s=c->asstring();
  p_viewsql=replace_all("`",s,"\"");
  cerr <<"setze sql="<<c->asstring()<<endl;
  delete r;
  return true;
}


bool    hk_mysqlview::driver_specific_create_view_now(void)
{
  hk_actionquery* a = p_database->new_actionquery();
  if (a==NULL) return false;
  hk_string query = "CREATE VIEW ";
  query+=p_identifierdelimiter+name()+p_identifierdelimiter+" AS "+replace_all("\"",p_viewsql,"`");
  if (p_viewsql.size()==0) show_warningmessage(hk_translate("Bug: View-SQL is empty!"));
  a->set_sql(query.c_str(),query.size());
  bool result=a->execute();
  delete a;


return result;
}
