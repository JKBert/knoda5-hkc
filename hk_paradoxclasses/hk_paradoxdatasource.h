// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_paradoxclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_PARADOXDATASOURCE
#define HK_PARADOXDATASOURCE
#include "hk_storagedatasource.h"
#include "hk_paradoxcolumn.h"
#include <list>
#include <vector>
#include <algorithm>
extern "C"
{
#include "pxlib/paradox.h"
}
class hk_paradoxdatabase;
class hk_paradoxconnection;
class hk_presentation;

class hk_paradoxdatasource:public hk_storagedatasource
{
    friend class hk_paradoxdatabase;

    public:

    protected:
        hk_paradoxdatasource(hk_paradoxdatabase* d,hk_presentation* p);
        virtual ~hk_paradoxdatasource();
        bool driver_specific_enable(void);
        virtual list<hk_column*>* driver_specific_columns(void);
        virtual bool driver_specific_create_columns(void);
        virtual hk_column* driver_specific_new_column(void);
        virtual bool driver_specific_batch_enable(void);
        virtual bool driver_specific_batch_disable(void);
        virtual bool driver_specific_batch_goto_next(void);
        virtual bool datasource_open(void);
	virtual bool datasource_fetch_next_row(void);
	virtual bool datasource_close(void);
        hk_paradoxdatabase* p_paradoxdatabase;
    pxdoc_t* p_paradoxhandle;
    pxhead_t* p_paradoxheader;
}

;
hk_string lasterror();
void errorhandler(pxdoc_t *p, int error, const char *str, void *data);

#endif
