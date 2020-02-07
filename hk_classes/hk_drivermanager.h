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
#ifndef HK_DRIVERMANAGER_H
#define HK_DRIVERMANAGER_H
#include "hk_definitions.h"
#include <list>
#include <vector>
#include "hk_class.h"
#include "hk_url.h"
#include "hk_definitions.h"
#include "hk_string.h"
using namespace std;

class hk_connection;
class hk_database;
class hk_drivermanagerprivate;
typedef hk_string requestdialogtype(void);

/**
 *
 *@short  Database Driver administration
 *@version $Revision: 1.23 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *This class allows you to handle the database drivers. You can get a new connection
 * to a database with @ref new_connection.

 */

class hk_drivermanager: public hk_class
{
    friend class hk_connection;
    public:
/**
 *@param runtime if true the user has no possibility to change table and form structures or query statements
 */
        hk_drivermanager(void);
        hk_drivermanager(bool runtime);
 virtual       ~hk_drivermanager(void);
/**
 *with this method you can create a new connection to a database.  If no driver is set
 *a dialog appears which asks for the driver. You can set a GUI dependend driverdialog
 *with @ref set_driverselectdialog.
 *@param drivername name of the wished database driver (i.e. mysql)
 *@param i if i is set to 'interactive' warningmessages and dialog boxes will be shown
@return a driver specific version of hk_connection or NULL if no driver could be found.
*/
        hk_connection* new_connection(const hk_string& drivername="",enum_interaction i=interactive);
/**
 *If an already created connection exists, it will be returned otherwise NULL
 */
        hk_connection* find_existing_connection(const hk_string& drivername,const hk_string& host,unsigned int tcp_port,const hk_string& user);

        static   vector<hk_string>* driverlist(void);
/**
 *Path to the directory where the database drivers are stored.(default: /usr/lib/hk_classes/drivers)
 */
        static   void set_path(const hk_string& path);
        static   hk_string path(void);
/**
 *you can write your own GUI dependend function to show a driver select dialog by defining a function of
 *requestdialogtype
 */
        static void set_driverselectdialog(requestdialogtype* dialog);
        static hk_string show_driverselectdialog(void);
        hk_database* open_connectionfile(const hk_url&connectionfile);
    protected:

    private:
        void init(bool runtime);
        void load_preferences(void);
	void set_preferences(xmlNodePtr preferences);

        void    connection_remove(hk_connection* c);
        static   hk_string p_hk_classespath;
        static   vector<hk_string> p_driverlist;
        static   void scan_directory(void);
        static requestdialogtype* p_driverselectdialog;
        list<hk_connection*> p_connections;
	hk_drivermanagerprivate* p_private;
};

extern "C"
{
 //void __attribute__((constructor)) hk_classes_init(void);
// void __attribute__((destructor)) hk_classes_finish(void);

}
#endif
