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
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

#include "hk_odbcdatabase.h"
#include "hk_odbcconnection.h"
#include "hk_odbctable.h"
#include "hk_odbcresultquery.h"
#include "hk_odbcactionquery.h"
#include "hk_odbcview.h"


class hk_odbcdatabaseprivate
{
public:
hk_string coltype[14][2];// contains the driver specific create statements for the different column types;
					//first value contains the name, the second the parameters
 bool values_set;
 hk_odbcdatabaseprivate(){values_set=false;}
};



hk_odbcdatabase::hk_odbcdatabase(hk_odbcconnection* c):hk_database(c)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcdatabase::hk_odbcdatabase");
#endif
    p_odbcconnection=c;
    p_private=new hk_odbcdatabaseprivate();


}


hk_odbcdatabase::~hk_odbcdatabase(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcdatabase::~hk_odbcdatabase");
#endif
delete p_private;
}


hk_odbcconnection* hk_odbcdatabase::connection(void)
{
    return p_odbcconnection;
}


void hk_odbcdatabase::driver_specific_tablelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcdatabase::driver_specific_tablelist");
#endif
    p_tablelist.erase(p_tablelist.begin(),p_tablelist.end());

    if (!p_connection->is_connected()) return;
    SQLHSTMT        hstmt;
    SQLRETURN   nReturn;
    SQLCHAR         szTableName[101]  = "";
    SQLCHAR         szTableType[1]    = "";
    SQLCHAR         szTableRemarks[1] = "";
    SQLLEN      nIndicatorName;
    nReturn = SQLAllocHandle(SQL_HANDLE_STMT, p_odbcconnection->connectionhandle(), &hstmt );
    if ( nReturn != SQL_SUCCESS )
    {
        show_warningmessage( "Failed to SQLAllocHandle STMT" );
        return ;
    }

    if ( SQL_SUCCESS != SQLTables( hstmt, 0, 0, 0, 0, 0, 0, 0, 0 ) )
    {
        show_warningmessage(  "Failed to SQLTables" );
        return ;
    }

    SQLBindCol( hstmt, 3/*tablename*/, SQL_C_CHAR, szTableName, sizeof(szTableName), &nIndicatorName );
// GET RESULTS
    nReturn = SQLFetch( hstmt );
    while ( nReturn == SQL_SUCCESS || nReturn == SQL_SUCCESS_WITH_INFO )
    {
        p_tablelist.insert(p_tablelist.end(),(char *)szTableName);

        szTableName[0]      = '\0';
        szTableType[0]      = '\0';
        szTableRemarks[0]   = '\0';
        nReturn = SQLFetch( hstmt );
    }

// FREE STATEMENT
    nReturn = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    if ( nReturn != SQL_SUCCESS )
        show_warningmessage(  "Failed to SQLFree Stmt" );

    sort(p_tablelist.begin(),p_tablelist.end());
}


void hk_odbcdatabase::driver_specific_viewlist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcdatabase::driver_specific_viewlist");
#endif
    p_viewlist.erase(p_viewlist.begin(),p_viewlist.end());
    if (!p_connection->is_connected()) return;

    SQLHSTMT        hstmt;
    SQLRETURN   nReturn;
    SQLCHAR         szTableName[101]  = "";
    SQLCHAR         szTableType[1]    = "";
    SQLCHAR         szTableRemarks[1] = "";
    SQLLEN      nIndicatorName;

    nReturn = SQLAllocHandle(SQL_HANDLE_STMT, p_odbcconnection->connectionhandle(), &hstmt );
    if ( nReturn != SQL_SUCCESS )
    {
        show_warningmessage( "Failed to SQLAllocHandle STMT" );
        return ;
    }

    if ( SQL_SUCCESS != SQLTables( hstmt, 0, 0, 0, 0, 0, 0, (SQLCHAR*)const_cast<char*>("VIEW"), 4 ) )
    {
        show_warningmessage(  "Failed to SQLTables" );
        return ;
    }

    SQLBindCol( hstmt, 3/*tablename*/, SQL_C_CHAR, szTableName, sizeof(szTableName), &nIndicatorName );
// GET RESULTS
    nReturn = SQLFetch( hstmt );
    while ( nReturn == SQL_SUCCESS || nReturn == SQL_SUCCESS_WITH_INFO )
    {
        p_viewlist.insert(p_viewlist.end(),(char *)szTableName);

        szTableName[0]      = '\0';
        szTableType[0]      = '\0';
        szTableRemarks[0]   = '\0';
        nReturn = SQLFetch( hstmt );
    }

// FREE STATEMENT
    nReturn = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    if ( nReturn != SQL_SUCCESS )
        show_warningmessage(  "Failed to SQLFree Stmt" );

    sort(p_viewlist.begin(),p_viewlist.end());
}


hk_datasource* hk_odbcdatabase::driver_specific_new_table(hk_presentation* p)
{
    return new hk_odbctable(this,p);
}


hk_datasource* hk_odbcdatabase::driver_specific_new_resultquery(hk_presentation* p)
{
    return new hk_odbcresultquery(this,p);
}


hk_actionquery* hk_odbcdatabase::driver_specific_new_actionquery(void)
{
    return new hk_odbcactionquery(this);
}


bool hk_odbcdatabase::driver_specific_select_db(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_odbcdatabase::driver_specific_select_db");
#endif
    return true;
    if (p_odbcconnection!=NULL)
    {
        if (p_odbcconnection->is_connected())p_odbcconnection->disconnect();
        if (p_odbcconnection->connect())return true;
    }
    return false;
}


void hk_odbcdatabase::driver_specific_set_name(void)
{
    if (p_odbcconnection->is_connected())p_odbcconnection->disconnect();
    if (p_odbcconnection->connect())return ;

}




hk_string hk_odbcdatabase::columntypeparameter(hk_column::enum_columntype p)
{
 if (!p_private->values_set) set_colvalues();

   return p_private->coltype[p][0]+p_private->coltype[p][1];
}


void hk_odbcdatabase::set_colvalues(void)
{
      hk_datasource*r =driver_specific_new_resultquery(NULL);
      if (!r)
      {
 	cerr <<"keine set_colvalues datasource!"<<endl;
	return;
      }
      r->set_sql("ODBCFIELDS",true);
      r->enable();
      hk_column* namecolumn=r->column_by_name("TYPE_NAME");
      hk_column* datatype=r->column_by_name("DATA_TYPE");
      if (!datatype) datatype=r->column_by_name("SQL_DATA_TYPE");// that's for the buggy SAPDB
      hk_column* parameters=r->column_by_name("CREATE_PARAMS");
//	cerr <<"name "<<name<<" datatype "<<datatype<<" parameters "<<parameters<<endl;
     if (namecolumn&&datatype&&parameters)
     {
        unsigned int pos=datatype->find(longint2string(SQL_CHAR),true);
	if (pos<r->max_rows())//textcolumn
	  {     const int n=hk_column::textcolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }

	pos=datatype->find(longint2string(SQL_SMALLINT),true);//smallintegercolumn
	if (pos<r->max_rows())
	  {     const int n=hk_column::smallintegercolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }

	pos=datatype->find(longint2string(SQL_INTEGER),true);//smallintegercolumn
	if (pos<r->max_rows())
	  {     const int n=hk_column::integercolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }

	pos=datatype->find(longint2string(SQL_FLOAT),true);
	if (pos<r->max_rows())
	  {     const int n=hk_column::smallfloatingcolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }

	pos=datatype->find(longint2string(SQL_DOUBLE),true);
	if (pos<r->max_rows())
	  {     const int n=hk_column::floatingcolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }

	pos=datatype->find(longint2string(SQL_TYPE_DATE),true);
	if (pos>=r->max_rows())pos=datatype->find(longint2string(SQL_DATE),true);
	if (pos<r->max_rows())
	  {     const int n=hk_column::datecolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }


	pos=datatype->find(longint2string(SQL_TYPE_TIME),true);
	if (pos>=r->max_rows())pos=datatype->find(longint2string(SQL_TIME),true);
	if (pos<r->max_rows())
	  {     const int n=hk_column::timecolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }

	pos=datatype->find(longint2string(SQL_TYPE_TIMESTAMP),true);
	if (pos>=r->max_rows())pos=datatype->find(longint2string(SQL_TIMESTAMP),true);
	if (pos<r->max_rows())
	  {     const int n=hk_column::timestampcolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }


	pos=datatype->find(longint2string(SQL_LONGVARBINARY),true);
	if (pos<r->max_rows())
	  {     const int n=hk_column::binarycolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }

	pos=datatype->find(longint2string(SQL_LONGVARCHAR),true);
	if (pos<r->max_rows())
	  {     const int n=hk_column::memocolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }

	pos=datatype->find(longint2string(SQL_BIT),true);
	if (pos<r->max_rows())
	  {     const int n=hk_column::boolcolumn;
	  	r->goto_row(pos);
		p_private->coltype[n][0]=namecolumn->asstring();
		p_private->coltype[n][1]=parameters->asstring();
	  }


     }
     if (p_odbcconnection->backendtype()==hk_odbcconnection::sapdb||p_odbcconnection->backendtype()==hk_odbcconnection::postgresql)
     {
		p_private->coltype[hk_column::auto_inccolumn][0]="SERIAL";
     }
     else
     if (p_odbcconnection->backendtype()==hk_odbcconnection::mysql)
     {
		p_private->coltype[hk_column::auto_inccolumn][0]="BIGINT(1) NOT NULL AUTO_INCREMENT";
     }

/*    for (int i=0;i<14;++i)
    {
	cerr <<"i: "<<i<<" '"<<p_private->coltype[i][0]<<"' param: '"<<p_private->coltype[i][1]<<"'"<<endl;
    }
*/
     parse_parameters();
	r->disable();
      delete r;
p_private->values_set=true;

}

void hk_odbcdatabase::parse_parameters(void)
{
    for (int pos=0;pos<14;++pos)
    {
	hk_string value=string2upper(p_private->coltype[pos][1]);
	p_private->coltype[pos][1]="";
	const char * delimiters = " \t\n";
	  const string::size_type len = value.length();
          string::size_type i = 0;
    bool end=false;
    while ( i < len &&!end )
    {
	hk_string token;
	// eat leading whitespace
        i = value.find_first_not_of (delimiters, i);
        if (i == string::npos)
            return;   // nothing left but white space
        // find the end of the token
        string::size_type j =value.find_first_of (delimiters, i);
        // push token
        if (j == string::npos)
	{
            token=value.substr(i);
	    end=true;
	}
	 else
            token=value.substr(i, j-i);
        // set up for next loop
        i = j + 1;
	bool valid_token=false;
	hk_string newtoken;
     if (token=="PRECISION")
      {
       newtoken="%PRECISION%";
       valid_token=true;
      }
     else
     if (token=="SCALE")
      {
       newtoken="%SCALE%";
       valid_token=true;
      }
     else
     if (token.find("LENGTH")!=hk_string::npos)
      {
       newtoken="%LENGTH%";
       valid_token=true;
      }

     if (valid_token)
     {
     if (p_private->coltype[pos][1].size()==0)
		p_private->coltype[pos][1]="(";
       else
          	p_private->coltype[pos][1]+=",";
     p_private->coltype[pos][1]+=newtoken;
     }

    }//end while

   if (p_private->coltype[pos][1].size()>0) p_private->coltype[pos][1]+=")";


    }

/*cerr <<"nach params"<<endl;
    for (int i=0;i<14;++i)
    {
	cerr <<"i: "<<i<<" '"<<p_private->coltype[i][0]<<"' param: '"<<p_private->coltype[i][1]<<"'"<<endl;
    }
*/

}


hk_datasource* hk_odbcdatabase::driver_specific_new_view(hk_presentation* p)
{
    hk_odbcview *x;
    x = new hk_odbcview(this , p);
    return x ;

}


