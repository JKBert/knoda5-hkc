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
#ifndef HK_DSCOMBOBOX
#define HK_DSCOMBOBOX
#include "hk_dsdatavisible.h"
using namespace std;
class hk_listvisible;
class hk_dsgridcolumn;
class hk_dscomboboxprivate;
class hk_dscomboboxmodeprivate;
/**
 *
 *@short  base class for combobox fields
 *@version $Revision: 1.24 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *base class for combobox fields
 *Just set the datasource and the column (defined in the parent class @ref hk_dsdatavisible).
 *To fill the listbox with values set the datasource which contains the wished values with
 *@ref set_listdatasource. The columnname of the row which should be displayed in the listbox
 *must be set with @ref set_viewcolumn. The row which contains the data to be written has to
 *be specified with @ref set_listcolumn.
 */
class hk_dscombobox:public hk_dsdatavisible
{
    public:
        hk_dscombobox(hk_form* form=NULL);
virtual        ~hk_dscombobox();
/**
 *comboboxes need 2 datasources. One (the normal) datasource is set with @ref set_datasource and stores the values in a field.
 *The second datasource contains a list of possible values for this field.

 *@param list the datasource which contains the values displayed in the combobox
     */
        void    set_listdatasource(hk_datasource* list);
        hk_datasource* listdatasource(void);
        virtual void set_datasource(hk_datasource* d);

/**
 * The column of the listdatasource which will be displayed in the combobox
 */
        void    set_viewcolumnname(const hk_string& viewcolumn,bool registerchange=true);
        hk_string  viewcolumnname(void);
/**
 * The column containing the equivalent key to the column set with @ref set_column
 */
        void    set_listcolumnname(const hk_string& listcolumn,bool registerchange=true);
        hk_string listcolumnname(void);
        virtual void savedata(ostream& s );
        virtual void loaddata(xmlNodePtr definition);
/**
 *if this combobox is part of a hk_presentation object (i.e. a  form) you can set the listdatasource by its unique presentation number
 */
        virtual bool        set_listpresentationdatasource(long n, bool registerchange=true);
        long        listpresentationdatasource(void);
        virtual void  load_listitems(void){}
/**
 *if the comboboxmode is set to combo it reacts like a normal datasource. If set to 'selector' the datasource will be moved
 * to the selected row in the datasource. You then just have to add the listcolumnname
 */
        enum enum_mode{combo,combo_noedit,selector};
        void set_mode(enum_mode);
        enum_mode mode(void);

        hk_string     value_at(unsigned long);

	hk_column* viewcolumn(void);
        hk_column* listcolumn(void);

	void set_gridcolumn(hk_dsgridcolumn* );
	hk_dsgridcolumn*gridcolumn (void) const;

	void set_use_textlist(bool,bool registerchange=true);
	bool use_textlist(void) const;
	void set_textlist(list<hk_string>&,bool registerchange=true);
	void add_text(const hk_string& , bool registerchange=true);
list<hk_string>* textlist(void) const;


virtual bool action_on_select(void);
hk_string on_select_action(void);
void set_on_select_action(const hk_string&,bool registerchange=true,bool force_setting=true);

    protected:
        bool    datasource_enable(void);
        bool    datasource_disable(void);
        virtual bool    widget_specific_row_change(void);
        virtual void    widget_specific_mode_change(void){}
        virtual void    after_store_changed_data(void);
        virtual void columndata_has_changed(void);
	virtual bool presentationmode_changed(void);

    //needed for gridcolumn actions where a line editor takes the input (eg. action_on_key)
    hk_dsgridcolumn* p_gridcolumn;
    private:
        void create_filternames();
        hk_listvisible* p_listvisible;
        long p_listdatasource;
        enum_mode p_mode;
	hk_dscomboboxprivate* p_private;
	hk_dscomboboxmodeprivate* p_designdata;
	hk_dscomboboxmodeprivate* p_viewdata;

};
#endif
