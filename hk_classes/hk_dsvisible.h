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
#ifndef  HK_DSVISIBLE
#define  HK_DSVISIBLE
#include "hk_definitions.h"
#include "hk_visible.h"
#include "hk_datetime.h"

using namespace std;
class hk_datasource;
class hk_dsvisiblemodeprivate;
class hk_dsvisibleprivate;
/**
 *
 *@short base class for visible widgets without column data connection
 *@version $Revision: 1.39 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *This is the base class for visible widgets which do not need access to the data,
 *but do have to know whether or not a datasource is enabled and which row is the actual one.
 */

class hk_dsvisible:public hk_visible

{
    friend class hk_datasource;
    public:
        hk_dsvisible(hk_presentation* presentation=NULL);
        virtual         ~hk_dsvisible(void);
        virtual void        set_datasource(hk_datasource* d);
        virtual bool        set_presentationdatasource(long n, bool registerchange=true);
        long        presentationdatasource(void);
        hk_datasource*  datasource(void);
        virtual void savedata(ostream& s, bool savedatasource,bool saveall);
        virtual void savedata(ostream& s  );
        virtual void loaddata(xmlNodePtr definition);
        void    set_readonly(bool readonly);
        bool    is_readonly(void) const;
/**
* sets the script for the before_row_change action
* The before_row_change action before the current row changes
*/
	void set_before_row_change_action(const hk_string&, bool registerchange=true,bool force_setting=true);
	hk_string before_row_change_action(void);
/**
* sets the script for the after_row_change action
* The after_row_change action after the current row changes
*/
	void set_after_row_change_action(const hk_string&, bool registerchange=true,bool force_setting=true);
	hk_string after_row_change_action(void);
/**
* sets the script for the before_update action
* The before_update action before data changes will be stored
*/
	void set_before_update_action(const hk_string&, bool registerchange=true,bool force_setting=true);
	hk_string before_update_action(void);
/**
* sets the script for the after_update action
* The after_update action after data changes will be stored
*/
	void set_after_update_action(const hk_string&, bool registerchange=true,bool force_setting=true);
	hk_string after_update_action(void);
/**
* sets the script for the before_delete action
* The before_delete action before the current row will be deleted
*/
	void set_before_delete_action(const hk_string&, bool registerchange=true,bool force_setting=true);
	hk_string before_delete_action(void);
/**
* sets the script for the after_delete action
* The after_delete action after the current row was deleted
*/
	void set_after_delete_action(const hk_string&, bool registerchange=true,bool force_setting=true);
	hk_string after_delete_action(void);
/**
* sets the script for the before_insert action
* The before_insert action before a new  row will be inserted
*/
	void set_before_insert_action(const hk_string&, bool registerchange=true,bool force_setting=true);
	hk_string before_insert_action(void);
/**
* sets the script for the after_insert action
* The after_insert action after a new  row was inserted
*/
	void set_after_insert_action(const hk_string&, bool registerchange=true,bool force_setting=true);
	hk_string after_insert_action(void);



virtual hk_string tag_value(int tagnumber, bool& ok);

virtual	void action_before_store_changed_data(void);
virtual	void action_after_store_changed_data(void);
virtual	void action_before_row_change(void);
virtual	void action_after_row_change(void);
virtual	void action_before_delete(void);
virtual	void action_after_delete(void);
virtual	void action_before_insert(void);
virtual	void action_after_insert(void);





    protected:
        void        row_change(void);
	void        batch_next(void);
	void        batch_previous(void);

        void        insert_mode(void);
/**
 *FOR WIDGET DEVELOPERS ONLY:
 * will be automatically called when the datasource will be set to insert_mode.
 *Has to be rewritten for a new widget
 * @see before_row_change.
 *
 */
        virtual void        widget_specific_insert_mode(void){}
/**
 *FOR WIDGET DEVELOPERS ONLY:
 * will be automatically called when the datasource has changed the row.
 *Has to be rewritten for a new widget
 * @see before_row_change.
 *
 */
        virtual bool        widget_specific_row_change(void) {return false;}
        virtual void        widget_specific_batch_next(void) {}
        virtual void        widget_specific_batch_previous(void) {}
/**
 *FOR WIDGET DEVELOPERS ONLY:
 * defines whether the widget should be enabled or disabled. Has to be rewritten
 * for a new widget
 */
        virtual void        widget_specific_enable_disable(void){}
        virtual void        widget_specific_enable(void){}
        virtual void        widget_specific_disable(void){}
        virtual void        widget_specific_row_added(void){}
        virtual void        widget_specific_row_deleted(void){}
/**
 *FOR WIDGET DEVELOPERS ONLY:
 *called from hk_datasource before it tries to change the row or disable
 *widget can i.e. store changed data there
 */
        virtual void        before_row_change(void);
        virtual void        before_store_changed_data(void);
        virtual void        after_store_changed_data(void);
        virtual void        before_insert_row(void){}
        virtual void        before_datasource_enables(void){}
/**
 *FOR WIDGET DEVELOPERS ONLY:
 *will be automatically called when the datasources is disabling
 */
        virtual bool        datasource_disable(void);
/**
 *FOR WIDGET DEVELOPERS ONLY:
 *will be automatically called when the datasources is enabling
 */
        virtual bool        datasource_enable(void);
/**
 *FOR WIDGET DEVELOPERS ONLY:
 *will be automatically called when the datasources is destroyed
 */
        virtual void        datasource_delete(void);
/**
 *FOR WIDGET DEVELOPERS ONLY:
 *will be automatically called when the datasources creates new columns
 *i.e.when enabling,disabling or when a depending datasource has to requery
 *its rows.
 */
        virtual void        row_added(void);
        virtual void        row_deleted(void);
        virtual bool        columns_new_created(void){return true;}
        virtual void        list_changes(listtype type);
/**
 *FOR WIDGET DEVELOPERS ONLY:
 *will be automatically called before the columns will be deleted.
 *usually column pointers will be set to NULL in this function.
 *i.e.when enabling,disabling or when a depending datasource has to requery
 *its rows.
 */

        virtual bool        before_columns_deleted(void){return true;}
/**
 *FOR WIDGET DEVELOPERS ONLY:
 *will be automatically called after the columnstructure of a table
 *was successfully altered.
 */
        virtual bool        table_structure_changes(void){return true;}
/**
 *FOR WIDGET DEVELOPERS ONLY:
 *will be automatically called before the database will be closed.(i.e when a new database will
 *be created and the old one will be destroyed by hk_connection
 */

        virtual void        before_source_vanishes(void);
/**
 *FOR WIDGET DEVELOPERS ONLY:
 *will be automatically called before the presentationdatasource will be set
 */

        virtual void        widget_specific_presentationdatasource(long){}
	virtual bool	presentationmode_changed(void);
        hk_datasource*  p_datasource;
        hk_datetime p_date;
#ifdef HK_DEBUG
        hk_string   debug_p_columnname;
#endif



    private:
	hk_dsvisiblemodeprivate* p_designdata;
	hk_dsvisiblemodeprivate* p_viewdata;
	hk_dsvisibleprivate* p_private;
//p_already_handled  is used from hk_datasource in bulk operations to find out if this object has been already handled.
        bool    p_already_handled;

};
#endif
