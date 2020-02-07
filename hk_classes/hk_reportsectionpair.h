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
#ifndef HK_REPORTSECTIONPAIR
#define HK_REPORTSECTIONPAIR
#include "hk_class.h"
#include "hk_string.h"
#include "hk_presentation.h"
using namespace std;

class hk_reportsection;
class hk_report;
/**
 *contains a header and a footer section
 *@short contains a header and a footer section
 *@version $Revision: 1.13 $
 *@author Horst Knorr (hk_classes@knoda.org)
 */
class hk_reportsectionpair :public hk_class
{
    friend class hk_reportsection;
    friend class hk_report;
    public:
virtual        ~hk_reportsectionpair(void);
/**
 * returns the headersection or NULL if non is set
 */
        hk_reportsection* headersection(void);
/**
 *returns the footersection or NULL if non is set
 */
        hk_reportsection* footersection(void);
/**
 * sets the sections
 *@param header if true a header section will be created else destroyed
 *@param footer if true a footer section will be created else destroyed
 */
        void set_sections(bool header=true,bool footer=true);
/**
 *brings the section into their initial state to be header or footer section
 */
        void init_sections(void);
/**
 *the column name which should be set for grouping of the reportsections
 */
        void set_columnname(const hk_string& n);
        void set_ascending_order(bool asc, bool registerchange=true);
        bool ascending_order(void) const;
        hk_string columnname(void) const;
        void reset(void);
/**
 *calls the reportsections clear_countingfields
 */
        void clear_countingfields(void);
        virtual void  savedata(ostream& s,bool userdefined=false);
        virtual void  loaddata(xmlNodePtr definition,bool userdefined=false);
        virtual bool        set_presentationdatasource(long n, bool registerchange=true);
        long        presentationdatasource(void);
        void neutralize_definition(bool registerchange);
        void bulk_operation(hk_presentation::enum_bulkoperation bulkoperation);

    protected:
        hk_reportsectionpair(hk_report*);

        void    remove_section(hk_reportsection*);

    private:
        hk_report* p_report;
        hk_reportsection* p_header;
        hk_reportsection* p_footer;
        hk_string p_name;
        long int p_presentationdatasource;
        bool p_ascending;

};
#endif
