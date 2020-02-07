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
#ifndef HK_COLUMN
#define HK_COLUMN
#include "hk_class.h"
#include <list>
using namespace std;

class hk_datasource;
class hk_dsdatavisible;
/**
 *
 *@short represents one column of a datasource.
 *@version $Revision: 1.54 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *represents one column of a datasource. The data of the current row (set in @ref hk_datasource)
 *can be modified by using the functions @ref set_asstring, @ref set_asbool and @ref set_asbinary
 *and read by using the functions @ref asstring, @ref asbool and @ref asbinary.
 *
 *If the the datasource is in alter or create mode you can change the column definition by using the following
 *functions: @ref set_name, @ref set_columntype, @ref set_size, @ref set_notnull, @ref set_primary

 */

class hk_column:public hk_class

{
    friend class hk_datasource;
    friend class hk_dsdatavisible;
    public:
/**
 *name of the column
 */
        hk_string    name(void) const;
/**
 *name of the column. Can just be edited if the datasource is in the mode ALTER or CREATE.
 */
        void    set_name(const hk_string& n);

/**
 * @return true if data in this column can be changed.
 * Always false if the datasource is a result_query.

 */
        bool    is_readonly(void) const;
/**
 * If the datasource is of type ds_table you can allow or disallow data changes in this column.
 */
        void    set_readonly(bool r);

/**
 *used from hk_column to define of what type it is.
 *For details how the different column types are interpreted
 * look up the documentation of your database server and the database driver.
 *
 *textcolumn    : fixed length (usually in SQL defined as CHAR(size))
 *
 *auto_inccolumn:	if the databasedriver supports this columntype it will get a unique number automatically, whenever
 *		a new row will be added. Usually a readonly column
 *
 *smallintegercolumn: a integer range of at least -32768 to 32767
 *
 *integercolumn: 	a integer with the maximum range the database server can handle
 *
 *smallfloatingcolumn: a floating column with a range of at least ????
 *
 *floatingcolumn: a floating column with the maximum range the database server can handle
 *
 *datecolumn:    a column to keep the day,month and year of a date
 *
 *datetimecolumn: a column to keep the day,month and year of a date PLUS the hour,minute and second of a time
 *
 *timecolumn:    a column to keep the hour,minute and second of a time.
 *
 *binarycolumn:	a column to store binary data of more or less any size.
 *
 *memocolumn:	a column to store text data of more or less any size.
 *
 *boolcolumn:	a column that has 2 states 'true' or 'false'
 *
 *othercolumn:	a column hk_classes does not know, but tries to handle as good as it can.

 */
        enum enum_columntype {textcolumn,auto_inccolumn,smallintegercolumn,integercolumn,smallfloatingcolumn,floatingcolumn,datecolumn,datetimecolumn,timecolumn,timestampcolumn,binarycolumn,memocolumn,boolcolumn,othercolumn};

/**
 *
 *If you need the column type as a hk_string use @ref columntype_name to translate.
 *@return  the type of the column of type @ref enum_columntype.
 */
        enum_columntype     columntype(void) const;
/**
 * can just be edited if the datasource is in the mode ALTER or CREATE.
 *
 */
        void    set_columntype(enum_columntype f);
/**
 *column length in bytes
 */
        long    size(void);
/**
 * can just be edited if the datasource is in the mode ALTER or CREATE.
 */
        void    set_size(long s);
/**
 * to translate the columntype into a hk_string.
 */
        hk_string   columntype_name(void);
/**
 *@return true if this column is part of the primary key (primary index).
 */
        bool    is_primary(void);
/**
 * can only be edited if the datasource is in the mode ALTER or CREATE.
 */
        bool    set_primary(bool i);
/**
 *@return  true if this column has to have a value.
 */
        bool    is_notnull(void);

/**
 *@return  A string value representing the proper delimiter to use.
 */
        hk_string    get_delimiter(void);

/**
 * can just be edited if the datasource is in the mode ALTER or CREATE.
 */
        bool    set_notnull(bool i);
/**
 *@return true if the value of the column has changed and not yet saved in the database.
 */
        bool    has_changed(void);
/**
 *returns the value of this column, type converted to a hk_string. (All columntypes except BINARY).
 *@param as_locale if true numbers, times and dates will be displayed in the users localized version
 */
        hk_string   asstring(bool as_locale=true);
/**
 * @param s new value of the column.
 */
        void    set_asstring(const hk_string& s, bool registerchange=true);
        void    set_asstring(const hk_string& s,bool registerchange,bool is_locale);
/**
 * Columns can contain characters such as \\0, \\n, ' and so on.
 * Some SQL Servers need to encode this data, so this function delivers the encoded and
 * nonlocalized version.
 */
        const   char*   transformed_asstring(void);
        const   char*   transformed_asstring_at(unsigned long position);
/**
 *if new data was inserted with the method @ref asstring and the
 *changes have not yet been stored this method returns the changed data
 */
        hk_string   changed_data_asstring(bool as_locale=true);
/**
 *changes the value of the column in native form.(i.e. Binary data)
 *@see load_from_file
 */

        void    set_asbinary(const struct_raw_data* rd);
/**
 *returns the value of the column as raw data.
 *@see save_to_file.
 */
        const   struct_raw_data* asbinary(void);
/**
 *returns the value of the column at a specific rowposition as raw data.
 */
        const   struct_raw_data* asbinary_at(unsigned long a);

/**
 * @param b new value of the column.
 */
        void    set_asbool(bool b);
/**
 *returns the value of this column, type converted to a bool. (All columntypes except Binary).
 */
        bool    asbool(void);
        bool    asbool_at(unsigned long a);
        bool    changed_data_asbool(void);


/**
 * @param s new value of the column.
 */
	void set_asinteger(long int s, bool registerchange=true);
	long int asinteger(void);
	long int asinteger_at(unsigned long );
	long int changed_data_asinteger(void);

/**
 * @param s new value of the column.
 */
	void set_asdouble(double s, bool registerchange=true);
	double asdouble(void);
	double asdouble_at(unsigned long );
	double changed_data_asdouble(void);

/**
 * A temporarily given unique number of this row
 */
        unsigned int    fieldnumber(void);

/**
 *returns the value at a specific position in the column. The row selector will not be changed,
 *so the visible objects will not be informed. Use @ref asstring in combination with
 *@ref hk_datasource::goto_row instead.
 */
        hk_string   asstring_at(unsigned long a,bool as_locale=true);

        bool is_nullvalue(void);
	bool changed_data_is_nullvalue(void);
        bool is_nullvalue_at(unsigned long a);
        void set_asnullvalue(bool registerchange=true);

	long int count(unsigned int from,unsigned int to);
	double sum(unsigned int from,unsigned int to);
	long int count();
	double sum();

/**
 *search for a specific value in a column
 *@param from number of the first row where the search has to start. First row is number 0
 *@param to number of the last row where the search has to end. Last row is number max_rows-1;
 *@param searchtext value as hk_string to look for
 *@param wholephrase if false the searchtext is part of columnvalue, if true it has to be an exact match
 *@param casesensitive if true a case sensitive compararation is used
 *@param backwards  searching backwards, default is forward
 *@return row number if found, max_rows()+1 if not found
 */
        unsigned int find(unsigned int from,unsigned int to,const hk_string& searchtext,bool wholephrase=false,bool casesensitive=false,bool backwards=false);
/**
 * search from the beginning to the end for searchtext
@param searchtext value as hk_string to look for
*@return row number if found, max_rows()+1 if not found
*/
        unsigned int find(const hk_string& searchtext,bool wholephrase=false,bool casesensitive=false,bool backwards=false);
/**
 *an internal used function which compares the column value at row number "pos"
 */
        bool is_findstring(unsigned int pos,const hk_string& searchtext,bool wholephrase=false,bool casesensitive=false);

/**
 *returns the new data set by a function like asstring(hk_string& n);
 */
        const   struct_raw_data*    changed_data(void);

/**
 *returns the new data set by a function like asstring(hk_string& n) in SQL server native form.
 */

        const   struct_raw_data*    transformed_changed_data(void);
/**
 *set all changed data back to it's default value. Usually you don´t
 *have to call this function
 */
        void    reset_changed_data(void);

/**
 * you have the possibility to set the values which the column will accept as
 * true and false. i.e. 0 and 1 for numeric columns or for text columns yes and no,
 * or true and false.
 */
        void set_boolvalues(const hk_string& TRUESTRING, const hk_string& FALSESTRING);
        hk_string bool_truevalue(void);
        hk_string bool_falsevalue(void);
/**
 * if the column is of type date, time or datetime you can specify the wished
 * format with this function. See @ref hk_datetime for details.
 */
        void set_dateformat(const hk_string& df){p_dateformat=df;}
        hk_string dateformat(void) const {return p_dateformat;}
/**
 * if the column is of type date, time or datetime you can specify the wished
 * format with this function. See @ref hk_datetime for details.
 */
        void set_timeformat(const hk_string& tf){p_timeformat=tf;}
        hk_string timeformat(void) const{return p_timeformat;}
/**
 * if the column is of type date, time or datetime you can specify the wished
 * format with this function. See @ref hk_datetime for details.
 */
        void set_datetimeformat(const hk_string& dtf){p_datetimeformat=dtf;}
        hk_string datetimeformat(void) const{return p_datetimeformat;}
/**
 * save the value of this column in a file. Very useful for binary-columns
 */
        bool save_to_file(const hk_string& filename);
/**
 *changes the value of the column with the content of the file"filename". Very useful
 *for binary columns.
 */
        bool load_from_file(const hk_string& filename);

        void save_columndefinition(ostream&);
        void load_columndefinition(xmlNodePtr );
        hk_datasource* datasource(void);
/**
 *a column of type autoinc is usually readonly. if allow_autoincwrite is set to true
 * it will be writeable if the database driver supports it
 */
        void    set_allow_autoincwrite(bool r);
        bool    allow_autoincwrite(void);
/**
 *data type, name and some other metadata can only be changed if the datasource is either in create_mode or
 *alter_mode (or while the datasource enables and automatically creates columns)
 *@return true if metadata can be modified, else false
 */
        bool in_definitionmode(void);
/**
*returns the current value as a string. The difference to asstring is, that it will return the changed_data if the current data was
*changed by setting the data with e.g. set_asstring()
*/
hk_string curval_asstring(void);
/**
*returns the current value as a floating point number . The difference to asdouble is, that it will return the changed_data if the current data was
*changed by setting the data with e.g. set_asdouble()
*/
double curval_asdouble(void);
/**
*returns the current value as a integer. The difference to asinteger is, that it will return the changed_data if the current data was
*changed by setting the data with e.g. set_asinteger()
*/
long int curval_asinteger(void);
/**
*returns the current value as a boolean value. The difference to asbool is, that it will return the changed_data if the current data was
*changed by setting the data with e.g. set_asbool()
*/
bool      curval_asbool(void);

void set_definitionmode(bool def);

hk_string tableorigin(void);

    protected:
        hk_column(hk_datasource* ds, const hk_string& TRUESTRING,const hk_string& FALSESTRING);
        virtual ~hk_column();
        virtual bool        driver_specific_asstring(const hk_string& d);
        virtual hk_string driver_specific_asstring_at(unsigned long a);
        virtual const char* driver_specific_transformed_asstring_at(unsigned long ){return "";}
        virtual const struct_raw_data* driver_specific_asbinary_at(unsigned long a);
        virtual bool        driver_specific_asbinary(const char* b);
        virtual void        driver_specific_asbool(bool b);
        virtual bool        driver_specific_asbool_at(unsigned long a);
        virtual bool        driver_specific_is_nullvalue_at(unsigned long);
        virtual void data_has_changed(void);
        void datavisible_add(hk_dsdatavisible*);
        void datavisible_remove(hk_dsdatavisible*);
        hk_datasource*  p_datasource;
        unsigned int    p_fieldnr;
        bool        p_readonly;
	bool        p_driver_specific_delimit_bool;
        unsigned long   p_driver_specific_data_size;
        char*       p_driver_specific_data;
        char*       p_original_new_data;          // where datachanges are stored temporarily
        unsigned long   p_original_new_data_size;
    	hk_string   p_sqltextdelimiter;
    	hk_string   p_sqlnulldelimiter;
        hk_string   p_new_data_asstring;
        hk_string   p_true;
        hk_string   p_false;
        hk_string   p_dateformat;
        hk_string   p_timeformat;
        hk_string   p_datetimeformat;
        hk_string   p_driverspecific_dateformat;
        hk_string   p_driverspecific_timeformat;
        hk_string   p_driverspecific_datetimeformat;
        hk_string   p_driverspecific_timestampformat;
        hk_string   p_driverspecific_locale;
        int         p_driverspecific_digits;
    private:
       void before_alter_table();
        hk_string       p_columnname,p_originalcolumnname, p_tableorigin;
        enum_columntype p_columntype,p_originalcolumntype;
        long        p_size,p_originalsize;
        bool        p_primary_index,p_originalprimary_index;
        bool        p_notnull,p_originalnotnull;
        bool        p_has_changed;
        bool        p_allow_autoincwrite;
	bool        p_table_origin_already_set;
        struct_raw_data p_binary;
        struct_raw_data p_changed_data_struct;
        struct_raw_data p_transformed_changed_data_struct;
        list<hk_dsdatavisible*> p_datavisibles;
        void set_has_changed();
        void set_has_not_changed();
        bool p_definitionmode;

} ;
#endif
