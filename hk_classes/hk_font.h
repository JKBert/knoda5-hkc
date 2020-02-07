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
#ifndef HK_FONT
#define HK_FONT
#include "hk_class.h"
#include "hk_url.h"
class hk_fontprivate;
class hk_encodingtab;
typedef unsigned int HK_CHARSIZE;
/**
 *@short represents a font
 *@version $Revision: 1.16 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 */


class hk_font : public hk_class
{
public:
  hk_font();
  hk_font(const hk_string& fontname,int fontsize);
  hk_font(const hk_font&);
  virtual ~hk_font();
  void set_font(const hk_string& fontname,int fontsize);
  void set_font(const hk_font&);
  hk_string fontname(void) const;
  int fontsize(void) const;
  void set_bold(bool);
  bool bold(void) const;
  void set_italic(bool);
  bool italic(void) const;
  bool operator=(const hk_font&);
  bool operator==(const hk_font&);
  bool operator!=(const hk_font&);
  hk_string psfontname(void);
  static void set_defaultfont(const hk_string& d,int s);
  static hk_string defaultfontname(void);
  static int defaultfontsize(void);
static list<hk_string>* fontlist();
/**
*fixedwidth in zmm
**/
  bool text2lines(int fixedwidth,list<hk_string>* l,const hk_string& text);

virtual void savedata(ostream& s );
virtual void loaddata(xmlNodePtr definition);

void set_encodingtab(hk_encodingtab* t);
hk_encodingtab* encodingtab(void) const;
hk_url fontfile(void);
hk_string psfontembeddefinition(void);
 HK_CHARSIZE char_ascender_size();
 HK_CHARSIZE char_descender_size();
 bool is_scaleable(void) ;
 list<int>* available_sizes() ;


void register_string(const hk_string&);

protected:

void set_fontface(void);
private:
bool is_equal(const hk_font&);
void init(void);
hk_string p_fontname;
int	  p_fontsize;
bool p_italic;
bool p_bold;
hk_fontprivate* p_private;
static hk_string p_defaultfont;
static int   p_defaultfontsize;
static int   p_counter;

static list<hk_string> p_fontlist;

};

ostream& operator<<(ostream&,hk_font&);
ostream& operator<<(ostream&,const hk_font&);


class hk_encodingtabprivate;
class hk_encodingtab: public hk_class
{
 public:
 hk_encodingtab();
 ~hk_encodingtab();
 void register_unicode(HK_CHARSIZE code);
 void register_unicode(HK_CHARSIZE code,const hk_string & glyphname);
 HK_CHARSIZE max_nr() const;
 HK_CHARSIZE local (HK_CHARSIZE unicode);
 hk_string   glyphname (HK_CHARSIZE unicode);
 HK_CHARSIZE unicode(HK_CHARSIZE local);
 void reset();
 void reset_has_changed(void);
 bool has_changed(void) const;
  
 private:
 hk_encodingtabprivate* p_private;
};

#endif
