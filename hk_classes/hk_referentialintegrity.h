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
#ifndef HK_REFERENTIALINTEGRITY
#define HK_REFERENTIALINTEGRITY
#include "hk_presentation.h"

class hk_referentialintegrity:public hk_presentation
{
  public:
  hk_referentialintegrity(void);
  virtual ~hk_referentialintegrity(void);
  void load_referentialintegrity();
  bool save_referentialintegrity();
};

#endif //HK_REFERENTIALINTEGRITY
