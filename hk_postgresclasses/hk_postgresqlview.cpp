// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.5 $
#include "hk_postgresqlview.h"
#include "hk_database.h"

hk_postgresqlview::hk_postgresqlview(hk_postgresqldatabase* db,hk_presentation* p)
:hk_postgresqldatasource(db,p)
 {
 
 }
 
  
hk_postgresqlview::enum_datasourcetypes hk_postgresqlview::type() const
{
  return ds_view;
}






bool hk_postgresqlview::driver_specific_load_view()
{  cerr <<"driver_specific_load_view: "<<name()<<endl;
  hk_string s="select pg_get_viewdef( (select oid  from pg_class where relname='"+name()+"')) as viewselect";
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
  hk_string::size_type p=s.find_last_of(';');
  if (p!=hk_string::npos) s.replace(p,1,"");
  p_viewsql=s;
  cerr <<"setze sql="<<c->asstring()<<endl;
  delete r;
  return true;
}

