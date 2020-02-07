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
#ifndef HK_DSGRID
#define HK_DSGRID
#include "hk_dsvisible.h"
#include "hk_dsgridcolumn.h"
#include <vector>
#include <list>
using namespace std;
class hk_column;
/**
 *@short  base class for widgets which show multi column data in a grid.
 *@version $Revision: 1.33 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *base class  for widgets which show multi column and multi row data in a grid.
 */
class hk_dsgrid:public hk_dsvisible
{
    friend class hk_dsgridcolumn;
    public:
        hk_dsgrid(hk_form* form=NULL );
        virtual ~hk_dsgrid(void);
/**
 *returns a specific gridcolumn
 *@param c number of the column.
 */
        hk_dsgridcolumn* gridcolumn(unsigned int c);
        hk_dsgridcolumn* gridcolumn(const hk_string& identfier);
/**
 * returns the number of columns shown in the grid.
 */
        unsigned int columnscount(void);
/**
 * sets the new width of a specific column.
 *@param col number of the column
 *@param newsize the new width of the column
 */
        void set_gridcolumnwidth(unsigned int col,int newwidth);
/**
 *it's possible to change the order of the columns
 *@param from position where the column was originally
 *@param to target position
 */
        void change_columnposition(unsigned int from,unsigned int to);
/**
 *you can manually set the gridcolumn definition.
 *@param c the new defintion of the gridcolumns, all values in these definitions will be used
 */
        void set_gridcolumns(vector<hk_dsgridcolumn>& c,bool registerchange=true);
/**
 *you can manually set the gridcolumn definition.The new defintion of the gridcolumns, all columns get default values.
 *@param sl the strings contain the names of the columns.
 */
        void set_gridcolumns(list<hk_string>&sl);
        void clear_gridcolumn(void);
/**
 *If true and you change the row definitions (i.e. column size) after disabling and reenabling
 *your definitions still exist, if false the rows will be newly built. Default is true.
 *@see set_enablingbehaviour
 */
        void set_hold_rowdefinition(bool h=true);
/**
 *if @ref set_hold_rowdefinition is true, this function defines what exactly to do during enabling
 *@param add_col if true, columns of the datasource will be added to the grid if no equivalent gridcolumn exists.
 *Default is false.
 *@param del_col if true, gridcolumns will be removed if no equivalent columns in the datasource exist. Default is false.
 */
        void set_enablingbehaviour(bool add_col,bool del_col);
//	bool save_grid(const hk_string& name="",bool ask=true);
        virtual   void  savedata(ostream& s,bool saveall);
        virtual   void savedata(ostream&s );
        virtual   void loaddata(xmlNodePtr definition);
        virtual void set_datasource(hk_datasource*);
        list <hk_string>* visible_columns(void);
        list <hk_string>* nonvisible_columns(void);
        bool save_table(const hk_string& s="",bool ask=true);
        bool load_table(void);
        bool save_view(const hk_string& s="",bool ask=true);
        bool load_view(void);
	void set_rowheight(int,bool registerchange=true);
	int rowheight(void);




    protected:
        bool    datasource_enable(void);
        bool    datasource_disable(void);
        bool    columns_new_created(void);
        virtual void    before_store_changed_data(void);
        virtual bool table_structure_changes(void);
	virtual void widget_specific_columns_created(void){}
	virtual void widget_specific_rowheight_changes(void){}



    private:

        void delete_nonexisting_columns(void);
        void add_new_columns(void);
        void clear_cols(void);
        void resize_cols(int n);
        vector<hk_dsgridcolumn*> p_cols;
        bool    p_automatic_columns;
        bool    p_delete_nonexisting_columns;
        bool    p_add_new_columns;
        bool    p_holdrowdefinition;
	int 	  p_rowheight;
        list<hk_string> p_visiblecols;
        list<hk_string> p_nonvisiblecols;
};
#endif
