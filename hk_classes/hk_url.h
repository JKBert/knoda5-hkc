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
//$Revision: 1.2 $
#ifndef HK_URL
#define HK_URL

#include "hk_class.h"
#include "hk_string.h"
class hk_url: public hk_class
{
 public:
 hk_url();
 hk_url(const hk_string&);
 bool operator=(const hk_url& url);
 bool operator=(const hk_string& url);
 bool is_valid() const {return p_isvalid;}
 bool is_local() const {return p_islocal;}
 hk_string url(void) const {return p_url;}
 hk_string filename(void) const {return p_filename;}
 hk_string directory(void) const { return p_directory;}
 hk_string extension(void) const { return p_extension;}
 private:
 void init();
 bool parse_url();
 bool set_url(const hk_string&);
 bool p_isvalid;
 bool p_islocal;
 hk_string p_url;
 hk_string p_filename;
 hk_string p_directory;
 hk_string p_extension;
};

#endif // HK_URL
