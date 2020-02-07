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
#ifndef HK_REPORTUTILS
#define HK_REPORTUTILS
#include "hk_report.h"
#include "hk_reportsection.h"
#include "hk_reportdata.h"
using namespace std;
/**
 *
 *@short  A predefined report that prints your data in a html table
 *@version $Revision: 1.22 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *A report that prints all data in html format. Special characters like Umlaute, accents etc. will be
 *automatically recoded.
 *
 *You can see what functions influence which parts of the html document in the following listing:
 *<pre>
 *set_beforehtmltag()
 *<HTML>
 *   <HEAD>
 *     set_betweenheadtags()
 *     set_title()
 *     set_author()
 *   </HEAD>
 *
 *   <BODY  set_bodytag()>
 *      set_beforetable()
 *	<TABLE  set_tabletag()>
 *	   <TR  set_trtags()><TH  set_thtags()>...</TH>  ...  </TR>
 *	   <TR  set_trtags()><TD  set_tdtags()>...</TD>  ...  </TR>
 *	     .
 *	     .
 *	     .
 *	</TABLE>
 *      set_aftertable()
 *   </BODY>
 *</HTML>
 *</pre>
 * See the simple usage in the following example:
<pre>
* // first create a datasource
*  hk_classes p_class;
*  hk_connection* connect=p_class.hk_create("mysql");
*  connect->user("root");
*  connect->host("localhost");
*  connect->password("my_password");
*  connect->connect();
*  hk_database* database=connect->new_database();
*  database->name("vokabeltrainer");
*  hk_datasource* datasource=database->new_resultquery();
 *  datasource->sql("SELECT * FROM vokabeln");
 *
 * // now create the output as a html document
 *
 *  hk_reporthtml e;
 *  e.set_filename("webdocument.html");
 *  e.datasource(datasource);
 *  e.set_author("Horst Knorr");
 *  e.set_title("�erschrift");
 *  e.execute();
</pre>
*4 sections will be defined, of which the datasection (the <TR><TD>..row) and the tableheadersection
*(the <TR><TH>.. row) can be reached directly.
*
*  *      beforetablesection;
*
*  *      tableheadersection;
*
*  *      datasection;
*
*  *      tableendsection;
 */
class hk_reporthtml:public hk_report
{

    public:
        hk_reporthtml(void);
        virtual ~hk_reporthtml(void);
/**
 *You can define some parameter for the <TABLE> tag. See yout html documentation for details
 */
        void    set_tabletag(const hk_string& t="");
/**
 * A hk_string that will be displayed in the body of the html document before the table with the data.
 *Internally it is represented by a @ref hk_reportdata , so you can use all variables.
 *@param value the hk_string to be displayed
 *@param column The columnname of a column which will be base for the variables %SUM%,%COUNT%,%AVERAGE% etc.
 */
        void    set_beforetable(const hk_string& value="%VALUE%",const hk_string& columnname="");
/**
 * A hk_string that will be displayed in the body of the html document after the table with the data.
 *@see set_beforetable
 */
        void    set_aftertable(const hk_string& value="%VALUE%",const hk_string& columnname="");
/**
 *You can define some parameter for the <TR> tag. See your html documentation for details
 */
        void    set_trtag(const hk_string& t);
/**
 *You can define some parameter for the <TH> tag. See your html documentation for details
 */
        void    set_thtag(const hk_string& t);
/**
 *You can define some parameter for the <TD> tag. See your html documentation for details
 */
        void    set_tdtag(const hk_string& t);
/**
 *You can define what should stand before the  <HTML> tag
 *(i.e.<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">.
 *See your html documentation for details
 */

        void    set_beforehtmltag(const hk_string& b);
/**
 *Data which has to be in the <HEAD>...</HEAD> section before the body.
 *i.e.<meta http-equiv=\"Content-Type\" content=\"text/html;charset=iso-8859-1\">
 *See your html documentation for details
 */
        void    set_betweenheadtags(const hk_string& b,const hk_string& colname="");
/**
 *If you want to create more tables depending on the value of a field
 *set the field name with set_groupcolumn
 */
        void    set_groupcolumn(const hk_string& c);
/**
 *If you want each grouped table (@ref set_groupcolumn) in its own file set this to true
 */
        void    set_multiplefiles(bool m=true);
/**
 *The document title. Will be set in the <HEAD>..</HEAD>section
 */
        void    set_title(const hk_string& t);
/**
 *The document author. Will be set in the <HEAD>..</HEAD>section
 */
        void    set_author(const hk_string& a);

    protected:

        void    configure_page(void);
        void    configure_table(void);
    private:
        bool p_withheader;
        hk_string p_pagehead;
        hk_string p_bodytag;
        hk_string p_tabletag;
        hk_string p_trtag;
        hk_string p_thtag;
        hk_string p_tdtag;
        hk_string p_header;
        hk_string p_groupcolumn;
        hk_string p_columnbefore;
        hk_string p_columnafter;
        hk_string p_textafter;
        hk_string p_beforevalue;
        hk_string p_aftervalue;
        hk_string p_head;
        hk_string p_title;
        hk_string p_author;
        hk_reportsection* p_beforetablesection;
        hk_reportsection* p_tableheadersection;
        hk_reportsection* p_tableendsection;
        hk_reportsection* p_datasection;
        hk_reportdata*    p_pageheaddata;
        hk_reportdata*    p_beforetabledata;
        hk_reportdata*    p_aftertabledata;

};

hk_string  recode_html(const hk_string& where,hk_report*);
hk_string  recode_postscript(const hk_string& where,hk_report*);
hk_string  fullpagereplace_postscript(const hk_string& where,hk_report*);
hk_string  recode_utf8(const hk_string& where,hk_report*);

unsigned long recount_postscript(hk_reportsection* s);
void   configure_postscriptdata(hk_reportdata* d);
bool   configure_postscriptreport(hk_report*,int);
bool set_reporttype_postscript(hk_report*,bool);
hk_string replacepsdatafunction(hk_reportdata* rd,const hk_string& value);

/**
 *CVS is a standard exportformat. Usually it is a comma separated list with on datarow per line.
 *The data is usually delimited by "". The first line may contain the columnnames.
 *Normally you just have to set the datasource and then execute the report.
 *
 *Example
 *<pre>
 * // first create a datasource
 *  hk_classes p_class;
 *  hk_connection* connect=p_class.hk_create("mysql");
 *  connect->user("root");
 *  connect->host("localhost");
 *  connect->password("my_password");
 *  connect->connect();
 *  hk_database* database=connect->new_database();
 *  database->name("vokabeltrainer");
 *  hk_datasource* datasource=database->new_resultquery();
 *  datasource->sql("SELECT * FROM vokabeln");
 *
 * // now create the output as a CVS document
 *
 *  hk_reportcsv e;
 *  e.datasource(datasource);
 *  e.execute();
 *</pre>
 *@short  A predefined report that exports your data as cvs
 *@version 0.1
 *@author Horst Knorr (hk_classes@knoda.org)
 */
class hk_reportcsv:public hk_report
{
    public:

        hk_reportcsv(void);
virtual ~hk_reportcsv(void);
        void    set_groupcolumn(const hk_string& c);
        void    set_multiplefiles(bool m=true);
/**
 *Here you can easily define other delimiters and data separators.To get a tabulatorseparated list
just set set_dataformat("\"","\t");
*/
        void    set_filedefinition(const hk_string& textdelimiter="\"",const hk_string& betweenfields=",", const hk_string& rowdelimiter="\n",const hk_string& datetimeformat="",const hk_string& dateformat="",const hk_string& timeformat="", const hk_string& charset="",const hk_string& locale="");
/**
 *set it to false if you don't want the columnnames in the first line (default ist true)
 */
        void    set_withheader(bool with);
        hk_string textdelimiter(void){return p_delimiter;}
        hk_string between(void) {return p_between;}
    protected:
        hk_reportsection* p_datasection;
        hk_reportsection* p_footsection;
        void configure_page(void);
        virtual void    widget_specific_before_execute(void);
        virtual void    widget_specific_after_execute(void);

    private:
        hk_string p_groupcolumn;
        hk_string p_between;
        hk_string p_delimiter;
        bool p_withheader;


};
hk_string encodefilecharsetfunction(hk_reportdata*,const hk_string&);
void automatic_create_csvfields(hk_reportsection*);

unsigned long zmm2ps(unsigned long);
unsigned long ps2zmm(unsigned long);

class hk_reportxml: public hk_report
{
    public:
        hk_reportxml(void);
virtual ~hk_reportxml(void);
        bool set_maindocumenttag(const hk_string& t);
        hk_string maindocumenttag(void);
        bool set_rowtag(const hk_string& t);
        hk_string rowtag(void);
        void set_includetableschema(bool);
        void set_excelxml(bool);
        bool excelxml(void);
        bool includetableschema(void);
        enum enum_fieldtag {fieldname,fieldnameattribute};
        void set_fieldname_as_attribute(enum_fieldtag tag);
        enum_fieldtag fieldname_as_attribute(void);
    protected:
        void configure_page(void);
        void configure_excelpage(void);
        virtual void    widget_specific_before_execute(void);
        virtual void    widget_specific_after_execute(void);
        virtual void    widget_specific_before_last_outstream_closed(void);

    private:
        hk_string p_maindocumenttag;
        hk_string p_rowtag;
        bool p_include_tableschema;
        bool p_excelxml;
        enum_fieldtag p_fieldtag;

};

hk_string  fullpagereplace_excelxml(const hk_string& where,hk_report* report);
hk_string replaceexcelxmldatafunction(hk_reportdata* rd,const hk_string& value);
void   configure_excelxmldata(hk_reportdata* d);

#endif
