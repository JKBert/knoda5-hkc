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
#ifndef HK_IMPORTCSV
#define HK_IMPORTCSV
#include <iostream>
#include <fstream>
#include "hk_class.h"
#include "hk_datasource.h"
#include "hk_dsvisible.h"
#include "hk_column.h"
#include <vector>

using namespace std;

class hk_column;

typedef class colstruct
{
    public:
        colstruct(void){col=NULL;}
        hk_string colname;
        hk_column* col;
        int currentvalue;
} colstruct;
/**
 *
 *@short imports comma delimited textfiles
 *@version $Revision: 1.27 $
 *@author Horst Knorr (hk_classes@knoda.org)

 */

class hk_importcsv:public hk_dsvisible
{
    public:
        hk_importcsv(void);
        virtual ~hk_importcsv(void);


/**
 *after setting all infos start the import with execut
 *@return true if successful else false
 */
        bool    execute(enum_interaction i=interactive);
/**
 *defines the fielddelimiter and the columndelimiter of the csv-file.
 */
        void    set_filedefinition(const hk_string& textdelimiter="\"",const hk_string& betweenfields=",", const hk_string& rowdelimiter="\n",const hk_string& datetimeformat="",const hk_string& dateformat="",const hk_string& timeformat="", const hk_string& charset="",const hk_string& locale="");
/**
 *Name of the CSV-file
 */
        void    set_filename(const hk_string& filename);
/**
 *If the first row in the CSV file contains set this function to true else false
 *Default is true
 */
        void    set_firstrow_contains_fieldnames(bool f=true);
/**
 * If user wants to append data, instead of creating a new table or overwriting
 * an existing table.
 * Default is false.
 */
        void    set_append_rows(bool f=false);
/**
 *If set to true a new table will be created else it will be appended to an existing table
 */
        void    set_create_new_table(bool n=true);
/**
 * Define the equivalent column names in an existing table. Column 1 of the CSV File will be added to
 *the first column of the table.
 */
        void    add_columnname(const hk_string& col);
        void    clear_columnlist(void);
        void set_detect_autoinc(bool);
        bool detect_autoinc(void) const;
        hk_string textdelimiter(void){return p_textdelimiter;}
        hk_string betweenfield(void) {return p_betweenfields;}
        hk_string rowdelimiter(void) {return p_rowdelimiter;}
        hk_string datetimeformat(void) {return p_datetimeformat;}
        hk_string dateformat(void) {return p_dateformat;}
        hk_string timeformat(void) {return p_timeformat;}
        hk_string filelocale(void) {return p_locale;}
        hk_string filecharset(void){return p_charset;}
        void set_overwrite_table(bool);
        bool overwrite_table(void);
        vector<hk_string>* create_valuelist( hk_string& row);
	void    get_line(ifstream&,hk_string&);

    protected:
        void set_progressdialog(progress_dialogtype* progressdialog);
        progress_dialogtype* progressdialog(void)const;
        void    set_columns(void);
        void    create_automatic_columns(void);
        bool    initialize_table(enum_interaction);
        virtual bool before_columns_deleted(void);
/**
 * If this function returns false the execution of the import will be cancelled
 */

        virtual bool widget_specific_after_new_row(void);

    private:
        void    reset(void);
        bool        p_create_new_table;
/*
 * 0= hk_string
 * 1= float
 * 2= Integer
 * 3= boolean
 * 4=datetime
 * 5=date
 * 6=time
 */
        hk_column::enum_columntype         interpret_columntype(hk_string&f);
        ifstream*   p_filestream;
        hk_string   p_filename;
        hk_string   p_textdelimiter;
        hk_string   p_rowdelimiter;
        hk_string   p_betweenfields;
        hk_string   p_charset;
        hk_string   p_locale;
        bool        p_firstrow_contains_fieldnames;
        vector<hk_string>  p_valuelist;
        vector<colstruct> p_columnlist;
        bool        p_datamode;
        bool        p_cancelimport;
        bool        p_overwrite;
        bool	    p_detect_autoinc;
        hk_datasource::enum_accessmode p_accessmode;
        hk_string p_datetimeformat,p_dateformat,p_timeformat;
        progress_dialogtype* p_progressdialog;
};
#endif
