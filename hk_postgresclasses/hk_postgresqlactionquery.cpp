// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include <hk_postgresqlactionquery.h>
#include <hk_postgresqldatabase.h>
#include <hk_postgresqlconnection.h>
hk_postgresqlactionquery::hk_postgresqlactionquery(hk_postgresqldatabase* db):hk_actionquery(db)
{
    postgresqldb=db;
    p_identifierdelimiter="\"";
}


bool hk_postgresqlactionquery::driver_specific_execute(void)
{
    PGresult *result;
    if (postgresqldb==NULL) return false;
    if (!postgresqldb->connection()->dbhandler())return false;
    result = PQexec(postgresqldb->connection()->dbhandler() , p_sql);
    if (PQresultStatus(result) == PGRES_COMMAND_OK || PQresultStatus(result) == PGRES_TUPLES_OK)
    {
        PQclear(result);
        return true;
    }
    else
    {
        postgresqldb->connection()->servermessage();
        PQclear(result);
        return false;
    }

}


bool hk_postgresqlactionquery::driver_specific_sql(const char* )
{
    return true;
}
