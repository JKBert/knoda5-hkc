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
#ifndef HK_DATASOURCE
#define HK_DATASOURCE
//#include <hk_definitions.h>
#include "hk_data.h"
#include <list>
#include <utility>
#include <algorithm>
#include "hk_column.h"
using namespace std;

class hk_database;
class hk_dsvisible;
class hk_presentation;
class hk_qbe;
class hk_actionquery;
class hk_datasourceprivate;
/**
 *
 *@short  represents a resultquery or a table of a database.
 *@version $Revision: 1.102 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *hk_datasource is the basic class which represents a resultquery or a table of a database.
 * Never create this table directly. Use the @ref hk_database::new_table method instead.
 *
 * Two types of this class can be distinguished. Tables and Resultqueries.
 *
 *Resultqueries are Queries with
 * a SELECT - SQL-statement. The resulting data cannot be edited or manipulated.
 *
 *Tables are a special form of resultqueries. The SQL-statement is fixed ("SELECT * FROM <tablename>),
 * but the resulting dataset can be edited.
 */

class hk_datasource:public hk_data
{
    friend class hk_column;
    friend class hk_dsvisible;
    friend class hk_database;
    friend class hk_presentation;
    public:
        virtual     ~hk_datasource();
/**
 *sets the name of the datasource
 *@param n the name
 *@param registerchange if this class is part of a hk_presentation object (i.e. a form or a report) and registerchange
 *is true, then the changes will be stored when the hk_presentation object is closed.
 */
        virtual void    set_name(const hk_string& n,bool registerchange=true);
        virtual hk_string name(void);
        enum enum_tablemodes {mode_normal,mode_createtable,mode_altertable,mode_disabled,mode_insertrow,mode_deleterow,mode_unknown};
/**
 *if this datasource is a table, it can be in different "modes".
 *For example if you want to create a new table and define
 *the different columns, the table has to be in "mode_create"
 *See @ref enum_tablemodes for the different modetypes.
 */
        enum_tablemodes mode(void);
/**
 *brings the datasource in insertmode. To add a new row call this function, set the data of the new row
 *as usual in the columns (i.e.with @ref hk_column::as_string ) and finally call
 *either @ref setmode_normal or @ref store_changed_data.
 */
        void    setmode_insertrow(void);
/**
 *this function tries to delete the actual row. If there is a depending datasource with equivalent rows
 *the actual row will not be deleted.
 */
        void    delete_actualrow(enum_interaction c=interactive);
/**
 *This is the usual mode, where you can browse the data and if the data is not readonly change the data.
 */
        void    setmode_normal(void);

/**
 *If you want to create a new table first  bring the table in createmode. Define new columns with
 *@ref  new_column. Afterwards you can create the table by calling @ref create_table_now
 */
        void    setmode_createtable(void);

/**
 *If you want to alter an existing table first  bring the table in altermode. Define new columns with
 *@ref  new_column. Alter it with @ref alter_column and delete a column by calling @ref alter_column.
 * Afterwards you can alter the table by calling @ref alter_table_now
 */
        void setmode_altertable(void);

/**
 *if this datasource is of type "table" and in mode "create" or "alter" you can
 *create a new column to define its parameters.
 */
        hk_column* new_column(void);

/**
 *if this datasource is of type "table" and in mode "alter" you can
 *alter an existing column. NULL values mean, that this part will not be changed
 */

        bool alter_column(const hk_string& col, const hk_string* name=NULL,hk_column::enum_columntype* newtype=NULL,long* size=NULL,const hk_string* defaultvalue=NULL,const bool* primary=NULL,const bool* notnull=NULL);
/**
 *if this datasource is of type "table" and in mode "alter" you can
 *delete an existing column.Just enter the name here.
 */
        bool delete_column(const hk_string& col);
/**
 * After defining a new table with setmode_createtable and new_column this function
 *creates physically the table
 */
        bool create_table_now(void);
/**
 * After altering an existing table with setmode_altertable, new_column
 *,alter_column and delete_column this function
 *alters physically the table
 */
        bool alter_table_now(void);



	bool create_view_now(void);
	bool alter_view_now(void);

/**
 *Goto a specific row. The first row number is 0, the last n-1 where n
 *is the number of rows as delivered from @ref max_rows().All depending objects
 *will be informed (visible objects, depending datasources etc)
 * @param r is the wished new row number to go.
 * @return true if success, else false.
 */
        virtual bool    goto_row(unsigned long r);
		bool    while_goto_row(void) const;

/**
 *move the row selector to the first row. All depending objects
 *will be informed (visible objects, depending datasources etc)
 * @return true if success, else false.
 */
        bool    goto_first(void);
/**
 *move the row selector to the last row. All depending objects
 *will be informed (visible objects, depending datasources etc)
 * @return true if success, else false.
 */
        bool    goto_last(void);
/**
 *move the row selector to the next row. All depending objects
 *will be informed (visible objects, depending datasources etc)
 * @return true if success, else false.
 */
        bool    goto_next(void);
/**
 *move the row selector to the previous row.  All depending objects
 *will be informed (visible objects, depending datasources etc)
 * @return true if success, else false.
 */
        bool    goto_previous(void);
/**
 *moves the rowselector to a random row.
 */
        bool    goto_random(void);
/**
 *number of the current row.
 */
        unsigned long row_position(void);
/**
 *number of rows in this query.
 */
        unsigned long max_rows(void);
/**
 * the accessmode regulates the behaviour of the datasource.
 * In mode 'standard' the datasource is readable and writeable, but usually first loads all the data from the
 * SQL server. This is in some cases not necessary. If you just need to serially read one row after another, you
 * can use the 'batchread' mode. In this case random access is not possible and it is not possible to edit the data
 * or insert rows (e.g. this is used in @ref hk_report). In 'batchwrite' mode data can't be read and is only used
 * for inserting data (e.g. used @ref hk_importcsv)
 *The batchmodes do not need as much memory as the standard mode.
 */
        enum enum_accessmode {standard,batchread,batchwrite};
        bool set_accessmode(enum_accessmode);
        enum_accessmode accessmode(void) const;

/**
 *if hk_connection is connected, this method will enable the datasource. The SQL-Statement in @ref SQL
 *will be executed. If the SQL-statement is ok, the resulting data can be reached via the columns in @ref columns. You can browse
 *the data by using the methods @ref goto_row, @ref goto_first,
 *@ref goto_last, @ref goto_next and @ref goto_previous.
 *@return true if enable was successful.
 */
        bool    enable(void);
	bool    while_enabling(void) const;
/**
 *if the datasource is enabled, this function will disable it. The columnlist will be deleted.
 */
        bool    disable(void);
	bool    while_disabling(void) const;
/**
 *convenience function: if e is true the datasource will be enabled by calling @ref enable
 *else it will disable the datasource by calling @ref disable.
 */

        bool    set_enabled(bool e);
/**
 *shows whether the datasource is enabled.
 */
        bool    is_enabled(void) const;
/**
 * @return true if data in this datasource can be changed.
 * Always false if this datasource is a dml_query.
 */
        bool    is_readonly(void) const;
/**
 *  If the datasource is of type ds_table you can allow or disallow data changes.
 */
        void    set_readonly(bool r);
/**
 *if the datasource is enabled you get a list of all columns.
 *The fields can be reached by columns. See @ref hk_column for details.
 * returns NULL if no columns are defined yet (e.g. non-enabled queries)
 */
        list<hk_column*>* columns(void);
	list<hk_string>* columnnames(void);
/**
 *if the datasource is enabled, you can reach a particular column by name
 *@return @ref hk_column if there is a column with this name else NULL.
 */
        hk_column* column_by_name(const hk_string& c);
/**
 *if the datasource is enabled, you can reach a particular column by name.
 * @param colnumber If there are more than 1 columns with the same name this parameter tells which one to
 *take (1 is the first column)
 *
 *@return @ref hk_column if there is a column with this name else NULL.
 */

        hk_column* column_by_name(const hk_string& c, int colnumber);
/**
 *returns how many columns exist in this datasource with the name 'colname'.
 * Usually columnnames should be unique, but it is unfortunately technically possible to create
 * SQL statements that return ambigous columnnames
 */
        int columnname_occurances(const hk_string& colname);
/**
 * returns if this is the first, second ... nth column with this name
 */
        int columnname_occurance(hk_column*);

/**
 * set your own SQL statement, that will be executed when you enable the datasource.
 * if this datasource is of type "table" it will be automatically created.
 *@param  s is the sql statement
 *@param rawsql if true the sql statement will used as is (otherwise it may be changed )
 *@param registerchange if this class is part of a hk_presentation object (i.e. a form or a report) and registerchange
 *is true, then the changes will be stored when the hk_presentation object is closed.
 *sorting, filtering or depending on statements). So it is possible to execute driver specific
 *result queries which do not contain the "SELECT" statement, i.e. SHOW FIELDS in Mysql.
 */
        bool    set_sql(const hk_string& s,bool rawsql=false,bool registerchange=true);
virtual bool    set_query(hk_qbe*,bool registerchange=true);
        hk_string   sql(void) const;
        bool    is_rawsql(void);
/**
*returns the SQL statement as it is sent to the SQL server
*/
	hk_string   backendsql(void) const;
        typedef class
        {
            public:
                hk_string  name;
                bool    unique;
                list<hk_string> fields;
        } indexclass;
/**
 * returns the list of all existing indices of a table or NULL in case of an error (i.e. not a table)
 */
        list<indexclass>* indices(void);
	bool index_exists(const hk_string&);
/**
 * deletes an index of a table. returns true if successful otherwise false;
 *@param i name of the index
 */
        bool    drop_index(const hk_string& i);
/**
 * creates an index of a table. returns true if successful otherwise false;
 * make sure you have set the tablename first.
 *@param name new name of the index
 *@param unique true if this should be a unique index
 *@param fields the field(column) names used by this index
 *@return true if successful otherwise false;
 */
        bool    create_index(const hk_string& name,bool unique, list<hk_string>& fields);
/**
 * Alters an existing index by first dropping it with @ref drop_index and then by trying to recreate it
 * with @ref create_index
 */
        bool    alter_index(const hk_string& name,bool unique,list<hk_string>& fields);
/**
 *See @ref dependingmode for details
 */
        enum enum_dependingmodes {depending_nohandle,depending_standard,depending_change,depending_delete,depending_changedelete};
/**
<pre>
*   depending_nohandle     = ignore that this is a depending datasource
*   depending_standard     = can't delete a master row when there are
*                            depending rows in the depending datasource
*                            and can't change the value of a master connection field.
*   depending_change       = if a master connection field is changed, the
*                            equivalent depending fields will change their values too.
*   depending_delete       = if the master row is deleted the depending rows will also be deleted
*   depending_changedelete = combination of depending_change and depending_delete
</pre>
See also @ref set_depending_on for details
*/
        enum_dependingmodes dependingmode(void);
/**
 *if there are 2 datasources in a 1:n relation it is possible to connect the 2 datasources.
 *Add the master datasource here ( it's the '1'-datasource in the '1:n'-relation).
 *Except of the master datasource you also have to add the connected fields with @ref add_depending_fields
 *This (the depending) datasource will enable/disable automatically
 *when the master datasource will enable/disable.
 *if this datasource is part of a @ref hk_presentation use @ref set_depending_on_presentationdatasource instead
 *@param d is the master datasource
 *@param react_on_data_changes true if the datasource should react when a value of the masterdatasource is changed
 *but not yet stored
 *@param mode  how to react when the masterdatasource changes its data. See @ref enum_dependingmodes for details.
 */
        bool    set_depending_on(hk_datasource* d,bool react_on_data_changes=false,enum_dependingmodes mode=depending_standard);
        hk_datasource* depending_on(void);
        
        void set_depending_on_is_left_join(bool, bool registerchange=true);
        bool depending_on_is_left_join(void);
/**
 *See @ref dependingmode for details
 */
        bool depending_on_react_on_data_changes(void);
/**
 * a list of the depending on columns of this datasource set with @ref add_depending_fields
 */
        list<hk_string>* depending_on_thisfields(void);
/**
 * a list of the depending on columns of the master datasource set with @ref add_depending_fields
 */
        list<hk_string>* depending_on_masterfields(void);
/**
 *if this datasource is part of a hk_presentation object (form or report) then set the depending on datasource with
 *this function. You can get the unique datasource number of a datasource with @ref presentationnumber
 *@param d the presentationnumber of the master datasource
 *@param registerchange if this class is part of a hk_presentation object (i.e. a form or a report) and registerchange
 *is true, then the changes will be stored when the hk_presentation object is closed.
 */
        bool   set_depending_on_presentationdatasource(long d,bool react_on_data_changes=false,enum_dependingmodes mode=depending_standard,bool registerchange=true);
        long    depending_on_presentationdatasource(void);
/**
 * use this method in connection with @ref depending_on.
 *@param this_ds_field fieldname of a column of this datasource which has to be connected to the master datasource.
 *@param master_ds_field columnname of the masterdatasource which value has to be equal with the value of this_ds_field.
 *@param registerchange if this class is part of a hk_presentation object (i.e. a form or a report) and registerchange
 *is true, then the changes will be stored when the hk_presentation object is closed.
 */
        void    add_depending_fields(const hk_string& this_ds_field, const hk_string& master_ds_field,bool registerchange=true);
/**
 *clears the depending list.
 */
        void    clear_depending_fields(bool registerchange=true);
/**
 * if the data of the actual row has changed you can manually send the changes to the SQL Server
 * by calling this function. The function will be called automatically before the datasource disables
 * or the row selector will be moved to another row.
 */
        bool    store_changed_data(enum_interaction c=interactive);
	void    set_ignore_changed_data(void);
	bool   ignore_changed_data(void) const;
/**
 * it is possible to filter only specific rows from a datasource by setting this filter.
 * just add the conditions with this function.
 *@param f same syntax as a SQL statement in the 'WHERE' section, but without the word 'WHERE'
 *@param registerchange if this class is part of a hk_presentation object (i.e. a form or a report) and registerchange
 *is true, then the changes will be stored when the hk_presentation object is closed.
 *Example: SELECT * from addresses WHERE city="Mnchen", so you would call set_filter("city=\"Mnchen\"");
 *
 */
        void    set_filter(const hk_string& f,bool registerchange=true);
        hk_string filter(void);
/**
 *temporary filters just work like filters (see @ref set_filter ), but have to be manually activated with
 *@ref set_use_temporaryfilter. When loaded with @ref loaddata temporaryfilters are deactivated by default.
 */
        void set_temporaryfilter(const hk_string&f);
        hk_string temporaryfilter(void);
        void set_use_temporaryfilter(bool use);
        bool use_temporaryfilter(void);
/*
*hk_classes internal use only, don't use the function set_internalfilter
*/
        void set_internalfilter(const hk_string&f);
        hk_string internalfilter(void);
        void set_use_internalfilter(bool use);
        bool use_internalfilter(void);


/**
 * clears the filter which was set with @ref set_filter
 *@param registerchange if this class is part of a hk_presentation object (i.e. a form or a report) and registerchange
 *is true, then the changes will be stored when the hk_presentation object is closed.
 */
        void    clear_filter(bool registerchange=true);
/**
 * it is possible to sort the datasource.
 * just add the conditions with this function.
 *@param f same syntax as a SQL statement in the "ORDER BY" section, but without the word "ORDER BY"
 *@param registerchange if this class is part of a hk_presentation object (i.e. a form or a report) and registerchange
 *is true, then the changes will be stored when the hk_presentation object is closed.
 *Example: SELECT * from addresses ORDER BY city DESC , so you would call set_sorting("city DESC");
 */
        void    set_sorting(const hk_string& s,bool registerchange=true);
        hk_string sorting(void);
        void set_temporarysorting(const hk_string& s);
        hk_string temporarysorting(void);
        void set_use_temporarysorting(bool use);
        bool use_temporarysorting(void);

/**
 *clears the sorting instruction
 *@param registerchange if this class is part of a hk_presentation object (i.e. a form or a report) and registerchange
 *is true, then the changes will be stored when the hk_presentation object is closed.
 */
        void    clear_sorting(bool registerchange=true);
/**
 *when data has changed and the row selector is moved to another row, usually the datasource will
 *be automatically updated. If the value is set to false, you have to update the data manually by calling
 * @ref store_changed_data.
 *@param u If you don't want the automatic update of data changes, set this paramter to false. The default
 *value is true.
 */
        void    set_automatic_data_update(bool u);
        bool    is_automatic_data_update(void);
        
        void    set_sqldelimiter(const hk_string&);
/**
 *sets the default date and timeformats for new created columns
 */
        void    set_datetimeformat(const hk_string& datetime,const hk_string& date,const hk_string& time)
        {
            p_datetimeformat=datetime;
            p_dateformat=date;
            p_timeformat=time;
        }
	hk_string dateformat(void)const {return p_dateformat;}
	hk_string datetimeformat(void)const {return p_datetimeformat;}
	hk_string timeformat(void)const {return p_timeformat;}
        hk_database* database(void);
/**
 *stores the object's values to stream s. The following tags will be used
 *
 *&lt;TYPE> either QUERY or TABLE
 *
 *&lt;DATASOURCENAME>  the name set with @ref set_name
 *
 *&lt;SQL> the used sql statement
 *
 *Additionally the section &lt;DATASOURCEEXTRA> has the following tags
 *
 *&lt;FILTER> the filter set with @ref set_filter
 *
 *&lt;SORTING> the sorting direction set with @ref set_sorting
 *
 *&lt;DATETIMEFORMAT>
 *
 *&lt;DATEFORMAT>
 *
 *&lt;TIMEFORMAT>
 *
 *if this datasource is part of a @ref hk_presentation also the following tags will be stored
 *
 *&lt;UPN>   the unique presentation number. See @ref presentationnumber
 *
 *&lt;DEPENDING_ON>  the depending on datasource set with @ref set_depending_on_presentationdatasource
 *
 *one or more &lt;DEPENDINGFIELDS> sections with the following tags
 *
 *&lt;THISFIELD>   set with @ref add_depending_fields
 *
 *&lt;MASTERFIELD> set with @ref add_depending_fields
 *
 *@param s the stream to which the data will be written
 *@param saveall if true the section &lt;DATASOURCEEXTRA> will be stored additionally to the normal data
 */
        void  savedata(ostream& s,bool saveall=true);
/**
 *loads the object's values out of the definition string.
 */
        void  loaddata(xmlNodePtr definition,bool loadsql=true);
/**
 *  stores the datasource as a query file
 *@return true if successful else false
 */
//      bool 	savequerytofile(void);
/**
 *  load the datasource with the values of a query file
 *@return true if successful else false
 */
//      bool 	loadqueryfromfile(void);
/**
 * if the datasource is part of a @ref hk_presentation each datasource gets a unique datasource number
 *as an identifier
 *@return  the unique datasource number if part of a presenation else -1
 */
        long  presentationnumber(void) const {return p_dscounter;}
/**
 * @return true if a hk_visible object or a depending datasource is using this datasource
 *else false
 */
        bool  datasource_used(void) const;
#ifdef HK_DEBUG
/**
 *@internal
 * prints all data of this datasource to the standard output
 */
        virtual        void   dump_data() ;
#endif
/**
 *asks interactively for a name
 */
        bool ask_name(void);
/**
 *  Delete rows beginning from row number "from" to row number "to"
 */
virtual        bool delete_rows(unsigned long from,unsigned long to,bool check_depending_datasources=true,enum_interaction c=interactive);
/**
 * Start a transaction. If the SQL server does not support transaction this function does nothing
 *@param name the name of the transaction
 */
        void transaction_begin(hk_string name="");
/**
 * Commits  a transaction. If the SQL server does not support transaction this function does nothing
 *@param name the name of the transaction
 */
        void transaction_commit(hk_string name="");
/**
 * Undo a transaction. If the SQL server does not support transaction this function does nothing
 *@param name the name of the transaction
 */

        void transaction_rollback(hk_string name="");

/**
 *if true no visual objects will be informed when data changes in any kind, except the datasource will be deleted
 *Default is false
 */
        void set_blockvisiblesignals(bool v);
        bool blockvisiblesignals(void);
/**
 *if true no data will be sent to the database server when data changes in any kind
 *Default is false
 */
        void set_blockserversignals(bool);
        bool blockserversignals(void);
/**
 *if true no depending datasources will be informed when data changes in any kind
 *Default is false
 */
        void set_blockdatasourcesignals(bool);
        bool blockdatasourcesignals(void);
/**
 *stores the structure of the datasource (fields,indices) in an xml format file and is database driver independent
 */
        void save_datasourcedefinition(ostream& s);
/**
 *loads the structure of the datasource (fields,indices) in an xml format file and creates the tableand is database driver independent
 */
        bool load_datasourcedefinition(xmlNodePtr definition,bool use_xmltablename=true ,bool ask=true);
hk_presentation* presentation(void);
/**
*the position will be used for the database designer GUI. Mostly you don't have to manipulate the values
*/
 void set_position(int x,int y, bool registerchange=true);
/**
*the size will be used for the database designer GUI. Mostly you don't have to manipulate the values
*/
 void set_size(int width,int height, bool registerchange=true);
 int x(void) const;
 int y(void) const;
 int width(void) const;
 int height(void) const;
 void set_designsize(int w,int h,bool registerchange=true);
 int designwidth() const;
 int designheight() const;
 void   automatic_position_datasource(void);

list<hk_datasource*>* dependinglist(void);
/**
* returns true, if this datasource could not be enabled last time when tried
*/
  bool previous_enable_problems(void) const;

list <referentialclass>* referenceslist(void);
/** will try to add a new reference
* to a table
*/
bool  add_reference(const referentialclass&);
bool  drop_reference(const hk_string&);


hk_string fieldorigin(const hk_string&);
hk_string totalfieldorigin(const hk_string&);
void    reset_changed_data(void);
bool has_changed(void) const;
/**
*returns a transformed column name in case the driver does only support e.g. ASCII characters
*/
hk_string systemcolumnname(const hk_string& n);

static void set_enablefunction(voidfunction*, long counter);

        bool   depending_on_datasource_deleterow_ok(void);
        bool   depending_on_datasource_before_delete_row(void);
        bool   depending_on_datasource_updaterow_ok(void);
        bool check_store_changed_data();
        void set_progressdialog(progress_dialogtype* progressdialog);
        progress_dialogtype* progressdialog(void)const;
        void set_progressinterval(int);
        int progressinterval(void) const;

    
    
    
    
    
    protected:
/**
 *if database driver supports cassensitive column and table names p_casesensitive is true else p_casesensitive is false.
 *Then internally column_by_name() and name() return values in small letters
 */
        bool p_casesensitive;
        hk_datasource(hk_database* db,hk_presentation* p=NULL);
        void    column_remove(hk_column* col);
        void    visible_add(hk_dsvisible* v);
        void    visible_remove(hk_dsvisible* v);
        virtual void driver_specific_transaction_begin(hk_string);
        virtual void driver_specific_transaction_commit(hk_string);
        virtual void driver_specific_transaction_rollback(hk_string);
        virtual unsigned long driver_specific_max_rows(void){return 0;}
        virtual bool driver_specific_set_accessmode(enum_accessmode){return true;}
        virtual bool driver_specific_batch_enable(void){return false;}
        virtual bool driver_specific_batch_disable(void){return false;}
        virtual bool driver_specific_batch_goto_next(void){return false;}
        virtual bool driver_specific_batch_goto_previous(void){return false;}
/**
 * has to make sure that the columns-list has been created. It also has to provide access to the data
 * of the datasource
 */
        virtual bool    driver_specific_enable(void){return false;}
        virtual bool    driver_specific_disable(void){return false;}
/**
 * returns a list of the columns of the datasource. If the datasource is a table the list hast to be created
 * even if the datasource is disabled. Fill the p_columns list for this reason and then return it.
 * If necessary has to create one hk_column object per column and to add them to p_columns. Each column object has to
 * get a unique number in its (protected) hk_column::p_fieldnr .
 */
        virtual list<hk_column*>* driver_specific_columns(void){return NULL;}
        virtual bool    driver_specific_goto_row(unsigned long r);
        virtual hk_column* driver_specific_new_column(void){return NULL;}
        virtual bool    driver_specific_update_data(void){return true;}
        virtual bool    driver_specific_delete_data_at(unsigned long){return true;}
        virtual bool    driver_specific_insert_data(void){return true;}
        virtual bool    driver_specific_create_table_now(void){return false;}
        virtual bool    driver_specific_alter_table_now(void){return false;}
        virtual bool    driver_specific_create_view_now(void);
        virtual bool    driver_specific_alter_view_now(void);

	virtual void    driver_specific_before_drop_table(void){}
        virtual void    filelist_changes(listtype t);
        virtual list<indexclass>* driver_specific_indices(void){return NULL;}
        virtual bool    driver_specific_drop_index(const hk_string& i);
        virtual bool    driver_specific_create_index(const hk_string& name,bool unique,list<hk_string>& fields);
        virtual void before_connection_disconnects(void);
        virtual void before_source_vanishes(void);
	/**
	*will be called from hk_database
	*/
        virtual void driver_specific_after_copy_table(){}
	virtual bool driver_specific_load_view(){return false;}
	virtual void driver_specific_create_viewsql(){}// used for ds_view
	virtual bool driver_specific_add_reference(const referentialclass &);
	virtual bool driver_specific_drop_reference(const hk_string& );
	virtual void driver_specific_referenceslist(void);
        void    depending_ds_add(hk_datasource* d);
        void    depending_ds_remove(hk_datasource* d);
        void    depending_on_datasource_row_change(bool take_changed_data=false);
        void    depending_on_datasource_data_has_changed(void);
        void    depending_on_datasource_after_store_changed_data(void);
        void    depending_on_datasource_insertmode(void);
        void   depending_on_datasource_before_update_row(void);
        void   depending_on_datasource_after_update_row(void);
        void    reload_data(bool take_changed_data=false);
        void    create_actual_row_where_statement(void);
        hk_string create_row_where_statement_at(unsigned long int position,bool withwhere=true);
        void    create_new_sql_statement(bool take_changed_data=false);
        hk_string whole_datasource_where_statement(bool take_changed_data=false);
	void print_sql(void);

	bool    p_enabled;
        bool    p_readonly;
        bool    p_primary_key_used;
        bool    p_ignore_changed_data;
        void    clear_columnlist(void);
        void    clear_modecolumnlists(void);
        void    clear_visiblelist(void);
        void    parse_sql(void);
        virtual bool    update_row(enum_interaction c=interactive);
        virtual bool    insert_row(enum_interaction c=interactive);
        virtual bool    delete_row(enum_interaction c=interactive);
        unsigned long   p_counter;                // actual row number
        hk_database*    p_database;
        list<hk_column*>*      p_columns;
        typedef class
        {
            public:
                hk_string name;
                hk_string newname;
                hk_column::enum_columntype type;
                long size;
                hk_string defaultvalue;
                bool primary;
                bool notnull;
        } class_altercolumns;

	list<class_altercolumns>       p_altercolumns;
        list<hk_string>       p_deletecolumns;
        list<hk_column*>       p_newcolumns;
        list<hk_string> depending_this_fields;
        list<hk_string> depending_master_fields;

       typedef class fieldoriginclass
       {
         public:
	 hk_string fieldname;
	 hk_string alias;
	 fieldoriginclass()
	  {
	  }
	 bool operator=(const fieldoriginclass& o)
	   {
	     fieldname=o.fieldname;
	     alias=o.alias;
	     return true;
	   }
       };

/**
 *struct_parsed_sql is needed for parsing th SQL-statement in hk_datasource
 */
        typedef class
        {
            public:
                hk_string
                    select_part,
                    from_part,
                    where_part,
                    groupby_part,
                    having_part,
                    orderby_part ;
		    list <fieldoriginclass> fieldpart;
		    list <pair<hk_string,hk_string> > tablepart;
        } struct_parsed_sql;



        struct_parsed_sql* p_parsed_sql;
	void parse_fieldpart(void);
	void parse_tablepart(void);
        hk_string   p_sql;
	hk_string   p_viewsql;
        hk_string  p_actual_row_where;
        bool    p_automatic_data_update;
        hk_actionquery* p_actionquery;
        hk_string p_datetimeformat;
        hk_string p_dateformat;
        hk_string p_timeformat;
        hk_string   p_sql_delimiter;
        hk_string       p_true;
        hk_string       p_false;
        bool   p_depending_on_is_left_join;

        bool    p_tablesql_set;
        void set_has_changed(void);
        void set_has_not_changed(void);
	hk_string delimit_identifier(const hk_string&);
        static voidfunction* p_enablefunction;// a  function that will be called during enable
	// used from the GUI, example: KDE should call processEvents to refresh display
        static long p_enablefunctioncounter;
        void    inform_before_row_change(void);
	void    inform_visible_objects_batch_goto_next(void);
	void    inform_visible_objects_batch_goto_previous(void);
        void    inform_visible_objects_before_store_changed_data(void);
        void    inform_visible_objects_after_store_changed_data(void);
        void    inform_visible_objects_row_change(void);
        void    inform_visible_objects_row_add(void);
        void    inform_visible_objects_row_delete(void);
        void    inform_visible_objects_before_insert_row(void);
        void    inform_before_enable(void);
        void    inform_before_disable(void);
        void    inform_visible_objects_ds_disable(void);
        void    inform_visible_objects_ds_enable(void);
        void    inform_depending_ds_goto_row(void);
        void    inform_depending_ds_store_data(void);
        void    inform_depending_ds_data_has_changed(void);
        void    inform_depending_ds_after_store_changed_data(void);
        void    inform_depending_ds_before_update_row(void);
        void    inform_depending_ds_after_update_row(void);
        void    inform_depending_ds_disable(void);
        void    inform_depending_ds_enable(void);
        void    inform_depending_ds_insertmode(void);
        void    inform_visible_objects_new_columns_created(void);
        void    inform_visible_objects_before_columns_deleted(void);
        void    inform_visible_objects_insertmode(void);
        void    inform_when_table_structure_changes(void);
        void    inform_when_indexlist_changes(void);
	void execute_visible_object_script_before_update(void);
	void execute_visible_object_script_after_update(void);
	void execute_visible_object_before_row_change(void);
	void execute_visible_object_after_row_change(void);
	void execute_visible_object_before_delete(void);
	void execute_visible_object_after_delete(void);
	void execute_visible_object_before_insert(void);
	void execute_visible_object_after_insert(void);


        list<hk_datasource*> p_dependinglist;
        enum_tablemodes p_mode;
        list<referentialclass> p_references;
	hk_string new_referencename(const referentialclass&);
	bool is_newreferencename(const hk_string&);
	hk_string replace_dates(const hk_string&);

    private:
        hk_string   p_original_sql;
        unsigned long p_length;
        bool    p_has_changed;
        list<hk_dsvisible*> p_visibles;
        hk_datasource*  p_depending_on_datasource;
        bool    p_depending_on_datasource_react_on_changed_data;
        void    mark_visible_objects_as_not_handled(void);
	void set_columns_tableorigin(void);
	hk_string columntablepart(const hk_string&);
        hk_presentation* p_presentation;
// unique number given from a hk_presentation object
        long    p_dscounter;

	hk_datasourceprivate* p_private;
}


;
#endif
