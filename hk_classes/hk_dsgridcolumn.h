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
#ifndef HK_DSGRIDCOLUMN_H
#define HK_DSGRIDCOLUMN_H
#include "hk_dsdatavisible.h"
using namespace std;
class hk_column;
class hk_dsgrid;
class hk_dscombobox;
const int default_cellwidth=100;
const int default_cellheight=20;
class hk_dsgridcolumnmodeprivate;
class hk_dsgridcolumnprivate;
/**
 *@short  structure of a column in a grid
 *@version $Revision: 1.21 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *structure of a column in a grid.
 */
class  hk_dsgridcolumn :public hk_dsdatavisible
{
    friend class hk_dsgrid;
    public:
        enum  enum_columntype{columnedit,columnbool,columncombo};
        hk_dsgridcolumn(void);
        virtual ~hk_dsgridcolumn(void);
/**
 *the text displayed in the column header. If not set the columnname will be displayed
 */
        void  set_displayname(hk_string,bool registerchange=true);
        hk_string displayname(void);
/**
 *if columntype is columnedit the celldata will be shown as a string
 *if columntype is columnbool the celldata will be shown as a boolean field
 */
        void set_columntype(enum_columntype,bool registerchange=true);
        enum_columntype columntype(void);
/**
 *the (visible) width of the column
 */
        void set_columnwidth(int ,bool registerchange=true);
        int columnwidth(void);
        virtual   void  savedata(ostream& s);
        virtual   void loaddata(xmlNodePtr definition);

/**
 *comboboxes need 2 datasources. One (the normal) datasource is set with @ref set_datasource and stores the values in a field.
 *The second datasource contains a list of possible values for this field.

 *@param list the datasource which contains the values displayed in the combobox
 */
        void    set_listdatasource(const hk_string& datasourcename,datasourcetype dt=dt_table, bool registerchange=true);
        hk_datasource* listdatasource(void);
        hk_string      listdatasourcename(void);
	datasourcetype listdatasourcetype() const;
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

/**
 *if this combobox is part of a hk_presentation object (i.e. a  form) you can set the listdatasource by its unique presentation number
 */
        virtual bool        set_listpresentationdatasource(long n, bool registerchange=true);
        long        listpresentationdatasource(void);
        hk_string   value_at(unsigned long);

        virtual void set_datasource(hk_datasource*);
        hk_dscombobox* combobox(void);

	unsigned int find(unsigned int from,unsigned int to,const hk_string& searchtext,bool wholephrase=false,bool casesensitive=false,bool backwards=false);
	bool is_findstring(unsigned int pos,const hk_string& searchtext,bool wholephrase=false,bool casesensitive=false);


virtual	bool action_on_click(void);
virtual	bool action_on_doubleclick(void);
virtual     bool  action_on_close(void);
virtual     bool  action_on_open(void);
virtual	void action_before_store_changed_data(void);
virtual	void action_after_store_changed_data(void);
virtual	void action_before_row_change(void);
virtual	void action_after_row_change(void);
virtual	void action_before_delete(void);
virtual	void action_after_delete(void);
virtual	void action_before_insert(void);
virtual	void action_after_insert(void);
virtual  bool action_on_getfocus(void);
virtual  bool action_on_loosefocus(void);
virtual bool action_on_key(void);
        bool action_on_select(hk_dscombobox*);
hk_string on_select_action(void);
void set_on_select_action(const hk_string&,bool registerchange=true,bool force_setting=true);

protected:
    	virtual bool presentationmode_changed(void);


    private:
        void set_grid(hk_dsgrid*);
        void set_combovalues(void);
        hk_string p_displayname;
        hk_string     p_comboviewcolumnname;
        hk_string p_combolistcolumnname;
        long      p_combopresentationdatasource;
        hk_string p_combolistdatasourcename;
        datasourcetype      p_combolistdatasourcetype;

        enum_columntype   p_columntype;
        int       p_columnwidth;
        hk_dsgrid* p_grid;
        hk_dscombobox* p_combobox;
        hk_dsgridcolumnprivate* p_private;
        hk_dsgridcolumnmodeprivate* p_designdata;
        hk_dsgridcolumnmodeprivate* p_viewdata;

};
#endif
