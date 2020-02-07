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
//$Revision: 1.72 $
#include "hk_reportsection.h"
#include "hk_reportsectionpair.h"
#include "hk_report.h"
#include "hk_reportdata.h"
#include <list>
#include "hk_reportutils.h"
#include "hk_database.h"
#include "hk_column.h"
#include "hk_datasource.h"

const  double teiler=10000.0;

//********************************************
//** hk_reportsection                       **
//********************************************

hk_reportsection::sectioncountlisttype  hk_reportsection::p_sectioncountfunctions;
list<hk_string> hk_reportsection::p_sectioncountfunctionlist;

hk_reportsection::sectionreplacelisttype  hk_reportsection::p_sectionreplacefunctions;
list<hk_string> hk_reportsection::p_sectionreplacefunctionlist;

hk_reportsection::hk_reportsection(hk_report* e):hk_dsdatavisible(e)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::hk_reportsection");
#endif
    p_visibletype=hk_visible::reportsection;
    p_report=e;
    p_unique=false;
    p_printsection=true;
    p_automatic_create_datas=true;
    p_already_automaticcreated_data=false;
    p_uniqueendsection=false;
    p_newpageaftersection=false;
    p_sectionwasprinted=false;
    p_subreport=NULL;
    p_replacefunction=NULL;
    p_sectioncountfunction=NULL;
    p_subreportdatasource=NULL;
    p_pair=NULL;
    p_automatic_created_datasource=false;
    p_default_data="%VALUE%";
    p_forcenewsection=false;
    p_default_precision=-1;
    p_default_use_thousandsseparator=false;
    p_print_subreport_before_data=false;
    set_default_reportdataconfigurefunction("None",false);
    p_offset=0;
    p_automaticreatedatafunction=NULL;
    if (p_sectioncountfunctions.size()==0)
    {
        add_sectioncountfunctiontype("None",NULL);
        add_sectioncountfunctiontype("Postscript",&recount_postscript);
    }
    set_sectioncountfunction("None");
//  wanna_debug(true);
    if (p_sectionreplacefunctions.size()==0)
    {
        add_sectionreplacefunctiontype("None",NULL);
    }
    set_replacefunction("None");

}


hk_reportsection::~hk_reportsection()
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::~hk_reportsection");
#endif

    if (p_pair!=NULL) p_pair->remove_section(this);
    else    if (p_report!=NULL) p_report->remove_section(this);

    remove_all_datas();
}


void hk_reportsection::remove_all_datas(void)
{

    vector<hk_reportdata*>::iterator it=p_data.begin();
    while (it!=p_data.end())
    {
        hk_reportdata* d=(*it);
        it=p_data.erase(it);
        d->p_section=NULL;
        delete d;
    }

}


hk_reportdata* hk_reportsection::widget_specific_new_data(void)
{
    hk_reportdata* d= new hk_reportdata(this);
    return d;
}


hk_reportdata* hk_reportsection::new_data(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::new_data");
#endif
    if (p_report==NULL) return NULL;
    hk_reportdata* d=widget_specific_new_data();
    p_report->register_object(d);
    d->set_data( default_reportdata());
    d->set_beforedata(default_beforereportdata());
    d->set_afterdata(default_afterreportdata());
    d->set_configurefunction(default_reportdataconfigurefunction());

    if (d->configurefunction()!=NULL)
    {
        (d->configurefunction())(d);
    }
    p_data.insert(p_data.end(),d);
    d->set_presentationdatasource(presentationdatasource());
    d->set_numberformat(default_use_reportseparator(),default_reportprecision(),false);
    return d;

}


hk_reportdata*  hk_reportsection::data_at(unsigned int a)
{

    if (a >= p_data.size()) return NULL;
    return p_data[a];
}


bool hk_reportsection::new_uniquevalue(bool justcheck)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::new_uniquevalue");
#endif
    if (p_report->datasource()==NULL) return false;
    if (p_forcenewsection)return true;
    if (!p_unique) return false;
    bool result=false;
    if (p_report->datasource()->max_rows()==0)return true;

//now check endsection
    if (p_uniqueendsection)
    {
        if (columnname().size()==0||column()==NULL)
        {
            if (p_report->datasource()->row_position()==p_report->datasource()->max_rows()-1)return true;
            else return false;
        }
        else
        {
            result=(p_lastuniquevalue!=column()->asstring_at(p_report->datasource()->row_position()+1,true));
            if (justcheck)p_lastuniquevalue=column()->asstring_at(p_report->datasource()->row_position()+1,true);
            if (p_report->datasource()->row_position()==0
                &&p_report->datasource()->max_rows()>1)return false;
            if ( p_report->datasource()->row_position()>=p_report->datasource()->max_rows()-1)return true;
            return result;
        }

    }

//now check beginning sections

    if (column()!=NULL)
    {
        result=(p_lastuniquevalue!=column()->asstring(true));
        if (!justcheck) p_lastuniquevalue=column()->asstring(true);

    }
    else if (p_report->datasource()!=NULL)
    if (p_report->datasource()->row_position()==0) return true;

        return result;

}


hk_string hk_reportsection::sectionbegin(void)
{
    return p_sectionbegin;
}


void hk_reportsection::set_sectionbegin(const hk_string& b,bool registerchange)
{
    p_sectionbegin=b;
    has_changed(registerchange);
}


vector<hk_reportdata*>* hk_reportsection::datalist(void)
{
    return &p_data;
}


hk_string hk_reportsection::sectionend(void)
{
    return p_sectionend;
}


void hk_reportsection::set_sectionend(const hk_string& e,bool registerchange)
{
    p_sectionend=e;
    has_changed(registerchange);
}


hk_string hk_reportsection::betweendata(void)
{
    return p_betweendata;
}


void hk_reportsection::set_betweendata(const hk_string& b,bool registerchange)
{
    p_betweendata=b;
    has_changed(registerchange);
}


void hk_reportsection::set_automatic_create_data(bool d,bool registerchange)
{
    p_automatic_create_datas=d;
    has_changed(registerchange);
}


bool hk_reportsection::new_page_after_section(void)
{
    return p_newpageaftersection;
}


void hk_reportsection::set_new_page_after_section(bool p,bool registerchange)
{
    p_newpageaftersection=p;
    has_changed(registerchange);
}


bool hk_reportsection::section_was_printed(void)
{
    return p_sectionwasprinted;
}


void hk_reportsection::set_replacefunction(const hk_string& f,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::set_replacefunction");
#endif
    if (f==p_replacefunctionstring)return;

    sectionreplacelisttype::iterator it=p_sectionreplacefunctions.find(f);
    if (it==p_sectionreplacefunctions.end())
    {

        show_warningmessage(hk_translate("sectionreplacefunction not found"));
        p_replacefunction=NULL;
        p_replacefunctionstring="None";
        return;
    }
    section_replacefunctiontype* c=*it->second;

    p_replacefunction=c;
    p_replacefunctionstring=f;
    has_changed(registerchange);
}


void hk_reportsection::set_sectioncountfunction(const hk_string& f,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::set_sectioncountfunction");
#endif

    if (f==p_sectioncountfunctionstring)return;

    sectioncountlisttype::iterator it=p_sectioncountfunctions.find(f);
    if (it==p_sectioncountfunctions.end())
    {

        show_warningmessage(hk_translate("Section Countfunction not found"));
        p_sectioncountfunction=NULL;
        p_sectioncountfunctionstring="None";
        return;
    }
    reportsectioncounttype* c=*it->second;

    p_sectioncountfunction=c;
    p_sectioncountfunctionstring=f;
    has_changed(registerchange);
}


reportsectioncounttype* hk_reportsection::sectioncountfunction(void)
{
    return p_sectioncountfunction;
}


bool hk_reportsection::endsection(void)
{
    return p_uniqueendsection;
}


void hk_reportsection::reset_count(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::reset_count");
#endif
    clear_countingfields();
    vector<hk_reportdata*>::iterator vit=p_data.begin();
    while (vit!=p_data.end())
    {
        (*vit)->reset_count();
        vit++;
    }
    if (p_pair==NULL) return;

    vector<hk_reportsectionpair*>::iterator vs=std::find(p_report->sectionpairs()->begin(),p_report->sectionpairs()->end(),p_pair);
    if (vs!=p_report->sectionpairs()->end()&&this==p_pair->headersection()&& p_pair->footersection()!=NULL)
    {
        if(!p_pair->footersection()->unique()) p_pair->footersection()->reset_count();
    }
    if (vs!=p_report->sectionpairs()->end()) vs++;
    while (vs!=p_report->sectionpairs()->end())
    {
        vs++;
        if (vs!=p_report->sectionpairs()->end())
        {
            hk_reportsection* h= (*vs)->headersection();
            hk_reportsection* f= (*vs)->footersection();
            if (this==p_pair->headersection())
            {
                if (h!=NULL)if(!h->unique())h->reset_count();

            }

            if (f!=NULL)if(!f->unique())f->reset_count();
        }
    }
}


void hk_reportsection::clear_counting()
{
    clear_countingfields();
    vector<hk_reportdata*>::iterator vit=p_data.begin();
    while (vit!=p_data.end())
    {
        (*vit)->clear_counting();
        vit++;
    }

}



void hk_reportsection::automatic_create_datafields(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::automatic_create_datafields");
#endif
//automatic create datafields if wished
    int z=10;
    if (p_report->datasource()==NULL) return;
    if (p_automatic_create_datas&&!p_already_automaticcreated_data&&p_data.size()==0)
    {
        if (p_automaticreatedatafunction) p_automaticreatedatafunction(this);
        else
        {
            list<hk_column*>* cols=p_report->datasource()->columns();
            if (cols!=NULL)
            {
                list<hk_column*>::iterator cit=cols->begin();
                while (cit!=cols->end())
                {
                    hk_reportdata* dat=new_data();
                    dat->set_x(z);
                    z+=dat->width()+10;
                    dat->set_columnname((*cit)->name());
                    dat->set_data(default_reportdata());
                    cit++;
                }
            }
        }
        p_already_automaticcreated_data=true;
    }
//End automatic create datafields if wished

}


void hk_reportsection::set_automaticcreatedatafunction(reportsectionautomaticcreatedatatype* f)
{
    p_automaticreatedatafunction=f;
}


bool hk_reportsection::actual_string(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::actual_string");
#endif
if (!p_printsection) return true;
    bool result=true;
    hk_string   p_buffer;
    p_buffer="";
    automatic_create_datafields();
    bool u=unique();
    bool uni=(u&&new_uniquevalue());
    if (uni&&!p_uniqueendsection) reset_count();

//    if (!p_uniqueendsection)count_countingfields();

    if ((!u)||(uni))                              //=>not unique section or unique section to be printed
    {
        p_forcenewsection=false;
        vector<hk_reportdata*>::iterator vit=p_data.begin();
        while (vit!=p_data.end())
        {
          (*vit)->reset_data_fully_printed();
          ++vit;
        }
        
        
        
        bool first_try=true;
        if (p_print_subreport_before_data) result=print_subreport();
        do 
        {
        if (!first_try)
        {
          p_report->new_page();
         p_buffer="";
        }
        else
        first_try=false;
        
        vit=p_data.begin();
        p_buffer+=replace(sectionbegin());
        int c=0;
        while (vit!=p_data.end()&&!report()->execution_stopped())
        {

            if (!(*vit)->data_fully_printed())
            {
            if (c>0)
                p_buffer+=replace(betweendata());
            p_buffer+=(*vit)->actual_string();
            ++c;
            }
//if(uni)(*vit)->reset_count();
            vit++;

        }

        p_buffer+=replace(sectionend());
        p_sectionwasprinted=true;
        if (p_replacefunction!=NULL) p_buffer=p_replacefunction(this,p_buffer);
//         if (p_sectioncountfunction!=NULL) p_counts_as=p_sectioncountfunction(this);
        if (p_report->page_header()!=this &&p_report->page_footer()!=this  )p_report->rowcount(counts_as());
        *(p_report->outputstream()) <<p_buffer;
        } while (!is_fully_printed()) ;// end !is_fully printed
        
//         p_report->rowcount(p_report->sizetype()==hk_presentation::relative?relativ2vertical(offset()):offset());
        if (uni&&p_uniqueendsection) reset_count();
        if (!p_print_subreport_before_data) result=print_subreport();
    }
    else p_sectionwasprinted=false;
 return result;
}

bool hk_reportsection::is_fully_printed(void)
{
        if (!p_printsection) return true;
        if (p_report->execution_stopped()) return true;
        vector<hk_reportdata*>::iterator vit=p_data.begin();
        while (vit!=p_data.end())
        {
          if (!(*vit)->data_fully_printed())
           {
             return false;
           }
          ++vit;
        }
return true;
}


void hk_reportsection::create_countingfields(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::create_countingfields");
#endif
    if (p_report->datasource()==NULL) return;
    list<hk_column*>* collist=p_report->datasource()->columns();
    if ((collist!=NULL) &&(p_countings.size()==0))
    {
        list<hk_column*>::iterator colit =collist->begin();
        while (colit!=collist->end())
        {

            struct_countnumbers c;
            strcpy(c.colname,(*colit)->name().c_str());
            c.sum.real=c.squaresum.real=c.min.real=c.max.real=c.value.real=0;

            if ((*colit)->columntype()==hk_column::integercolumn
                ||(*colit)->columntype()==hk_column::auto_inccolumn)
            {
                hk_string s=(*colit)->asstring(true);
                int r=atoi(s.c_str());
                c.sum.integer+=r;
                c.squaresum.integer+=r*r;
                c.min.integer=r;
                c.max.integer=r;
                c.value.integer=r;

            }
            else
            if((*colit)->columntype()==hk_column::floatingcolumn)
            {
                hk_string s=(*colit)->asstring();
                double r=localestring2double(s);
                c.sum.real+=r;
                c.squaresum.real+=r*r;
                c.min.real=r;
                c.max.real=r;
                c.value.real=r;
            }

            colit++;
        }                                         //end WHILE
    }                                             //end IF p_countings.size()==0

}


void hk_reportsection::count_countingfields(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::count_countingfields");
#endif
    if (p_report->datasource()==NULL) return;
    if (p_countings.size()==0)create_countingfields();
    if (p_report->datasource()->max_rows()>0)
    {
        vector<hk_reportdata*>::iterator vit=p_data.begin();
        while (vit!=p_data.end())
        {
            (*vit)->count();
            vit++;
        }
    }
    list<hk_column*>* collist=p_report->datasource()->columns();
    list<hk_column*>::iterator colit =collist->begin();
    vector<struct_countnumbers>::iterator countit= p_countings.begin();
    while (colit!=collist->end()&&countit!=p_countings.end())
    {
        (*countit).count++;
        if ((*colit)->columntype()==hk_column::integercolumn
            ||(*colit)->columntype()==hk_column::auto_inccolumn)

        {
            hk_string s=(*colit)->asstring(true);
            int r=atoi(s.c_str());
            (*countit).value.integer=r;
            (*countit).sum.integer+=r;
            (*countit).squaresum.integer+=r*r;
            if (r<(*countit).min.integer) (*countit).min.integer=r;
            if (r>(*countit).max.integer) (*countit).max.integer=r;

        }
        else if((*colit)->columntype()==hk_column::floatingcolumn)
        {
            hk_string s=(*colit)->asstring();
            double r=localestring2double(s);
            (*countit).value.real=r;
            (*countit).sum.real+=r;
            (*countit).squaresum.real+=r*r;
            if (r<(*countit).min.real)(*countit).min.real=r;
            if (r>(*countit).max.real)(*countit).max.real=r;
        }

        colit++;
        countit++;
    }

}


void hk_reportsection::clear_countingfields(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::clear_countingfields");
#endif
    p_countings.erase(p_countings.begin(),p_countings.end());

}


bool hk_reportsection::print_subreport(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::print_subreport");
#endif
    if (p_subreport!=NULL)
       return p_subreport->execute();
   return true;
}


void    hk_reportsection::set_unique(bool u, bool end,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::set_unique");
#endif
    p_unique=u;
    p_uniqueendsection=end;
    has_changed(registerchange);
}


bool hk_reportsection::unique(void)
{
    return p_unique;
}


void hk_reportsection::reset(void)
{
    clear_countingfields();

}


hk_font hk_reportsection::font(void)
{
    hk_font f;
    if (hk_dsdatavisible::font().fontname().size()==0) f= p_report->font();
    else f=hk_dsdatavisible::font();
     f.set_encodingtab(p_report->encodingtab());

return f;
}




void    hk_reportsection::set_subreport(const hk_string& sub,bool before_data,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::set_subreport");
#endif

    if (p_subreport!=NULL)
    {
    hk_datasource* subds=p_subreport->datasource();
    if (subds)
    {
       subds->clear_depending_fields();
       subds->set_depending_on(NULL,false);
    }
    delete p_subreport;
    p_subreport=NULL;

    }
    p_subreportstring=sub;
    if (p_subreportstring.size()==0)return;
    hk_database* db=p_report->database();

    if (db!=NULL)
    {
        p_subreport=new hk_report();
        p_subreport->set_database(db);
        if (!p_subreport->load_report(sub))
        {
            delete p_subreport;
            p_subreport=NULL;
        }
    }

    p_print_subreport_before_data=before_data;
    if (p_subreport!=NULL)
        p_subreport->set_masterreport(p_report);
    has_changed(registerchange);
}


void  hk_reportsection::savedata(ostream& s,bool userdefined)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::savedata");
#endif
    start_mastertag(s,"HK_REPORTSECTION");
    hk_dsdatavisible::savedata(s);
    set_tagvalue(s,"UNIQUESECTION",p_unique);
    set_tagvalue(s,"UNIQUEENDSECTION",p_uniqueendsection);
    set_tagvalue(s,"AUTOMATICCREATEDATA",p_automatic_create_datas);
    set_tagvalue(s,"NEWPAGEAFTERSECTION",p_newpageaftersection);
    set_tagvalue(s,"SUBREPORT",p_subreportstring);
    set_tagvalue(s,"SUBREPORT_BEFOREDATA",p_print_subreport_before_data);
    set_tagvalue(s,"DEFAULTPRECISION",(long)p_default_precision);
    set_tagvalue(s,"DEFAULTUSETHOUSANDSSEPARATOR",p_default_use_thousandsseparator);
    set_tagvalue(s,"SECTIONOFFSET",p_offset);
    hk_string mtag1="REPORTDEPENDINGFIELDS";
    start_mastertag(s,mtag1);
    list<hk_string>::iterator thisit=p_depending_thisreport_fields.begin();
    list<hk_string>::iterator subit=p_depending_subreport_fields.begin();
    while (thisit!=p_depending_thisreport_fields.end())
    {
        set_tagvalue(s,"THISREPORTFIELD",(*thisit));
        set_tagvalue(s,"SUBREPORTFIELD",(*subit));

        thisit++;
        subit++;
    }
    end_mastertag(s,mtag1);

    start_mastertag(s,"SECTIONDATA");

    if ((p_data.size()>0)&&!p_already_automaticcreated_data)
    {
        vector<hk_reportdata*>::iterator it= p_data.begin();
        while (it!=p_data.end())
        {
            (*it)->savedata(s,userdefined);
            it++;
        }
    }
    end_mastertag(s,"SECTIONDATA");

    if (userdefined)
    {
        set_tagvalue(s,"REPORTSECTIONBEGIN",p_sectionbegin);
        set_tagvalue(s,"REPORTSECTIONEND",p_sectionend);
        set_tagvalue(s,"BETWEENDATA",p_betweendata);
        hk_string recfunc="NONE";
        set_tagvalue(s,"RECOUNTFUNCTION",p_sectioncountfunctionstring);
        set_tagvalue(s,"DEFAULTDATA",p_default_data);
        set_tagvalue(s,"DEFAULTBEFOREDATA",p_default_beforedata);
        set_tagvalue(s,"DEFAULTAFTERDATA",p_default_afterdata);
        set_tagvalue(s,"SECTIONREPLACEFUNCTION",p_replacefunctionstring);
    }

    end_mastertag(s,"HK_REPORTSECTION");

}


void  hk_reportsection::loaddata(xmlNodePtr definition,bool userdefined)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::loaddata");
#endif
    hk_string sbuffer;
    long int ibuffer;
    bool bbuffer;
    hk_dsdatavisible::loaddata(definition);
    get_tagvalue(definition,"UNIQUESECTION",p_unique);
    get_tagvalue(definition,"UNIQUEENDSECTION",p_uniqueendsection);
    get_tagvalue(definition,"SECTIONOFFSET",p_offset);
    if (p_unique) set_unique(p_unique,p_uniqueendsection);
    get_tagvalue(definition,"AUTOMATICCREATEDATA",p_automatic_create_datas);
    get_tagvalue(definition,"NEWPAGEAFTERSECTION",p_newpageaftersection);
    if (get_tagvalue(definition,"DEFAULTPRECISION",ibuffer))p_default_precision=ibuffer;
    if (get_tagvalue(definition,"DEFAULTUSETHOUSANDSSEPARATOR",bbuffer))p_default_use_thousandsseparator=bbuffer;

    if (get_tagvalue(definition,"SUBREPORT",sbuffer))
    {
        get_tagvalue(definition,"SUBREPORT_BEFOREDATA",p_print_subreport_before_data);
        if (sbuffer.size()>0)set_subreport(sbuffer,p_print_subreport_before_data,false);
    }

    int i=1;
    clear_depending_fields();
    xmlNodePtr dpf;
    hk_string thisfield;
    hk_string subfield;
    if ((dpf=get_tagvalue(definition,"REPORTDEPENDINGFIELDS")))
    {
        while (get_tagvalue(dpf,"THISREPORTFIELD",thisfield,i))
        {
            get_tagvalue(dpf,"SUBREPORTFIELD",subfield,i);
            add_depending_fields(thisfield,subfield,false);
            i++;
        }
    }

    xmlNodePtr databuffer;
    if ((databuffer=get_tagvalue(definition,"SECTIONDATA")))
    {
        int counter=1;
        xmlNodePtr node;
        while ((node=get_tagvalue(databuffer,"HK_REPORTDATA",sbuffer,counter,mastertag)))
        {
            hk_reportdata* newdata=new_data();
            if (newdata!=NULL)
            {
                newdata->loaddata(node);
            }

            counter++;
        }
    }

    if (userdefined)
    {
        get_tagvalue(definition,"REPORTSECTIONBEGIN",p_sectionbegin);
        get_tagvalue(definition,"REPORTSECTIONEND",p_sectionend);
        get_tagvalue(definition,"BETWEENDATA",p_betweendata);
        if (get_tagvalue(definition,"RECOUNTFUNCTION",sbuffer))set_sectioncountfunction(sbuffer);
        if (get_tagvalue(definition,"SECTIONREPLACEFUNCTION",sbuffer))set_replacefunction(sbuffer);
        get_tagvalue(definition,"DEFAULTDATA",p_default_data);
        get_tagvalue(definition,"DEFAULTBEFOREDATA",p_default_beforedata);
        get_tagvalue(definition,"DEFAULTAFTERDATA",p_default_afterdata);
    }

    widget_specific_after_loaddata();

}


unsigned long hk_reportsection::counts_as(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::counts_as");
#endif
    if (p_sectioncountfunction!=NULL)
        set_counts_as(p_sectioncountfunction(this));
    return (hk_dsdatavisible::counts_as());
}


hk_string hk_reportsection::replace(const hk_string& where)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::replace");
#endif
    hk_string p_buffer;
    p_buffer=replace_all("%ABSOLUTEPAGENUMBER%",where,longint2string(p_report->absolutepagenumber()));
    p_buffer=replace_all("%PAGENUMBER%",p_buffer,longint2string(p_report->pagenumber()));
    hk_colour fg=p_report->foregroundcolour();
    hk_colour bg=p_report->backgroundcolour();
    p_buffer=replace_all("%FGRED%",p_buffer,format_standard_number(fg.red(),0,0));
    p_buffer=replace_all("%FGGREEN%",p_buffer,format_standard_number(fg.green(),0,0));
    p_buffer=replace_all("%FGBLUE%",p_buffer,format_standard_number(fg.blue(),0,0));
    p_buffer=replace_all("%BGRED%",p_buffer,format_standard_number(bg.red(),0,0));
    p_buffer=replace_all("%BGGREEN%",p_buffer,format_standard_number(bg.green(),0,0));
    p_buffer=replace_all("%BGBLUE%",p_buffer,format_standard_number(bg.blue(),0,0));

    return hk_dsdatavisible::replace(p_buffer);
}


void hk_reportsection::set_datasource(hk_datasource* d)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::set_datasource");
#endif
    hk_dsdatavisible::set_datasource(d);
    vector<hk_reportdata*>::iterator it=p_data.begin();
    while (it!=p_data.end())
    {
        (*it)->set_datasource(d);
        it++;
    }
}


void hk_reportsection::remove_data(hk_reportdata* s)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::remove_data");
#endif
    if (s==NULL)return;

    vector<hk_reportdata*>::iterator i=std::find(p_data.begin(),p_data.end(),s);
    p_data.erase(i);
}


void hk_reportsection::set_default_reportdataconfigurefunction(const hk_string& f,bool registerchange)
{
    p_default_dataconfigurefunction=f;
    has_changed(registerchange);
}


hk_string hk_reportsection::default_reportdataconfigurefunction(void)
{
    return p_default_dataconfigurefunction;
}


void    hk_reportsection::neutralize_definition(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::neutralize_definition");
#endif

    set_sectionbegin("",registerchange);
    set_sectionend("",registerchange);
    set_betweendata("",registerchange);
    set_sectioncountfunction("None",registerchange);
    set_replacefunction("None",registerchange);
    set_default_reportdataconfigurefunction("None",registerchange);
    set_default_beforereportdata("",registerchange);
    set_default_afterreportdata("",registerchange);
    set_default_reportdata("%VALUE%",registerchange);
    p_automaticreatedatafunction=NULL;
    vector<hk_reportdata*>*d=datalist();
    if (d!=NULL)
    {
        vector<hk_reportdata*>::iterator data_it=d->begin();
        while(data_it!=d->end())
        {
            (*data_it)->neutralize_definition(registerchange);
            data_it++;
        }
    }
}


void hk_reportsection::add_sectioncountfunctiontype(const hk_string& name,reportsectioncounttype* f)
{

    if (name.size()==0) return;
    typedef sectioncountlisttype::value_type sectioncountvalue;
    p_sectioncountfunctions.insert(sectioncountvalue(name,f));
    p_sectioncountfunctionlist.insert(p_sectioncountfunctionlist.end(),name);

}


list<hk_string>* hk_reportsection::sectioncountfunctionlist(void)
{
    return &p_sectioncountfunctionlist;
}


void hk_reportsection::add_sectionreplacefunctiontype(const hk_string& name,section_replacefunctiontype* f)
{

    if (name.size()==0) return;
    typedef sectionreplacelisttype::value_type sectionreplacevalue;
    p_sectionreplacefunctions.insert(sectionreplacevalue(name,f));
    p_sectionreplacefunctionlist.insert(p_sectionreplacefunctionlist.end(),name);

}


list<hk_string>* hk_reportsection::sectionreplacefunctionlist(void)
{
    return &p_sectionreplacefunctionlist;
}


void        hk_reportsection::set_default_reportdata(const hk_string& v,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::set_default_reportdata");
#endif
    p_default_data=v;
    has_changed(registerchange);
}


void        hk_reportsection::set_default_beforereportdata(const hk_string& v,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::set_default_beforereportdata");
#endif
    p_default_beforedata=v;
    has_changed(registerchange);
}


void        hk_reportsection::set_default_afterreportdata(const hk_string& v,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::set_default_afterreportdata");
#endif
    p_default_afterdata=v;
    has_changed(registerchange);
}


void    hk_reportsection::add_depending_fields(const hk_string& thisreport_field, const hk_string& subreport_field,bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::add_depending_fields");
#endif

    if ((thisreport_field=="")||(subreport_field=="")) return;
    has_changed(registerchange);
    p_depending_thisreport_fields.insert(p_depending_thisreport_fields.end(),thisreport_field);
    p_depending_subreport_fields.insert(p_depending_subreport_fields.end(),subreport_field);
#ifdef HK_DEBUG
    hkdebug("hk_report::add_depending_fields ENDE");
#endif

}


void hk_reportsection::clear_depending_fields(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::clear_depending_fields");
#endif
    p_depending_thisreport_fields.erase(p_depending_thisreport_fields.begin(),p_depending_thisreport_fields.end());
    p_depending_subreport_fields.erase(p_depending_subreport_fields.begin(),p_depending_subreport_fields.end());

}


list<hk_string>* hk_reportsection::depending_on_thisreportfields(void)
{
    return &p_depending_thisreport_fields;
}


list<hk_string>* hk_reportsection::depending_on_subreportfields(void)
{
    return &p_depending_subreport_fields;
}


void    hk_reportsection::before_datasource_enables(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::before_datasource_enables");
#endif
    hk_dsdatavisible::before_datasource_enables();
    if (p_subreport==NULL) return;

    hk_datasource* subds=p_subreport->datasource();
    if (subds==NULL) return;

    subds->clear_depending_fields();
    subds->set_depending_on(datasource(),false);

    list<hk_string>::iterator thisreport_ds=p_depending_thisreport_fields.begin();
    list<hk_string>::iterator subreport_ds;
    subreport_ds=p_depending_subreport_fields.begin();

    while (thisreport_ds!=p_depending_thisreport_fields.end())
    {

        subds->add_depending_fields((*subreport_ds),(*thisreport_ds),false);
        thisreport_ds++;subreport_ds++;
    }

}


hk_report* hk_reportsection::report(void)
{
    return p_report;
}


unsigned int    hk_reportsection::horizontal2relativ(unsigned int h)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::horizontal2relativ");
#endif
    if (p_report->sizetype()==hk_presentation::relative)
      return (unsigned int) ((h*teiler) / (p_report->designwidth()-p_report->relativ2horizontal(p_report->border_left()+p_report->border_right()))+0.5);
    else
      return (unsigned int) ((h*teiler) / (p_report->designwidth()- p_report->border_left()-p_report->border_right())+0.5);

}


unsigned int    hk_reportsection::vertical2relativ(unsigned int v)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::vertical2relativ");
#endif
    if (p_report->sizetype()==hk_presentation::relative)
     return (unsigned int) ((v*teiler) / (p_report->designheight()-p_report->relativ2vertical(p_report->border_top()+p_report->border_bottom()))+0.5);
    else
     return (unsigned int) ((v*teiler) / (p_report->designheight()-p_report->border_top()-p_report->border_bottom())+0.5);
}


unsigned int    hk_reportsection::relativ2horizontal(unsigned int r)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::relativ2horizontal");
#endif
    if (p_report->sizetype()==hk_presentation::relative)
      return (unsigned int)((r*(p_report->designwidth()-p_report->relativ2horizontal(p_report->border_left()+p_report->border_right())) )/teiler+0.5);
    else
      return (unsigned int) ((r*(p_report->designwidth()-p_report->border_left()-p_report->border_right()) )/teiler+0.5);

}


unsigned int    hk_reportsection::relativ2vertical(unsigned int r)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsection::relativ2vertical");
#endif
    if (p_report->sizetype()==hk_presentation::relative)
    return (unsigned int) ((r*(p_report->designheight()-p_report->relativ2vertical(p_report->border_top()+p_report->border_bottom())))/teiler+0.5);
   else
   return (unsigned int) ((r*(p_report->designheight()-p_report->border_top()-p_report->border_bottom()))/teiler+0.5);

}


bool hk_reportsection::set_presentationdatasource(long n, bool registerchange)
{
    bool res=hk_dsdatavisible::set_presentationdatasource(n,registerchange);
    if (res==false) return false;
    vector<hk_reportdata*>::iterator it=p_data.begin();
    while (it!=p_data.end())
    {
        (*it)->set_presentationdatasource(n,registerchange);
        it++;
    }
    return true;
}


hk_string   hk_reportsection::subreportname(void)
{
    return p_subreportstring;
}


hk_report*  hk_reportsection::subreport(void)
{
    return p_subreport;
}


void hk_reportsection::force_new_section(void)
{
    p_forcenewsection=true;

}


int     hk_reportsection::default_reportprecision(void)
{
    return p_default_precision;
}


void    hk_reportsection::set_default_reportprecision(int p, bool registerchange)
{
    p_default_precision=(p<0?-1:p);
    has_changed(registerchange);
}


bool    hk_reportsection::default_use_reportseparator(void)
{
    return p_default_use_thousandsseparator;
}


void    hk_reportsection::set_default_use_reportseparator(bool u, bool registerchange )
{
    p_default_use_thousandsseparator=u;
    has_changed(registerchange);
}


bool hk_reportsection::set_offset(unsigned int o,bool registerchange)
{
    if (p_report->sizetype()==hk_presentation::relative
        && o>10000) return false;
    p_offset=o;
    has_changed(registerchange);
    widget_specific_offset_changed();
    return true;

}


unsigned int hk_reportsection::offset(void)
{

    return p_offset;
}


void hk_reportsection::bulk_operation(hk_presentation::enum_bulkoperation bulkoperation)
{
    vector<hk_reportdata*>::iterator it=p_data.begin();
    while (it!=p_data.end())
    {
        switch (bulkoperation)
        {
            case hk_presentation::bulkfont:  (*it)->set_font(p_report->font());break;
            case hk_presentation::bulkforeground:  (*it)->set_foregroundcolour(p_report->foregroundcolour());break;
            case hk_presentation::bulkbackground:  (*it)->set_backgroundcolour(p_report->backgroundcolour());break;
        }
        ++it;
    }

}


void hk_reportsection::raise_widget(hk_reportdata* v)
{
    remove_data(v);
    p_data.insert(p_data.end(),v);
}


void hk_reportsection::lower_widget(hk_reportdata* v)
{
    remove_data(v);
    p_data.insert(p_data.begin(),v);

}

void hk_reportsection::sizetype_changed(void)
{
    vector<hk_reportdata*>::iterator it=p_data.begin();
    while (it!=p_data.end())
    {
        (*it)->sizetype_changed();
	++it;
    }
 if (p_report->sizetype()==hk_presentation::relative)
  {
// this means previous sizetype was hk_presentation::absolute
// and changed now to hk_presentation::relative
   p_offset=vertical2relativ(p_offset);
  }
else
  {
// this means previous sizetype was hk_presentation::relative
// and changed now to hk_presentation::absolute
   p_offset=relativ2vertical(p_offset);

  }



}

hk_reportdata* hk_reportsection::get_reportdatavisible(long nr)
{
    vector<hk_reportdata*>::iterator it=p_data.begin();
    while (it!=p_data.end())
    {
        if ( (*it)->presentationnumber()==nr) return (*it);
        it++;
    }


return NULL;
}

hk_reportdata* hk_reportsection::get_reportdatavisible(const hk_string& i)
{

    vector<hk_reportdata*>::iterator it=p_data.begin();
    while (it!=p_data.end())
    {
        if ( (*it)->identifier()==i) return (*it);
        it++;
    }


return NULL;


}


bool	hk_reportsection::presentationmode_changed(void)
{
   bool r=true;
   vector<hk_reportdata*>::iterator vit=p_data.begin();
    while (r&&vit!=p_data.end())
    {
        if (!(*vit)->presentationmode_changed())r=false;
        vit++;
    }
  return r;
}

hk_string hk_reportsection::sectioncountfunctionstring(void)
{
return p_sectioncountfunctionstring;
}



void hk_reportsection::set_print_section(bool p)
{
p_printsection=p;
}

bool hk_reportsection::print_section(void)
{
  return p_printsection;
}



