// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_firebirdclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_FIREBIRDCONNECTION
#define HK_FIREBIRDCONNECTION
#include "hk_connection.h"
#include <list>
#include <vector>
#include <algorithm>
#include <ibase.h>

//#include <hk_string.h>
class hk_drivermanager;
extern "C"
{
    hk_connection* create_connection(hk_drivermanager*);
    hk_string hk_classesversion(void);
}


class hk_firebirddatabase;
class hk_firebirdconnection: public hk_connection
{
    friend class hk_firebirddatabase;
    public:
        hk_firebirdconnection(hk_drivermanager* c);
        ~hk_firebirdconnection();
        void servermessage(const hk_string& error);
        virtual bool server_supports(support_enum)const;
        virtual bool server_needs(need_enum)const;
        virtual hk_string drivername(void) const;
        virtual unsigned int    default_tcp_port(void) const;
       isc_db_handle handle() {return p_firebirdhandle;}
virtual        bool    create_database(const hk_string& dbase);
virtual long int maxcharsize(void) const {return 32765;}

   protected:
	bool            driver_specific_connect();
        bool            driver_specific_disconnect();
        hk_database*        driver_specific_new_database(void);
        vector<hk_string>*  driver_specific_dblist(void);
        void set_error();
        bool driver_specific_delete_database(const hk_string& dbase);
        hk_string p_firebirddatabase;
        bool p_connectionsuccessfullyestablished;
        isc_db_handle p_firebirdhandle;
    ISC_STATUS_ARRAY p_status;

} ;
#endif
