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
#ifndef HK_QBE
#define HK_QBE
#include "hk_presentation.h"
#include "hk_string.h"
#include <list>
#include <vector>
class hk_qbeprivate;
class hk_qbe:public hk_presentation
{
public:
hk_qbe(void);
virtual ~hk_qbe();
/**
*automatically generated SQL statement
*/
enum enum_querytype{qt_select,qt_groupselect,qt_update,qt_delete};
enum enum_functiontype {ft_condition,ft_group,ft_sum,ft_count,ft_avg,ft_min,ft_max};
void set_querytype(enum_querytype,bool registerchange=true);
enum_querytype querytype(void) const;
void set_distinct(bool d,bool registerchange=true);
bool distinct(void) const;
bool create_sql(hk_string& sql);
enum enum_order{none,ascending,descending};
typedef class 
{
   public:
   hk_string  	     field;
   int        	     table;
   hk_string  	     alias;
   enum_functiontype functiontype;
   enum_order 	     order;
   bool       	     show;
   hk_string	     updatevalue;
   vector<hk_string>   conditions;
   
}hk_qbedataclass;

void clear_definition(bool registerchange=true);
void add_definition(	const hk_string& field,
			int table, 
			const hk_string& alias,
			enum_functiontype functiontype,
			enum_order order,
			bool show,
			const hk_string& updatevalue,
			vector<hk_string>& conditions,
			bool registerchange=true
			);
void add_definition( hk_qbedataclass*, bool registerchange=true);
const list<hk_qbe::hk_qbedataclass>* definitionlist(void) ;



virtual void                savedata(ostream& s);
virtual void                loaddata(xmlNodePtr definition);

protected:
 virtual void widget_specific_querytype_has_changed(enum_querytype oldtype,enum_querytype newtype);
private:
  hk_string joindefinition(hk_datasource*);
hk_qbeprivate* p_private;
hk_string create_where(void);
hk_string create_depending_where(void);
hk_string create_from(void);
hk_string create_what(void);
hk_string create_order_by(void);
hk_string create_group_by(void);
hk_string create_update_set(void);
hk_string create_select_sql(bool&);
hk_string create_groupselect_sql(bool&);
hk_string create_update_sql(bool&);
hk_string create_delete_sql(bool&);
hk_string fieldname(hk_qbedataclass*,bool withalias=false);
//hk_string tablename(hk_qbedataclass*);

};

#endif //HK_QBE
