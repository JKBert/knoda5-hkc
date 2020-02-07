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
#ifndef HK_REPORTSECTION
#define HK_REPORTSECTION
#include "hk_definitions.h"
#include "hk_dsdatavisible.h"
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <time.h>
#include "hk_presentation.h"

using namespace std;

class hk_reportsection;
class hk_reportsectionpair;
class hk_reportdata;
class hk_report;
class hk_column;
/**
 *You can define your own functions for sections. See @ref hk_reportsection::set_replacefunction for details.
the function will be called after replacing the built-in variables and will give the "this"-Pointer and the text
*that would be printed as parameters. Your function has to return a hk_string that will be printed instead.
*/
typedef hk_string   section_replacefunctiontype(hk_reportsection*,const hk_string& );

typedef struct
{
    number sum;
    number min;
    number max;
    number value;
    unsigned long count;
    longnumber squaresum;
    char colname[75];
//  enum_fieldtype fieldtype;
} struct_countnumbers;

/**
 * hk_reportsection represents a section in a hk_report element. Within a section
 * you can define the outputdata,use it as headers for your data and so in
 *If it is needed as a section header or footer use @ref set_unique in combination with @ref set_uniquecolumn
 *It also can contain subreports.
 *Usually section come as "twins", a header section and a footer section belonging to each other,
 *see @ref hk_reportsectionpair for details
 *
 *<pre>Example: Defining a CVS output (no first row with columnnames)
 * //
 * // first define a datasource
 * //
 *hk_drivermanager p_driver;
 *hk_connection* connect=p_driver.new_connection("mysql");
 *connect->set_user("root");
 *connect->set_host("localhost");
 *connect->set_password("my_password");
 *connect->connect();
 *hk_database* database=connect->new_database("vokabeltrainer");
 * //
 * //now start defining the report
 * //
 *hk_report e;
 * long int datasrc =e.new_datasource("reportabfrage",true);//loads an existing query
 * e.set_presentationdatasource(datasrc);
 *hk_reportsection* s=e.datasection();
 *s->set_default_data("\"%VALUE%\""); // define " before and after the value of a column
 *s->set_betweendata(" , "); // a comma separated list
 *s->set_sectionend("\n"); // one row per datarow
 *e.execute();
 *</pre>
 * you can use the variables
 *
<pre>
%PAGENUMBER%         the number of the page
%ABSOLUTEPAGENUMBER% physical number of the page in file
</pre>

* @short represents a section of the report.
*@version $Revision: 1.46 $
* @author Horst Knorr (hk_classes@knoda.org)
*/

class hk_reportsection : public hk_dsdatavisible
{
    friend class hk_report;
    friend class hk_reportsectionpair;
    friend class hk_reportdata;
    public:
/**
 *If the section should be printed just once per block and not for each datarow use this function
 *@param unique true if this section should be printed once
 *interesting four grouped data
 *@param endsection true if this section should not be printed as a header. Instead it will be printed
 *at the end of the block. Sections created with @ref hk_reportsectionpair are already set correctly.
 */
        void        set_unique(bool unique=true, bool endsection=false,bool registerchange=true);
//hk_string		unique_column(void){return p_uniquecolumnname;}
        bool        unique(void);
/**
 *the printed data. See @ref hk_reportdata and @ref default_data for details
 */
        hk_reportdata*  new_data(void);
//hk_reportdata*  new_data_at(unsigned int);
        hk_reportdata*  data_at(unsigned int);
//void		delete_at(unsigned int);
        vector<hk_reportdata*>* datalist(void);
/**
 *will send the actual data to the outputstream  @ref hk_report::outputstream
 */
        virtual bool        actual_string(void);
/**
 *The sectionbegin will be printed as a header of the section (before the data)
 */
        hk_string       sectionbegin(void);
        void        set_sectionbegin(const hk_string& b,bool registerchange=true);
/**
 *The sectionend will be printed as a footer of the section (after the data)
 *i.e. a \n at the end of a datarow
 */
        hk_string   sectionend(void);
        void        set_sectionend(const hk_string& e,bool registerchange=true);
/**
 *A string that will be printed between two datasegments defined with @ref new_data
 *i.e. If you want a comma separated list you would need set_betweendata(" , ");
 */
        hk_string   betweendata(void);
        void        set_betweendata(const hk_string& b,bool registerchange=true);
/**
 *@param justcheck if true no variables(lastvalue) will be changed
 *@return true if this section is a unique section and this section has to be printed
 */

        bool        new_uniquevalue(bool justcheck=false);
/**
 * @return true if this section is a unique section and will be printed behind the data
 */
        bool        endsection(void);

/**
 *If set true and you haven't defined any data with @ref new_data it will automatically create the data
 *set with @ref default_data
 */
        void        set_automatic_create_data(bool d=true,bool registerchange=true);
/**
 *true if a new page will be started after printing this section. Usually this function should only be used
 *in combination with @ref set_unique as an  endsection
 */
        bool        new_page_after_section(void);
        void        set_new_page_after_section(bool p,bool registerchange=true);
/**
 * @return true if this section has been printed (unique sections will not be printed for every row) else false
 */
        bool        section_was_printed(void);
/**
 *A complete other report can be printed within a section
 *@param subreport the subreport
 *@param before_data if true the subreport will be printed before the data of this section will be printed, else
 *it will be printed after the data
 */
        void        set_subreport(const hk_string& subreport="",bool before_data=false,bool registerchange=true);
        hk_string   subreportname(void);
        hk_report*  subreport(void);
/**
 *If you set a subreport with @ref set_subreport add the fields which are connected between the reports
 */
        void        add_depending_fields(const hk_string& thisreport_field, const hk_string& subreport_field,bool registerchange=true);
        void        clear_depending_fields(void);
        list<hk_string>* depending_on_thisreportfields(void);
        list<hk_string>* depending_on_subreportfields(void);

        bool        print_subreport_before_data(void){return p_print_subreport_before_data;}
        bool        print_subreport(void);
/**
 *You can enhance the possibilities of the section by writing your own function.This function could for example handle your
 *own variables.
 *
 */
        void        set_replacefunction(const hk_string& f,bool registerchange=true);
        virtual hk_string replace(const hk_string& where);

        static void add_sectionreplacefunctiontype(const hk_string& name,section_replacefunctiontype* f);
        static list<hk_string>* sectionreplacefunctionlist(void);

/**
 *Will set all COUNT, SUM, AVERAGE and so on variables to 0, if they are not "running counts" (which mean global
 *for the whole report).See @ref hk_reportdata for details. Usually you dont't have to call this function
 */
        void        reset_count(void);
        virtual hk_font font(void);
        virtual unsigned long counts_as(void);
/**
 *needed by @ref hk_report::set_periodic. It is possible to compute as how much a section counts.
 *@param f is a userdefined function that will be called whenever @ref counts_as is called.
 */
        static void add_sectioncountfunctiontype(const hk_string& name,reportsectioncounttype* f);
        static list<hk_string>* sectioncountfunctionlist(void);

        void set_sectioncountfunction(const hk_string& f,bool registerchange=true);
        hk_string sectioncountfunctionstring(void);
        reportsectioncounttype* sectioncountfunction(void);
        virtual void  savedata(ostream& s,bool userdefined=false);
        virtual void  loaddata(xmlNodePtr definition,bool userdefined=false);
        virtual void set_datasource(hk_datasource* d);
        virtual bool        set_presentationdatasource(long n, bool registerchange=true);
/**
 *the default column value will be used as a default when a new data will be created with @ref new_data
 *default is @VALUE@
 */
        hk_string   default_reportdata(void){return p_default_data;}
        void        set_default_reportdata(const hk_string& v,bool registerchange=true);
        hk_string   default_beforereportdata(void){return p_default_beforedata;}
        void        set_default_beforereportdata(const hk_string& v,bool registerchange=true);
        hk_string   default_afterreportdata(void){return p_default_afterdata;}
        void        set_default_afterreportdata(const hk_string& v,bool registerchange=true);
        void            set_default_reportdataconfigurefunction(const hk_string& f,bool registerchange=true);
        hk_string   default_reportdataconfigurefunction(void);
        int         default_reportprecision(void);
        void        set_default_reportprecision(int, bool registerchange=true);

        bool        default_use_reportseparator(void);
        void        set_default_use_reportseparator(bool, bool registerchange=true);

        void    neutralize_definition(bool registerchange);
        virtual void    reset(void);
        void clear_countingfields(void);
        void    remove_data(hk_reportdata*);
        hk_report* report(void);
/**
 * 100% is report width - left border - right border
 */
        unsigned int        horizontal2relativ(unsigned int h);
/**
 * 100% is report height - top border - bottom border
 */
        unsigned int        vertical2relativ(unsigned int v);
/**
 * 100% is report width - left border - right border
 */
        unsigned int        relativ2horizontal(unsigned int r);
/**
 * 100% is report height - top border - bottom border
 */
        unsigned int        relativ2vertical(unsigned int r);
        hk_reportsectionpair*   reportsectionpair(void){return p_pair;}
/**
 *The offset is additional space at the end of the section (totalsectionsize= size needed by reportdata + offset)
 */
        bool set_offset(unsigned int o,bool registerchange=true);
        unsigned int offset(void);
        void bulk_operation(hk_presentation::enum_bulkoperation bulkoperation);
        void set_automaticcreatedatafunction(reportsectionautomaticcreatedatatype*);
        reportsectionautomaticcreatedatatype* automaticcreatedatafunction(void){ return p_automaticreatedatafunction;}
hk_reportdata* get_reportdatavisible(long nr);
hk_reportdata* get_reportdatavisible(const hk_string& identifier);
         bool is_fully_printed(void);
         void set_print_section(bool);
         bool print_section(void);


    protected:
        hk_reportsection(hk_report*);
        virtual     ~hk_reportsection(void);
        virtual void    before_datasource_enables(void);
        void    remove_all_datas(void);
        virtual hk_reportdata* widget_specific_new_data(void);
        virtual void widget_specific_after_loaddata(void){}
        virtual void widget_specific_offset_changed(void){}
	virtual void sizetype_changed(void);
	virtual bool	presentationmode_changed(void);
        void    automatic_create_datafields(void);
        void force_new_section(void);

        bool p_already_automaticcreated_data;

        vector<hk_reportdata*> p_data;
//        hk_string 	p_uniquecolumnname;
        bool   p_unique;
        hk_string p_lastuniquevalue;
        hk_string p_sectionbegin;
        hk_string p_sectionend;
        hk_string p_betweendata;
        hk_report* p_report;
        hk_report* p_subreport;
        hk_string  p_subreportstring;
        bool       p_print_subreport_before_data;
        bool p_uniqueendsection;
        bool p_automatic_create_datas;
        bool p_newpageaftersection;
        bool p_sectionwasprinted;
        section_replacefunctiontype* p_replacefunction;
        hk_string   p_replacefunctionstring;
        hk_string p_default_data;
        hk_string p_default_beforedata;
        hk_string p_default_afterdata;
        hk_string p_default_dataconfigurefunction;
        int     p_default_precision;
        bool    p_default_use_thousandsseparator;
        unsigned int p_offset;
        void raise_widget(hk_reportdata*);
        void lower_widget(hk_reportdata*);

    private:
        void create_countingfields(void);
        void count_countingfields(void);
	void clear_counting(void);
        reportsectioncounttype* p_sectioncountfunction;
        hk_string  p_sectioncountfunctionstring;
        hk_datasource* p_subreportdatasource;
        bool p_automatic_created_datasource;
        vector<struct_countnumbers> p_countings;

        typedef  map<hk_string,reportsectioncounttype*,less<hk_string> >  sectioncountlisttype ;
        static    sectioncountlisttype  p_sectioncountfunctions;
        static  list<hk_string> p_sectioncountfunctionlist;

        typedef  map<hk_string,section_replacefunctiontype*,less<hk_string> >  sectionreplacelisttype ;
        static    sectionreplacelisttype  p_sectionreplacefunctions;
        static  list<hk_string> p_sectionreplacefunctionlist;
        list<hk_string> p_depending_thisreport_fields;
        list<hk_string> p_depending_subreport_fields;
        hk_reportsectionpair* p_pair;
        bool p_forcenewsection;
        reportsectionautomaticcreatedatatype* p_automaticreatedatafunction;
        bool p_printsection;
};
#endif
