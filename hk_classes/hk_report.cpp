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
// $Revision: 1.130 $
#include "hk_report.h"
#include "hk_reportsection.h"
#include "hk_reportsectionpair.h"
#include "hk_reportdata.h"
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "hk_reportutils.h"
#include "hk_column.h"
#include "hk_datasource.h"
#include "hk_database.h"
#include "hk_connection.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define VERSION UNKNOWN
#endif

class hk_reportprivate
{
public:
hk_reportprivate()
  {
    p_while_executing=false;
    p_print_full_pages_only=false;
    p_nostrstream=false;
    p_rowposition=0;
    p_fullpagereplacefunction=NULL;
    p_progressdialog=NULL;
    p_locale="";
    p_useencodingtab=false;
    p_cleanpage=true;
    p_while_load_report=false;
  }
~hk_reportprivate(){}
hk_string p_on_new_page_action;
        bool p_cancelreport;
        bool p_top;
        bool p_left;
        bool p_right;
        bool p_bottom;
        bool p_loru;
        bool p_luro;
        unsigned int p_borderleft;
        unsigned int p_borderright;
        unsigned int p_bordertop;
        unsigned int p_borderbottom;
        bool p_cleanpage;//used to mark whether a page is still virgin
        bool p_while_load_report;
        hk_string       p_begin;
        hk_string       p_end;
        hk_string       p_filebegin;
        hk_string       p_fileend;
        hk_string       p_filename;
        hk_string       p_originalfilename;
        hk_string       p_fileextension;
        hk_string     p_pagedelimiter;
        ostream*      p_output;
        ofstream*     p_filestream;
        hk_column*  p_pagenamecolumn;
        hk_string   p_pagename;
        hk_string   p_includeperiodicstring;
        unsigned long   p_includeperiodic;
        hk_report*  p_masterreport;
        long       p_rowcounter;
        bool       p_newpage_on_periodic;
        time_t     p_currenttime;
        long       p_pagenumber;
        long       p_startpagenumber;
        long       p_absolutepagenumber;
	unsigned int p_rowposition;
        hk_string p_defaultsectioncountfunction;
        hk_string p_defaultsectionbegin;
        hk_string p_defaultsectionend;
        hk_string p_defaultsectionbetweendata;
        hk_string p_reporttype;
        hk_string p_recodefunctionstring;
        int     p_default_precision;
        bool    p_default_use_thousandsseparator;
        list<hk_string> p_usedfonts;
        list<hk_string> p_usedpsfonts;
        list<hk_string> p_suppliedfonts;
        list<hk_string> p_suppliedfontspsnames;
        list<hk_string> p_neededfonts;
        hk_reportsection* p_datasection;
        bool p_use_standardpath;
        bool p_print_full_pages_only;
        hk_string p_defaultfilename;
        hk_string p_outputfile;
        recodefunctiontype*     p_recodefunction;
        recodefunctiontype*     p_fullpagereplacefunction;
        hk_string p_fullpagereplacefunctionstring;
        hk_report::pageformattype p_pageformat;
        hk_report::orientationtype p_orientation;
        report_configurefunction* p_configurefunction;
        hk_string p_configurefunctionstring;

        hk_reportsection* p_pageheader;
        hk_reportsection* p_pagefooter;
        hk_reportsection* p_reportheader;
        hk_reportsection* p_reportfooter;
        hk_string p_locale;
        hk_string p_charset;
        bool p_while_executing;
        progress_dialogtype* p_progressdialog;
	hk_string p_loadreportname;
	hk_encodingtab p_encodingtab;
	STRINGSTREAM p_stringstream;
	bool p_nostrstream;
	bool p_useencodingtab;
	hk_string p_embeddedfontsdefinition;
	int tag_FONT,tag_PSFONT,tag_PSEMBEDDEDFONTS,tag_FONTSIZE,tag_PTIME,tag_ABSOLUTEPAGENUMBER,tag_PAGENUMBER,
	tag_BORDERLEFT,tag_BORDERRIGHT,tag_BORDERTOP,tag_BORDERBOTTOM,tag_RELBORDERTOP,tag_RELBORDERBOTTOM,
	tag_RELBORDERLEFT,tag_RELBORDERRIGHT,tag_BOUNDINGBOX,tag_PSNEEDEDFONTS,tag_PSUSEDFONTS,tag_PSSUPPLIEDFONTS;
};



const double teiler=10000.0;

hk_report::reporttypelisttype  hk_report::p_reporttypefunctions;
list<hk_string>  hk_report::p_reporttypelist;

hk_report::recodefunctionlisttype hk_report::p_recodefunctions;
list<hk_string>  hk_report::p_recodelist;

hk_report::recodefunctionlisttype hk_report::p_fullpagereplacefunctions;
list<hk_string>  hk_report::p_fullpagereplacelist;

hk_report::reportconfigurelisttype  hk_report::p_reportconfigurefunctions;
list<hk_string> hk_report::p_reportconfigurelist;

bool hk_report::p_embedfonts=false;
int hk_report::p_defaultfilenumber=0;
hk_string hk_report::p_printcommand="lpr";
//********************************************
//**   hk_report                            **
//********************************************

hk_report::hk_report(void):hk_presentation()
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_report::hk_report");
#endif

    p_private=new hk_reportprivate;
    set_automatic_enable_datasources(false);
    p_presentationtype=hk_presentation::report;
    p_visibletype=hk_visible::report;
    p_private->p_configurefunction=NULL;
    p_private->p_recodefunction=NULL;
    p_private->p_filestream=NULL;
    p_private->p_pageheader=NULL;
    p_private->p_pagefooter=NULL;
    p_private->p_reportheader=NULL;
    p_private->p_reportfooter=NULL;
    p_private->p_datasection=NULL;
    p_private->p_pageformat=hk_report::A4;
    p_private->p_orientation=hk_report::portrait;
    p_private->p_cancelreport=false;
    p_private->p_use_standardpath=true;
    p_private->p_defaultfilename=hk_translate("unnamed")+longint2string(p_defaultfilenumber);
    p_defaultfilenumber++;
    set_backgroundcolour(hk_white,false,true);
    set_foregroundcolour(hk_black,false,true);
    p_private->tag_FONT=register_tag("FONT");
    p_private->tag_PSFONT=register_tag("PSFONT");
    p_private->tag_PSEMBEDDEDFONTS=register_tag("PSEMBEDDEDFONTS");
    p_private->tag_FONTSIZE=register_tag("FONTSIZE");
    p_private->tag_PTIME=register_tag("PTIME");
    p_private->tag_ABSOLUTEPAGENUMBER=register_tag("ABSOLUTEPAGENUMBER");
    p_private->tag_PAGENUMBER=register_tag("PAGENUMBER");
    p_private->tag_BORDERLEFT=register_tag("BORDERLEFT");
    p_private->tag_BORDERRIGHT=register_tag("BORDERRIGHT");
    p_private->tag_BORDERTOP=register_tag("BORDERTOP");
    p_private->tag_BORDERBOTTOM=register_tag("BORDERBOTTOM");
    p_private->tag_RELBORDERTOP=register_tag("RELBORDERTOP");
    p_private->tag_RELBORDERBOTTOM=register_tag("RELBORDERBOTTOM");
    p_private->tag_RELBORDERLEFT=register_tag("RELBORDERLEFT");
    p_private->tag_RELBORDERRIGHT=register_tag("RELBORDERRIGHT");
    p_private->tag_BOUNDINGBOX=register_tag("BOUNDINGBOX");
    p_private->tag_PSNEEDEDFONTS=register_tag("PSNEEDEDFONTS");
    p_private->tag_PSUSEDFONTS=register_tag("PSUSEDFONTS");
    p_private->tag_PSSUPPLIEDFONTS=register_tag("PSSUPPLIEDFONTS");

    
    
    setup_reportbasics();
    init_report();
}

void hk_report::setup_reportbasics(void)
{
    if (p_reporttypefunctions.size()==0)
    {
        add_reporttype("Postscript",&set_reporttype_postscript);
        add_reporttype("Userdefined",NULL);
    }
    if (p_recodefunctions.size()==0)
    {
        add_recodetype("Postscript",&recode_postscript);
        add_recodetype("Html",&recode_html);
        add_recodetype("UTF8",&recode_utf8);
        add_recodetype("None",NULL);
    }

    if (p_fullpagereplacefunctions.size()==0)
    {
/*        add_recodetype("Postscript",&recode_postscript);
        add_recodetype("Html",&recode_html);
        add_recodetype("UTF8",&recode_utf8);*/
        add_fullpagereplacefunctiontype("Postscript",&fullpagereplace_postscript);
        add_fullpagereplacefunctiontype("ExcelXML",&fullpagereplace_excelxml);
        add_fullpagereplacefunctiontype("None",NULL);
    }

    if (p_reportconfigurefunctions.size()==0)
    {
        add_configurefunctiontype("Postscript",&configure_postscriptreport);
        add_configurefunctiontype("None",NULL);
    }





}




void hk_report::init_report(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::init_report");
#endif
    p_private->p_output=&cout;
    p_private->p_default_precision=-1;
    p_private->p_default_use_thousandsseparator=false;
    set_mode(hk_presentation::designmode);
    reset_has_changed();
    remove_all_sections();
    neutralize_definition(false);
    if (p_private->p_datasection!=NULL) delete p_private->p_datasection;
    p_private->p_datasection=widget_specific_new_section();
    init_section(p_private->p_datasection);
    p_private->p_datasection->set_automatic_create_data(false);
    if (p_private->p_pageheader!=NULL) delete p_private->p_pageheader;
    p_private->p_pageheader= widget_specific_new_section();
    init_section(p_private->p_pageheader);
    p_private->p_pageheader->set_automatic_create_data(false);
    if (p_private->p_pagefooter!=NULL) delete p_private->p_pagefooter;
    p_private->p_pagefooter= widget_specific_new_section();
    init_section(p_private->p_pagefooter);
    p_private->p_pagefooter->set_automatic_create_data(false);
    if (p_private->p_reportheader!=NULL)delete p_private->p_reportheader;
    p_private->p_reportheader= widget_specific_new_section();
    p_private->p_reportheader->set_automatic_create_data(false);

    if (p_private->p_reportfooter!=NULL)delete p_private->p_reportfooter;
    p_private->p_reportfooter= widget_specific_new_section();
    p_private->p_reportfooter->set_automatic_create_data(false);

    register_object(p_private->p_pageheader);
    register_object(p_private->p_pagefooter);
    register_object(p_private->p_reportheader);
    register_object(p_private->p_reportfooter);
    register_object(p_private->p_datasection);
    if (p_private->p_filestream!=NULL) delete p_private->p_filestream;
    p_private->p_filestream=NULL;
    p_private->p_top=p_private->p_bottom=p_private->p_left=p_private->p_right=p_private->p_luro=p_private->p_loru=false;
    p_multiplefiles=false;
    p_private->p_pagenamecolumn=NULL;
    p_private->p_includeperiodic=0;
    p_private->p_rowcounter=0;
    p_default_data="%VALUE%";
    p_private->p_masterreport=NULL;
    p_private->p_newpage_on_periodic=false;
    p_private->p_pagenumber=p_private->p_startpagenumber=1;
    p_private->p_absolutepagenumber=1;
    set_reporttype("Userdefined",false);
    if (sizetype()==hk_presentation::relative)set_borders(950,950,950,1000,false);
    else set_borders(200,200,200,250,false);
    set_pageformat(hk_report::A4,false);
    set_orientation(hk_report::portrait,false);
    p_private->p_outputfile="";
    reset_has_changed();
}


hk_report::~hk_report(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::~hk_report");
#endif
   stop_execution();
    remove_all_sections();
    delete p_private->p_pageheader;
    delete p_private->p_pagefooter;
    delete p_private->p_reportheader;
    delete p_private->p_reportfooter;
    delete p_private->p_filestream;
    delete p_private->p_datasection;

    if (database()!=NULL)
    {
        hk_string f=database()->database_path()+"/output/"+p_private->p_defaultfilename;
        unlink(f.c_str());
    }

    delete p_private;
}


void hk_report::remove_all_sections(void)
{
    vector<hk_reportsectionpair*>::iterator it=p_sectionpairs.begin();
    while (it!=p_sectionpairs.end())
    {
        hk_reportsectionpair* s=(*it);
        it=p_sectionpairs.erase(it);
        s->p_report=NULL;
        delete s;
    }

}


void hk_report::set_filename(const hk_string& f,bool registerchange)
{
    p_private->p_originalfilename=p_private->p_filename=f;
    has_changed(registerchange);
}


hk_string hk_report::filename(void)
{
    return p_private->p_filename;
}


void    hk_report::set_begin(const hk_string& b,bool registerchange)
{
    p_private->p_begin=b;
    has_changed(registerchange);
}


hk_string   hk_report::begin(void)
{
    return p_private->p_begin;
}


void        hk_report::set_end(const hk_string& e,bool registerchange)
{
    p_private->p_end=e;
    has_changed(registerchange);
}


hk_string   hk_report::end(void)
{
    return p_private->p_end;
}


hk_reportsection* hk_report::widget_specific_new_section(void)
{
    hk_reportsection* s= new hk_reportsection(this);
    return s;
}


hk_reportsectionpair* hk_report::new_sectionpair(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::new_sectionpair");
#endif
    return new_sectionpair_at(0,registerchange);

}


hk_reportsectionpair* hk_report::new_sectionpair_at(unsigned int position,bool registerchange)
{

    hk_reportsectionpair* p=new hk_reportsectionpair(this);
    if (p!=NULL)
    {
        register_object(p->headersection());
        register_object(p->footersection());
        has_changed(registerchange);
        p_sectionpairs.insert(position<p_sectionpairs.size()?p_sectionpairs.begin()+position:p_sectionpairs.end(),p);
    }
    return p;

}


bool hk_report::set_mode(hk_report::enum_mode m)
{
bool result=true;

if (m==designmode && while_executing())
{ 
 stop_execution();
}
else
if (m==viewmode && while_executing())
  return true;
hk_presentation::set_mode(m);

    presentationmode_changed();
     if(m== viewmode)
     {
//        action_on_open();
       result= internal_execute();
     }
    vector<hk_reportsectionpair*>::iterator sit=p_sectionpairs.begin();
    while (sit!=p_sectionpairs.end())
    {
    if ((*sit)->headersection())(*sit)->headersection()->presentationmode_changed();
    if ((*sit)->footersection())(*sit)->footersection()->presentationmode_changed();
     ++sit;
    }
    if (p_private->p_datasection) p_private->p_datasection->presentationmode_changed();
     if (p_private->p_reportfooter)p_private->p_reportfooter->presentationmode_changed();
    if (p_private->p_reportheader)p_private->p_reportheader->presentationmode_changed();
    if (m==designmode) action_on_close();
  return result;
}



bool hk_report::execute()
{
   return set_mode(viewmode);
}



bool hk_report::internal_execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::internal_execute");
#endif
//prepare datasource for output, resort data
    if (p_private->p_while_load_report) return false;
    if (datasource()==NULL) return false;
    if (p_private->p_while_executing)
    { 
     return true;
    }
    p_private->p_while_executing=true;
    p_private->p_encodingtab.reset();
    p_private->p_encodingtab.register_unicode(0x0A,".notdef");
    p_private->p_encodingtab.register_unicode(0x20,"space");
    p_private->p_cancelreport=false;
    p_private->p_nostrstream=false;
    p_private->p_stringstream.str("");

    //enum_mode modebuffer=p_mode;
    p_private->p_currenttime=time(NULL);
    hk_string ds_sort=datasource()->temporarysorting();
    hk_string sort;
    vector<hk_reportsectionpair*>::iterator sit=p_sectionpairs.begin();
    //p_mode=view;
    while (sit!=p_sectionpairs.end())
    {
        if ((*sit)->columnname().size()>0)
        {
            if ((*sit)->headersection()!=NULL)
            {
                (*sit)->headersection()->clear_counting();
                (*sit)->headersection()->p_lastuniquevalue="";

            }
            if ((*sit)->footersection()!=NULL)
            {
                (*sit)->footersection()->clear_counting();
                (*sit)->footersection()->p_lastuniquevalue="";
            }
            hk_string st=(*sit)->columnname();
	    if (datasource()->type()==hk_datasource::ds_query)
	    {
	       hk_datasource* d=datasource()->database()->new_resultquery();
	       d->set_sql(datasource()->sql());
	       d->set_filter("0=1");
	       d->enable();
	       hk_string t;
	       hk_column*c=d->column_by_name(st);
	       if (c)
	       {
	        t=c->tableorigin();
	       }
	       delete d;
	       if (t.size()>0)
	       {
	       st="\""+t+"\".\""+st+"\"";
	       }
	       else
	       st=datasource()->totalfieldorigin(st);
	    }
	    else st="\""+st+"\"";
            if (st.size()>0 &&!(*sit)->ascending_order()) st+=" DESC";
            if (sort.find(st)==hk_string::npos)
            {
                if (sort.size()>0)sort+=" , ";

                sort+=st;
            }
        }
        sit++;
    }
    p_private->p_datasection->clear_counting();
    p_private->p_reportfooter->clear_counting();
    p_private->p_reportheader->clear_counting();
    enable_datasources(false);
    datasource()->set_temporarysorting(sort);     //cout <<"sort: "<<sort<<endl;
    datasource()->set_use_temporarysorting(true);
    enable_datasources(true,hk_datasource::batchread);
    widget_specific_before_execute();

    if (!datasource()->is_enabled())
    {
        show_warningmessage(hk_translate("Datasource of this report could not be enabled")
	+"\n"+hk_translate("Servermessage: ")+datasource()->database()->connection()->last_servermessage());
        datasource()->set_temporarysorting(ds_sort);
        widget_specific_after_execute();
	//p_mode=modebuffer;
	p_private->p_while_executing=false;

        return false;
    }

    p_private->p_pagenamecolumn=datasource()->column_by_name(p_private->p_pagename);
    if (p_private->p_pagenamecolumn!=NULL)
    {
        p_private->p_pagenamecolumn->set_datetimeformat(datetimeformat());
        p_private->p_pagenamecolumn->set_dateformat(dateformat());
        p_private->p_pagenamecolumn->set_timeformat(timeformat());
    }
    create_fontslists();
    new_outputstream();
    widget_specific_after_first_outstream_created();
//start output
    p_private->p_rowcounter=0;
    p_private->p_pagenumber=p_private->p_startpagenumber;
    p_private->p_absolutepagenumber=1;
    if (p_private->p_masterreport==NULL)
    {
        if (p_private->p_output==&cout)*outputstream()<<replace(p_private->p_filebegin);
	if (!action_on_new_page())stop_execution();
        if (p_private->p_pageheader!=NULL )
	if (!p_private->p_pageheader->actual_string())
	  stop_execution();
	;
    }
    *outputstream()<<  replace(p_private->p_begin);
    if (p_private->p_masterreport==NULL)
    {
        if (!report_header()->actual_string()) stop_execution();
        if (report_header()->new_page_after_section()&&report_header()->section_was_printed())
        {
            new_page();
            p_private->p_rowcounter=0;
        }

    }
    p_private->p_rowposition=0;
//MAIN DATA LOOP
    do
    {
//print header sections
     bool stop=false;
     if (p_private->p_progressdialog)
        {
        stop= p_private->p_progressdialog(p_private->p_rowposition,datasource()->max_rows(), hk_translate("Creating output ..."));
        }
        if (stop)p_private->p_cancelreport=true;
        if (p_private->p_cancelreport) stop_execution();
        vector<hk_reportsectionpair*>::iterator it=p_sectionpairs.begin();
        int p=0;
        bool printfootersection=true;
        while (it!=p_sectionpairs.end())
        {
            hk_reportsection* h=(*it)->headersection();
            if (h!=NULL)
            {
                h->count_countingfields();
                if (h->unique()&&h->new_uniquevalue(true)&&datasource()->row_position()>0&&printfootersection)
                {
//a new unique section will be printed
// so print end sections first

                    long int c=p_sectionpairs.size();
                    if (c>0)
                    {
                        if (datasource())
			 {
			  datasource()->goto_previous();
			 }


                        while (c>=p+1)
                        {
                            hk_reportsection* f=p_sectionpairs[c-1]->footersection();
                            if (f!=NULL)
                            {
                                f->force_new_section();
                                if (!f->actual_string())stop_execution();
                                if (f->new_page_after_section()&&f->section_was_printed())
                                {
                                    if (datasource())datasource()->goto_next();
				    new_page();
				    if (execution_stopped())
				     {
				       p_private->p_while_executing=false;
				       return false;
				     }
				    if (datasource())
				     {
				       datasource()->goto_previous();

				     }
                                    p_private->p_rowcounter=0;
                                }
                            }
                            f=p_sectionpairs[c-1]->headersection();
                            if (f!=NULL)
                            {
                                f->p_lastuniquevalue="";
                            }
                            f->reset_count();
                            c--;
                        }
                        if (datasource())datasource()->goto_next();
                        printfootersection=false;
                        p_private->p_datasection->reset_count();
                    }                             //end footersections
                }

                if (!h->actual_string())stop_execution();
                if (h->new_page_after_section()&&h->section_was_printed())
                {
                    new_page();
                    p_private->p_rowcounter=0;
                }
                hk_reportsection* fo=(*it)->footersection();
                if (fo!=NULL) fo->count_countingfields();
                it++; p++;
            }

        }

//print datasection
        if (p_private->p_datasection!=NULL)
        {
            p_private->p_datasection->count_countingfields();
            if (!p_private->p_datasection->actual_string())stop_execution();
            if (p_private->p_datasection->new_page_after_section()&&p_private->p_datasection->section_was_printed())
            {
                new_page();
                p_private->p_rowcounter=0;
            }
        }
        if (p_private->p_reportfooter!=NULL)p_private->p_reportfooter->count_countingfields();
        if (p_private->p_pageheader!=NULL)p_private->p_pageheader->count_countingfields();
        if (p_private->p_pagefooter!=NULL)p_private->p_pagefooter->count_countingfields();
        if (datasource())datasource()->goto_next();
	//p_private->p_rowposition++;
    } while(datasource()&& p_private->p_rowposition<datasource()->max_rows()&&!execution_stopped());

//print footersections
    long int c=p_sectionpairs.size();
    if (c>0)
    {
        while (c>0)
        {
            hk_reportsection* h=p_sectionpairs[c-1]->footersection();
            if (h!=NULL)
            {
                if (!h->actual_string())stop_execution();
                if (h->new_page_after_section()&&h->section_was_printed()&&(c>1||(c==1&&report_footer()->datalist()->size()>0)))
                {
                    new_page();
                    p_private->p_rowcounter=0;
                }
            }
            c--;
        }

    }                                             //end footersections
    
    if (p_private->p_masterreport==NULL) if (!report_footer()->actual_string())stop_execution();
    
    
    (*outputstream()) << replace(p_private->p_end);
    if (p_private->p_masterreport==NULL)
    {
        if (p_private->p_pagefooter!=NULL) if (!p_private->p_pagefooter->actual_string())stop_execution();
    }
    
    if (p_private->p_print_full_pages_only)
    {
         write_bufferedpage();
         p_private->p_nostrstream=true;
    
    }
    
    if (p_private->p_masterreport==NULL)
    {
        *outputstream()<<p_private->p_pagedelimiter;
        *outputstream()<<replace(p_private->p_fileend);
        widget_specific_before_last_outstream_closed();
        if (p_private->p_filestream!=NULL)
        {
            p_private->p_filestream->close();
            delete p_private->p_filestream;
            p_private->p_filestream=NULL;
        }
        p_private->p_output=&cout;
    }

    if (!p_private->p_masterreport) enable_datasources(false);

//if (p_private->p_masterreport==NULL) datasource()->disable();
    if (datasource())
    {
    datasource()->set_temporarysorting(ds_sort);
    datasource()->set_use_temporarysorting(false);
    }
//datasource()->set_accessmode(accessmode);
    widget_specific_after_execute();
    //p_mode=modebuffer;
    p_private->p_while_executing=false;
// if (!p_private->p_masterreport)
// {
// cerr <<"encoding:"<<endl;
// HK_CHARSIZE w=0;
// while (w<p_private->p_encodingtab.max_nr())
// {
// cerr <<"w: "<<w<<" uni2local:"<<p_private->p_encodingtab.local(w)<<" local2uni: "<<p_private->p_encodingtab.unicode(w)<<" glyph:"<<p_private->p_encodingtab.glyphname(w)<<endl;
// ++w;
// }
// }


    return !execution_stopped();
}


void hk_report::new_outputstream(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::new_outputstream");
#endif
    if (datasource()==NULL) return;
    if (p_private->p_filestream!=NULL&&p_private->p_masterreport==NULL)
    {
        bool c=false;
        if (p_private->p_absolutepagenumber>1)
        {
            p_private->p_absolutepagenumber--;
            p_private->p_pagenumber--;
            c=true;
        }
        *p_private->p_filestream<<replace(p_private->p_fileend);
        delete p_private->p_filestream;
        p_private->p_filestream=NULL;
        if (c)
        {
            p_private->p_absolutepagenumber++;
            p_private->p_pagenumber++;
        }
    }

    if (p_private->p_masterreport!=NULL)
    {
        p_private->p_output=p_private->p_masterreport->outputstream();
        return;
    }
    hk_string p_pagenumberstring=longint2string(p_private->p_pagenumber);

    if (p_private->p_use_standardpath)
    {
        p_private->p_filename=database()->database_path()+"/output/"+(name().size()>0?name():p_private->p_defaultfilename)+p_private->p_fileextension;
        p_multiplefiles=false;
        p_private->p_outputfile=p_private->p_filename;
    }
    if (p_private->p_filename.size()>0)
    {
        if (p_multiplefiles)
        {
            if (p_private->p_pagenamecolumn==NULL)
                p_private->p_filename=p_private->p_originalfilename+p_pagenumberstring;
            else
                p_private->p_filename=p_private->p_pagenamecolumn->asstring_at(datasource()->row_position()+1,false);
            p_private->p_filename+=p_private->p_fileextension;

        }
        else if (!p_private->p_use_standardpath)p_private->p_filename=p_private->p_originalfilename+p_private->p_fileextension;
        p_private->p_filestream=new ofstream(p_private->p_filename.c_str());
        *p_private->p_filestream<<replace(p_private->p_filebegin);
        p_private->p_output=p_private->p_filestream;
        p_private->p_absolutepagenumber=1;
        if (p_private->p_output!=NULL) return;
    }

    p_private->p_output=&cout;

}


hk_string hk_report::outputfile(void)
{
    return p_private->p_outputfile;
}


void hk_report::new_page(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::new_page");
#endif
    if (!widget_specific_before_new_page())
    {
        stop_execution();
        return;
    }
    if (p_private->p_masterreport!=NULL)
    {
        p_private->p_masterreport->new_page();
        p_private->p_output=p_private->p_masterreport->outputstream();
	if (!action_on_new_page())stop_execution();
        widget_specific_after_new_page();
        return;
    }
    if (p_private->p_pagefooter!=NULL) if (!p_private->p_pagefooter->actual_string())stop_execution();
    *outputstream() << p_private->p_pagedelimiter;
    
    if (p_private->p_print_full_pages_only )
    {
           write_bufferedpage();
    }
    p_private->p_pagenumber++;
    p_private->p_absolutepagenumber++;
    set_pagenumber(p_private->p_pagenumber);
    p_private->p_rowcounter=0;
    if(p_multiplefiles) new_outputstream();
    if (!action_on_new_page())stop_execution();
    
    if (p_private->p_pageheader!=NULL ) 
        if (!p_private->p_pageheader->actual_string())stop_execution();
    
    if (!execution_stopped()&& datasource())
            if (!widget_specific_after_new_page()) stop_execution();
    if (!datasource()) stop_execution();
    p_private->p_cleanpage=true;
}



void hk_report::write_bufferedpage(void)
{
    if (!p_private->p_print_full_pages_only ) return;
    
    *p_private->p_output<<(p_private->p_fullpagereplacefunction?
    				p_private->p_fullpagereplacefunction(p_private->p_stringstream.str(),this)
    				:p_private->p_stringstream.str());
    p_private->p_stringstream.str("");

}

void hk_report::set_pagenumber(long f)
{
    p_private->p_pagenumber=f;
}


long hk_report::pagenumber(void)
{
    return p_private->p_pagenumber;
}


long hk_report::absolutepagenumber(void)
{
    return p_private->p_absolutepagenumber;
}


void hk_report::set_firstpagenumber(long f,bool registerchange)
{
    set_pagenumber(f);
    p_private->p_startpagenumber=f;
    has_changed(registerchange);
}


long int hk_report::firstpagenumber(void)
{
    return p_private->p_startpagenumber;

}


void hk_report::set_pagenamecolumn(const hk_string& c,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::set_pagenamecolumn");
#endif
    p_private->p_filename=p_private->p_pagename=c;
    if (p_private->p_pagename.size()==0) p_multiplefiles=false;
    else p_multiplefiles=true;
    has_changed(registerchange);
}


hk_string   hk_report::pagenamecolumn(void)
{
    return p_private->p_pagename;
}


bool  hk_report::datasource_disable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::datasource_disable");
#endif
    p_private->p_pagenamecolumn=NULL;
    vector<hk_reportsectionpair*>::iterator it=p_sectionpairs.begin();
    while (it!=p_sectionpairs.end())
    {
        (*it)->reset();
        it++;

    }

    return true;

}


bool  hk_report::before_columns_deleted(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::before_columns_deleted");
#endif
    vector<hk_reportsectionpair*>::iterator it=p_sectionpairs.begin();
    while (it!=p_sectionpairs.end())
    {
        (*it)->clear_countingfields();
        it++;

    }
    return datasource_disable();
}

unsigned long   hk_report::current_rowcount(void)
{
  return p_private->p_rowcounter;
}

void hk_report::rowcount(unsigned long c)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::rowcount");
#endif
   //cerr <<"rowcount:"<< c <<" counter:" <<p_private->p_rowcounter<<" periodic:"<<p_private->p_includeperiodic;
    if (p_private->p_masterreport!=NULL)
    {
        p_private->p_masterreport->rowcount(c);
        return;
    }
    p_private->p_rowcounter+=c;
    if ((p_private->p_includeperiodic>0)&&(p_private->p_rowcounter>(signed)p_private->p_includeperiodic))
    { //cerr <<"newpage "<<endl<<endl;
        *outputstream()<<p_private->p_includeperiodicstring;
        if (p_private->p_newpage_on_periodic)
        {
            if (!p_private->p_cleanpage)
               new_page();
            else p_private->p_cleanpage=false;
        }
        p_private->p_rowcounter=c;
        //cerr <<"neuer rowcounter="<<p_private->p_rowcounter<<endl;
    }
    else p_private->p_cleanpage=false;
 //cerr <<endl;
}


void hk_report::set_periodic(unsigned long when, const hk_string& what,bool newpage,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::set_periodic");
#endif
    p_private->p_includeperiodic=when;
    p_private->p_includeperiodicstring=what;
    p_private->p_newpage_on_periodic=newpage;
    has_changed(registerchange);

}


unsigned long   hk_report::periodic_when(void)
{
    return p_private->p_includeperiodic;
}


hk_string   hk_report::periodic_what(void)
{
    return p_private->p_includeperiodicstring;
}


bool        hk_report::periodic_newpage(void)
{
    return p_private->p_newpage_on_periodic;
}

/*
hk_string hk_report::replace(const hk_string& where)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::replace");
#endif
    hk_string p_buffer;
    p_buffer=hk_presentation::replace(where);
    p_buffer=replace_all("%FONT%",p_buffer,font().fontname());
    p_buffer=replace_all("%PSFONT%",p_buffer,font().psfontname());
//     if (p_private->p_embeddedfontsdefinition.size()>0)
       p_buffer=replace_all("%PSEMBEDDEDFONTS%",p_buffer,p_private->p_embeddedfontsdefinition);
//     p_buffer=replace_all("%PSEMBEDDEDFONTS%",p_buffer,p_private->p_embeddedfontsdefinition);
//       cerr <<"%PSEMBEDDEDFONTS% wird erseztzt durch #"<<p_private->p_embeddedfontsdefinition<<"#"<<endl;
    p_buffer=replace_all("%FONTSIZE%",p_buffer,longint2string(font().fontsize()));

    time_t     p_nowtime=time(NULL);
    time_t     p_timediff=p_nowtime - p_private->p_currenttime;

    p_buffer=replace_all("%PTIME%",p_buffer,longint2string(p_timediff));

    p_buffer=replace_all("%ABSOLUTEPAGENUMBER%",p_buffer,longint2string(absolutepagenumber()));
    p_buffer=replace_all("%PAGENUMBER%",p_buffer,longint2string(pagenumber()));

    p_buffer=replace_all("%BORDERLEFT%",p_buffer,sizetype()==relative?ulongint2string(relativ2horizontal(border_left())):ulongint2string(border_left()));
    p_buffer=replace_all("%BORDERRIGHT%",p_buffer,sizetype()==relative?ulongint2string(relativ2horizontal(border_right())):ulongint2string(border_right()));
    p_buffer=replace_all("%BORDERTOP%",p_buffer,sizetype()==relative?ulongint2string(relativ2vertical(border_top())):ulongint2string(border_top()));
    p_buffer=replace_all("%BORDERBOTTOM%",p_buffer,sizetype()==relative?ulongint2string(relativ2vertical(border_bottom())):ulongint2string(border_bottom()));
    p_buffer=replace_all("%RELBORDERLEFT%",p_buffer,sizetype()==relative?ulongint2string(border_left()):ulongint2string(horizontal2relativ(border_left())));
    p_buffer=replace_all("%RELBORDERRIGHT%",p_buffer,sizetype()==relative?ulongint2string(border_right()):ulongint2string(horizontal2relativ(border_right())));
    p_buffer=replace_all("%RELBORDERTOP%",p_buffer,sizetype()==relative?ulongint2string(border_top()):ulongint2string(vertical2relativ(border_top())));
    p_buffer=replace_all("%RELBORDERBOTTOM%",p_buffer,sizetype()==relative?ulongint2string(border_bottom()):ulongint2string(vertical2relativ(border_bottom())));

//Postscriptstuff
    hk_string boundingbox="%%BoundingBox: 0 0 ";

    boundingbox+=longint2string(zmm2ps(p_private->p_orientation==portrait?designwidth():designheight()))+" ";
    boundingbox+=longint2string(zmm2ps(p_private->p_orientation==portrait?designheight():designwidth()));
//  cout <<"BOUNDINGBOX= "<<boundingbox<<endl;

    p_buffer=replace_all("%BOUNDINGBOX%",p_buffer,boundingbox);

    hk_string r;
    list<hk_string>::iterator it=p_private->p_neededfonts.begin();
    while (it!=p_private->p_neededfonts.end())
    {
        r+="%%+ font "+(*it)+"\n";
        it++;
    }
    p_buffer=replace_all("%PSNEEDEDFONTS%",p_buffer,r);
    
    it=p_private->p_suppliedfontspsnames.begin();
    r="";
    while (it!=p_private->p_suppliedfontspsnames.end())
    {
        r+="%%+ font "+(*it)+"\n";
        it++;
    }
    p_buffer=replace_all("%PSSUPPLIEDFONTS%",p_buffer,r);
    
    it=p_private->p_usedpsfonts.begin();
    r="";
    while (it!=p_private->p_usedpsfonts.end())
    {
        r+="%%+ font "+(*it)+"\n";
        it++;
    }
    p_buffer=replace_all("%PSUSEDFONTS%",p_buffer,r);
    return p_buffer;
}

*/




hk_string hk_report::tag_value(int tagnumber, bool& ok)
{
ok=true;
    if (tagnumber==p_private->tag_FONT) return font().fontname();
    if (tagnumber==p_private->tag_PSFONT) return font().psfontname();
    if (tagnumber==p_private->tag_PSEMBEDDEDFONTS) return   p_private->p_embeddedfontsdefinition;
    if (tagnumber==p_private->tag_FONTSIZE) return longint2string(font().fontsize());

    if (tagnumber==p_private->tag_PTIME)
    {
    time_t     p_nowtime=time(NULL);
    time_t     p_timediff=p_nowtime - p_private->p_currenttime;

    return longint2string(p_timediff);
    }
    if (tagnumber==p_private->tag_ABSOLUTEPAGENUMBER) return longint2string(absolutepagenumber());
    if (tagnumber==p_private->tag_PAGENUMBER) return longint2string(pagenumber());

    if (tagnumber==p_private->tag_BORDERLEFT) return sizetype()==relative?ulongint2string(relativ2horizontal(border_left())):ulongint2string(border_left());
    if (tagnumber==p_private->tag_BORDERRIGHT) return    sizetype()==relative?ulongint2string(relativ2horizontal(border_right())):ulongint2string(border_right());
    if (tagnumber==p_private->tag_BORDERTOP)    return sizetype()==relative?ulongint2string(relativ2vertical(border_top())):ulongint2string(border_top());
    if (tagnumber==p_private->tag_BORDERBOTTOM) return    sizetype()==relative?ulongint2string(relativ2vertical(border_bottom())):ulongint2string(border_bottom());
    if (tagnumber==p_private->tag_RELBORDERLEFT) return sizetype()==relative?ulongint2string(border_left()):ulongint2string(horizontal2relativ(border_left()));
    if (tagnumber==p_private->tag_RELBORDERRIGHT) return (sizetype()==relative?ulongint2string(border_right()):ulongint2string(horizontal2relativ(border_right())));
    if (tagnumber==p_private->tag_RELBORDERTOP) return (sizetype()==relative?ulongint2string(border_top()):ulongint2string(vertical2relativ(border_top())));
    if (tagnumber==p_private->tag_RELBORDERBOTTOM) return (sizetype()==relative?ulongint2string(border_bottom()):ulongint2string(vertical2relativ(border_bottom())));

//Postscriptstuff
    if (tagnumber==p_private->tag_BOUNDINGBOX)
    {
    hk_string boundingbox="%%BoundingBox: 0 0 ";

    boundingbox+=longint2string(zmm2ps(p_private->p_orientation==portrait?designwidth():designheight()))+" ";
    boundingbox+=longint2string(zmm2ps(p_private->p_orientation==portrait?designheight():designwidth()));
//  cout <<"BOUNDINGBOX= "<<boundingbox<<endl;

    return boundingbox;
    }
    
    if (!p_private->p_masterreport)
    {
    list<hk_string>::iterator it=p_private->p_neededfonts.begin();
     hk_string r;
    if (tagnumber==p_private->tag_PSNEEDEDFONTS)
    {
     while (it!=p_private->p_neededfonts.end())
     {
        r+="%%+ font "+(*it)+"\n";
        it++;
     }
     return r;
     }
    
    if (tagnumber==p_private->tag_PSSUPPLIEDFONTS)
    {it=p_private->p_suppliedfontspsnames.begin();
    r="";
    while (it!=p_private->p_suppliedfontspsnames.end())
    {
        r+="%%+ font "+(*it)+"\n";
        it++;
    }
    return r;
    }
    if (tagnumber==p_private->tag_PSUSEDFONTS)
    {
    it=p_private->p_usedpsfonts.begin();
    r="";
    while (it!=p_private->p_usedpsfonts.end())
    {
        r+="%%+ font "+(*it)+"\n";
        it++;
    }
    return r;
    }
    }
return hk_dsvisible::tag_value(tagnumber,ok);
}



hk_font hk_report::font(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::font");
#endif
    hk_font f;
    
    if (hk_presentation::font().fontname().size()==0) f= hk_font(hk_font::defaultfontname(),hk_font::defaultfontsize());
    else f= hk_presentation::font();
    
    f.set_encodingtab(encodingtab());
    return f;
}




void  hk_report::set_filebegin(const hk_string& b,bool registerchange)
{
    p_private->p_filebegin=b;
    has_changed(registerchange);

}


hk_string  hk_report::filebegin(void)
{
    return p_private->p_filebegin;
}


void    hk_report::set_fileend(const hk_string& e,bool registerchange)
{
    p_private->p_fileend=e;
    has_changed(registerchange);
}


hk_string hk_report::fileend(void)
{
    return p_private->p_fileend;

}


unsigned long hk_report::rownumber(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::rownumber");
#endif
    if (datasource()!=NULL) return datasource()->row_position();
    else return 0;
}


bool hk_report::save_report(const hk_string& n,bool ask)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::save_report");
#endif
    if (database()==NULL)
    {
        show_warningmessage(hk_translate("Form error: No database defined!"));
        return false;

    }
    if (n.size()>0) set_name(n);

    reset_has_changed();
    if (name().size()==0) if (!ask_name())return false;
    hk_database::enum_storage storage=hk_database::st_overwrite;
    if (p_private->p_loadreportname==name()) storage=hk_database::st_storechanges;
     if (database()->storagemode(ft_report)==hk_database::local)
    {
      ofstream* p_save=database()->savestream(storage,name(),ft_report,ask,true,false);
      if (p_save==NULL) return false;
      savedata(*p_save);
      p_save->close();
      delete p_save;
    }
    else
    { // store central
       STRINGSTREAM*  p_save;
      p_save=database()->savestringstream(ft_report);
      if (p_save==NULL) return false;
      savedata(*p_save);
      hk_string s=p_save->str();
      database()->save(storage,s,name(),ft_report,ask);
      delete p_save;
    }
    p_private->p_loadreportname=name();
    reset_has_changed();
    return true;

}


void  hk_report::savedata(ostream& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::savedata");
#endif

    start_mastertag( s,"HK_REPORT");
    start_mastertag(s,"REPORTDEFINITION");
    set_tagvalue(s,"HK_REPORTVERSION",(hk_string)VERSION);
    hk_presentation::savedata(s);
    set_tagvalue(s,"REPORTTYPE",p_private->p_reporttype);
    set_tagvalue(s,"FILENAME",p_private->p_originalfilename);
    set_tagvalue(s,"STARTPAGENUMBER",p_private->p_startpagenumber);
    set_tagvalue(s,"MULTIPLEFILES",p_multiplefiles);
    set_tagvalue(s,"PRINTFULLPAGESONLY",p_private->p_print_full_pages_only);
    set_tagvalue(s,"PAGENAME",p_private->p_pagename);
    set_tagvalue(s,"BORDERLEFT",(long unsigned int) p_private->p_borderleft);
    set_tagvalue(s,"BORDERRIGHT",(long unsigned int) p_private->p_borderright);
    set_tagvalue(s,"BORDERTOP",(long unsigned int) p_private->p_bordertop);
    set_tagvalue(s,"BORDERBOTTOM",(long unsigned int) p_private->p_borderbottom);
    set_tagvalue(s,"TOPLINE",p_private->p_top);
    set_tagvalue(s,"LEFTLINE",p_private->p_left);
    set_tagvalue(s,"RIGHTLINE",p_private->p_right);
    set_tagvalue(s,"BOTTOMLINE",p_private->p_bottom);
    set_tagvalue(s,"LORULINE",p_private->p_loru);
    set_tagvalue(s,"LUROLINE",p_private->p_luro);
    set_tagvalue(s,"DEFAULTPRECISION",(long)p_private->p_default_precision);
    set_tagvalue(s,"DEFAULTUSETHOUSANDSSEPARATOR",p_private->p_default_use_thousandsseparator);
    set_tagvalue(s,"ONPRINTNEWPAGE_ACTION",p_private->p_on_new_page_action);
    hk_string p_buffer=(p_private->p_orientation==hk_report::landscape?"LANDSCAPE":"PORTRAIT");
    set_tagvalue(s,"ORIENTATION",p_buffer);
    hk_string p_formattyp;
    switch (p_private->p_pageformat)
    {
        case hk_report::A0 : p_formattyp="A0";break;
        case hk_report::A1 : p_formattyp="A1";break;
        case hk_report::A2 : p_formattyp="A2";break;
        case hk_report::A3 : p_formattyp="A3";break;
        case hk_report::A4 : p_formattyp="A4";break;
        case hk_report::A5 : p_formattyp="A5";break;
        case hk_report::A6 : p_formattyp="A6";break;
        case hk_report::A7 : p_formattyp="A7";break;
        case hk_report::A8 : p_formattyp="A8";break;
        case hk_report::A9 : p_formattyp="A9";break;
        case hk_report::A10 : p_formattyp="A10";break;
        case hk_report::letter : p_formattyp="LETTER";break;
        case hk_report::legal : p_formattyp="LEGAL";break;
        case hk_report::executive : p_formattyp="EXECUTIVE";break;
        case hk_report::tabloid : p_formattyp="TABLOID";break;
        default: p_formattyp="USERDEFINED";
    }

    set_tagvalue(s,"PAGEFORMAT",p_formattyp);

    start_mastertag(s,"PAGEHEADSECTION");
    if (p_private->p_pageheader!=NULL) p_private->p_pageheader->savedata(s);
    end_mastertag(s,"PAGEHEADSECTION");

    start_mastertag(s,"PAGEFOOTSECTION");
    if (p_private->p_pagefooter!=NULL) p_private->p_pagefooter->savedata(s);
    end_mastertag(s,"PAGEFOOTSECTION");

    start_mastertag(s,"REPORTHEADERSECTION");
    if (p_private->p_reportheader!=NULL) p_private->p_reportheader->savedata(s);
    end_mastertag(s,"REPORTHEADERSECTION");

    start_mastertag(s,"REPORTFOOTERSECTION");
    if (p_private->p_reportfooter!=NULL) p_private->p_reportfooter->savedata(s);
    end_mastertag(s,"REPORTFOOTERSECTION");

    start_mastertag(s,"REPORTDATASECTION");
    if (p_private->p_datasection!=NULL) p_private->p_datasection->savedata(s);
    end_mastertag(s,"REPORTDATASECTION");

    if (p_private->p_reporttype=="Userdefined")
    {
        set_tagvalue(s,"REPORTBEGIN",p_private->p_begin);
        set_tagvalue(s,"REPORTEND",p_private->p_end);
        set_tagvalue(s,"FILEBEGIN",p_private->p_filebegin);
        set_tagvalue(s,"FILEEND",p_private->p_fileend);
        set_tagvalue(s,"FILEEXTENSION",p_private->p_fileextension);
        set_tagvalue(s,"PAGEDELIMITER",p_private->p_pagedelimiter);
        set_tagvalue(s,"INCLUDEPERIODICSTRING",p_private->p_includeperiodicstring);
        set_tagvalue(s,"INCLUDEPERIODIC",p_private->p_includeperiodic);
        set_tagvalue(s,"NEWPAGEPERIODIC",p_private->p_newpage_on_periodic);
        set_tagvalue(s,"DEFAULTDATA",p_default_data);
        set_tagvalue(s,"DEFAULTBEFORECOLUMN",p_default_beforedata);
        set_tagvalue(s,"DEFAULTAFTERCOLUMN",p_default_afterdata);
        set_tagvalue(s,"RECODEFUNCTION",p_private->p_recodefunctionstring);
        set_tagvalue(s,"FULLPAGEREPLACEFUNCTION",p_private->p_fullpagereplacefunctionstring);
        set_tagvalue(s,"DEFAULTDATACONFIGUREFUNCTION",p_default_dataconfigurefunction);
        set_tagvalue(s,"DEFAULTSECTIONCOUNTFUNCTION",p_private->p_defaultsectioncountfunction);
        set_tagvalue(s,"REPORTCONFIGUREFUNCTION",p_private->p_configurefunctionstring);
        set_tagvalue(s,"PRINTFULLPAGEONLY",p_private->p_print_full_pages_only);
    }

    end_mastertag(s,"REPORTDEFINITION");

    start_mastertag(s,"SECTIONPAIRS");
    int size = p_sectionpairs.size();
    if (size>0)
    {
        size--;
        while (size>=0)
        {
            p_sectionpairs[size]->savedata(s,(p_private->p_reporttype=="Userdefined"));

            size--;
        }

    }
    end_mastertag(s,"SECTIONPAIRS");

    end_mastertag(s,"HK_REPORT");
}


bool hk_report::load_report(const hk_string& n)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::load_report");
#endif
    if (database()==NULL)
    {
        show_warningmessage(hk_translate("Report error: No database defined!"));
        return false;

    }
    if (has_changed())
    {
        save_report();
        reset_has_changed();
    }
    p_private->p_while_load_report=true;
    init_report();
    if (n.size()>0) set_name(n);
    enum_mode modebuffer=mode();
    hk_dsmodevisible::set_mode(designmode);
    if (name().size()==0) if (!ask_name())
    				{
    				 p_private->p_while_load_report=false;
				 return false;
    				}
    xmlNodePtr res= database()->xmlload(name(),ft_report);
    p_private->p_loadreportname=n;
    if (!res)
    {
        show_warningmessage(hk_translate("No such report!"));
            p_private->p_while_load_report=false;
        return false;
    }
//     if ( res.find("encoding=\"UTF-8\"")<res.size() ) res=u2l(res);
    loaddata(res);
    hk_dsmodevisible::set_mode(modebuffer);
    widget_specific_after_loadreport();
    reset_has_changed();

#ifdef HK_DEBUG
    hkdebug("hk_report::load_form ENDE");
#endif
p_private->p_while_load_report=false;
    return true;
}

bool hk_report::while_load_report(void) const
{
   return p_private->p_while_load_report;
}

bool hk_report::while_loading(void)
{
  return while_load_report();
}

void  hk_report::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::loaddata");
#endif

    xmlNodePtr sbuffer;
    long unsigned int ibuffer;
    bool bbuffer;
    hk_string xxx;
    sbuffer=get_tagvalue(definition,"PRESENTATION",xxx,1,mastertag);
    hk_presentation::loaddata(sbuffer);

    hk_string reptype;

    get_tagvalue(definition,"REPORTTYPE",reptype);
    xmlNodePtr r=get_tagvalue(definition,"REPORTDEFINITION");
//	hk_dsvisible::loaddata(r);
    if (get_tagvalue(r,"DEFAULTPRECISION",ibuffer))p_private->p_default_precision=ibuffer;
    if (get_tagvalue(r,"DEFAULTUSETHOUSANDSSEPARATOR",bbuffer))p_private->p_default_use_thousandsseparator=bbuffer;

    if (get_tagvalue(r,"FILENAME",xxx))set_filename(xxx);
    if (get_tagvalue(r,"STARTPAGENUMBER",ibuffer))p_private->p_startpagenumber=ibuffer;
    if (get_tagvalue(r,"MULTIPLEFILES",bbuffer))p_multiplefiles=bbuffer;
    get_tagvalue(r,"PRINTFULLPAGESONLY",p_private->p_print_full_pages_only);
    if (get_tagvalue(r,"PAGENAME",xxx))p_private->p_pagename=xxx;
    if (get_tagvalue(r,"BORDERLEFT",ibuffer))p_private->p_borderleft=ibuffer;
    if (get_tagvalue(r,"BORDERRIGHT",ibuffer)) p_private->p_borderright=ibuffer;
    if (get_tagvalue(r,"BORDERTOP",ibuffer)) p_private->p_bordertop=ibuffer;
    if (get_tagvalue(r,"BORDERBOTTOM",ibuffer)) p_private->p_borderbottom=ibuffer;
    if (get_tagvalue(r,"TOPLINE",bbuffer))p_private->p_top=bbuffer;
    if (get_tagvalue(r,"LEFTLINE",bbuffer))p_private->p_left=bbuffer;
    if (get_tagvalue(r,"RIGHTLINE",bbuffer))p_private->p_right=bbuffer;
    if (get_tagvalue(r,"BOTTOMLINE",bbuffer))p_private->p_bottom=bbuffer;
    if (get_tagvalue(r,"LORULINE",bbuffer))p_private->p_loru=bbuffer;
    if (get_tagvalue(r,"LUROLINE",bbuffer))p_private->p_luro=bbuffer;
    get_tagvalue(r,"ONPRINTNEWPAGE_ACTION",p_private->p_on_new_page_action);
    if (get_tagvalue(r,"ORIENTATION",xxx))
    {
        if (xxx=="LANDSCAPE") set_orientation(hk_report::landscape);
        else        set_orientation(hk_report::portrait);
    }

    pageformattype p_formattyp;
    get_tagvalue(r,"PAGEFORMAT",xxx);
    if(xxx=="A0") p_formattyp=hk_report::A0;else
        if(xxx=="A1") p_formattyp=hk_report::A1;else
            if(xxx=="A2") p_formattyp=hk_report::A2;else
                if(xxx=="A3") p_formattyp=hk_report::A3;else
                    if(xxx=="A4") p_formattyp=hk_report::A4;else
                        if(xxx=="A5") p_formattyp=hk_report::A5;else
                            if(xxx=="A6") p_formattyp=hk_report::A6;else
                                if(xxx=="A7") p_formattyp=hk_report::A7;else
                                    if(xxx=="A8") p_formattyp=hk_report::A8;else
                                        if(xxx=="A9") p_formattyp=hk_report::A9;else
                                            if(xxx=="A10") p_formattyp=hk_report::A10;else
                                                if(xxx=="LETTER") p_formattyp=hk_report::letter;else
                                                    if(xxx=="LEGAL") p_formattyp=hk_report::legal;else
                                                        if(xxx=="EXECUTIVE") p_formattyp=hk_report::executive;else
                                                            if(xxx=="TABLOID") p_formattyp=hk_report::tabloid;else
                                                                p_formattyp=hk_report::userdefined;
    set_pageformat(p_formattyp);
     xmlNodePtr s=NULL;
    if ((s=get_tagvalue(r,"PAGEHEADSECTION",xxx,1,mastertag)))
    {
        if (p_private->p_pageheader!=NULL) p_private->p_pageheader->loaddata(s);
    }
    if ((s=get_tagvalue(r,"PAGEFOOTSECTION",xxx,1,mastertag)))
    {
        if (p_private->p_pagefooter!=NULL) p_private->p_pagefooter->loaddata(s);
    }
    if ((s=get_tagvalue(r,"REPORTHEADERSECTION",xxx,1,mastertag)))
    {
        if (p_private->p_reportheader!=NULL) p_private->p_reportheader->loaddata(s);
    }
    if ((s=get_tagvalue(r,"REPORTFOOTERSECTION",xxx,1,mastertag)))
    {
        if (p_private->p_reportfooter!=NULL) p_private->p_reportfooter->loaddata(s);
    }

    if ((s=get_tagvalue(r,"REPORTDATASECTION",xxx,1,mastertag)))
    {
        if (p_private->p_datasection!=NULL) p_private->p_datasection->loaddata(s);
    }

    xmlNodePtr sectionbuffer;
    if ((sectionbuffer=get_tagvalue(definition,"SECTIONPAIRS")))
    {
        int counter=1;
        while ((sbuffer=get_tagvalue(sectionbuffer,"HK_REPORTSECTIONPAIR",xxx,counter,mastertag)))
        {
            hk_reportsectionpair* newsection=new_sectionpair();
            if (newsection!=NULL)
            {
                newsection->loaddata(sbuffer,(reptype=="Userdefined"));
            }
            counter++;
        }
    }

    if (reptype=="Userdefined")
    {
        get_tagvalue(r,"REPORTBEGIN",p_private->p_begin);
        get_tagvalue(r,"REPORTEND",p_private->p_end);
        get_tagvalue(r,"FILEBEGIN",p_private->p_filebegin);
        get_tagvalue(r,"FILEEND",p_private->p_fileend);
        get_tagvalue(r,"FILEEXTENSION",p_private->p_fileextension);
        get_tagvalue(r,"PAGEDELIMITER",p_private->p_pagedelimiter);
        get_tagvalue(r,"INCLUDEPERIODICSTRING",p_private->p_includeperiodicstring);
        get_tagvalue(r,"INCLUDEPERIODIC",p_private->p_includeperiodic);
        get_tagvalue(r,"NEWPAGEPERIODIC",p_private->p_newpage_on_periodic);
        get_tagvalue(r,"DEFAULTDATA",p_default_data);
        get_tagvalue(r,"DEFAULTBEFORECOLUMN",p_default_beforedata);
        get_tagvalue(r,"DEFAULTAFTERCOLUMN",p_default_afterdata);
        get_tagvalue(r,"DEFAULTDATACONFIGUREFUNCTION",p_default_dataconfigurefunction);
        if (get_tagvalue(r,"RECODEFUNCTION",xxx))
            set_recodefunction(xxx);
        if (get_tagvalue(r,"FULLPAGEREPLACEFUNCTION",xxx))
            set_fullpagereplacefunction(xxx);

        get_tagvalue(r,"DEFAULTSECTIONCOUNTFUNCTION",p_private->p_defaultsectioncountfunction);
        if (get_tagvalue(r,"REPORTCONFIGUREFUNCTION",xxx))
            set_reportconfigurefunction(xxx,false);
        get_tagvalue(r,"PRINTFULLPAGEONLY",p_private->p_print_full_pages_only);
}

    set_reporttype(reptype);

    configure_page();
}


bool hk_report::set_presentationdatasource(long n, bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::set_presentationdatasource");
#endif
    bool b=hk_presentation::set_presentationdatasource(n,registerchange);
    vector<hk_reportsectionpair*>::iterator it=p_sectionpairs.begin();
    while (it!= p_sectionpairs.end())
    {
        (*it)->set_presentationdatasource(n,registerchange);
        it++;
    }
    if (p_private->p_pageheader!=NULL)
    {
        p_private->p_pageheader->set_presentationdatasource(n,registerchange);
    }
    if (p_private->p_pagefooter!=NULL)
    {
        p_private->p_pagefooter->set_presentationdatasource(n,registerchange);
    }
    if (p_private->p_reportheader!=NULL)
    {
        p_private->p_reportheader->set_presentationdatasource(n,registerchange);
    }
    if (p_private->p_reportfooter!=NULL)
    {
        p_private->p_reportfooter->set_presentationdatasource(n,registerchange);
    }
    if (p_private->p_datasection!=NULL)
    {
        p_private->p_datasection->set_presentationdatasource(n,registerchange);
    }

    return b;
}


void hk_report::remove_sectionpair(hk_reportsectionpair* s,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::remove_sectionpair");
#endif
    if (s==NULL)return;
    vector<hk_reportsectionpair*>::iterator i=find(p_sectionpairs.begin(),p_sectionpairs.end(),s);
    p_sectionpairs.erase(i);
    has_changed(registerchange);
}


void hk_report::remove_section(hk_reportsection* s)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::remove_section");
#endif
    if (s==NULL)return;

    if (s==p_private->p_pageheader) p_private->p_pageheader=NULL;
    else
    if (s==p_private->p_pagefooter) p_private->p_pagefooter=NULL;
        else
        if (s==p_private->p_reportheader)p_private->p_reportheader=NULL;
            else
            if (s==p_private->p_reportfooter)p_private->p_reportfooter=NULL;
                else
                if (s==p_private->p_datasection)p_private->p_datasection=NULL;
}


void hk_report::set_default_reportdataconfigurefunction(const hk_string& f,bool registerchange)
{
    p_default_dataconfigurefunction=f;
    has_changed(registerchange);
}


hk_string hk_report::default_reportdataconfigurefunction(void)
{
    return p_default_dataconfigurefunction;
}


void hk_report::set_default_reportsectionbegin(const hk_string& e,bool registerchange)
{
    p_private->p_defaultsectionbegin=e;
    has_changed(registerchange);
}


void hk_report::set_default_reportsectionend(const hk_string& e,bool registerchange)
{
    p_private->p_defaultsectionend=e;
    has_changed(registerchange);
}


void hk_report::set_default_reportsectioncountfunction(const hk_string& f,bool registerchange)
{
    p_private->p_defaultsectioncountfunction=f;
    has_changed(registerchange);
}


hk_string hk_report::default_reportsectionbegin(void)
{
    return p_private->p_defaultsectionbegin;
}


hk_string hk_report::default_reportsectionend(void)
{
    return p_private->p_defaultsectionend;
}


hk_string hk_report::default_reportsectioncountfunction(void)
{
    return p_private->p_defaultsectioncountfunction;
}


void hk_report::set_default_reportsectionbetweendata(const hk_string& b,bool registerchange)
{
    p_private->p_defaultsectionbetweendata=b;
    has_changed(registerchange);

}


hk_string hk_report::default_reportsectionbetweendata(void)
{
    return p_private->p_defaultsectionbetweendata;
}


hk_string   hk_report::default_reportdata(void)
{
    return p_default_data;
}


void        hk_report::set_default_reportdata(const hk_string& v,bool registerchange)
{
    p_default_data=v;
    has_changed(registerchange);
}


hk_string   hk_report::default_beforereportdata(void)
{
    return p_default_beforedata;
}


void        hk_report::set_default_beforereportdata(const hk_string& v,bool registerchange)
{
    p_default_beforedata=v;
    has_changed(registerchange);
}


hk_string   hk_report::default_afterreportdata(void)
{
    return p_default_afterdata;
}


void        hk_report::set_default_afterreportdata(const hk_string& v,bool registerchange)
{
    p_default_afterdata=v;
    has_changed(registerchange);
}


void hk_report::set_borders(unsigned int left,unsigned  int right, unsigned int top, unsigned int bottom,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::set_borders");
#endif
    const unsigned int MAXTOTALBORDER=8000;
    const unsigned int MAXSINGLEBORDER=7000;
    if (sizetype()==relative&&(left+right>MAXTOTALBORDER||top+bottom>MAXTOTALBORDER))
    {
        show_warningmessage(hk_translate("Borders too large! At least 20% have to be left for the data section"));
        return;
    }
    if (left>MAXSINGLEBORDER
        ||right>MAXSINGLEBORDER
        ||top>MAXSINGLEBORDER
        ||bottom>MAXSINGLEBORDER
        )
    {
        show_warningmessage(hk_translate("Borders too large!"));
        return;

    }
    p_private->p_borderleft=left;
    p_private->p_borderright=right;
    p_private->p_borderbottom=bottom;
    p_private->p_bordertop=top;
    configure_page();
    has_changed(registerchange);
    widget_specific_presentationresize();
}


void    hk_report::set_topline(bool l,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::set_topline");
#endif
    p_private->p_top=l;
    configure_page();
    has_changed(registerchange);
}


void    hk_report::set_bottomline(bool l,bool registerchange)
{
    p_private->p_bottom=l;
    configure_page();
    has_changed(registerchange);
}


void    hk_report::set_leftline(bool l,bool registerchange)
{
    p_private->p_left=l;
    configure_page();
    has_changed(registerchange);
}


void    hk_report::set_rightline(bool l,bool registerchange)
{
    p_private->p_right=l;
    configure_page();
    has_changed(registerchange);
}


void    hk_report::set_diagonalluro(bool l,bool registerchange)
{
    p_private->p_luro=l;
    configure_page();
    has_changed(registerchange);

}


void    hk_report::set_diagonalloru(bool l,bool registerchange)
{
    p_private->p_loru=l;
    configure_page();
    has_changed(registerchange);
}


void    hk_report::set_frame(bool l,bool registerchange)
{
    p_private->p_top=l;
    p_private->p_bottom=l;
    p_private->p_left=l;
    p_private->p_right=l;
    configure_page();
    has_changed(registerchange);
}


void hk_report::set_orientation(orientationtype o,bool registerchange)
{
    p_private->p_orientation=o;
    set_pageformat(p_private->p_pageformat,registerchange);
}


void hk_report::set_pageformat(pageformattype t,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::set_pageformat(pageformattype)");
#endif
    unsigned int p_pagewidth=formatwidth(p_private->p_pageformat);
    unsigned int p_pageheight=formatheight(p_private->p_pageformat);
    p_private->p_pageformat=t;
    if (p_private->p_orientation==landscape)
    {
        unsigned int b=p_pagewidth;
        p_pagewidth=p_pageheight;
        p_pageheight=b;
    }

    set_designsize(p_pagewidth,p_pageheight,registerchange);
    configure_page();

}


void hk_report::set_pageformat(unsigned int w,unsigned  int h,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::set_pageformat(unsigned int,unsigned int)");
#endif
    p_private->p_pageformat=userdefined;
    set_designsize(w,h,registerchange);
    configure_page();
    has_changed(registerchange);

}


void hk_report::configure_page(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::configure_page");
#endif
    if (p_private->p_configurefunction!=NULL)
    {
        p_private->p_configurefunction(this,0);
    }

}


unsigned int    hk_report::formatwidth(pageformattype t)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::formatwidth");
#endif

    unsigned int res;
    switch(t)
    {
        case A0  : res=8410;
        break;
        case A1  : res=5940;
        break;
        case A2  : res=4200;
        break;
        case A3  : res=2970;
        break;
        case A4  : res=2100;
        break;
        case A5  : res=1480;
        break;
        case A6  : res=1050;
        break;
        case A7  : res=740;
        break;
        case A8  : res=520;
        break;
        case A9  : res=370;
        break;
        case A10 : res=260;
        break;
        case letter :res=2160;
        break;
        case legal :res=2160;
        break;
        case executive :res=1900;
        break;
        case tabloid :res=2790;
        break;
        case userdefined :res=designwidth();
        break;

        default  :res=2100;
    }
    return res;
}


unsigned int    hk_report::formatheight(pageformattype t)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::formatheight");
#endif

    unsigned int res;
    switch(t)
    {
        case A0  : res=11890;
        break;
        case A1  : res=8410;
        break;
        case A2  : res=5940;
        break;
        case A3  : res=4200;
        break;
        case A4  : res=2970;
        break;
        case A5  : res=2100;
        break;
        case A6  : res=1480;
        break;
        case A7  : res=1050;
        break;
        case A8  : res=740;
        break;
        case A9  : res=520;
        break;
        case A10 : res=370;
        break;
        case letter :res=2790;
        break;
        case legal :res=3560;
        break;
        case executive :res=2540;
        break;
        case tabloid :res=4320;
        break;
        case userdefined :res=designheight();
        break;
        default  :res=2970;
    }
    return res;
}


unsigned int hk_report::border_left(void)
{
    if (p_private->p_masterreport) return p_private->p_masterreport->border_left();
    return p_private->p_borderleft;
}


unsigned int hk_report::border_right(void)
{
    if (p_private->p_masterreport) return p_private->p_masterreport->border_right();
    return p_private->p_borderright;
}


unsigned int hk_report::border_top(void)
{
    if (p_private->p_masterreport) return p_private->p_masterreport->border_top();
    return p_private->p_bordertop;
}


unsigned int hk_report::border_bottom(void)
{
    if (p_private->p_masterreport) return p_private->p_masterreport->border_bottom();
    return p_private->p_borderbottom;
}


bool    hk_report::leftline(void)
{
    return p_private->p_left;
}


bool    hk_report::rightline(void)
{
    return p_private->p_right;
}


bool    hk_report::topline(void)
{
    return p_private->p_top;
}


bool    hk_report::bottomline(void)
{
    return p_private->p_bottom;
}


bool    hk_report::diagonalluro(void)
{
    return p_private->p_luro;
}


bool    hk_report::diagonalloru(void)
{
    return p_private->p_loru;
}


hk_report::orientationtype hk_report::orientation(void)
{
    if (p_private->p_masterreport) return p_private->p_masterreport->orientation();
    else return p_private->p_orientation;
}


hk_report::pageformattype hk_report::pageformat(void)
{
    if (p_private->p_masterreport) return p_private->p_masterreport->pageformat();
    else return p_private->p_pageformat;
}


void    hk_report::set_reportconfigurefunction(const hk_string& f,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::set_reportconfigurefunction");
#endif
    if (f==p_private->p_configurefunctionstring)return;

    reportconfigurelisttype::iterator it=p_reportconfigurefunctions.find(f);
    if (it==p_reportconfigurefunctions.end())
    {

        show_warningmessage(hk_translate("Reportconfigurefunction not found"));
        p_private->p_configurefunction=NULL;
        p_private->p_configurefunctionstring="None";
        return;
    }

    report_configurefunction* rf=*it->second;
    p_private->p_configurefunction=rf;
    p_private->p_configurefunctionstring=f;
    has_changed(registerchange);
}


hk_string hk_report::reporttype(void)
{
    return (p_private->p_reporttype.size()==0?"Userdefined":p_private->p_reporttype);
}


void    hk_report::set_reporttype(const hk_string&t,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::set_reporttype");
#endif

    if (t==p_private->p_reporttype)return;

    reporttypelisttype::iterator it=p_reporttypefunctions.find(t);
    if (it==p_reporttypefunctions.end())
    {

        show_warningmessage(hk_translate("Reporttype not found"));
        return;
    }
    reporttypefunction* f=*it->second;
    if (f!=NULL) f(this,registerchange);

    p_private->p_reporttype=t;
    has_changed(registerchange);

}


void hk_report::add_reporttype(const hk_string& name,reporttypefunction* f)
{
    if (name.size()==0) return;
    typedef reporttypelisttype::value_type reportvalue;
    p_reporttypefunctions.insert(reportvalue(name,f));
    p_reporttypelist.insert(p_reporttypelist.end(),name);
}


list<hk_string>* hk_report::reporttypelist(void)
{
    return &p_reporttypelist;
}


void hk_report::add_recodetype(const hk_string& name,recodefunctiontype* f)
{
    if (name.size()==0) return;
    typedef recodefunctionlisttype::value_type recodevalue;
    p_recodefunctions.insert(recodevalue(name,f));
    p_recodelist.insert(p_recodelist.end(),name);

}


list<hk_string>* hk_report::recodelist(void)
{
    return &p_recodelist;
}


hk_string hk_report::recodefunctionstring(void)
{
    return p_private->p_recodefunctionstring;
}


recodefunctiontype* hk_report::recodefunction(void)
{
    return p_private->p_recodefunction;

}


void    hk_report::set_recodefunction(const hk_string& r,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::set_recodefunction");
#endif

    if (r==p_private->p_recodefunctionstring)return;

    recodefunctionlisttype::iterator it=p_recodefunctions.find(r);
    if (it==p_recodefunctions.end())
    {

        show_warningmessage(hk_translate("Recodetype not found"));
        p_private->p_recodefunction=NULL;
        p_private->p_recodefunctionstring="None";
        return;
    }

    recodefunctiontype* rf=*it->second;
    p_private->p_recodefunction=rf;
    p_private->p_recodefunctionstring=r;

    has_changed(registerchange);

}




void        hk_report::set_fullpagereplacefunction(const hk_string& r,bool registerchange)
{
    if (r==p_private->p_fullpagereplacefunctionstring)return;

    recodefunctionlisttype::iterator it=p_fullpagereplacefunctions.find(r);
    if (it==p_fullpagereplacefunctions.end())
    {

        show_warningmessage(hk_translate("Replacefunctiontype not found"));
        p_private->p_fullpagereplacefunction=NULL;
        p_private->p_fullpagereplacefunctionstring="None";
        return;
    }

    recodefunctiontype* rf=*it->second;
    p_private->p_fullpagereplacefunction=rf;
    p_private->p_fullpagereplacefunctionstring=r;

    has_changed(registerchange);

}

recodefunctiontype* hk_report::fullpagereplacefunction(void)
{
    return p_private->p_fullpagereplacefunction;

}

hk_string      hk_report::fullpagereplacefunctionstring(void)
{
    return p_private->p_fullpagereplacefunctionstring;

}


void hk_report::add_fullpagereplacefunctiontype(const hk_string& name,recodefunctiontype* f)
{
    if (name.size()==0) return;
    typedef recodefunctionlisttype::value_type recodevalue;
    p_fullpagereplacefunctions.insert(recodevalue(name,f));
    p_fullpagereplacelist.insert(p_fullpagereplacelist.end(),name);

}

list<hk_string>* hk_report::fullpagereplacefunctionlist(void)
{
    return &p_fullpagereplacelist;

}







void    hk_report::neutralize_definition(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::neutralize_definition");
#endif

    set_default_beforereportdata("",registerchange);
    set_default_afterreportdata("",registerchange);
    set_default_reportsectionbegin("",registerchange);
    set_default_reportsectionend("",registerchange);
    set_default_reportsectionbetweendata("",registerchange);
    set_default_reportsectioncountfunction("None",registerchange);
    set_default_reportdataconfigurefunction("None",registerchange);
    set_default_reportdata("%VALUE%",registerchange);
    set_pagedelimiter("",registerchange);
    set_begin("",registerchange);
    set_end("",registerchange);
    set_reportconfigurefunction("None",registerchange);
    set_recodefunction("None",registerchange);
    set_fullpagereplacefunction("None",registerchange);
    vector<hk_reportsectionpair*>* s=sectionpairs();
    if (s!=NULL)
    {
        vector<hk_reportsectionpair*>::iterator section_it=s->begin();
        while(section_it!=s->end())
        {
            (*section_it)->neutralize_definition(registerchange);
            section_it++;
        }
    }
p_private->p_print_full_pages_only=false;
p_private->p_useencodingtab=false;
}


void    hk_report::set_pagedelimiter(const hk_string& d,bool registerchange)
{
    p_private->p_pagedelimiter=d;
    has_changed(registerchange);
}


hk_string hk_report::pagedelimiter(void)
{
    return p_private->p_pagedelimiter;

}


vector<hk_reportsectionpair*>* hk_report::sectionpairs(void)
{
    return &p_sectionpairs;
}


void hk_report::set_multiplefiles(bool m,bool registerchange)
{
    p_multiplefiles=m;
    has_changed(registerchange);
}


bool hk_report::multiplefiles(void)
{
    return p_multiplefiles;
}


void    hk_report::set_fileextension(const hk_string& ext,bool registerchange)
{
    p_private->p_fileextension=ext;
    has_changed(registerchange);
}


hk_string hk_report::fileextension(void)
{
    return p_private->p_fileextension;
}


void hk_report::add_configurefunctiontype(const hk_string& n,report_configurefunction* f)
{
    if (n.size()==0) return;
    typedef reportconfigurelisttype::value_type confvalue;
    p_reportconfigurefunctions.insert(confvalue(n,f));
    p_reportconfigurelist.insert(p_reportconfigurelist.end(),n);

}


list<hk_string>* hk_report::configurefunctionlist(void)
{
    return &p_reportconfigurelist;
}


void hk_report::set_masterreport(hk_report* m)
{
    p_private->p_masterreport=m;
    if (m!=NULL) set_reporttype(m->reporttype(),false);
}


hk_report* hk_report::masterreport(void)
{
    return p_private->p_masterreport;

}


bool hk_report::is_subreport(void) const
{
  return (p_private->p_masterreport!=NULL);
}

bool hk_report::is_subpresentation(void)
{
  return is_subreport();
}




ostream* hk_report::outputstream(void)
{
    if (p_private->p_masterreport==NULL)
    {
     if (p_private->p_print_full_pages_only && !p_private->p_nostrstream)
      return &p_private->p_stringstream;
     else
      return p_private->p_output;
    }
    else return p_private->p_masterreport->outputstream();
}



void hk_report::create_fontslists(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::create_fontslists");
#endif
    p_private->p_neededfonts.clear();
    p_private->p_usedpsfonts.clear();
    p_private->p_usedfonts.clear();
    p_private->p_suppliedfonts.clear();
    p_private->p_suppliedfontspsnames.clear();
    p_private->p_embeddedfontsdefinition="";

    search_sectionfonts(p_private->p_reportheader);
    search_sectionfonts(p_private->p_reportfooter);
    search_sectionfonts(p_private->p_pageheader);
    search_sectionfonts(p_private->p_pagefooter);
    search_sectionfonts(p_private->p_datasection);
    vector<hk_reportsectionpair*>::iterator it=p_sectionpairs.begin();
    while (it!=p_sectionpairs.end())
    {
        if ((*it)->headersection()!=NULL) search_sectionfonts((*it)->headersection());
        if ((*it)->footersection()!=NULL) search_sectionfonts((*it)->footersection());
        it++;
    }
//     hk_font f=font();
//     registerfont(&f);
}


list<hk_string>* hk_report::neededfonts(void)
{
    return &p_private->p_neededfonts;
}


list<hk_string>* hk_report::usedfonts(void)
{
    return &p_private->p_usedfonts;
}


list<hk_string>* hk_report::usedpsfonts(void)
{
    return &p_private->p_usedpsfonts;
}


bool    hk_report::is_newfont(const hk_string&f)
{
    list<hk_string>::iterator it=p_private->p_usedpsfonts.begin();
    while (it!=p_private->p_usedpsfonts.end())
    {
        if ((*it)==f) return false;
        it++;
    }
    return true;
}

void hk_report::registerfont(hk_font* fnt)
{
  if (!fnt) return;
  if (p_private->p_masterreport)
      p_private->p_masterreport->registerfont(fnt);
  hk_string f=fnt->psfontname();
        if (is_newfont(f))
        {
            p_private->p_usedfonts.insert(p_private->p_usedfonts.end(),fnt->fontname());
            p_private->p_usedpsfonts.insert(p_private->p_usedpsfonts.end(),f);
        
        if (p_embedfonts)
        {
          hk_string def=fnt->psfontembeddefinition();
          if(def.size()>0)
          {
           p_private->p_embeddedfontsdefinition+="%%BeginResource: font "+f+"\n"+def+"\n%%EndResource\n";
           p_private->p_suppliedfonts.push_back(fnt->fontname());
           p_private->p_suppliedfontspsnames.push_back(f);
          }
          else
            p_private->p_neededfonts.insert(p_private->p_neededfonts.end(),f);

    
        }
        else
            p_private->p_neededfonts.insert(p_private->p_neededfonts.end(),f);
        }
}

void hk_report::search_sectionfonts(hk_reportsection* s)
{
    if (s==NULL) return;
    vector<hk_reportdata*>::iterator it=s->datalist()->begin();
    while (it!=s->datalist()->end())
    {
       hk_font fo=(*it)->font();
       registerfont(&fo);
        it++;
    }
    
    if (s->subreport())
    {
      s->subreport()->create_fontslists();
    }
}


hk_reportsection* hk_report::datasection(void)
{
    return p_private->p_datasection;

}


void   hk_report::set_use_standard_storagepath(bool s)
{
    p_private->p_use_standardpath=s;
}


bool   hk_report::use_standard_storagepath(void)
{
    return p_private->p_use_standardpath;

}


hk_reportsection* hk_report::report_header(void)
{
    return p_private->p_reportheader;
}


hk_reportsection* hk_report::report_footer(void)
{
    return p_private->p_reportfooter;
}


hk_reportsection* hk_report::page_header(void)
{
    return p_private->p_pageheader;
}


hk_reportsection* hk_report::page_footer(void)
{
    return p_private->p_pagefooter;
}


bool hk_report::widget_specific_before_new_page(void)
{
    return true;
}


bool hk_report::widget_specific_after_new_page(void)
{
    return true;
}


bool hk_report::print_report(bool execute_before_printing)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::print_report");
#endif
    if (execute_before_printing)
        if (!execute()) return false;
    hk_string p=p_printcommand+" \'"+outputfile()+"\'";
    bool res = true;
/*   hk_string languagevariable;
   hk_string homevariable;
   hk_string displayvariable;
   hk_string pathdirvariable;
   hk_string ld_library_pathvariable;
   char* sb;*/

/*   sb=getenv("LANG");
   if (sb!=NULL) languagevariable=sb;
   sb=getenv("HOME");
   if (sb!=NULL) homevariable=sb;
   sb=getenv("DISPLAY");
   if (sb!=NULL) displayvariable=sb;
   sb=getenv("PATH");
   if (sb!=NULL) pathdirvariable=sb;
   sb=getenv("LD_LIBRARY_PATH");
   if (sb!=NULL) ld_library_pathvariable=sb;*/
#ifdef HAVE_CLEARENV
/*   clearenv();
   setenv("PATH","/bin:/usr/bin:/usr/local/bin",1);
   setenv("IFS"," \t\n",1);
   setenv("LANG",languagevariable.c_str(),1);
   setenv("HOME",homevariable.c_str(),1);
   setenv("DISPLAY",displayvariable.c_str(),1);
   setenv("LD_LIBRARY_PATH",ld_library_pathvariable.c_str(),1);*/
#endif
    int r=system(p.c_str());
    if (r!=0)
    {
        show_warningmessage(hk_translate("Error calling print command. Perhaps you have to add the full path."));
        res=false;
    }
//  setenv("PATH",pathdirvariable.c_str(),1);
    return res;

}


void hk_report::set_printcommand(const hk_string& printcommand)
{
    if( &printcommand==NULL) return;
    if (printcommand.size()==0) return;
    p_printcommand=printcommand;
}


hk_string hk_report::printcommand(void)
{
    return p_printcommand;

}


bool hk_report::move_sectionpair(unsigned int actualposition,int steps)
{
    if (actualposition>=p_sectionpairs.size()) return false;
    if (steps==0)return true;
    hk_reportsectionpair* p_buffer= p_sectionpairs[actualposition];
    int p_steps = steps;
    if (steps>0&& actualposition+steps>=p_sectionpairs.size()) p_steps=p_sectionpairs.size()-1;
    if (steps<0)p_steps=actualposition;

    int s=0;
    if (steps>0)
    {
        while (s<steps)
        {
            p_sectionpairs[actualposition+s]=p_sectionpairs[actualposition+s+1];
            s++;
        }
    }
    else
    {                                             //steps < 0
        while (s>steps)
        {
            p_sectionpairs[actualposition+s]=p_sectionpairs[actualposition+s-1];
            s--;
        }
    }
    p_sectionpairs[actualposition+steps]=p_buffer;
    return true;
}


int     hk_report::default_reportprecision(void)
{
    return p_private->p_default_precision;
}


void    hk_report::set_default_reportprecision(int p, bool registerchange)
{
    p_private->p_default_precision=(p<0?-1:p);
    has_changed(registerchange);
}


bool    hk_report::default_use_reportseparator(void)
{
    return p_private->p_default_use_thousandsseparator;
}


void    hk_report::set_default_use_reportseparator(bool u, bool registerchange )
{
    p_private->p_default_use_thousandsseparator=u;
    has_changed(registerchange);
}


void hk_report::init_section(hk_reportsection* s)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsectionpair::init_section(hk_reportsection*)");
#endif
    if (s==NULL) return;
    s->set_automatic_create_data(false,false);
    s->set_default_reportdata( default_reportdata(),false);
    s->set_default_beforereportdata(default_beforereportdata(),false);
    s->set_default_afterreportdata(default_afterreportdata(),false);
    s->set_default_reportdataconfigurefunction(default_reportdataconfigurefunction(),false);
    s->set_sectionbegin(default_reportsectionbegin(),false);
    s->set_sectionend(default_reportsectionend(),false);
    s->set_sectioncountfunction(default_reportsectioncountfunction(),false);
    s->set_betweendata(default_reportsectionbetweendata(),false);
    s->set_default_reportprecision(default_reportprecision(),false);
    s->set_default_use_reportseparator(default_use_reportseparator(),false);
    s->set_presentationdatasource(presentationdatasource(),false);
}


unsigned int    hk_report::designwidth(void)
{

    if (p_private->p_masterreport) return p_private->p_masterreport->designwidth();
    else return hk_presentation::designwidth();
}


unsigned int    hk_report::designheight(void)
{
    if (p_private->p_masterreport) return p_private->p_masterreport->designheight();
    else return hk_presentation::designheight();

}


unsigned int    hk_report::horizontal2relativ(unsigned int h)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::horizontal2relativ");
#endif
    return (unsigned int)(h*teiler/ designwidth()+0.5);

}


unsigned int    hk_report::vertical2relativ(unsigned int v)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::vertical2relativ");
#endif
    return (unsigned int)(v*teiler / designheight()+0.5);

}


unsigned int    hk_report::relativ2horizontal(unsigned int r)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::relativ2horizontal");
#endif
    return (unsigned int) (r*designwidth() /teiler+0.5);

}


unsigned int    hk_report::relativ2vertical(unsigned int r)
{
#ifdef HK_DEBUG
    hkdebug("hk_report::relativ2vertical");
#endif
    return (unsigned int) (r*designheight()/teiler+0.5);

}


void hk_report::bulk_operation(enum_bulkoperation bulkoperation)
{
    vector<hk_reportsectionpair*>::iterator it=p_sectionpairs.begin();
    while (it!=p_sectionpairs.end())
    {
        (*it)->bulk_operation(bulkoperation);
        ++it;
    }

    if (p_private->p_pageheader) p_private->p_pageheader->bulk_operation(bulkoperation);
    if (p_private->p_pagefooter) p_private->p_pagefooter->bulk_operation(bulkoperation);
    if (p_private->p_reportheader)p_private->p_reportheader->bulk_operation(bulkoperation);
    if (p_private->p_reportfooter)p_private->p_reportfooter->bulk_operation(bulkoperation);
    p_private->p_datasection->bulk_operation(bulkoperation);

}


void hk_report::sizetype_changed(void)
{

  if (sizetype()==hk_presentation::relative)
  {
// this means previous sizetype was hk_presentation::absolute
// and changed now to hk_presentation::relative
   p_private->p_borderleft=(unsigned int)(p_private->p_borderleft*teiler/designwidth()+0.5);
   p_private->p_borderright=(unsigned int)(p_private->p_borderright*teiler/designwidth()+0.5);
   p_private->p_bordertop=(unsigned int)(p_private->p_bordertop*teiler/designheight()+0.5);
   p_private->p_borderbottom=(unsigned int)(p_private->p_borderbottom*teiler/designheight()+0.5);
  }
else
  {
// this means previous sizetype was hk_presentation::relative
// and changed now to hk_presentation::absolute
   p_private->p_borderleft=(unsigned int)(p_private->p_borderleft*designwidth()/teiler+0.5);
   p_private->p_borderright=(unsigned int)(p_private->p_borderright*designwidth()/teiler+0.5);
   p_private->p_bordertop=(unsigned int)(p_private->p_bordertop*designheight()/teiler+0.5);
   p_private->p_borderbottom=(unsigned int)(p_private->p_borderbottom*designheight()/teiler+0.5);

  }

    vector<hk_reportsectionpair*>::iterator it=p_sectionpairs.begin();
    while (it!=p_sectionpairs.end())
    {
        if ((*it)->headersection()) (*it)->headersection()->sizetype_changed();
        if ((*it)->footersection()) (*it)->footersection()->sizetype_changed();
        ++it;
    }

    if (p_private->p_pageheader) p_private->p_pageheader->sizetype_changed();
    if (p_private->p_pagefooter) p_private->p_pagefooter->sizetype_changed();
    if (p_private->p_reportheader)p_private->p_reportheader->sizetype_changed();
    if (p_private->p_reportfooter)p_private->p_reportfooter->sizetype_changed();
    p_private->p_datasection->sizetype_changed();


}


hk_reportdata* hk_report::get_reportdatavisible(long nr)
{
hk_reportdata* result=NULL;
    vector<hk_reportsectionpair*>::iterator it=p_sectionpairs.begin();
    while (it!=p_sectionpairs.end()&&result==NULL)
    {
        if ((*it)->headersection()) result=(*it)->headersection()->get_reportdatavisible(nr);
        if ((*it)->footersection()&&!result) result=(*it)->footersection()->get_reportdatavisible(nr);
        ++it;
    }

   if (!result) result=p_private->p_datasection->get_reportdatavisible(nr);
    if (!result&&p_private->p_pageheader) result=p_private->p_pageheader->get_reportdatavisible(nr);
    if (!result&&p_private->p_pagefooter) result=p_private->p_pagefooter->get_reportdatavisible(nr);
    if (!result&&p_private->p_reportheader)result=p_private->p_reportheader->get_reportdatavisible(nr);
    if (!result&&p_private->p_reportfooter)result=p_private->p_reportfooter->get_reportdatavisible(nr);

 return result;
}

hk_reportdata* hk_report::get_reportdatavisible(const hk_string& i)
{
hk_reportdata* result=NULL;
    vector<hk_reportsectionpair*>::iterator it=p_sectionpairs.begin();
    while (it!=p_sectionpairs.end()&&result==NULL)
    {
        if ((*it)->headersection()) result=(*it)->headersection()->get_reportdatavisible(i);
        if ((*it)->footersection()&&!result) result=(*it)->footersection()->get_reportdatavisible(i);
        ++it;
    }

   if (!result) result=p_private->p_datasection->get_reportdatavisible(i);
    if (!result&&p_private->p_pageheader) result=p_private->p_pageheader->get_reportdatavisible(i);
    if (!result&&p_private->p_pagefooter) result=p_private->p_pagefooter->get_reportdatavisible(i);
    if (!result&&p_private->p_reportheader)result=p_private->p_reportheader->get_reportdatavisible(i);
    if (!result&&p_private->p_reportfooter)result=p_private->p_reportfooter->get_reportdatavisible(i);
    return result;
}

void hk_report::set_on_new_page_action(const hk_string& a,bool registerchange)
{
p_private->p_on_new_page_action=a;
has_changed(registerchange);
}

hk_string hk_report:: on_new_page_action(void)
{
return p_private->p_on_new_page_action;

}


 bool hk_report:: action_on_new_page(void)
 {

return interpreter()->on_print_new_page(this);
 }


 void hk_report::stop_execution(void)
 {
p_private->p_cancelreport=true;
 if (p_private->p_masterreport)
   p_private->p_masterreport->stop_execution();
 }

 bool hk_report::execution_stopped(void)
 {
   return p_private->p_cancelreport;
 }

bool    hk_report::while_executing(void) const
{
return p_private->p_while_executing;
}

void hk_report::database_delete(void)
{
 if(while_executing())
   stop_execution();
hk_presentation::database_delete();
}

bool hk_report::widget_specific_row_change(void)
{
  return true;
}

void hk_report::widget_specific_batch_next(void)
{
  p_private->p_rowposition++;
}

void hk_report::widget_specific_batch_previous(void)
{
  p_private->p_rowposition--;

}

hk_string hk_report::filelocale(void)
{
  return p_private->p_locale;
}

hk_string hk_report::filecharset(void)
{
  return p_private->p_charset;

}

void hk_report::set_filelocale(const hk_string& l)
{
p_private->p_locale=l;
}

void hk_report::set_filecharset(const hk_string& l)
{
p_private->p_charset=l;
}

void hk_report::set_progressdialog(progress_dialogtype* p)
{
p_private->p_progressdialog=p;
}

progress_dialogtype* hk_report::progressdialog(void)const
{
  return p_private->p_progressdialog;
}

hk_encodingtab* hk_report::encodingtab(void) const
{
if (!p_private->p_useencodingtab) return NULL;
   if (p_private->p_masterreport) return p_private->p_masterreport->encodingtab();
   else
   return &p_private->p_encodingtab;
}


void hk_report::set_print_full_pages_only(bool p, bool registerchange)
{
p_private->p_print_full_pages_only=p;
has_changed(registerchange);
}

bool hk_report::print_full_pages_only(void)
{
  return p_private->p_print_full_pages_only;
}


void hk_report::set_embedfonts(bool e)
{
  p_embedfonts=e;
}

bool hk_report::embedfonts(void)
{
  return p_embedfonts;
}

void hk_report::set_useencodingtab(bool u)
{
  p_private->p_useencodingtab=u;
}

bool hk_report::useencodingtab(void)
{
return p_private->p_useencodingtab;
}


