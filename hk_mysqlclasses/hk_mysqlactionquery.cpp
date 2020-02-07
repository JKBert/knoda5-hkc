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
#include "hk_mysqlactionquery.h"
#include "hk_mysqldatabase.h"
#include "hk_mysqlconnection.h"
#include <mysql.h>
#include <errmsg.h>

hk_mysqlactionquery::hk_mysqlactionquery(hk_mysqldatabase* db) :hk_actionquery(db)
{
    mysqldb=db;
    p_identifierdelimiter="`";
}


bool hk_mysqlactionquery::driver_specific_execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_mysqlactionquery::driver_specific_execute");
#endif

    if (mysqldb==NULL) return false;
    if (!mysqldb->connection()->dbhandler()) return false;


    int i=mysql_real_query(mysqldb->connection()->dbhandler(),p_sql,p_length);


#ifdef HK_DEBUG
    hkdebug("hk_mysqlactionquery::driver_specific_execute after query");
#endif
    if (i==0)  return true;
    else
    {
        hk_string error;
        switch (i)
        {
            case CR_COMMANDS_OUT_OF_SYNC : error="CR_COMMANDS_OUT_OF_SYNC";break;
            case CR_SERVER_GONE_ERROR : error="CR_SERVER_GONE_ERROR";break;
            case CR_SERVER_LOST : error="CR_SERVER_LOST";break;
            case CR_UNKNOWN_ERROR : error="CR_UNKNOWN_ERROR";break;
            default : error="misc. Mysql error message!";
        }
        cerr <<"MYSQL Error: "<<error<<endl;
        mysqldb->connection()->servermessage();
        return false;
    }

}


bool hk_mysqlactionquery::driver_specific_sql(const char* )
{
    return true;
}
