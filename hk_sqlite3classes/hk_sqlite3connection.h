// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_sqlite3classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_SQLITE3CONNECTION
#define HK_SQLITE3CONNECTION
#include <sqlite3.h>
#include "hk_connection.h"
#include <list>
#include <vector>
#include <algorithm>
//#include <hk_string.h>
class hk_drivermanager;
extern "C"
{
    hk_connection* create_connection(hk_drivermanager*);
    hk_string hk_classesversion(void);
}


class hk_sqlite3database;
class hk_sqlite3connection: public hk_connection
{
    public:
        hk_sqlite3connection(hk_drivermanager* c);
        ~hk_sqlite3connection();
        void servermessage(const hk_string& error);
//virtual int server_supports(void);
        virtual bool server_supports(support_enum)const;
//virtual int server_needs(void);
        virtual bool server_needs(need_enum)const;
        virtual hk_string drivername(void) const;
        virtual unsigned int    default_tcp_port(void) const;
virtual        bool    create_database(const hk_string& dbase);
virtual        bool    delete_database(const hk_string& dbase,enum_interaction c=interactive);
virtual hk_string mimetype(void) const {return "application/x-sqlite3";}

    protected:
        bool            driver_specific_connect();
        bool            driver_specific_disconnect();
        bool            driver_specific_new_password(const hk_string&);
        vector<hk_string>*  driver_specific_dblist(void);
        hk_database*        driver_specific_new_database(void);

    private:


} ;
#endif
