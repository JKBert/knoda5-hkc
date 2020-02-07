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
#ifndef HK_SUBFORM
#define HK_SUBFORM
#include "hk_dsvisible.h"
#include "hk_definitions.h"
#include <list>
class hk_form;
class hk_subformprivate;
class hk_subformmodeprivate;

/**
 *@short represents a subform in a form
 *@version $Revision: 1.6 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 */


class hk_subform: public hk_dsvisible
{
friend class hk_form;
public:
hk_subform(hk_presentation* parent);
virtual ~hk_subform();
/**
*sets the name of the subform.
*/
void set_name(const hk_string&, bool registerchange=true);
hk_string name(void) const;
/**
*returns the form object that will internally be used to display the subform
*/
hk_form* subform();
/**
 *@param subfield fieldname of a column of the main subform-datasource which has to be connected to the main datasource of the master form.
 *@param masterfield columnname of the main masterform-datasource which value has to be equal with the value of subfield.
 *@param registerchange if this class is part of a hk_presentation object (i.e. a form or a report) and registerchange
 *is true, then the changes will be stored when the hk_presentation object is closed.

*/
void        add_depending_fields( const hk_string& subfield,const hk_string& masterfield,bool registerchange=true);
/**
*clears the list of fields set with add_depending_fields
*/
void        clear_depending_fields(bool registerchange=true);
/**
*when the subform is set during runtime, the subform will be loaded and displayed when calling this function
*/
void   activate_subform(void);
list<dependingclass>* depending_on_fields(void);
virtual void savedata(ostream& s, bool savedatasource,bool saveall);
virtual void savedata(ostream& s  );
virtual void loaddata(xmlNodePtr definition);

protected:
void set_subform(hk_form*);
virtual void    before_datasource_enables(void);
virtual bool	presentationmode_changed(void);
void internal_set_datasourcevalues(void);
private:

hk_subformprivate* p_private;
hk_subformmodeprivate* p_viewdata;
hk_subformmodeprivate* p_designdata;
};

#endif
