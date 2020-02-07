// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Edited by Anirban Biswas <utpal@cal2.vsnl.net.in>
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_POSTGRESQLCOLUMN
#define HK_POSTGRESQLCOLUMN
#include <hk_storagecolumn.h>
class hk_postgresqldatasource;

class hk_postgresqlcolumn:public hk_storagecolumn

{
    friend class hk_postgresqldatasource;
    friend class hk_postgresqltable;
    public:
        unsigned int fieldnr(void);
        void     fieldnr(unsigned int f);
    protected:
        hk_postgresqlcolumn(hk_postgresqldatasource* ds, const hk_string& TRUESTRING,const hk_string& FALSESTRING);
        virtual ~hk_postgresqlcolumn(void);
        bool    driver_specific_asstring(const hk_string& s);
        const char* driver_specific_transformed_asstring_at(unsigned long position);
        virtual bool    driver_specific_asbinary(const char* b);
    private:
        hk_string p_asstringbuffer;

        hk_postgresqldatasource*    p_postgresqldatasource;
        hk_string p_autoincdefault;

} ;
#endif
