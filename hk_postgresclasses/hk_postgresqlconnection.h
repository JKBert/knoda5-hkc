// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Edited by Anirban Biswas <utpal@cal2.vsnl.net.in>
// This file is part of the hk_postgresclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_POSTGRESQLCONNECTION
#define HK_POSTGRESQLCONNECTION
#include <libpq-fe.h>
#include <hk_connection.h>
#include <list>
#include <vector>
#include <algorithm>
class hk_drivermanager;
extern "C"
{
    hk_connection* create_connection(hk_drivermanager*);
    hk_string hk_classesversion(void);
}


class hk_postgresqldatabase;
class hk_postgresqlconnection: public hk_connection
{
    friend class hk_postgresqldatabase;
    public:
        hk_postgresqlconnection(hk_drivermanager* c);
        ~hk_postgresqlconnection();
        PGconn* dbhandler(void);
        void servermessage(void);
        void getdbhandler(PGconn *newconnection);
//virtual int server_supports(void);
        virtual bool server_supports(support_enum) const;
        virtual bool server_needs(need_enum) const;
//virtual int server_needs(void);
        virtual hk_string drivername(void) const;
        virtual unsigned int    default_tcp_port(void) const;

        void    set_postgresdatabase(const hk_string& db);
    protected:
        bool            driver_specific_connect();
        bool            driver_specific_disconnect();
        bool            driver_specific_new_password(const hk_string&);
        vector<hk_string>*  driver_specific_dblist(void);
        hk_database*        driver_specific_new_database(void);
    virtual bool    driver_specific_delete_database(const hk_string& dbase);

    private:

//MYSQL* p_SQL_Connection;

        PGconn *p_SQL_Connection;
        hk_string p_postgresdatabase;
        bool p_connectionsuccessfullyestablished;
} ;
#endif
