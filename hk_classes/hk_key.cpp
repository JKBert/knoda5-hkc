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
#include "hk_key.h"

class hk_keyprivate
{
 public:
 hk_keyprivate()
 {
 p_key=0;
 p_keystate=0;
 p_accept_key=true;
 
 }
 int p_key,p_keystate;
 bool p_accept_key;
 hk_string p_keytext;
 

};


 hk_key::hk_key():hk_class()
 {
   p_private=new hk_keyprivate();
    p_private->p_key=0;
    p_private->p_keystate=0;
    p_private->p_keytext="";
    p_private->p_accept_key=true;
  
 }
 
 hk_key::hk_key(const hk_key& k):hk_class()
 {
  p_private=new hk_keyprivate();
  *this=k;
 
 
 }
 
 

  hk_key::hk_key(int keycode,int keystate,const hk_string& keytext):hk_class()
  {
    p_private=new hk_keyprivate();
    p_private->p_key=keycode;
    p_private->p_keystate=keystate;
    p_private->p_keytext=keytext;
    p_private->p_accept_key=true;
  }
  
  hk_key::~hk_key()
  {
 
  delete p_private;
  }

  int hk_key::state() const
  {
    return p_private->p_keystate;
  }
  
  int hk_key::key() const
  {
   return p_private->p_key;
  }
  
  hk_string hk_key::text() const
  {
   return p_private->p_keytext;
  }
  
  
  bool hk_key::operator=(const hk_key&v)
  {
   

    p_private->p_key=v.p_private->p_key;
    p_private->p_keystate=v.p_private->p_keystate;
    p_private->p_keytext=v.p_private->p_keytext;
    p_private->p_accept_key=v.p_private->p_accept_key;
    return true;
  }
  
  
  
  bool hk_key::operator==(const hk_key& v)
  {
   return (p_private->p_key==v.p_private->p_key
          && p_private->p_keystate==v.p_private->p_keystate);
  }
  
  
  bool hk_key::operator!=(const hk_key& v)
  {
   return (p_private->p_key!=v.p_private->p_key
          || p_private->p_keystate!=v.p_private->p_keystate);
  
  }

  
  bool hk_key::accept_key() const
  {
    return p_private->p_accept_key;
  }
  
  
  void hk_key::set_accept_key(bool a)
  {
  p_private->p_accept_key=a;
  }
