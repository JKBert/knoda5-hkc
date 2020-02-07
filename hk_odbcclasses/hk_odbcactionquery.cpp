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
#include <hk_odbcactionquery.h>
#include <hk_odbcdatabase.h>
#include <hk_odbcconnection.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

//#include <errmsg.h>

hk_odbcactionquery::hk_odbcactionquery(hk_odbcdatabase* db) :hk_actionquery(db)
{
    odbcdb=db;
    SQLCHAR delimiter[50]="";
    SQLSMALLINT size;
    SQLGetInfo(odbcdb->connection()->connectionhandle(),SQL_IDENTIFIER_QUOTE_CHAR,delimiter,sizeof(delimiter),&size);
    p_identifierdelimiter=(char*) &delimiter;
}


bool hk_odbcactionquery::driver_specific_execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcactionquery::driver_specific_execute");
#endif
    if (odbcdb==NULL) return false;
    if (!odbcdb->connection()->connectionhandle()) return false;
    SQLHSTMT p_result;
    SQLRETURN ret= SQLAllocHandle(SQL_HANDLE_STMT,odbcdb->connection()->connectionhandle(),&p_result);
    ret= SQLExecDirect(p_result ,(SQLCHAR*)const_cast<char*>((p_sql)) ,p_length);
    bool result=true;
    if (ret!=SQL_SUCCESS)
    {
        const int buffersize=300;
        SQLCHAR* buffer=new SQLCHAR[buffersize];
        *buffer=0;
        SQLSMALLINT ressize;
        SQLGetDiagField(SQL_HANDLE_STMT,p_result,1,SQL_DIAG_MESSAGE_TEXT,buffer,buffersize,&ressize);
        odbcdb->connection()->set_last_servermessage((const char*)buffer);
        delete[] buffer;
        result=false;
    }
    SQLFreeHandle(SQL_HANDLE_STMT,p_result);
    return result;
}


bool hk_odbcactionquery::driver_specific_sql(const char* )
{
    return true;
}
