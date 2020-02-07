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
//$Revision: 1.3 $
#include "hk_module.h"
#include "hk_database.h"

class hk_moduleprivate
{
public:
hk_moduleprivate()
	{
	 p_has_changed=false;
	 p_block_has_changed=false;
	}
hk_string p_script;
hk_string p_name;
hk_string p_loadname;
bool p_has_changed;
bool p_block_has_changed;

};

hk_module::hk_module():hk_dbvisible()
{
p_private=new hk_moduleprivate;
}



hk_module::~hk_module()
{
delete p_private;
}

void hk_module::set_script(const hk_string& s,bool registerchange)
{
 p_private->p_script=s;
 if (registerchange )
  {
   if (!p_private->p_block_has_changed)
      p_private->p_has_changed=true;
   widget_specific_has_changed();
  }
}

bool hk_module::has_changed()
{
  return p_private->p_has_changed;
}

void hk_module::set_name(const hk_string& n)
{
  p_private->p_name=n;
}

hk_string hk_module::name(void)
{
  return p_private->p_name;
}

hk_string hk_module::script()
{
  return p_private->p_script;
}


bool hk_module::load_module(const hk_string& n)
{

     if (database()==NULL)
    {
        show_warningmessage(hk_translate("Module error: No database defined!"));
        return false;

    }

    if (has_changed())
    {
        save_module();
        reset_has_changed();
    }
    if (n.size()>0) set_name(n);
    if (name().size()==0) if (!ask_name())
      {
       return false;
      }
    xmlNodePtr cur =database()->xmlload(name(),ft_module);
    if (!cur)
    {
     return false;
    }
    p_private->p_loadname=n;
//     if (res.size()==0) show_warningmessage(hk_translate("Bug: form is empty!"));
//     if ( res.find("encoding=\"UTF-8\"")<res.size() ) res=u2l(res);
    loaddata(cur);
    reset_has_changed();

#ifdef HK_DEBUG
    hkdebug("hk_form::load_module ENDE");
#endif
return true;

}

bool hk_module::save_module(const hk_string& n,bool ask)
{
    if (database()==NULL)
    {
        show_warningmessage(hk_translate("Module error: No database defined!"));
        return false;

    }
    if (n.size()>0) set_name(n);
    hk_database::enum_storage storage=hk_database::st_overwrite;
    reset_has_changed();
    if (name().size()==0) if (!ask_name())return false;
    if (p_private->p_loadname==name()) storage=hk_database::st_storechanges;
    if (database()->storagemode(ft_module)==hk_database::local)
    {
      ofstream* p_save=database()->savestream(storage,name(),ft_module,ask,true,false);
      if (p_save==NULL) return false;
      savedata(*p_save);
      p_save->close();
      delete p_save;
    }
    else
    { // store central
       STRINGSTREAM*  p_save;
      p_save=database()->savestringstream(ft_module);
      if (p_save==NULL) return false;
      savedata(*p_save);
      hk_string s=p_save->str();
      database()->save(storage,s,name(),ft_module,ask);
      delete p_save;
    }
    p_private->p_loadname=name();
    reset_has_changed();
    return true;
}



void hk_module::savedata(ostream& s )
{
    start_mastertag(s,"MODULE");
    set_tagvalue(s,"SCRIPT",p_private->p_script);
   end_mastertag(s,"MODULE");
   p_private->p_has_changed=false;

}


void hk_module::loaddata(xmlNodePtr definition)
{
    xmlNodePtr r=get_tagvalue(definition,"MODULE");
    hk_string s;
    if (!get_tagvalue(r,"SCRIPT",s)) cerr<<"script not found r="<<r<<endl;
    p_private->p_script=s;
    p_private->p_has_changed=false;

}

void hk_module::reset_has_changed(void)
{
  p_private->p_has_changed=false;
}



bool hk_module::ask_name(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_module::ask_name");
#endif
    hk_string text=hk_translate("Please enter the name of the module");
    hk_string res=show_stringvaluedialog(text);
    if (res.size()==0) return false;

    set_name(res);

    return true;

}



void hk_module::set_block_has_changed(bool b)
{
  p_private->p_block_has_changed=b;
}

bool hk_module::block_has_changed(void)
{
  return p_private->p_block_has_changed;
}


