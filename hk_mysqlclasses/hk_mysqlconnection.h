// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_mysqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_MYSQLCONNECTION
#define HK_MYSQLCONNECTION
#include <mysql.h>
#include "hk_connection.h"
#include <list>
#include <vector>
#include <algorithm>
//#include <hk_string.h>

#ifndef mysql_library_init
#define mysql_library_init mysql_server_init
#endif

#ifndef mysql_library_end
#define mysql_library_end mysql_server_end
#endif

class hk_drivermanager;
extern "C"
{
    hk_connection* create_connection(hk_drivermanager*);
    hk_string hk_classesversion(void);
}


class hk_mysqldatabase;
class hk_mysqlconnection: public hk_connection
{
    public:
        hk_mysqlconnection(hk_drivermanager* c);
        ~hk_mysqlconnection();
        MYSQL* dbhandler(void);
        void servermessage(void);
//virtual int server_supports(void);
        virtual bool server_supports(support_enum)const;
//virtual int server_needs(void);
        virtual bool server_needs(need_enum)const;
        virtual hk_string drivername(void) const;
        virtual unsigned int    default_tcp_port(void) const;

    protected:
        bool            driver_specific_connect();
        bool            driver_specific_disconnect();
        bool            driver_specific_new_password(const hk_string&);
        vector<hk_string>*  driver_specific_dblist(void);
        hk_database*        driver_specific_new_database(void);

    private:

        MYSQL* p_SQL_Connection;
static int  p_reference;

} ;
#endif
