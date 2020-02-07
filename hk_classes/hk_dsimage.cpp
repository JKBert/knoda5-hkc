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
#include "hk_dsimage.h"
#include "hk_form.h"
#include "hk_url.h"


hk_dsimage::imagetypelisttype  hk_dsimage::p_imagetypefunctions;


class hk_dsimageprivate
{
 public:
 hk_dsimageprivate()
    {
    }
     hk_string p_value;
};

class hk_dsimagemodeprivate
{
 public:
 hk_dsimagemodeprivate()
    {
      p_zoom=0;
    }
	bool  operator=(const hk_dsimagemodeprivate* v)
     {
	if (!v) return false;
	p_path=v->p_path;
	p_zoom=v->p_zoom;
	if (p_localimage.data)
	{
	  delete[] p_localimage.data;
	  p_localimage.data=NULL;
	  p_localimage.length=0;
	}
	p_localimage.length=v->p_localimage.length;
	p_localimage.data=new char[p_localimage.length];
	memcpy(p_localimage.data,v->p_localimage.data,p_localimage.length);
        return true;
     }
  hk_string p_path;
  int p_zoom;
  struct_raw_data p_localimage;
};










hk_dsimage::hk_dsimage(hk_form*f ):hk_dsdatavisible(f)
{
    p_visibletype=hk_visible::image;
    p_private=new hk_dsimageprivate;
    p_viewdata=new hk_dsimagemodeprivate;
    p_designdata=new hk_dsimagemodeprivate;
}

hk_dsimage::~hk_dsimage()
{
  delete p_designdata;
  delete p_viewdata;
  delete p_private;
}

void hk_dsimage::savedata(ostream& s )
{
    hk_string tag="HK_DSIMAGE";
    start_mastertag(s,tag);
    hk_dsdatavisible::savedata(s);
    set_tagvalue(s,"IMAGEPATH",p_designdata->p_path);
    set_tagvalue(s,"ZOOM",p_designdata->p_zoom);
    hk_string d;
    if (data2hex(&p_designdata->p_localimage,d))
    {
    start_mastertag(s,"LOCALIMAGE");
       set_tagvalue(s,"LOCALIMAGEDATA",d);
       set_tagvalue(s,"LOCALIMAGESIZE",p_designdata->p_localimage.length);
    end_mastertag(s,"LOCALIMAGE");
    }
    end_mastertag(s,tag);
}

void hk_dsimage::loaddata(xmlNodePtr definition)
{
  hk_dsdatavisible::loaddata(definition);
  get_tagvalue(definition,"IMAGEPATH",p_designdata->p_path);
  get_tagvalue(definition,"ZOOM",p_designdata->p_zoom);
    hk_string d;
    xmlNodePtr buffer;
    if ((buffer=get_tagvalue(definition,"LOCALIMAGE")))
    {
      unsigned long size=0;
       if (get_tagvalue(buffer,"LOCALIMAGEDATA",d)
          &&get_tagvalue(buffer,"LOCALIMAGESIZE",size))
        {
      	  hex2data(d,size,&p_designdata->p_localimage);
      	  show_image();
        }
    }
  *p_viewdata=*p_designdata;

}

void hk_dsimage::set_path(const hk_string& p,bool registerchange)
{
 set_path(p,registerchange,false);
}

void hk_dsimage::set_path(const hk_string& p,bool registerchange,bool force_setting)
{
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_path=p;
    }
    p_viewdata->p_path=p;
    has_changed(registerchange);
    widget_specific_path_changed();

}

hk_string hk_dsimage::path(void) const
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_path;
return p_designdata->p_path;

}


void hk_dsimage::set_zoom(int s,bool registerchange)
{
  set_zoom(s,registerchange,false);
}


void hk_dsimage::set_zoom(int s,bool registerchange,bool force_setting)
{
    if (s<0)s=0;
    if (allow_datachanging(force_setting))
    {
    p_designdata->p_zoom=s;
    }
    p_viewdata->p_zoom=s;
    has_changed(registerchange);
    widget_specific_zoom_changed();

}

int hk_dsimage::zoom(void) const
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return p_viewdata->p_zoom;
return p_designdata->p_zoom;


}

void hk_dsimage::set_value(const hk_string& v)
{
p_private->p_value=v;
hk_dsdatavisible::set_value(v);
}

hk_string hk_dsimage::value(void)
{
  if (!column())
  {
    	hk_url n=p_private->p_value;
	if (n.directory().size()==0)
  	{
	   if (path().size()>0)
	    {
	     n=path()+"/"+n.url();
	    }

  	}
   return n.url();
  }
  else return hk_dsdatavisible::value();

}

hk_string hk_dsimage::value_at(unsigned long row)
{
hk_url n=hk_dsdatavisible::value_at(row);

if (n.directory().size()==0)
  {
   if (path().size()>0)
    {
     n=path()+"/"+n.url();
    }

  }

return n.url();
}

bool hk_dsimage::datasource_enable(void)
{
  p_private->p_value="";
  *p_viewdata=*p_designdata;
  return hk_dsdatavisible::datasource_enable();
}


bool hk_dsimage::load_localimage(const hk_string& filename,bool registerchange)
{
    has_changed(registerchange);
    return load_file(filename,localimage());
}

struct_raw_data* hk_dsimage::localimage()
{
if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    return &(p_viewdata->p_localimage);
return &(p_designdata->p_localimage);

}



bool hk_dsimage::get_postscript_image(const hk_string& /*file*/,hk_string& /*psimagedata*/)
{
   cerr <<"hk_dsimage::get_postscript_image() not implemented yet"<<endl;
   return false;
}

bool hk_dsimage::get_postscript_image(const struct_raw_data* /*data*/,hk_string& /*psimagedata*/)
{
   cerr <<"hk_dsimage::get_postscript_image() not implemented yet"<<endl;
   return false;

}

void hk_dsimage::add_postscriptimagetype(const hk_string& name,psimagedatatype* f)
{
    if (name.size()==0) return;
    typedef imagetypelisttype::value_type imagevalue;
    p_imagetypefunctions.insert(imagevalue(name,f));

}

