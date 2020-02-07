// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_odbcclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_ODBCCONNECTION
#define HK_ODBCCONNECTION
#include <sql.h>
#include "hk_connection.h"
#include <list>
#include <vector>
#include <algorithm>
class hk_drivermanager;
extern "C"
{
    hk_connection* create_connection(hk_drivermanager*);
    hk_string hk_classesversion(void);
}


class hk_odbcdatabase;
class hk_odbcconnectionprivate;
class hk_odbcconnection: public hk_connection
{
    friend class hk_odbcdatasource;
    friend class hk_odbcactionquery;
    public:
        hk_odbcconnection(hk_drivermanager* c);
        ~hk_odbcconnection();
        SQLHENV envhandler(void);
        SQLHDBC connectionhandle(void);

        void servermessage(void);
        virtual bool server_supports(support_enum) const;
        virtual bool server_needs(need_enum) const;
        virtual hk_string drivername(void) const;
        virtual unsigned int    default_tcp_port(void) const;
        enum enum_backend
        {
            unknown,
            mysql,
	    sapdb,
	    mssql,
	    oracle,
            postgresql
        };
        enum_backend backendtype(void){return p_backend;}
	bool use_unicodenames(void )const {return p_unicodenames;}
	;
    protected:
        bool            driver_specific_connect();
        bool            driver_specific_disconnect();
        bool            driver_specific_new_password(const hk_string&);
        vector<hk_string>*  driver_specific_dblist(void);
        hk_database*        driver_specific_new_database(void);

    private:

        SQLHENV p_odbcenvironment;
        SQLHDBC p_odbcconnectionhandle;
        enum_backend p_backend;
	bool p_unicodenames;
	hk_odbcconnectionprivate* p_private;

} ;
#endif
