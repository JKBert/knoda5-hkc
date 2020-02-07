// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_ACTIONQUERY
#define HK_ACTIONQUERY
#include <hk_data.h>
using namespace std;
class hk_database;
class hk_actionqueryprivate;
/**
 *
 *@short Action query (i.e. ALTER TABLE) with no result
 *@version $Revision: 1.16 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *In contrary to Result Queries (queries with a SELECT - SQL-statement) action queries manipulate data
 *like ALTER TABLE or INSERT INTO.
 *There is no resulting data.

 */

class hk_actionquery: public hk_data
{
    friend class hk_database;
    friend class hk_datasource;
    public:
        virtual ~hk_actionquery(void);
        enum_datasourcetypes type(void);
/**
 *SQL-serverdependend SQL statement
 * @param s SQL statement.
 * @param l length of the field s. Maybe that there is \0 somewhere in
 * the SQL statement (i.e. BLOB field), so this is necessary.
 */
        void    set_sql(const char*s, unsigned long l);
	void set_sql(const hk_string& s,bool convertdelimiter=false);
	const char* sql(void);
        unsigned long length(void);
/**
 *tries to execute the sql statement
 *@return true if it was successful.
 */
        bool    execute(void);

    protected:
        hk_actionquery(hk_database* db);
        virtual bool    driver_specific_execute(void){return false;}
        virtual bool    driver_specific_sql(const char*s);
        virtual void   before_source_vanishes(void);
        const char*     p_sql;
        unsigned long p_length;
	void print_sql(void);
	hk_string sqlconvertdelimiter(const hk_string& );
    private:
        hk_database* p_database;
	hk_actionqueryprivate* p_private;

};
#endif
