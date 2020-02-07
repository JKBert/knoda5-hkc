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
#ifndef HK_REPORTDATA
#define HK_REPORTDATA
#include "hk_definitions.h"
#include "hk_dsdatavisible.h"
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include <map>
using namespace std;

class hk_reportsection;
class hk_reportdata;
class hk_report;
class hk_reportdatamodeprivate;
class hk_reportdataprivate;

/**
 * represents the data of a field within a section
 * @short represents the data of a field within a section.
 *@version $Revision: 1.42 $
 * @author Horst Knorr (hk_classes@knoda.org)

 *
 *you can use the columnnames delimited by #
 *as variablenames. If a @ref hk_report::recodefunction is set, the values will be recoded.
 *<pre>
%XPOS%		the absolute value of the x-coordinate
%YPOS%  	the absolute value of the y-coordinate
%WIDTH%         the absolute value of the width
%HEIGHT%        the absolute value of the height
%RELXPOS%       the relative value of the x-coordinate
%RELYPOS%       the relative value of the y-coordinate
%RELWIDTH%      the relative value of the width
%RELHEIGHT%     the relative value of the height
%PAGENUMBER%     the number of the page
%ABSOLUTEPAGENUMBER%     physical number of the page in file
%ROWNUMBER%      the actual row number of the datasource
%COLUMNNAME%     the name of the column if set
%DATASOURCENAME% the name of the datasource if set
#column#         the value of the column as shown in the following two examples
#field1#         the value of the column with the name 'field1'
#first_name#     the value of the column with the name 'first_name'
</pre>
*If you have set @ref set_columnname  you have additionally the following variable names
<pre>
%VALUE%          the value of the column defined with @ref set_columnname 
with @ref hk_report::set_recodefunction
%FIELDNAME%      the  name of the column
defined with @ref set_columnname
%COUNT%          the number of values
</pre>
*If the column is of type integer or real you have the following variables
<pre>
%SUM%            the sum of all values
%MIN%            the minimum value
%MAX%            the maximum value
%STDDEV%         standard deviation
%STDDEVSAMPLE%   standard deviation of a sample
%AVERAGE%        the average of all values
</pre>
*/

class hk_reportdata :public hk_dsdatavisible
{
    friend class hk_reportsection;
    public:
/**
 *Set the data that should be printed when the section will be printed
 *You can use all of the above named variables.
 *Usually you connect a report_data part with a column via @ref set_columnname
 *The most easy way is to set the data to %VALUE%.
 * A more intermediate way will be shown in the following example
 *
 *Example:
 *
 *You want to automatically create links in a html report. For this reason
 *you have a datasource with 2 columns "url", and "name". "url" has the URL, "name" contains a description
i.e.
*<pre>
URL			name
http://www.suse.de 	SuseLinux
http://www.redhat.com	RedhatLinux
http://www.debian.org	Debian
*</pre>
To create a html link use set_data("<A HREF=\"#url#\">#name#</A>");
*/
        void set_data(const hk_string& d,bool registerchange=true);
        hk_string data(void);
        void set_beforedata(const hk_string& b,bool registerchange=true);
        void set_afterdata(const hk_string& a,bool registerchange=true);
	hk_string beforedata();
	hk_string afterdata();
/**
 *The name of a column of the datasource. Will be used for some variables such as %COUNT%,
 *%SUM%,AVERAGE% and so on.
 */
/**
 *returns the value that will be printed in the section
 */
        hk_string actual_string(void);
/**
 * Will be internally called to create the values for the variables %COUNT%,%SUM% etc.
 */
        void count(void);
/**
 *The values of the variables %SUM% %COUNT% %AVERAGE% %STDDEV% etc. can be relatively to the section.
 *i.e. if the section is a uniquesection and it changes all variables can be reset to 0 (default).
 * @param c if true these variables will change to global variables in the report and not be set to 0 when
 *the section changes.
 */
        void    set_runningcount(bool c=true,bool registerchange=true);
        bool    runningcount(void);
        virtual     hk_font font(void);
/**
 *needed by @ref hk_report::set_periodic. It is possible to compute as how much a section counts.
 *@param f is a userdefined function that will be called whenever @ref counts_as is called.
 */
        void set_datacountfunction(const hk_string& f,bool registerchange);
        hk_string datacountfunctionstring(void);

        reportdatacounttype* datacountfunction(void);
/**
 *if true a line should be painted above the data
 *
 *See @ref set_configurefunction  for details
 *
 */
        void    set_topline(bool l=true,bool registerchange=true);
/**
 *if true a line should be painted under the data
 *
 *See @ref set_configurefunction  for details
 */
        void    set_bottomline(bool l=true,bool registerchange=true);
/**
 *if true a line should be painted left of the data
 *
 *See @ref set_configurefunction  for details
 */

        void    set_leftline(bool l=true,bool registerchange=true);
/**
 *if true a line should be painted right the data
 *
 *See @ref set_configurefunction  for details
 */

        void    set_rightline(bool l=true,bool registerchange=true);
/**
 *if true a diagonal should be painted from _l_inks _u_nten to _r_echts _o_ben
 * (left down to right up)
 *
 *See @ref set_configurefunction  for details
 */

        void    set_diagonalluro(bool l=true,bool registerchange=true);
/**
 *if true a diagonal should be painted from _l_inks _o_ben to _r_echts _u_nten
 * (left top to right down)
 *
 *See @ref set_configurefunction  for details
 */
        void    set_diagonalloru(bool l=true,bool registerchange=true);
        bool    diagonalloru(void);
        bool    topline(void);
        bool    bottomline(void);
        bool    leftline(void);
        bool    rightline(void);
        bool    diagonalluro(void);
/**
 *if true a frame should be painted around the text (bottomline + topline + leftline + rightline)
 *
 *See @ref set_configurefunction  for details
 */
        void    set_frame(bool l=true,bool registerchange=true);
/**
 *if true the textline should be broken if the line is longer than the width
 *
 *See @ref set_configurefunction  for details
 */
        void    set_linebreak(bool b=true,bool registerchange=true);
        bool    linebreak(void);
	void    set_dynamic_height(bool dynamic,bool registerchange=true);
	bool    dynamic_height(void) const;

/**
 *in this function you have to define how to react on i.e. set_bottomline etc.
 */
        void    set_configurefunction(const hk_string& f,bool registerchange=true);
        data_configurefunctiontype* configurefunction(void);
        hk_string configurefunctionstring(void);

        virtual void    savedata(ostream& s,bool userdefined=false);
        virtual void  loaddata(xmlNodePtr definition,bool userdefined=false);
        void    neutralize_definition(bool registerchange);

        static void add_configurefunctiontype(const hk_string& name,data_configurefunctiontype* f);
        static list<hk_string>* configurefunctionlist(void);

        static void add_datacountfunctiontype(const hk_string& name,reportdatacounttype* f);
        static list<hk_string>* datacountfunctionlist(void);

/**
 *@see hk_reportsection::set_replacefunction
 */
        void        set_replacefunction(const hk_string& f,bool registerchange=true);
        static void add_datareplacefunctiontype(const hk_string& name,data_replacefunctiontype* f);
        static list<hk_string>* datareplacefunctionlist(void);
        void reset_count();
/**
 *returns the report this datafield belongs to
 */
        hk_report* report(void);
/**
 *returns the section this datafield belongs to
 */
        hk_reportsection* section(void);

        virtual void lower_widget(bool registerchange=true);
        virtual void raise_widget(bool registerchange=true);
	void set_on_print_action(const hk_string&,bool registerchange=true);
	hk_string on_print_action(void);
	list<hk_string>*  value_in_lines();
        
        unsigned int    height(void) ;
	bool data_fully_printed(void);
	void reset_data_fully_printed(void);
	int pagescount(void);
virtual hk_string tag_value(int tagnumber, bool& ok);
virtual hk_string column_value(const hk_string& colname,bool& ok);
void    set_is_image(bool,bool registerchange=true );
bool    is_image(void);

    protected:
        hk_reportdata(hk_reportsection*);
        virtual     ~hk_reportdata(void);
        virtual void new_column_pointer_created(void);
	virtual void sizetype_changed(void);
	virtual bool	presentationmode_changed(void);
virtual bool action_on_print(void);

    private:
      void clear_counting(void);

        typedef  map<hk_string,data_configurefunctiontype*,less<hk_string> >  reportdataconfigurelisttype ;
        static    reportdataconfigurelisttype  p_reportdataconfigurefunctions;
        static  list<hk_string> p_reportdataconfigurelist;

        hk_report* p_report;
        hk_reportsection* p_section;

        typedef  map<hk_string,reportdatacounttype*,less<hk_string> >  datacountlisttype ;
        static    datacountlisttype  p_datacountfunctions;
        static  list<hk_string> p_datacountfunctionlist;

        typedef  map<hk_string,data_replacefunctiontype*,less<hk_string> >  datareplacelisttype ;
        static    datareplacelisttype  p_datareplacefunctions;
        static  list<hk_string> p_datareplacefunctionlist;
	hk_reportdatamodeprivate* p_designdata;
	hk_reportdatamodeprivate* p_viewdata;
	hk_reportdataprivate* p_private;



};
#endif

