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
#ifndef HK_CONNECTION
#define HK_CONNECTION
#include "hk_class.h"
#include <list>
#include <vector>
#include <algorithm>
using namespace std;
class hk_drivermanager;
class hk_connectionprivate;

class hk_database;
/**
 *@short connects to the SQL Server
 *@version $Revision: 1.66 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *connects to the SQL Server. Set @ref set_host,@ref set_user, @ref set_password ,
 *@ref set_tcp_port if necessary and then call @ref connect.

 */
class hk_connection:public hk_class
{
    friend class hk_database;
    friend class hk_drivermanager;
    public:
/**
 * When the connection will be destroyed, the database created with @ref new_database() and all
 * its children will be destroyed
 **/
        virtual     ~hk_connection();
/**
 * name of the host where the SQL Server is. Default is "localhost".
 */
        void    set_host(const hk_string& h);
        hk_string   host(void);
/**
 * user name for the SQL Server.
 */
        void    set_user(const hk_string& u);
        hk_string   user(void);
/**
 * password for the SQL Server
 */
        void    set_password(const hk_string& p);
        hk_string   password(void);
/**
 *  TCP Port number of the SQL Server. Default is 3306.
 */
        void        set_tcp_port(unsigned int t);
        unsigned int    tcp_port(void);
	void set_defaultdatabase(const hk_string&);
	hk_string defaultdatabase(void) const;
/**
 * returns the default tcp port of the the database driver;
 */
        virtual unsigned int    default_tcp_port(void) const;
/**
 * tries to connect to the SQL Server. If some information is missing (i.e. password)
 * a dialog will ask for these values. You can set a GUI dependend dialog with @ref set_passworddialog
 * @return true if connecting was successful, else false.
 */
        bool    connect(enum_interaction c=interactive);
/**
 * disconnects from the SQL Server
 * @return true if successful.
 */
        bool    disconnect(void);
/**
 *@return true  if @ref connect was successful
 */
        bool    is_connected(void);
/**
 *if connected it returns a hk_stringlist of all existing databases on the SQL Server.
 *The list will be created new every time you call this function.
 *So be careful. Never use i.e. for_each(dblist()->begin(),dblist()->end(),anyfunction)
 *Use instead:
 *list&lt;hk_string>* mylist=dblist();
 *for_each(mylist->begin(),mylist->end(),anyfunction)

 */
        vector<hk_string>* dblist(void);
/**
 * if you want to use a database on the SQL Server create a hk_database element
 * with this class. For each connection you can define one database. If you define another
 * one the old one will be destroyed.

 */
        hk_database* new_database(const hk_string& name="");

	bool rename_database(const hk_string& originalname, const hk_string& new_name);
/**
 *see also @ref serversupports. Defines what abilities and fieldtypes a database driver supports
 */
        enum support_enum
        {
            SUPPORTS_AUTOINCCOLUMN=1,
            SUPPORTS_BOOLCOLUMN=2,
            SUPPORTS_DATECOLUMN=3,
            SUPPORTS_TIMECOLUMN=4,
            SUPPORTS_DATETIMECOLUMN=5,
            SUPPORTS_BINARYCOLUMN=6,
            SUPPORTS_MEMOCOLUMN=7,
            SUPPORTS_TIMESTAMPCOLUMN=8,

	    SUPPORTS_TEXTCOLUMN=9,
            SUPPORTS_INTEGERCOLUMN=10,
            SUPPORTS_SMALLINTEGERCOLUMN=11,
            SUPPORTS_FLOATINGCOLUMN=12,
            SUPPORTS_SMALLFLOATINGCOLUMN=13,

	    SUPPORTS_PROPRIETARYCOLUMN=19,//driver specific datatype that can not be mapped to a hk_classes type

            SUPPORTS_SQL=100,
            SUPPORTS_TRANSACTIONS=101,
            SUPPORTS_REFERENTIALINTEGRITY=102,
            SUPPORTS_NEW_DATABASE=103,
            SUPPORTS_DELETE_DATABASE=104,
            SUPPORTS_NEW_TABLE=105,
            SUPPORTS_ALTER_TABLE=106,
            SUPPORTS_DELETE_TABLE=107,
            SUPPORTS_CREATE_INDEX=108,
            SUPPORTS_DELETE_INDEX=109,
            SUPPORTS_CHANGE_PASSWORD=110,
            SUPPORTS_RENAME_DATABASE=111,
            SUPPORTS_RENAME_TABLE=112,
	    SUPPORTS_VIEWS=113,
	    SUPPORTS_NEW_VIEW=114,
	    SUPPORTS_ALTER_VIEW=115,
	    SUPPORTS_DELETE_VIEW=116,
	    SUPPORTS_ADD_COLUMN=117,
	    SUPPORTS_DELETE_COLUMN=118,
	    SUPPORTS_CHANGE_COLUMNTYPE=119,
	    SUPPORTS_CHANGE_COLUMNNAME=120,
	    SUPPORTS_ALTER_PRIMARY_KEY=121,
	    SUPPORTS_ALTER_NOT_NULL=122,

	    SUPPORTS_SQL_GROUP_BY=180,
	    SUPPORTS_SQL_ORDER_BY=181,
	    SUPPORTS_SQL_HAVING=182,
	    SUPPORTS_SQL_WHERE=183,
	    SUPPORTS_SQL_ALIAS=184,
	    SUPPORTS_SQL_SUBSELECTS=185,
	    SUPPORTS_SQL_JOINS=186, // means at least INNER JOIN and LEFT (OUTER) JOIN
	    SUPPORTS_SQL_UNION=187,

	    SUPPORTS_LOCAL_FILEFORMAT=200,
	    SUPPORTS_NONALPHANUM_FIELDNAMES=201, // if not set field name can only have [a-zA-Z0-9]
	    SUPPORTS_NONASCII_FIELDNAMES=202, //unicode characters etc
	    SUPPORTS_SPACE_FIELDNAMES=203 // means  a space character can be part
        };

/**
 *give information about the database server. See @ref hk_definitions.h for details
 */
        virtual bool server_supports(support_enum) const{return false;}
/**
 *see @ref server_needs
 */
        enum need_enum
        {

            NEEDS_LOGIN=0,
            NEEDS_HOST=1,
            NEEDS_USERNAME=2,
            NEEDS_PASSWORD=3,
            NEEDS_SQLDELIMITER=4,
            NEEDS_PORT=5,
            NEEDS_DATABASENAME=6,
	    NEEDS_BOOLEANEMULATION=7,//e.g. Mysql  doesn't have natively a boolean type, so tinyint is used
	    NEEDS_NULL_TERMINATED_SQL=8, //e.g. Postgres needs null terminated strings in PQExec
	    NEEDS_TABLE_ALIAS_AS=9, // set if the server needs 'AS' between the table name and its alias name in a FROM clause
	    NEEDS_MANUAL_CHARSET=10, // per database  see hk_database::set_databasecharset
	    NEEDS_DIRECTORY_AS_DATABASE=11 //local database formats like paradox, dbase etc needs a directory as a database which contains one file per table
        };
/**
 *shows what information the database server needs (e.g. password).
 */
        virtual bool server_needs(need_enum) const {return false;}
/**
 * the passworddialog will be called from @ref connect if necessary. You can define a GUI dependend
 *dialog and set it with this function. What exactly is asked in the dialog is database driver dependend
 *(i.e. the Mysql driver needs user, password, host and tcp_port)
 */
        static  void    set_passworddialog(password_dialogtype*);
/**
 * tries to create a new database. If successful it returns true else false
 */
virtual        bool    create_database(const hk_string& dbase);
/**
 * tries to delete a database.
 *@param dbase name of an existing database
 *@return true if successful else flase
 */
virtual        bool    delete_database(const hk_string& dbase,enum_interaction c=interactive);
/**
 * checks whether a database exists
 */
        bool    database_exists(const hk_string& databasename);
/**
 * sets a new password at the SQL server.
 *@param newpassword the new password
 *@return true if successful else flase

 */
        bool set_newpassword(const hk_string& newpassword);
/**
 *shows a password dialog and sets the new password by calling @ref set_newpassword
 *If no passworddialog is set with @ref set_newpassworddialog the necessary information
 *will be queried at the standard output
 */
        void show_newpassworddialog(void);
/**
 * It is possible to define a newpassworddialog,
 * which will be shown when @ref set_newpassword is called
 */
        static void set_newpassworddialog(newpassword_dialogtype*);
/*
 *shows a password dialog
 *@return false if it has been cancelled else true
 */
        bool show_passworddialog(void);
/**
 *returns the name of the driver (e.g Mysql,Postgres etc.)
 */

        virtual hk_string drivername(void) const;
/**
*If the database does not have a native boolean type(like Mysql),
*but hk_classes should use one datatype to emulate, set this to true (default) else switch it off.
*
*/
	void set_booleanemulation(bool emulate);
        bool booleanemulation(void) const;
        hk_drivermanager* drivermanager(void);
/**
* copies whole databases
*@param fromdatabase the database that should be copied
*@param schema_and_data if true both the table schemas will be created and the data will be copied, if false only the table will be created
*@param copy_local_files if true queries,forms and reports will be also copied
*@param progressdialog if set this function will be regularly called to show the progress
*/
        bool copy_database(hk_database* fromdatabase,bool schema_and_data,bool copy_local_files,progress_dialogtype* progressdialog=NULL);
        hk_string last_servermessage(void){return p_lastservermessage;}
        hk_string databasepath(void);
/**
*returns messages of possible problems when copying tables between two different backends
*/
static    hk_string check_capabilities(hk_connection* from,hk_connection* to);

/**
*returns the mimetype of a local database format, if it is not a local database format it returns an empty string
*/
virtual hk_string mimetype(void) const {return "";}
virtual unsigned int maxfieldnamesize(void) const {return (unsigned int)-1;}

/**
*returns the maximum length of the char() sql columntype
*/
virtual long int maxcharsize(void) const {return 255;}

virtual hk_string left_join_statement(void) {return " LEFT JOIN ";}
virtual hk_string inner_join_statement(void) {return " INNER JOIN ";}

bool is_sqlstatement(const hk_string&);

    protected:
        hk_connection(hk_drivermanager* c);
        bool    p_connected;
        virtual bool    driver_specific_connect(void){return false;}
        virtual bool    driver_specific_disconnect(void){return false;}
	virtual bool    driver_specific_rename_database(const hk_string& /*oldname*/,const hk_string& /*newname*/){return false;}
        virtual  vector<hk_string>* driver_specific_dblist(void){return NULL;}
        virtual hk_database* driver_specific_new_database(void){return NULL;}
        virtual bool    driver_specific_new_password(const hk_string&){return false;}
        virtual bool    driver_specific_delete_database(const hk_string& dbase);

        void    db_remove(hk_database* d);
        hk_string sqldelimiter(void) ;
        void inform_databases_connect(void);
        void inform_databases_disconnect(void);
        vector<hk_string> p_databaselist;
        list<hk_string> p_sqlstatements;
/**
 *saves the tags &lt;HOST>&lt;USER>&lt;TCP-PORT>&lt;SQLDELIMITER> with the equivalent values
 */
        virtual void savedata(ostream& s);
/**
 *load the tags &lt;HOST>&lt;USER>&lt;TCP-PORT><SQLDELIMITER> with the equivalent values
 */
	virtual void loaddata(xmlNodePtr definition);

        void set_last_servermessage(const hk_string& m);
        hk_database* database(void) const {return p_database;}
	static bool internal_copy_check(hk_connection* from, hk_connection* to,support_enum);
        void delete_databasedirectory(const hk_string& db);
        void delete_directory(const hk_string& d);
    private:
        void load_configuration(void);
        void save_configuration(void);
        bool copy_local_files(hk_database* fromdb,hk_database* todb,objecttype f,progress_dialogtype* progressdialog);
        hk_string ask_dbname(void);
        void make_databasedir(const hk_string& dbname);
        void set_classespath(hk_string& p);

        hk_database* p_database;
        unsigned int p_debug_counter;
        static  password_dialogtype* p_passworddialog;
        static  newpassword_dialogtype* p_newpassworddialog;
        hk_drivermanager* p_drivermanager;
        hk_string   p_lastservermessage;
      hk_connectionprivate* p_private;
} ;
#endif
