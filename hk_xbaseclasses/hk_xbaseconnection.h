// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_xbaseclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_XBASECONNECTION
#define HK_XBASECONNECTION
#include "hk_connection.h"
#include <list>
#include <vector>
#include <algorithm>
#include <xbsql.h>

//#include <hk_string.h>
class hk_drivermanager;
extern "C"
{
    hk_connection* create_connection(hk_drivermanager*);
    hk_string hk_classesversion(void);
}


class hk_xbasedatabase;
class hk_xbaseconnection: public hk_connection
{
    public:
        hk_xbaseconnection(hk_drivermanager* c);
        ~hk_xbaseconnection();
        void servermessage(const hk_string& error);
        virtual bool server_supports(support_enum)const;
        virtual bool server_needs(need_enum)const;
        virtual hk_string drivername(void) const;
//        MdbSQL* xbasesqlhandle(void) {return p_xbasesqlhandle;}
virtual        bool    create_database(const hk_string& dbase);
virtual        bool    delete_database(const hk_string& dbase,enum_interaction c=interactive);
virtual hk_string mimetype(void) const {return "application/x-dbase";}
// virtual unsigned int maxfieldnamesize(void) const;

   protected:
	bool            driver_specific_connect();
        bool            driver_specific_disconnect();
        hk_database*        driver_specific_new_database(void);
        vector<hk_string>*  driver_specific_dblist(void);
        //MdbSQL* p_xbasesqlhandle;
} ;
#endif
