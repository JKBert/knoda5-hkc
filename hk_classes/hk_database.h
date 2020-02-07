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
#ifndef HK_DATABASE
#define HK_DATABASE
#include <hk_class.h>
#include <list>
#include <vector>
#include <algorithm>
#include "hk_presentation.h"
#include "hk_url.h"
using namespace std;

class hk_connection;
class hk_datasource;
class hk_data;
class hk_dbvisible;
class hk_actionquery;
class hk_form;
class hk_report;
class hk_dsmodevisible;
class hk_databaseprivate;
class hk_dsquery;
class hk_dstable;

typedef hk_form* newform_type(hk_database*,hk_class*);
typedef hk_report* newreport_type(hk_database*,hk_class*);
typedef hk_dstable* newtable_type(hk_class*);
typedef hk_dsquery* newquery_type(hk_class*);

/**
 *@short represents a particular database on the SQL Server
 *@version $Revision: 1.64 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *represents a particular existing database on the SQL Server. To use
 *it set the name of an existing database with @ref set_name
 *To create a new database use @ref hk_connection::create_database.
 */

class hk_database:public hk_class
{
    friend class hk_datasource;
    friend class hk_actionquery;
    friend class hk_connection;
    friend class hk_dbvisible;
    friend class hk_presentation;
    public:
        virtual     ~hk_database();
        enum enum_storage{st_overwrite,st_storechanges};

/**
 * name of the database.
 */
virtual bool    set_name(const hk_string& n);
        hk_string   name(void);

/**
 *if hk_connection is connected, the tablelist shows all existing tables
 *in this database after you set the name of an existing database with @ref set_name.
 *The list will be created new every time you call this function.
 *So be careful. Never use i.e. for_each(tablelist()->begin(),tablelist()->end(),anyfunction)
 *Use instead:
 *vector<hk_string>* mylist=tablelist();
 *for_each(mylist->begin(),mylist->end(),anyfunction)

 */
        vector<hk_string>* tablelist(bool with_systemtable=false);

        vector<hk_string>* viewlist();
        vector<hk_string>* tableviewlist(bool with_systemtable=false);


/**
 * @return a list of the names of all existing queries in this database
 */
        vector<hk_string>* querylist(void);
/**
 * @return a list of  the names of all existing form in this database
 */
        vector<hk_string>* formlist(void);
/**
 * @return a list of the names of all existing reports in this database
 */
        vector<hk_string>* reportlist(void);

/**
 * @return a list of the names of all existing reports in this database
 */
        vector<hk_string>* modulelist(void);
/**
 *if you want to work with a particular table, create a hk_datasource element with this function.
 In a table, the data can be edited.
*/
        hk_datasource* new_table(const hk_string& name="",hk_presentation* p=NULL);
/**
 *if you want to work with your own dmlquery ( a SELECT ... statement), create a hk_datasource element with
 *this function (dml= data manipulation language).
 *All Select queries are possible (i.e. GROUP BY). The resulting data cant be edited.
 */
        hk_datasource* new_resultquery(hk_presentation* p=NULL);
/**
 *  If you want to work with your own ddl queries, create a hk_action element with this function.
 *A ddlquery (ddl= Data Definition Language) allows you to execute
 *all non-SELECT queries (i.e. UPDATE, INSERT queries). This type of query has no result.
 */
        hk_actionquery*   new_actionquery(void);


	hk_datasource* new_view(const hk_string& name="",hk_presentation* p=NULL);

/**
 * loads an existing table or resultquery
 *@param name name of the query or table
 *@param query if true this function tries to load a query with the name "name" else a table
 *@param p <internal use only> if this datasource is part of a form or a report this is the pointer to this
 *hk_presentation object
 *@return a datasource (either a table or a resultquery) if successful else NULL
 */
	hk_datasource*  load_datasource(const hk_string& name,datasourcetype dt=dt_table,hk_presentation* p=NULL);

/**
 * removes  a table from the database and deletes all data.
 */

virtual bool    delete_table(const hk_string& table,enum_interaction x=interactive);
virtual bool    delete_view(const hk_string& view, enum_interaction x=interactive);
/**
 * checks whether a table exists
 */
        bool    table_exists(const hk_string& tablename);
        bool    view_exists(const hk_string& viewname);
        bool    query_exists(const hk_string& queryname);
        bool    form_exists(const hk_string& formname);
        bool    report_exists(const hk_string& reportname);

/**
@return the parent connection
*/
        hk_connection*  connection(void)const{return p_connection;}
/**
 * creates a stream object to store a file.  Usually you don't have to use this function
 *@param name the name of the file
 *@param type the type of the file (i.e. query or form)
 *@param ask_before_overwrite if true and a file of this type and this name already exists you will
 *be asked whether or not to overwrite this file
 *@return the stream if successful else NULL
 */
        ofstream* savestream(const hk_string&name,filetype type,bool ask_before_overwrite=true );
        ofstream* savestream(const hk_string&name,filetype type,bool ask_before_overwrite,bool with_header,bool ask_for_new_name=false );
        
        ofstream* savestream(enum_storage storage,const hk_string&name,filetype type,bool ask_before_overwrite,bool with_header,bool ask_for_new_name);
    STRINGSTREAM* savestringstream(filetype type);

    STRINGSTREAM* savestringstream(filetype type,bool with_header);



/**
 *deletes a file physically.  Usually you don't have to use this function
 *@param name name of the file
 *@param type type of the file (i.e. query or form)
 *@return true if successful else false
 */
        bool  delete_file(const hk_string& name,filetype type,enum_interaction x=interactive);
/**
 * looks whether there is a already created hk_presentation object (form or report)
 *usefull if you need a handle to an already opened form
 *@param name name of the presentation
 *@param t type of the presentation( form or report)
 *@return the object if existing else NULL
 */
        hk_presentation* existing_presentation(const hk_string& name, hk_presentation::enum_presentationtype t);
	hk_form* existing_form(const hk_string name);
	hk_report* existing_report(const hk_string name);

/**
 * save physically in a file. Usually you don't have to use this function
 * @param statement the content to store
 * @param name the name of the file
 * @param type the type of the file (i.e. form or query)
 */
        void save(const hk_string& statement,const hk_string& name,filetype type,bool ask_before_overwrite=true,bool ask_for_new_name=false );
        
        void save(enum_storage storage,const hk_string& statement,const hk_string& name,filetype type,bool ask_before_overwrite=true,bool ask_for_new_name=false );
/**
 * load physically from a file. Usually you don't have to use this function
 * @param name the name of the file
 * @param type the type of the file (i.e. form or query)
 * @return the content of the file
 */
        hk_string load(const hk_string& name,filetype type);
	xmlNodePtr xmlload(const hk_string& name,filetype type);
/**
 *returns the path where hk_classes stores forms, queries, and reports
 *usually ~/.hk_classes/dbdriver/host/dbname
 */
        hk_string database_path(void);
        hk_string fileendings(filetype e);
        hk_string fileendings(objecttype e);
/**
 *creates a new table and uses an existing datasource as a template
 *@param fromdatasource the datasource which is used as a template
 *@param schema_and_data if true the table will be created and the data copied, if false the data will be not copied
 *@param replacetable see parameter ask for details
 *@param ask  ask=true and replacetable=true   => you will be warned before overwriting an old table
             ask=true and replacetable=false  => a new name will be asked
         ask=false and replacetable=true  => an old table will be overwritten without warning
         ask=false and replacetable=false => copy_table immediately stops and returns false, if a table already exists
*

*/
        bool copy_table(hk_datasource* fromdatasource,bool schema_and_data,bool replacetable, bool ask,progress_dialogtype* progressdialog=NULL);
        bool copy_view(hk_datasource* fromdatasource,progress_dialogtype* progressdialog=NULL,const hk_string& newname="");

hk_form* new_formvisible(void);
hk_form* new_dialogformvisible(void);
hk_report* new_reportvisible(void);
hk_dstable* new_tablevisible(void);
hk_dsquery* new_queryvisible(void);
static void	set_new_formvisiblefunction(newform_type*);
static void	set_new_dialogformvisiblefunction(newform_type*);
static void	set_new_reportvisiblefunction(newreport_type*);
static void	set_new_tablevisiblefunction(newtable_type*);
static void	set_new_queryvisiblefunction(newquery_type*);
void set_usewidgetparent(hk_class*);
/**
* renames the table
*@param originaltable old name of the table
*@param newname new name of the tbale,
*@param x if set to interactive warningmessages wil be displayed in case of an error
*/
bool rename_table(const hk_string& originalname, const hk_string& newname,enum_interaction x=interactive);
/**
* renames files (queries, forms and reports) and also handles table renaming
*@param originaltable old name of the table
*@param newname new name of the tbale,
*@param filetype the type of file, see @ref hk_definitions::filetype for details
*@param x if set to interactive warningmessages wil be displayed in case of an error
*/
bool rename_file(const hk_string& originalname, const hk_string& newname,filetype type,enum_interaction x=interactive);
/**
*informs the datasources and visible objects that the database shuts down and sets the
*database name to ""
*/
void disable(void);

bool store_connectionfile(const hk_url& url, bool store_password=false);


enum enum_storagemode {central,local};
void set_storagemode(filetype, enum_storagemode load,enum_storagemode store);
enum_storagemode storagemode(filetype);
enum_storagemode loadmode(filetype);
bool create_centralstoragetable(void);
bool has_centralstoragetable(void);
hk_string load_local(const hk_string& name,filetype type);
hk_string load_central(const hk_string& name,filetype type);
xmlNodePtr xmlload_local(const hk_string& name,filetype type);
xmlNodePtr xmlload_central(const hk_string& name,filetype type);
void save_local(enum_storage storage,const hk_string& statement,const hk_string& name,filetype type,bool ask_before_overwrite,bool ask_for_new_name);
void save_local(const hk_string& statement,const hk_string& name,filetype type,bool ask_before_overwrite,bool ask_for_new_name);
void save_central(const hk_string& statement,const hk_string& name,filetype type,bool ask_before_overwrite,bool ask_for_new_name);
void save_central(enum_storage storage,const hk_string& statement,const hk_string& name,filetype type,bool ask_before_overwrite,bool ask_for_new_name);
bool  delete_localfile(const hk_string& name,filetype type,enum_interaction x=interactive);
bool  delete_centralfile(const hk_string& name,filetype type,enum_interaction x=interactive);
vector<hk_string>* filelist(filetype type);
vector<hk_string>* local_filelist(filetype type);
vector<hk_string>* central_filelist(filetype type);

void save_configuration(void);
list <hk_presentation*>* presentationlist() const;

void set_databasecharset(const hk_string& charset);
hk_string databasecharset(void) const;
        void    set_automatic_data_update(bool u);
        bool    is_automatic_data_update(void);

    protected:
        hk_database(hk_connection* c);
        bool    select_db( const hk_string& newname);
        void inform_datasources_before_closing(void);
	/**
	*driver_specific_tablelist has to set the protected variable p_tablelist
	*/
        virtual  void driver_specific_tablelist(void){}
        virtual  void driver_specific_viewlist(void){}
        virtual hk_datasource* driver_specific_new_table(hk_presentation* p);
        virtual hk_datasource* driver_specific_new_view(hk_presentation* p);
        virtual hk_datasource* driver_specific_new_resultquery(hk_presentation* p);
        virtual hk_actionquery*   driver_specific_new_actionquery(void){return NULL;}
	virtual bool    driver_specific_rename_table(const hk_string& /*oldname*/,const hk_string& /*newname*/){return false;}
        virtual bool driver_specific_select_db(void){return false;}
        virtual void driver_specific_set_name(void){}
        hk_connection* p_connection;
        void    ds_remove(hk_data* ds);
        virtual void connection_connected(void);
        virtual void connection_disconnected(void);

        vector<hk_string> p_tablelist;
	vector<hk_string> p_viewlist;
	vector<hk_string> p_tableviewlist;
        void    dbvisible_add(hk_dbvisible* v);
        void    dbvisible_remove(hk_dbvisible* v);
        void    clear_visiblelist(void);
        virtual void before_source_vanishes(void);
        void    presentation_add(hk_presentation* p);
        void    presentation_remove(hk_presentation* p);
        void load_configuration(void);
        virtual void savedata(ostream& s);
	virtual void loaddata(xmlNodePtr definition);
        hk_url p_url;
        void inform_datasources_filelist_changes(listtype type);
    private:
        void clear_presentationlist();
        bool in_presentationload(void);
        void mark_datasources_as_not_handled(void);
        void mark_visible_objects_as_not_handled(void);
        hk_string savemessage(filetype t,enum_storage s, hk_string name);
	hk_databaseprivate* p_private;
        static newform_type *p_newformfunction;
        static newform_type *p_newdialogformfunction;
        static newreport_type *p_newreportfunction;
        static newtable_type *p_newtablefunction;
        static newquery_type *p_newqueryfunction;
	void save_storage(ostream&,filetype);
	void load_storage(xmlNodePtr,filetype);


} ;
#endif
