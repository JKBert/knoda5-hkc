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
#ifndef HK_DSMODEVISIBLE
#define HK_DSMODEVISIBLE
#include "hk_dsvisible.h"
class hk_dsgrid;

class hk_dsmodevisibleprivate;
class hk_dsmodevisible: public hk_dsvisible
{

public:
hk_dsmodevisible(hk_presentation* presentation=NULL);
virtual ~hk_dsmodevisible();
        enum            enum_mode {designmode,viewmode,filtermode};
/**
 * if mode is design then you can define or alter a presentation else
 * the data will be shown
 */
        virtual bool            set_mode(enum_mode s);
        virtual void set_designmode(void);
        virtual void set_viewmode(void);

        enum_mode       mode(void) const;
	bool has_changed(void) const;
	void has_changed(bool);
	void  set_block_has_changed(bool);
	bool  block_has_changed(void) const;

virtual	void reset_has_changed(void);
/**
*returns true if this object is new created or false if it has been loaded
*/
        bool is_new() const{return p_is_new;}
protected:
        virtual void    set_has_changed(enum_has_changed forcesetting=standard);
        virtual void    widget_specific_modechanges(enum_mode){}
        virtual bool    widget_specific_before_modechanges(enum_mode);
        bool p_is_new;

private:
	hk_dsmodevisibleprivate* p_private;


};




#endif //HK_DSMODEVISIBLE
