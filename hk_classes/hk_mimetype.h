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
#ifndef HK_MIMETYPE
#define HK_MIMETYPE
#include "hk_class.h"
/**
 *@short hk_mimetype tries to detect the mimetype of a file or a memory block
 *@version $Revision: 1.2 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 */


class hk_mimetype : public hk_class
{
public:
/**
*in most cases you should use the static members 'mimetype' directly, so there should be no need 
*to create this object 
*/
 hk_mimetype();
 /**
 *returns the mimetype of the memory block "data"
 */
static hk_string mimetype(struct_raw_data* data);
 /**
 *returns the mimetype of the file "filename"
 */
static hk_string mimetype(const hk_string& filename);
  /**
 *you can set the mimetype database file which includes the magic definitions, if not set '/usr/share/misc/magic' is used
 */

static void set_mimetypedatabasefile(const hk_string&);
  /**
 *returns the used mimetype database filename, an empty string if non has set
 */
static hk_string mimetypedatabasefile();
protected:


private:
static hk_string p_mimetypedatabasefile;
static void init(void);
};

#endif //HK_MIMETYPE
