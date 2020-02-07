// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_odbcclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.5 $
#include "hk_odbcview.h"
#include "hk_odbcconnection.h"
#include "hk_odbcdatabase.h"
#include "hk_database.h"

hk_odbcview::hk_odbcview(hk_odbcdatabase* db,hk_presentation* p)
:hk_odbcdatasource(db,p)
 {
 
 }
 
  
hk_odbcview::enum_datasourcetypes hk_odbcview::type() const
{
  return ds_view;
}






bool hk_odbcview::driver_specific_load_view()
{ 
    hk_string s;
    
    hk_odbcconnection::enum_backend backend=p_odbcdatabase->connection()->backendtype();
    switch (backend)
    {
     case hk_odbcconnection::postgresql: s="select pg_get_viewdef( (select oid  from pg_class where relname='"+name()+"')) as viewselect"; break;
     
     case hk_odbcconnection::sapdb: s="select DEFINITION as \"viewselect\" from VIEWDEFS where VIEWNAME='"+name()+"'"; break;
     
     
     
     default : cerr<<"unbekannter BACKENDTYPE!"<<endl;return false;
    }
  hk_datasource* r=p_database->new_resultquery();
  if (!r) return false;
  r->set_sql(s);
  r->enable();
  hk_column* c=r->column_by_name("viewselect");
  if (!c)
    {
      delete r;
      show_warningmessage(hk_translate("View definition could not be loaded,no such column 'viewselect'"));
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
  cerr <<"setze sql="<<c->asstring()<<endl;
  delete r;

return true;
}

