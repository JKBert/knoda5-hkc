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
#include "hk_referentialintegrity.h"
#include "hk_database.h"

hk_referentialintegrity::hk_referentialintegrity(void):hk_presentation()
{
p_presentationtype=hk_presentation::referentialintegrity;
set_name("database");
}

hk_referentialintegrity::~hk_referentialintegrity()
{

}


void hk_referentialintegrity::load_referentialintegrity()
{
 if (!database()) return;
 xmlNodePtr res =database()->xmlload(name(),ft_referentialintegrity);
 if (res)
    loaddata(res);

}

bool hk_referentialintegrity::save_referentialintegrity()
{
 if (!database()) return false;
 STRINGSTREAM* stream= new STRINGSTREAM();
  savedata(*stream);
 database()->save((*stream).str(),name(),ft_referentialintegrity,false);
 return true;
}



