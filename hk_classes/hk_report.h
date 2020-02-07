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
#ifndef HK_REPORT
#define HK_REPORT
#include "hk_definitions.h"
#include "hk_presentation.h"
#include <list>
#include <vector>
//#include <iostream>
//#include <fstream>
#include <time.h>
#include <map>
using namespace std;

class hk_reportsection;
class hk_reportsectionpair;
class hk_reportdata;
class hk_report;
class hk_column;

class hk_reportprivate;

/**
 * With hk_report you can create reports to print all your database data in all userdefined, textbased formats, such as
 * CVS, HTML, XML, Postscript etc. It has all functions you need to create reports in any kind and any format.
 * See also the convenience class @ref hk_reportcvs.
 * @short basic class to create reports
 *@version $Revision: 1.66 $
 * @author Horst Knorr (hk_classes@knoda.org)

 *

 * you can use the variables
 *
<pre>
%PTIME%              the time in seconds used to create the report output.
%PAGENUMBER%         the number of the page
%ABSOLUTEPAGENUMBER% physical number of the page in file
%PSNEEDEDFONTS%      adds the Postscript-style %%DocumentNeededFonts section
</pre>

*/

class hk_report :public hk_presentation
{
    friend class hk_reportsectionpair;
    friend class hk_reportsection;
    public:
        hk_report(void);
        virtual       ~hk_report(void);
/**
 * Sets the filename of the report. If you don't give the report a name the report and @ref use_standard_storagepath is false
 * will be printed at
 * the standard output.
 *@see set_fileextension
 *@see set_pagenamecolumn
 */
        void          set_filename(const hk_string&,bool registerchange=true);
        hk_string     filename(void);
/**
 *if true the report output will be written in a file in the .hk_classes directory
 *(~/.hk_classes/driver/host/database/output)
 *
 */
        void          set_use_standard_storagepath(bool s=true);
        bool          use_standard_storagepath(void);
/**
 *path and name of the file which contains the output of the report (if not set with set_filename)
 *will be set while executing the report.
 */
        hk_string     outputfile(void);

/**
 * Here you can set a string which will be printed at the very beginning of the report (report header)
 * Can be used for initialisation of the report. To print data on top of the report see @ref report_header()
 */
        void          set_begin(const hk_string&,bool registerchange=true);
        hk_string     begin(void);
/**
 * Here you can set a string which will be shown at the very end of the report (report footer)
 * To print data at the end of the report see @ref report_footer()
 */
        void          set_end(const hk_string&,bool registerchange=true);
        hk_string     end(void);
/**
 *The page head will be printed on top of every page
 */
        hk_reportsection* page_header(void);
/**
 *The page head will be printed on top of every page
 */
        hk_reportsection* page_footer(void);
/**
 *the section that will be printed at the beginning of the report
 */
        hk_reportsection* report_header(void);
/**
 *the section that will be printed at the end of the report
 */
        hk_reportsection* report_footer(void);

/**
 *This section is printed for every row in a datasource. It should be regarded as the main data section of the report
 *This section will be embraced by the user defined sections created with @ref new_sectionpair
 */
        hk_reportsection* datasection(void);

/**
 * convenience function. See @ref new_sectionpair_at for details
 */

        hk_reportsectionpair* new_sectionpair(bool registerchange=true);
/**
 *creates a new sectionpair at position "position"
 *  position most outside position is 0
 */
        hk_reportsectionpair* new_sectionpair_at(unsigned int position,bool registerchange=true);
/**
 * moves an existing sectionpair relative to the other existing sectionpairs.
 *@param actualposition position of the sectionpair to be moved
 *@param steps positive values move it closer to the datasection, negative values move it away
 */
        bool move_sectionpair(unsigned int actualposition,int steps);

/**
 *After you have defined the report with @ref new_section etc. you can start the report
 *by calling this function.
 */
        bool          execute(void);
	bool    while_executing(void) const;
	bool    while_load_report(void) const;
	virtual bool while_loading(void);
       virtual bool            set_mode(enum_mode s);

/**
 *If the data has to be recoded (i.e. ASCII to html -> &uuml;) you can write a function and
 *set it with @ref set_recodefunction
 *@param r name of an existing recodefunction. A new one can be set with @ref add_recodetype
 */
        void        set_recodefunction(const hk_string& r,bool registerchange=true);
        recodefunctiontype* recodefunction(void);
        hk_string       recodefunctionstring(void);

        static void add_recodetype(const hk_string& name,recodefunctiontype* f);
        static list<hk_string>* recodelist(void);

        void        new_page(void);

/**
 * The pagedelimiter is a hk_string sequence to be sent between to pages (i.e. an escape
 *sequence for the printer)
 */
        void        set_pagedelimiter(const hk_string& d,bool registerchange=true);
        hk_string   pagedelimiter(void);
/**
 *A list of all the sections
 */
        vector<hk_reportsectionpair*>* sectionpairs(void);
/**
 *If a report has more than one page and this function is set to true
 *every page will be stored in a separate file.
 */
        virtual void    set_multiplefiles(bool m=true,bool registerchange=true);
        bool    multiplefiles(void);
/**
 *The output filename will be completed with this extension, i.e. '.ps' for postscript files
 */
        void        set_fileextension(const hk_string& ext,bool registerchange=true);
        hk_string   fileextension(void);
/**
 *Set the pagenumber if the first page should not start with 1.
 */
        void        set_firstpagenumber(long int f,bool registerchange=true);
        long int    firstpagenumber(void);
/**
 *You have different possibilities to name the report files. First, with @ref set_filename @ref set_fileextension
 *Second, you can have the the filename in a column of your datasource. Give the name of the column with this function
 *and then it will look up the name in this column. It is your responsibility to make sure that every filename is unique
 */
        void        set_pagenamecolumn(const hk_string& c,bool registerchange=true);
        hk_string   pagenamecolumn(void);
/**
 *will be set at the beginning of a file. If a report has multiple files b will be written in each file
 */
        void        set_filebegin(const hk_string& b,bool registerchange=true);
        hk_string   filebegin(void);
/**
 *will be set at the end of a file. If a report has multiple files e will be written in each file
 */
        void        set_fileend(const hk_string& e,bool registerchange=true);
        hk_string   fileend(void);
/**
 *A string that will be printed whenever a internal counter will get as big or greater than "when"
 *To define "what counts what" use @ref set_counts_as
 *Whenever the event of "counting" occurs by calling @ref row_count.
 *Example: You want to print 50 lines per page, your header is 3 lines wide and your data isone line
 *per data row set your page head to "3" and your datasection to "1"
 */
        void        set_periodic(unsigned long when, const hk_string& what="",bool newpage=false,bool registerchange=true);
        unsigned long   periodic_when(void);
        hk_string   periodic_what(void);
        bool        periodic_newpage(void);

/**
 *If the report will be printed to the standard output the outputstream is cout, else a textstream.
 */
        ostream*    outputstream(void);
/**
 *A report can contain a subreport. If this report is a subreport this is its link to its master report
 *Usually you dont have to call this function
 */

        void    set_masterreport(hk_report* m);
        hk_report* masterreport(void);
	bool is_subreport(void) const;
	bool is_subpresentation(void);

/**
 *This function does the counting job for @ref set_periodic and will be called automaticallly
 */
        void        rowcount(unsigned long);
        unsigned long   current_rowcount(void);
        virtual hk_font font(void);
/**
 *the number of the page. To get more than one page you have to use either
 * @ref hk_reportsection::set_new_page_after_section
 *or @ref hk_report::set_periodic
 */

        long pagenumber(void);
        long absolutepagenumber(void);
        unsigned long rownumber(void);

        virtual bool        set_presentationdatasource(long n, bool registerchange=true);

        virtual bool save_report(const hk_string& name="",bool ask=true);
        virtual bool load_report(const hk_string& name="");
        virtual void  savedata(ostream& s);
        virtual void  loaddata(xmlNodePtr definition);

/**
 *the default column value will be used as a default when a new data will be created with @ref new_data
 *default is @VALUE@
 */
        hk_string   default_reportdata(void);
        void        set_default_reportdata(const hk_string& v,bool registerchange=true);
/**
 *default beforedata. Will be set when a datafield is created
 */
        hk_string   default_beforereportdata(void);
        void        set_default_beforereportdata(const hk_string& v,bool registerchange=true);
/**
 * default afterdata. Will be set when a datafield is created
 */
        hk_string   default_afterreportdata(void);
        void        set_default_afterreportdata(const hk_string& v,bool registerchange=true);

        int         default_reportprecision(void);
        void        set_default_reportprecision(int, bool registerchange=true);

        bool        default_use_reportseparator(void);
        void        set_default_use_reportseparator(bool, bool registerchange=true);

/**
 * default dataconfigurefunction. Will be set when a datafield is created
 */
        void            set_default_reportdataconfigurefunction(const hk_string& f,bool registerchange=true);
        hk_string   default_reportdataconfigurefunction(void);
/**
 * default sectionbegin. Will be set when a section is created
 *See @ref hk_reportsection::set_sectionbegin for details
 */

        void set_default_reportsectionbegin(const hk_string& e,bool registerchange=true);
        hk_string default_reportsectionbegin(void);

/**
 * default sectionend. Will be set when a section is created
 *See @ref hk_reportsection::set_sectionend for details
 */

        void set_default_reportsectionend(const hk_string& e,bool registerchange=true);
        hk_string default_reportsectionend(void);
/**
 * default sectionbetweendata. Will be set when a section is created
 *See @ref hk_reportsection::set_sectionbetweendata for details
 */

        void set_default_reportsectionbetweendata(const hk_string& b,bool registerchange=true);
        hk_string default_reportsectionbetweendata(void);
/**
 * default sectioncountfunction. Will be set when a section is created
 *See @ref hk_reportsection::set_sectioncountfunction for details
 */

        void set_default_reportsectioncountfunction(const hk_string& f,bool registerchange=true);
        hk_string default_reportsectioncountfunction(void);

/**
 * paints a horizontal line on top of the page
 */
        void    set_topline(bool l=true,bool registerchange=true);
        bool    topline(void);
/**
 * paints a horizontal line on the bottom of the page
 */
        void    set_bottomline(bool l=true,bool registerchange=true);
        bool    bottomline(void);
/**
 * paints a vertical line on the left side of the page
 */

        void    set_leftline(bool l=true,bool registerchange=true);
        bool    leftline(void);

/**
 * paints a vertical line on the right side of the page
 */
        void    set_rightline(bool l=true,bool registerchange=true);
        bool    rightline(void);
/**
 * paints a diagonal line on the left bottom corner to the right upper corner of the page
 */
        void    set_diagonalluro(bool l=true,bool registerchange=true);
        bool    diagonalluro(void);
/**
 * paints a diagonal line on the left top corner to the right bottom corner of the page
 */
        void    set_diagonalloru(bool l=true,bool registerchange=true);
        bool    diagonalloru(void);
/**
 * paints a frame on the page
 */

        void    set_frame(bool l=true,bool registerchange=true);

/**
 * defines the borders of the page
 */
        void    set_borders(unsigned int left=200,unsigned  int right=200,unsigned  int top=200,unsigned  int bottom=300,bool registerchange=true);
        unsigned int    border_left(void);
        unsigned int    border_right(void);
        unsigned int    border_top(void);
        unsigned int    border_bottom(void);
        enum    orientationtype{landscape,portrait};
/**
 *sets the page orientation. Possible values are portrait and landscape
 */
        void    set_orientation(orientationtype o=portrait,bool registerchange=true);
        orientationtype orientation(void);
        enum    pageformattype {A0=0,A1=1,A2=2,A3=3,A4=4,A5=5,A6=6,A7=7,A8=8,A9=9,A10=10,letter=11,legal=12,executive=13,tabloid=14,userdefined=15}
        ;
/**
 *sets the correct page height and width of the equivalent page formats. Default is A4
 */
        void    set_pageformat(pageformattype t=A4,bool registerchange=true);
/**
 *sets the page height and width. The pageformat will be changed to "userdefined"
 */
        void    set_pageformat(unsigned int width ,unsigned int height,bool registerchange);
        pageformattype pageformat(void);
/**
 *returns the width of the pageformat
 */
        unsigned int    formatwidth(pageformattype t=A4) ;
/**
 *returns the height of the pageformat
 */
        unsigned int    formatheight(pageformattype t=A4);
        void    set_reportconfigurefunction(const hk_string& f,bool registerchange);

        void    set_reporttype(const hk_string&,bool registerchange=true);
        hk_string reporttype(void);

        static void add_reporttype(const hk_string& name,reporttypefunction* f);
        static list<hk_string>* reporttypelist(void);

        void    configure_page(void);
        void    neutralize_definition(bool registerchange);
/**
 *sets the report in the same state as it would be after creating;
 */
        void    init_report(void);
        static void add_configurefunctiontype(const hk_string& name,report_configurefunction* f);
        static list<hk_string>* configurefunctionlist(void);

        void   remove_sectionpair(hk_reportsectionpair*,bool registerchange=true);
        list<hk_string>* neededfonts(void);
        list<hk_string>* usedfonts(void);
        list<hk_string>* usedpsfonts(void);
/**
 *sets the printercommand. default is "lpr"
 */
        static void set_printcommand(const hk_string& printcommand);
        static hk_string printcommand(void);

/**
 *sends the report to the printer
 */
        bool print_report(bool execute_before_printing=true);

        unsigned int    designwidth(void);
        unsigned int    designheight(void);
        unsigned int        horizontal2relativ(unsigned int h);
        unsigned int        vertical2relativ(unsigned int v);
        unsigned int        relativ2horizontal(unsigned int r);
        unsigned int        relativ2vertical(unsigned int r);
        virtual void bulk_operation(enum_bulkoperation bulkoperation);
static  void setup_reportbasics(void);
hk_reportdata* get_reportdatavisible(long nr);
hk_reportdata* get_reportdatavisible(const hk_string& identifier);

void set_on_new_page_action(const hk_string&,bool registerchange=true);
hk_string on_new_page_action(void);
void stop_execution(void);
bool execution_stopped(void);

        hk_string filelocale(void);
        hk_string filecharset(void);
        void set_filelocale(const hk_string&);
        void set_filecharset(const hk_string&);
        void set_useencodingtab(bool);
        bool useencodingtab(void);
        hk_encodingtab* encodingtab(void) const;
/**
*default is false: if true the section output will be buffered until a new_page is created, if false it is directly written to the stream
*/        
        void set_print_full_pages_only(bool, bool registerchange);
        bool print_full_pages_only(void);


        void        set_fullpagereplacefunction(const hk_string& r,bool registerchange=true);
        recodefunctiontype* fullpagereplacefunction(void);
        hk_string      fullpagereplacefunctionstring(void);

        static void add_fullpagereplacefunctiontype(const hk_string& name,recodefunctiontype* f);
        static list<hk_string>* fullpagereplacefunctionlist(void);
        static void set_embedfonts(bool);
        static bool embedfonts(void);
virtual hk_string tag_value(int tagnumber, bool& ok);

    protected:
        void set_progressdialog(progress_dialogtype* progressdialog);
        progress_dialogtype* progressdialog(void)const;
        void   remove_section(hk_reportsection*);
//         virtual hk_string replace(const hk_string& where);
        virtual bool datasource_disable(void);
        virtual bool before_columns_deleted(void);

/**
 * If this function returns false the execution of the report will be cancelled
 */
        virtual bool widget_specific_before_new_page(void);

/**
 * If this function returns false the execution of the report will be cancelled
 */
        virtual bool widget_specific_after_new_page(void);

        virtual hk_reportsection* widget_specific_new_section(void);
        virtual void    widget_specific_after_loadreport(void){}
        virtual void    widget_specific_before_execute(void){}
        virtual void    widget_specific_after_first_outstream_created(void){}
        virtual void    widget_specific_before_last_outstream_closed(void){}
        virtual void    widget_specific_after_execute(void){}
	virtual bool     widget_specific_row_change(void);
	virtual void     widget_specific_batch_next(void);
	virtual void     widget_specific_batch_previous(void);
        void remove_all_sections(void);

        bool        p_multiplefiles;
        vector<hk_reportsectionpair*> p_sectionpairs;

        hk_string p_default_data;
        hk_string p_default_beforedata;
        hk_string p_default_afterdata;
        hk_string p_default_dataconfigurefunction;
	virtual void sizetype_changed(void);
        void        database_delete(void);

	virtual bool action_on_new_page(void);

    private:
        void init_section(hk_reportsection*);
        bool internal_execute();
        void    new_outputstream(void);
        void    set_pagenumber(long int f);
        void    create_fontslists(void);
        bool    is_newfont(const hk_string&);
        void    search_sectionfonts(hk_reportsection*);
        void    write_bufferedpage(void);
        void    registerfont(hk_font*);
	hk_reportprivate* p_private;
//   	bool 		p_lastpage;

        typedef  map<hk_string,reporttypefunction*,less<hk_string> >  reporttypelisttype ;
        static    reporttypelisttype  p_reporttypefunctions;
        static  list<hk_string> p_reporttypelist;

        typedef  map<hk_string,recodefunctiontype*,less<hk_string> >  recodefunctionlisttype ;
        static    recodefunctionlisttype  p_recodefunctions;
        static  list<hk_string> p_recodelist;

        static    recodefunctionlisttype  p_fullpagereplacefunctions;
        static  list<hk_string> p_fullpagereplacelist;

        typedef  map<hk_string,report_configurefunction*,less<hk_string> >  reportconfigurelisttype ;
        static    reportconfigurelisttype  p_reportconfigurefunctions;
        static  list<hk_string> p_reportconfigurelist;

        static int p_defaultfilenumber;
        static hk_string p_printcommand;
        static bool p_embedfonts;

}


;
#endif
