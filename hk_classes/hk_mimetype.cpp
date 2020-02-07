// ****************************************************************************
// copyright (c) 2000-2006 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_mimetype.h"
// #include <magic.h>
#include <sys/param.h>
#include <unistd.h>
#include <errno.h>

const hk_string defaultmimetype="application/octet-stream";

// magic_set *magic=NULL;

hk_string hk_mimetype::p_mimetypedatabasefile;

hk_mimetype::hk_mimetype()
{

}

void hk_mimetype::init(void)
{
/* if (magic)
 {
  magic_close(magic);
  magic=NULL;
 }
 magic=magic_open(MAGIC_MIME);
 if (!magic)
  {
    cerr<<"error creating mimetype handle:"<<strerror(errno)<<endl;
    return;
  }
 hk_string magicfile;
 char* usermagic=NULL;
 if ((usermagic=getenv("MAGIC")))
   magicfile=usermagic;
   if (p_mimetypedatabasefile.size()>0)
     magicfile=p_mimetypedatabasefile;
 if (magic_load(magic,magicfile.size()>0?magicfile.c_str():NULL)==-1)
  {
   cerr << "error loading mimetype database:'"<<magicfile<<"'"<<endl<<"Errormessage:"<<magic_error(magic)<<endl;
   magic_close(magic);
   magic=NULL;
   return;
  }*/
  
}


hk_string hk_mimetype::mimetype(struct_raw_data* data)
{
if (!data) return defaultmimetype;

 hk_string result=defaultmimetype;
/* if (!magic) init();
 if (!magic) return defaultmimetype;
 const char* type=magic_buffer(magic,data->data,data->length); 
 if (type)
  result=type;
 else
 {
  cerr <<"Error detecting mimetype of buffer "<<endl<<"Errormessage:"<<magic_error(magic)<<endl;
 }*/
 return result;
}


hk_string hk_mimetype::mimetype(const hk_string& filename)
{
 hk_string result=defaultmimetype;
/* if (!magic) init();
 if (!magic) return defaultmimetype;
 const char* type=magic_file(magic,filename.c_str()); 
 if (type)
  result=type;
 else
 {
  cerr <<"Error detecting mimetype of file '"<<filename<<"'"<<endl<<"Errormessage:"<<magic_error(magic)<<endl;
 }*/
 return result;

}


void hk_mimetype::set_mimetypedatabasefile(const hk_string& mt)
{
p_mimetypedatabasefile=trim(mt);
init();
}



hk_string hk_mimetype::mimetypedatabasefile()
{
  return p_mimetypedatabasefile;
}

