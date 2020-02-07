// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_postgresqlclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_POSTGRESSQLDATASOURCE
#define HK_POSTGRESSQLDATASOURCE
#include <libpq-fe.h>
#include <hk_storagedatasource.h>

class hk_column;
class hk_postgresqldatabase;
class hk_presentation;

class hk_postgresqldatasource:public hk_storagedatasource
{
    friend class hk_postgresqldatabase;
    public:
    hk_postgresqldatabase* postgresdatabase(void) const { return p_postgresqldatabase;}

    protected:
        hk_postgresqldatasource(hk_postgresqldatabase* d,hk_presentation* p);
        virtual ~hk_postgresqldatasource();

        bool driver_specific_enable(void);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_create_columns(void);
        virtual bool driver_specific_insert_data(void);
        virtual void before_source_vanishes(void);
        virtual void driver_specific_before_drop_table(void);
        virtual hk_column* driver_specific_new_column(void);
        virtual bool driver_specific_batch_enable(void);
        virtual bool driver_specific_batch_disable(void);
        virtual bool driver_specific_batch_goto_next(void);
        hk_datasource* p_tempdatasource;

    private:
        void add_data(unsigned int numfields);
        hk_postgresqldatabase* p_postgresqldatabase;
        PGresult *p_result;
        unsigned long int zeile;
}


;

unsigned char * escapeBytea(unsigned char *bintext, size_t binlen, size_t *bytealen);
unsigned char * unescapeBytea(unsigned char *strtext, size_t *retbuflen);
#endif
