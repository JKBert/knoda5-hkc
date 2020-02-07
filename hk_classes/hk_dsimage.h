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
#ifndef HK_DSIMAGE
#define HK_DSIMAGE
#include "hk_dsdatavisible.h"
#include "hk_string.h"
#include <map>

class hk_dsimageprivate;
class hk_dsimagemodeprivate;

class hk_dsimage : public hk_dsdatavisible
{
public:
hk_dsimage(hk_form* form=NULL);
virtual ~hk_dsimage();
void set_path(const hk_string& p,bool registerchange=true);
void set_path(const hk_string& p,bool registerchange,bool force_setting);
hk_string path(void) const;
virtual void savedata(ostream& s );
virtual void loaddata(xmlNodePtr definition);
	virtual void set_value(const hk_string&);
	virtual hk_string value(void);
        virtual hk_string value_at(unsigned long row);
/*
zoom the image in percent. Values of 0 or smaller than 0 means 'fit to size',
100 (%) is the original size
*/
void set_zoom(int,bool registerchange=true);
void set_zoom(int,bool registerchange,bool force_setting);
int  zoom(void) const;

virtual bool load_localimage(const hk_string& filename,bool registerchange=true);
struct_raw_data* localimage();

virtual void show_image(void){}

static bool get_postscript_image(const hk_string& file,hk_string& psimagedata);
static bool get_postscript_image(const struct_raw_data* data,hk_string& psimagedata);
static void add_postscriptimagetype(const hk_string& name,psimagedatatype* f);

protected:

virtual void widget_specific_path_changed() {}
virtual void widget_specific_zoom_changed() {}
        virtual bool datasource_enable(void);

private:
	hk_dsimagemodeprivate* p_designdata;
	hk_dsimagemodeprivate* p_viewdata;
	hk_dsimageprivate* p_private;
	
	typedef  map<hk_string,psimagedatatype*,less<hk_string> >  imagetypelisttype ;
        static    imagetypelisttype  p_imagetypefunctions;


};
#endif // hk_dsimage
