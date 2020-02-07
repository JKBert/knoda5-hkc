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
#include "hk_reportsectionpair.h"
#include "hk_report.h"
#include "hk_reportsection.h"

hk_reportsectionpair::hk_reportsectionpair(hk_report* report)
:hk_class()
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_reportsectionpair::hk_reportsectionpair");
#endif
    p_report=report;
    p_header=NULL;
    p_footer=NULL;
    p_ascending=true;
    p_presentationdatasource=(p_report==0?-1:p_report->presentationdatasource());
    set_sections();

}


hk_reportsectionpair::~hk_reportsectionpair(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsectionpair::~hk_reportsectionpair",p_name);
#endif
    if (p_report!=NULL)
        p_report->remove_sectionpair(this);
    delete p_header;
    delete p_footer;
}


hk_reportsection* hk_reportsectionpair::headersection(void)
{
    return p_header;
}


hk_reportsection* hk_reportsectionpair::footersection(void)
{
    return p_footer;

}


void hk_reportsectionpair::remove_section(hk_reportsection* s)
{
    if (p_header==s) p_header=NULL;
    else
    if (p_footer==s) p_footer=NULL;

}


void hk_reportsectionpair::set_sections(bool header,bool footer)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsectionpair::set_sections");
#endif
    if (header)
    {
        if (p_header==NULL)
        {
            p_header=p_report->widget_specific_new_section();
            if (p_header!=NULL)
            {
                p_header->p_pair=this;
            }
        }
    }
    else if (p_header!=NULL)
    {
        delete p_header;
        p_header=NULL;
    }

    if (footer)
    {
        if (p_footer==NULL)
        {
            p_footer=p_report->widget_specific_new_section();
            if (p_footer!=NULL)
            {
                p_footer->p_pair=this;
            }
        }
    }
    else if (p_footer!=NULL)
    {
        delete p_footer;
        p_footer=NULL;
    }
    init_sections();
}


void hk_reportsectionpair::init_sections(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsectionpair::init_sections");
#endif
    if (p_header!=NULL)
    {
        p_report->init_section(p_header);
        p_header->set_unique(true,false,false);
        p_header->set_columnname(p_name);

    }
    if (p_footer!=NULL)
    {
        p_report->init_section(p_footer);
        p_footer->set_unique(true,true,false);
        p_footer->set_columnname(p_name);

    }

}


void hk_reportsectionpair::set_columnname(const hk_string& n)
{
    p_name=n;
    if (p_header!=NULL) p_header->set_columnname(n);
    if (p_footer!=NULL) p_footer->set_columnname(n);

}


hk_string hk_reportsectionpair::columnname(void) const
{
    return p_name;

}


void hk_reportsectionpair::set_ascending_order(bool asc,bool registerchange)
{
    p_ascending=asc;
    p_report->has_changed(registerchange);

}


bool hk_reportsectionpair::ascending_order(void) const
{
    return p_ascending;

}


void hk_reportsectionpair::reset(void)
{
    if (p_header!=NULL) p_header->reset();
    if (p_footer!=NULL) p_footer->reset();

}


void hk_reportsectionpair::clear_countingfields(void)
{
    if (p_header!=NULL) p_header->clear_countingfields();
    if (p_footer!=NULL) p_footer->clear_countingfields();

}


void  hk_reportsectionpair::savedata(ostream& s,bool userdefined)
{
    start_mastertag(s,"HK_REPORTSECTIONPAIR");
    set_tagvalue(s,"PAIRCOLUMNNAME",p_name);
    set_tagvalue(s,"ASCENDINGORDER",p_ascending);
    start_mastertag(s,"HEADERSECTION");
    if (p_header!=NULL) p_header->savedata(s,userdefined);
    end_mastertag(s,"HEADERSECTION");
    start_mastertag(s,"FOOTERSECTION");
    if (p_footer!=NULL) p_footer->savedata(s,userdefined);
    end_mastertag(s,"FOOTERSECTION");
    end_mastertag(s,"HK_REPORTSECTIONPAIR");

}


void  hk_reportsectionpair::loaddata(xmlNodePtr definition,bool userdefined)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsectionpair::loaddata");
#endif
    xmlNodePtr node;
    hk_string sbuffer;
    if ((node=get_tagvalue(definition,"HEADERSECTION",sbuffer,1,mastertag)))
    {
        if (p_header!=NULL) p_header->loaddata(node,userdefined);
    }
    if ((node=get_tagvalue(definition,"FOOTERSECTION",sbuffer,1,mastertag)))
    {
        if (p_footer!=NULL) p_footer->loaddata(node,userdefined);
    }
    if (get_tagvalue(definition,"PAIRCOLUMNNAME",sbuffer))set_columnname(sbuffer);
    get_tagvalue(definition,"ASCENDINGORDER",p_ascending);

}


bool hk_reportsectionpair::set_presentationdatasource(long n, bool /*registerchange*/)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsectionpair::set_presentationdatasource");
#endif
    p_presentationdatasource=n;
    if (p_header!=NULL) p_header->set_presentationdatasource(n);
    if (p_footer!=NULL) p_footer->set_presentationdatasource(n);
    return true;
}


long    hk_reportsectionpair::presentationdatasource(void)
{
    return p_presentationdatasource;
}


void hk_reportsectionpair::neutralize_definition(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_reportsectionpair::neutralize_definition");
#endif
    if (p_header!=NULL) p_header->neutralize_definition(registerchange);
    if (p_footer!=NULL) p_footer->neutralize_definition(registerchange);
}


void hk_reportsectionpair::bulk_operation(hk_presentation::enum_bulkoperation bulkoperation)
{
    if (p_header!=NULL) p_header->bulk_operation(bulkoperation);
    if (p_footer!=NULL) p_footer->bulk_operation(bulkoperation);

}
