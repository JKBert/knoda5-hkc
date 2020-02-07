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
#include "hk_font.h"
#include <fontconfig.h>
#include <ft2build.h>
#include <vector>
#include FT_FREETYPE_H
#include FT_TRUETYPE_TABLES_H
#include FT_GLYPH_H
#include FT_BBOX_H
#ifndef FC_FONTFORMAT
 #define FC_FONTFORMAT	    "fontformat"	/* String */
#endif

hk_string hk_font::p_defaultfont="Courier";
int hk_font::p_defaultfontsize=12;
int hk_font::p_counter=0;
list<hk_string> hk_font::p_fontlist;

const int RESOLUTION=1200;
class hk_fontprivate
{
public:
FT_Library  ftlibrary;
FT_Face  p_fontface;
    FcFontSet	*p_fontset;
bool p_isdirty;
hk_encodingtab* p_encodingtab;
hk_url p_fontfile;
list<int> p_available_sizes;
hk_fontprivate()
 {
  ftlibrary=NULL;
  p_fontface=NULL;
  p_encodingtab=NULL;
  p_fontset=0;
  p_isdirty=true;

 }
void init_library(void)
  {
    if (!ftlibrary)
    {
      FT_Error error=FT_Init_FreeType( &(ftlibrary) );
      if (error)
      {
       cerr <<"error init freetype"<<endl;
       return ;
      }
    } 
  }
  
hk_string pfa(hk_font*);
hk_string pfb(hk_font*);
hk_string ttf(hk_font*);
};


hk_string hk_fontprivate::pfa(hk_font* f)
{
  hk_string result;
  ifstream  font(f->fontfile().url().c_str());
  if (!font) return "";
  
  char c;
  while (font)
  {
    font.get(c);
    result+=c;
  }
  
  return result;
}

hk_string hk_fontprivate::pfb(hk_font* f)
{
  hk_string result;
  ifstream  font(f->fontfile().url().c_str(),std::ios::in|std::ios::binary);
  if (!font) return "";
//   cerr <<"FONT:"<<f->fontname()<<"  get pfb:"<<f->fontfile().url()<<endl;
   char c;
    unsigned int segmentsize=0;
  
  while (font.get(c))
  {
  if (c!=(char)128)
   {
     cerr <<"Error: "<<f->fontfile().url()<<" is not a pfb file!"<<endl;
     return "";
   }

  font.get(c);
    switch (c)
    {
     case 1: 	{// ASCII segment
     		  	unsigned long int o=0;
     			c=0;
     			if (!font.get(c))
     			{
     			  cerr <<"error calling get!"<<endl;
     			  return "";
     			}
     			segmentsize=(unsigned char)c;
//      			cerr <<"ascii segment:"<<segmentsize;
     			if (!font.get(c))
     			{
     			  cerr <<"error calling get!"<<endl;
     			  return "";
     			}
     			o=(unsigned char)c*256;
     			segmentsize+=o;
//      			cerr <<" "<<c;
     			
     			if (!font.get(c))
     			{
     			  cerr <<"error calling get!"<<endl;
     			  return "";
     			}
     			o=(unsigned char)c*65536;
//      			cerr <<" "<<c;
     			segmentsize+=o;
     			
     			if (!font.get(c))
     			{
     			  cerr <<"error calling get!"<<endl;
     			  return "";
     			}
     			o=(unsigned char)c*65536*256;
//      			cerr <<" "<<c;
     			segmentsize+=o;
//      			cerr <<" ergibt Segmentgröße:"<<segmentsize<<endl; 
     			o=0;
     			while (o<segmentsize)
     			{
     			  font.get(c);
     			  result+=c;
     			  ++o;
     			}
//      			cerr <<"Ascii segment=#"<<result<<"#"<<endl;
     		  break;
     		}
     case 2: 	{// Integer segment
     			unsigned long int o=0;
     			if (!font.get(c))
     			{
     			  cerr <<"error calling get!"<<endl;
     			  return "";
     			}
     			segmentsize=(unsigned char)c;
//      			cerr <<"Integer segment:"<<segmentsize;
     			
     			if (!font.get(c))
     			{
     			  cerr <<"error calling get!"<<endl;
     			  return "";
     			}
     			o=(unsigned char)c*256;
     			segmentsize+=o;
//      			cerr <<" "<<c;
     			
     			if (!font.get(c))
     			{
     			  cerr <<"error calling get!"<<endl;
     			  return "";
     			}
     			o=(unsigned char)c*65536;
//      			cerr <<" "<<c;
     			segmentsize+=o;
     			
     			if (!font.get(c))
     			{
     			  cerr <<"error calling get!"<<endl;
     			  return "";
     			}
     			o=(unsigned char)c*65536*256;
//      			cerr <<" "<<c;
     			segmentsize+=o;
//      			cerr <<" ergibt Segmentgröße:"<<segmentsize<<endl; 
     			o=0; int offset=40;
     			while (o<segmentsize)
     			{
     			  font.get(c);
     			  result+=bin2hex(c);
     			  ++o;
     			  if (offset==0) 
     			    {
     			      result+="\n";
     			      offset=40;
     			    }
     			    else --offset;
     			}
//      			cerr <<"Integer segment=#"<<result<<"#"<<endl;
     		  break;
     		}
     case 3: 	{// EOF segment
     		   return result;
     		  break;
     		}
     
     default:	{
     		  cerr <<f->fontfile().url()<<" error in pfb file! found char:"<<bin2hex(c)<<endl;
     		  return "";
     		}
    
    }
  
  
  }
  
  return result;
}

hk_string hk_fontprivate::ttf(hk_font* f)
{
  hk_string result;
  ifstream  font(f->fontfile().url().c_str());
  if (!font) return "";
  int emunits=p_fontface->units_per_EM;
  result="%!PS-TrueTypeFont\n11 dict begin\n/FontName /"+f->psfontname()+" def\n/Encoding /ISOLatin1Encoding where {pop ISOLatin1Encoding} {StandardEncoding} ifelse def\n/PaintType 0 def\n/FontMatrix [1 0 0 1 0 0] def\n/FontBBox[";
  result+=longint2string(p_fontface->bbox.xMin*1000/emunits)+" "+longint2string(p_fontface->bbox.yMin*1000/emunits)+" "+longint2string(p_fontface->bbox.xMax*1000/emunits)+" "+longint2string(p_fontface->bbox.yMax*1000/emunits)+"] def\n";
  result+="/FontType 42 def\n/FontInfo 8 dict dup begin\n/FamilyName ("+f->fontname()+") def\nend readonly def\n/sfnts [";
  char c;
  int offset=40;
  int count=0;
  while (font)
  {
    if (count==0) result+="\n<";
    font.get(c);
    result+=bin2hex(c);
    if (offset==0) 
     {
       result+="\n";
       offset=40;
     }
       else --offset;
  ++count;
  if (count==65534) 
    {
     result+=">\n";
     count=0;
    }
  }
  result+="00>] def\n/CharStrings ";
  FT_UInt  glyph=0;
  FT_ULong zeichen=FT_Get_First_Char(p_fontface,&glyph);
  char n[50];
  count=0;
  hk_string charstrings;
  while (glyph!=0)
  {
   FT_Get_Glyph_Name(p_fontface,glyph,&n,50);
   hk_string nam=n;
   charstrings+="/"+nam+" "+longint2string(glyph)+" def\n";
   zeichen=FT_Get_Next_Char(p_fontface,zeichen,&glyph);
   ++count;
  }
  result +=longint2string(count)+" dict dup begin\n"+charstrings+" end readonly def\nFontName currentdict end definefont pop\n";
  return result;
}







hk_font::hk_font()
{
init();

set_font(p_defaultfont,p_defaultfontsize);
p_bold=false;
p_italic=false;
}

void hk_font::init(void)
{
p_private=new hk_fontprivate;
if (p_counter==0)
{
 if (!FcInit())
  {
   cerr<<"error init fontconfig"<<endl;
   return ;
  }
  else FcInitLoadConfigAndFonts();
  

}
 ++p_counter;

}

hk_font::hk_font(const hk_string& fname,int fsize):hk_class()
{
init();
 set_font(fname,fsize);
p_bold=false;
p_italic=false;
}

hk_font::~hk_font()
{
if (p_private->ftlibrary)
  FT_Done_FreeType(p_private->ftlibrary);

delete p_private;
}

hk_font::hk_font(const hk_font& f):hk_class()
{
init();
  *this=f;

}


void hk_font::set_font(const hk_string& fname,int fsize)
{
 p_fontname=trim(fname);
 p_fontsize=fsize;
 p_private->p_isdirty=true;

}




void hk_font::set_font(const hk_font& f)
{
  p_fontname=f.p_fontname;
  p_fontsize=f.p_fontsize;
  p_bold=f.p_bold;
  p_italic=f.p_italic;
  p_private->p_encodingtab=f.encodingtab();
  p_private->p_isdirty=true;

}


hk_string hk_font::fontname(void) const
{
 return p_fontname;

}

int hk_font::fontsize(void) const
{
return p_fontsize;

}

void hk_font::savedata(ostream& s )
{
    start_mastertag(s,"HK_FONT");
    set_tagvalue(s,"FONT",p_fontname);
    set_tagvalue(s,"FONTSIZE",p_fontsize);
    set_tagvalue(s,"BOLD",p_bold);
    set_tagvalue(s,"ITALIC",p_italic);
    end_mastertag(s,"HK_FONT");



}

void hk_font::loaddata(xmlNodePtr definition)
{
    get_tagvalue(definition,"FONT",p_fontname);
    get_tagvalue(definition,"FONTSIZE",p_fontsize);
    get_tagvalue(definition,"BOLD",p_bold);
    get_tagvalue(definition,"ITALIC",p_italic);


}


bool hk_font::operator=(const hk_font& f)
{
  set_font(f);

 return true;
}

bool hk_font::operator==(const hk_font& f)
{
    return is_equal(f);
}


bool hk_font::operator!=(const hk_font& f)
{
    return !is_equal(f);
}

bool hk_font::is_equal(const hk_font&f)
{
return (p_fontname==f.p_fontname &&  p_fontsize==f.p_fontsize  &&(p_italic==f.p_italic)&&(p_bold=f.p_bold));

}


void hk_font::set_defaultfont(const hk_string& d, int s)
{
    p_defaultfont=d;
    p_defaultfontsize=s;

}


hk_string hk_font::defaultfontname(void)
{
    return p_defaultfont;

}


int hk_font::defaultfontsize(void)
{
    return p_defaultfontsize;
}


void hk_font::set_bold(bool b)
{
  p_bold=b;
  p_private->p_isdirty=true;

}

bool hk_font::bold(void) const
{
  return p_bold;
}


void hk_font::set_italic(bool i)
{
  p_italic=i;
p_private->p_isdirty=true;
}

bool hk_font::italic(void) const
{
  return p_italic;
}



hk_string hk_font::psfontname(void)
{
  if (p_private->p_isdirty)
     set_fontface();

hk_string psfont;
  	    if (p_private->p_fontface)
  	    {
  	    const char* psname=FT_Get_Postscript_Name(p_private->p_fontface);
  	    psfont= (psname?psname:"");
  	    }
 if (psfont.size()>0) return psfont;
   
    const hk_string obliques= "avantgarde courier helvetica";
    bool useoblique=(obliques.find(string2lower(p_fontname))!=hk_string::npos);
    hk_string result=p_fontname;
    if (p_bold || p_italic) result+="-";
    if (p_bold) result+="Bold";
    if (p_italic) result+=useoblique?"Oblique":"Italic";

  
  return replace_all(" ",result,"");

}

hk_url hk_font::fontfile(void)
{
  if (p_private->p_isdirty)
     set_fontface();
  return p_private->p_fontfile;
}


HK_CHARSIZE hk_font::char_ascender_size(void)
{
  if (p_private->p_isdirty)
     set_fontface();
  return p_private->p_fontface->ascender/64;
}

HK_CHARSIZE hk_font::char_descender_size(void)
{
  if (p_private->p_isdirty)
     set_fontface();
  return -p_private->p_fontface->descender/64;
}


list<hk_string>* hk_font::fontlist(void)
{
//  p_fontlist.clear();
 if (p_fontlist.size()>0)return &p_fontlist;

 
 FcFontSet* fs;
 FcObjectSet *os = 0;
 FcChar8* fontformat;
 FcPattern* pat= FcPatternCreate ();
    
	os = FcObjectSetBuild (FC_FAMILY, FC_STYLE, (char *) 0);
    fs = FcFontList (0, pat, os);
    if (fs)
    {

	for (int j = 0; j < fs->nfont; j++)
	{
	    FcChar8 *family;

	if (FcPatternGetString (fs->fonts[j], FC_FAMILY, 0, &family) != FcResultMatch)
		    family = (FcChar8 *)(const_cast<char*> ("<unknown family>"));
	else
	  {
	    hk_font f((const char*) family,10);
	    hk_string p=f.psfontname();
	    hk_string ext=string2upper(f.fontfile().extension());
// 	    cerr <<"extensions:"<<ext<<endl;
	    if (p.size()>0 &&ext!="PCF" && ext!="PCF.GZ") p_fontlist.push_back((const char*)family);
// 	    else cerr <<"ignored font: "<<family<<endl;
	  }
	
	if (FcPatternGetString (fs->fonts[j], FC_FONTFORMAT, 0, &fontformat) != FcResultMatch)
		    fontformat = (FcChar8 *)(const_cast<char*> ("<unknown font format>"));

	
// 	cerr <<"FONT:"<<family<<" format:"<<fontformat<<endl;
	
	}
	
	FcFontSetDestroy (fs);
    }

 p_fontlist.sort();
 p_fontlist.unique();

 return &p_fontlist;

}


void hk_font::set_fontface(void)
{
   if (!p_private->ftlibrary)
      p_private->init_library();

  if (p_private->p_fontface )
     FT_Done_Face( p_private->p_fontface );
   	
  if (p_private->p_fontset) FcFontSetDestroy (p_private->p_fontset);

  p_private->p_available_sizes.clear();
  p_private->p_fontface =NULL;
  p_private->p_fontset=0;
    FcPattern   *pat=0;
	pat = FcPatternCreate();
     	FcPatternAddString(pat,FC_FAMILY,(FcChar8 *)(const_cast<char*> (p_fontname.c_str())));
    	if (p_bold) FcPatternAddInteger(pat,FC_WEIGHT,FC_WEIGHT_BOLD);
    	if (p_italic) FcPatternAddInteger(pat,FC_SLANT,FC_SLANT_ITALIC);
    	FcPatternAddDouble(pat,FC_SIZE,p_fontsize);
	FcDefaultSubstitute(pat);

    p_private->p_fontset = FcFontSetCreate ();
    
    FcPattern* match;
    FcResult result;
FT_Error error;
    match=FcFontMatch(0,pat,&result);
    if (match)
      FcFontSetAdd(p_private->p_fontset,match);
    
    if (p_private->p_fontset)
    {
	int	j;
// 	cerr <<"fs->nfont:"<<fs->nfont<<endl;
	for (j = 0; j < p_private->p_fontset->nfont; j++)
	{
	FcChar8 *file;

           		
		
		
		if (FcPatternGetString (p_private->p_fontset->fonts[j], FC_FILE, 0, &file) != FcResultMatch)
		    return ;
  	    
  	    p_private->p_fontfile=(const char*) file;
//     	    cerr <<p_fontname<<": used fontfile:"<<(const char*) file<<endl;
  	    
  	    error = FT_New_Face( p_private->ftlibrary,(const char*)file,0,&p_private->p_fontface );
  	    
  	    if (error)
  	    {
  	     cerr <<"Could not load fontface"<<endl;
  	     return ;
  	    }
  	    else
  	    {
  	     
//   	     	cerr <<" fixed sizes:"<<p_private->p_fontface->num_fixed_sizes<<endl;
	int n=0;
	while (n<p_private->p_fontface->num_fixed_sizes)
	{
	  
	  p_private->p_available_sizes.push_back(p_private->p_fontface->available_sizes[n].size/64);
// 	  cerr <<"   availablesize: "<<p_private->p_fontface->available_sizes[n].size/64<<endl;
	  ++n;
	}

  	     
  	     error = FT_Set_Char_Size(p_private->p_fontface,0,p_fontsize*64,RESOLUTION,RESOLUTION);
  	     if (error)
  	      {
//   	        cerr <<p_fontname<<" error setting charsize"<<endl;
  	      }
//             cerr<<" "<<p_fontname<<" used charmap:"<<p_private->p_fontface->charmap<<std::boolalpha<<" has glyphnames:"<<FT_HAS_GLYPH_NAMES(p_private->p_fontface)<<endl;
//             if (p_private->p_fontface->charmap) 
//             cerr<<"   platform id:"<<p_private->p_fontface->charmap->platform_id<<
//             " encoding id:"<<p_private->p_fontface->charmap->encoding_id<<endl;
//             unsigned int f=0;
/*            while (f<p_private->p_fontface->num_glyphs)
            {
  	   	char glyphname[50];
  	   	error= FT_Get_Glyph_Name(p_private->p_fontface,f,&glyphname,50);
  	   	cerr <<"     Glyph:"<<f<<": #"<<glyphname<<"#"<<endl;
  	   	++f;
             
            }*/
            }
//   	     error = FT_Set_Char_Size(p_private->p_fontface,0,p_fontsize*64,0,0);
//   	     if (error)
//   	      {
//   	        cerr <<"error setting charsize"<<endl;
//   	      }

	}
   }
   p_private->p_isdirty=false;
 
}

void hk_font::register_string(const hk_string &s)
{
  if (!p_private->p_encodingtab)return;
  hk_string smuni=smallstringconversion(l2u(s),"UTF8","WCHAR_T");
  wstring text; 
  text.append((wchar_t*)(const_cast<char*> (smuni.data())),smuni.size()/sizeof(wchar_t));
    
    for (unsigned long int n=0;n<text.size();++n)
    { 
      p_private->p_encodingtab->register_unicode((HK_CHARSIZE) text[n],"");

    }
}

bool hk_font::text2lines(int fdwidth,list<hk_string>* l,const hk_string& txt)
{
//   return false;
  if (!l) return false; 
  if (p_private->p_isdirty)
     set_fontface();
  long int fixedwidth=fdwidth;
  if (l->size()>0) l->clear(); 
  if (fixedwidth>0) fixedwidth=((int)(((fixedwidth*(double)RESOLUTION)/254.0)));
  hk_string smuni=smallstringconversion(l2u(txt),"UTF8","WCHAR_T");
wstring text; text.append((wchar_t*)(const_cast<char*> (smuni.data())),smuni.size()/sizeof(wchar_t));

  
//   cerr <<"text:"<<text.size()<<" origsize:"<<txt.size()<<endl;
//   return true;

if (!p_private->p_fontface) return false;
  	    //hier die routine für Abmessung einfügen
  	    
FT_Error error;
  	   	int pen_x=0;
  	   	int lastspace=0;
//   	   	int pen_y=0;
  	   	int kerning=0;
  	   	int glyphsize=0;
  	   	FT_GlyphSlot slot= p_private->p_fontface->glyph;
  	   	FT_Bool use_kerning=FT_HAS_KERNING(p_private->p_fontface); 
  	   	FT_UInt previous=0;
//   	   	hk_string newline;
		unsigned long int offset=0;
  	    for (unsigned long int n=0;n<text.size();++n)
  	    { 
  	    
  	   	FT_UInt glyph_index=FT_Get_Char_Index(p_private->p_fontface,text[n]);
//   	   	cerr <<"   glyph_index:"<<glyph_index<<endl;
  	   	if (use_kerning && previous && glyph_index)
  	   	{
  	   	  FT_Vector delta;
  	   	  FT_Get_Kerning(p_private->p_fontface,previous,glyph_index,FT_KERNING_DEFAULT,&delta);
  	   	  kerning=delta.x >>6;
  	   	}
  	   	char glyphname[50];
  	   	error= FT_Get_Glyph_Name(p_private->p_fontface,glyph_index,&glyphname,50);
//   	   	cerr <<"   Glyphname:"<<glyphname<<endl;
  	      if (p_private->p_encodingtab) 
  	      {
  	           	      hk_string fr;
  	      wstring wtext(1,text[n]);
  	      fr.append ((char*)(const_cast<wchar_t*> (wtext.data())),wtext.size()*sizeof(wchar_t));
 	       
  	       hk_string cr=smallstringconversion(fr,"WCHAR_T","UTF-8");

  	         
//   	         cerr <<" "<<cr<<" "<<std::hex<<text[n]<<std::dec<<" ";
  	         p_private->p_encodingtab->register_unicode((HK_CHARSIZE) text[n],glyphname);
  	      }
  	   	error = FT_Load_Glyph(p_private->p_fontface, glyph_index, FT_LOAD_RENDER);
  	   	if (error) 
  	   	{
  	   	   cerr <<"ERROR FT_Load_Glyph"<<endl;
  	   	   continue;
  	   	}
  	   	
  	   	
  	   	glyphsize=slot->advance.x >>6;
  	    if ((fixedwidth>0)&&(pen_x+glyphsize+kerning > fixedwidth  || text[n]=='\n'))
  	    {  
//   	    cerr<<"fixedwidth:"<<fixedwidth<<" pen_x:"<<pen_x<<" glyphsize:"<<glyphsize<<" kerning:"<<kerning<<endl;
  	      // text exceeds width
  	      if (text[n]!='\n' && (unsigned int)lastspace > offset) n=lastspace;
  	      hk_string fr;
  	      wstring wtxt=text.substr(offset,n-offset);
  	      fr.append ((char*)(const_cast<wchar_t*> (wtxt.data())),wtxt.size()*sizeof(wchar_t));
  	      hk_string newline=smallstringconversion(fr,"WCHAR_T","UTF-8");
  	      l->push_back(newline);
//   	       cerr <<" add newline: #"<<newline<<"#"<<endl;
  	      if (text[n]=='\n')
  	       {
//   	        newline="";
		offset=n+1;
  	        pen_x=0;
  	        lastspace=0;
  	        previous=0;
  	       }
  	       else
  	       {
//   	       newline="";
		
  	       pen_x=0;
  	       previous=0;
  	       unsigned int buf=offset;
  	       offset=n+1;
  	       if ((unsigned int)lastspace > buf)
  	          n=lastspace;
  	       else
  	       {
  	        if (n>0)--n;
  	        --offset;
  	       }
  	       lastspace=0;
  	       }
  	    }
  	    else
  	    {
  	      // still same line
  	      if (text[n]==' ') 
  	      lastspace=n;
  	      pen_x+=glyphsize+kerning; 
//    	      cerr <<"\t   pen_x="<<pen_x<<"\t Zeichen:"<<text[n]<<"\t kerning:"<<kerning<<"\t glyphsize:"<<glyphsize<< " b:"<<glyph_bbox.xMax*64<<" "<<bboxx*64<<endl;
//   	      newline+=text[n];
  	      previous=glyph_index;
  	      if (text[n]==' ') 
  	      lastspace=n;
  	    }
  	    
  	    
  	    
  	    }
//   	    if (n>offset)
  	    {
  	      hk_string fr;
  	      wstring wtext=text.substr(offset);
  	      fr.append ((char*)(const_cast<wchar_t*> (wtext.data())),wtext.size()*sizeof(wchar_t));
 	       
  	       hk_string newline=smallstringconversion(fr,"WCHAR_T","UTF-8");
  	       if (newline.size()>0)l->push_back(newline);
  	    }
// cerr <<endl;
  return true;
}


bool hk_font::is_scaleable(void) 
{
  if (p_private->p_isdirty)
  	set_fontface();
  return (p_private->p_available_sizes.size()==0);
}

list<int>* hk_font::available_sizes() 
{
  if (p_private->p_isdirty)
  	set_fontface();
  return &p_private->p_available_sizes;
}






ostream& operator<<(ostream&    stream   ,hk_font& c)
{
    stream <<"["<<hk_class::hk_translate("font: ")<<c.fontname()<<"("<<c.fontsize()<<(c.bold()?" Bold":"")<<(c.italic()?" Italic":"")<<")]";
    return stream;
}


ostream& operator<<(ostream&    stream   ,const hk_font& c)
{
    stream <<"["<<hk_class::hk_translate("font: ")<<c.fontname()<<"("<<c.fontsize()<<(c.bold()?" Bold":"")<<(c.italic()?" Italic":"")<<")]";
    return stream;
}


void hk_font::set_encodingtab(hk_encodingtab* t)
{
 p_private->p_encodingtab=t;
}

hk_encodingtab* hk_font::encodingtab(void) const
{
  return p_private->p_encodingtab;
}

hk_string hk_font::psfontembeddefinition(void)
{
 //Not implemented yet
 hk_url f=fontfile();
 hk_string ext=string2upper(f.extension());
 if (ext=="PFA")
 	return p_private->pfa(this);
 else
 if (ext=="PFB")
 	return p_private->pfb(this);
 else
 if (ext=="TTF")
 	return p_private->ttf(this);
 else
 return "";

}



const char* defaultglyphs[]=
{
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","space","exclam","quotedbl","numbersign","dollar","percent","ampersand","quotesingle","parenleft","parenright","asterisk","plus","comma","hyphen","period","slash","zero","one","two","three","four","five","six","seven","eight","nine","colon","semicolon","less","equal","greater","question","at","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","bracketleft","backslash","bracketright","asciicircum","underscore","grave","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","braceleft","bar","braceright","asciitilde","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","space","exclamdown","cent","sterling","currency","yen","brokenbar","section","dieresis","copyright","ordfeminine","guillemotleft","logicalnot","hyphen","registered","macron","degree","plusminus","twosuperior","threesuperior","acute","mu","paragraph","periodcentered","cedilla","onesuperior","ordmasculine","guillemotright","onequarter","onehalf","threequarters","questiondown","Agrave","Aacute","Acircumflex","Atilde","Adieresis","Aring","AE","Ccedilla","Egrave","Eacute","Ecircumflex","Edieresis","Igrave","Iacute","Icircumflex","Idieresis","Eth","Ntilde","Ograve","Oacute","Ocircumflex","Otilde","Odieresis","multiply","Oslash","Ugrave","Uacute","Ucircumflex","Udieresis","Yacute","Thorn","germandbls","agrave","aacute","acircumflex","atilde","adieresis","aring","ae","ccedilla","egrave","eacute","ecircumflex","edieresis","igrave","iacute","icircumflex","idieresis","eth","ntilde","ograve","oacute","ocircumflex","otilde","odieresis","divide","oslash","ugrave","uacute","ucircumflex","udieresis","yacute","thorn","ydieresis","Amacron","amacron","Abreve","abreve","Aogonek","aogonek","Cacute","cacute","Ccircumflex","ccircumflex","Cdotaccent","cdotaccent","Ccaron","ccaron","Dcaron","dcaron","Dcroat","dcroat","Emacron","emacron","Ebreve","ebreve","Edotaccent","edotaccent","Eogonek","eogonek","Ecaron","ecaron","Gcircumflex","gcircumflex","Gbreve","gbreve","Gdotaccent","gdotaccent","Gcommaaccent","gcommaaccent","Hcircumflex","hcircumflex","Hbar","hbar","Itilde","itilde","Imacron","imacron","Ibreve","ibreve","Iogonek","iogonek","Idotaccent","dotlessi"
,"IJ","ij","Jcircumflex","jcircumflex","Kcommaaccent","kcommaaccent","kgreenlandic","Lacute","lacute","Lcommaaccent","lcommaaccent","Lcaron","lcaron","Ldot","ldot","Lslash","lslash","Nacute","nacute","Ncommaaccent","ncommaaccent","Ncaron","ncaron","napostrophe","Eng","eng","Omacron","omacron","Obreve","obreve","Ohungarumlaut","ohungarumlaut","OE","oe","Racute","racute","Rcommaaccent","rcommaaccent","Rcaron","rcaron","Sacute","sacute","Scircumflex","scircumflex","Scedilla","scedilla","Scaron","scaron","Tcommaaccent","tcommaaccent","Tcaron","tcaron","Tbar","tbar","Utilde","utilde","Umacron","umacron","Ubreve","ubreve","Uring","uring","Uhungarumlaut","uhungarumlaut","Uogonek","uogonek","Wcircumflex","wcircumflex","Ycircumflex","ycircumflex","Ydieresis","Zacute","zacute","Zdotaccent","zdotaccent","Zcaron","zcaron","longs","","","","","","","","","","","","","","","","","","","florin","","","","","","","","","","","","","","Ohorn","ohorn","","","","","","","","","","","","","","Uhorn","uhorn","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Gcaron","gcaron","","","","","","","","","","","","","","","","","","","Aringacute","aringacute","AEacute","aeacute","Oslashacute","oslashacute","","","","","","","","","","","","","","","","","","","","","","","","","Scommaaccent","scommaaccent","Tcommaaccent","tcommaaccent","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""
,"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","afii57929","afii64937","","","","","","","","","circumflex","caron","","macron","","","","","","","","","","","","","","","breve","dotaccent","ring","ogonek","tilde","hungarumlaut","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","gravecomb","acutecomb","","tildecomb","","","","","","hookabovecomb","","","","","","","","","","","","","","","","","","","","","","","","","","dotbelowcomb","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","tonos","dieresistonos","Alphatonos","anoteleia","Epsilontonos","Etatonos","Iotatonos","","Omicrontonos","","Upsilontonos","Omegatonos","iotadieresistonos","Alpha","Beta","Gamma","Delta","Epsilon","Zeta","Eta","Theta","Iota","Kappa","Lambda","Mu","Nu","Xi","Omicron","Pi","Rho","","Sigma","Tau","Upsilon","Phi","Chi","Psi","Omega","Iotadieresis","Upsilondieresis","alphatonos","epsilontonos","etatonos","iotatonos","upsilondieresistonos","alpha","beta","gamma","delta","epsilon","zeta","eta","theta","iota","kappa","lambda","mu","nu","xi","omicron","pi","rho","sigma1","sigma","tau","upsilon","phi","chi","psi","omega","iotadieresis","upsilondieresis","omicrontonos","upsilontonos","omegatonos","","","theta1","Upsilon1","","","phi1","omega1","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","afii10023","afii10051","afii10052","afii10053","afii10054","afii10055","afii10056","afii10057","afii10058","afii10059","afii10060","afii10061","","afii10062","afii10145","afii10017","afii10018","afii10019","afii10020","afii10021","afii10022","afii10024","afii10025","afii10026","afii10027","afii10028","afii10029","afii10030","afii10031","afii10032","afii10033","afii10034","afii10035","afii10036","afii10037","afii10038","afii10039","afii10040","afii10041","afii10042","afii10043","afii10044","afii10045","afii10046","afii10047","afii10048","afii10049","afii10065","afii10066","afii10067","afii10068","afii10069","afii10070","afii10072","afii10073","afii10074","afii10075","afii10076","afii10077","afii10078","afii10079","afii10080","afii10081","afii10082","afii10083","afii10084","afii10085","afii10086","afii10087","afii10088","afii10089","afii10090","afii10091","afii10092","afii10093","afii10094","afii10095","afii10096","afii10097","","afii10071","afii10099","afii10100","afii10101","afii10102","afii10103","afii10104","afii10105","afii10106","afii10107","afii10108","afii10109","","afii10110","afii10193","","","afii10146","afii10194","","","","","","","","","","","","","","","afii10147","afii10195","afii10148","afii10196","","","","","","","","","","","","","","","","","","","","","","","","","","","afii10050","afii10098","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","afii10846","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""
,"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","afii57799","afii57801","afii57800","afii57802","afii57793","afii57794","afii57795","afii57798","afii57797","afii57806","","afii57796","afii57807","afii57839","afii57645","afii57841","afii57842","afii57804","afii57803","afii57658","","","","","","","","","","","","","afii57664","afii57665","afii57666","afii57667","afii57668","afii57669","afii57670","afii57671","afii57672","afii57673","afii57674","afii57675","afii57676","afii57677","afii57678","afii57679","afii57680","afii57681","afii57682","afii57683","afii57684","afii57685","afii57686","afii57687","afii57688","afii57689","afii57690","","","","","","afii57716","afii57717","afii57718","","","","","","","","","","","","","","","","","","","","","","","","","","afii57388","","","","","","","","","","","","","","","afii57403","","","","afii57407","","afii57409","afii57410","afii57411","afii57412","afii57413","afii57414","afii57415","afii57416","afii57417","afii57418","afii57419","afii57420","afii57421","afii57422","afii57423","afii57424","afii57425","afii57426","afii57427","afii57428","afii57429","afii57430","afii57431","afii57432","afii57433","afii57434","","","","","","afii57440","afii57441","afii57442","afii57443","afii57444","afii57445","afii57446","afii57470","afii57448","afii57449","afii57450","afii57451","afii57452","afii57453","afii57454","afii57455","afii57456","afii57457","afii57458","","","","","","","","","","","","","","afii57392","afii57393","afii57394","afii57395","afii57396","afii57397","afii57398","afii57399","afii57400","afii57401","afii57381","","","afii63167","","","","","","","","","","","","afii57511","","","","","afii57506","","","","","","","","afii57507","","afii57512","","","","","","","","","afii57513","","","","","","","afii57508","","","","","","","","","","","","afii57505","","","","","","","","","","","afii57509","","","","","","","","","","","afii57514","","","","","","","","","","","","","",
"","","","","","","","","","","afii57519","","","afii57534","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Wgrave","wgrave","Wacute","wacute","Wdieresis","wdieresis","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Ygrave","ygrave","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""
,"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","afii61664","afii301","afii299","afii300","","","figuredash","endash","emdash","afii00208","","underscoredbl","quoteleft","quoteright","quotesinglbase","quotereversed","quotedblleft","quotedblright","quotedblbase","","dagger","daggerdbl","bullet","","onedotenleader","twodotenleader","ellipsis","","","","","","afii61573","afii61574","afii61575","","perthousand","","minute","second","","","","","","guilsinglleft","guilsinglright","","exclamdbl","","","","","","","","fraction","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","zerosuperior","","","","foursuperior","fivesuperior","sixsuperior","sevensuperior","eightsuperior","ninesuperior","","","","parenleftsuperior","parenrightsuperior","nsuperior","zeroinferior","oneinferior","twoinferior","threeinferior","fourinferior","fiveinferior","sixinferior","seveninferior","eightinferior","nineinferior","","","","parenleftinferior","parenrightinferior","","","","","","","","","","","","","","","","","","","colonmonetary","","franc","lira","","","peseta","","","afii57636","dong","Euro","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","afii61248","",
"","","","","","","","","","","Ifraktur","","afii61289","","","afii61352","","weierstrass","","","","Rfraktur","","prescription","","","","trademark","","","","Omega","","","","","","","","estimated","","","","","","","aleph","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","onethird","twothirds","","","","","","","oneeighth","threeeighths","fiveeighths","seveneighths","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","arrowleft","arrowup","arrowright","arrowdown","arrowboth","arrowupdn","","","","","","","","","","","","","","","","","","","arrowupdnbse","","","","","","","","","","","","","carriagereturn","","","","","","","","","","","","","","","","","","","","","","","","","","","arrowdblleft","arrowdblup","arrowdblright","arrowdbldown","arrowdblboth","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","universal","","partialdiff","existential","","emptyset","Delta","gradient","element","notelement","","suchthat","","","","product","","summation","minus","","","fraction","","asteriskmath","","periodcentered","radical","","","proportional","infinity","orthogonal","angle","","","","","","","logicaland","logicalor","intersection","union","integral","","","","","","","","","therefore","","","","","","","","similar","","","","","","","","","congruent","","","approxequal","","","","","","","","","","","","","","","","","","","","","","","","notequal","equivalence","","","lessequal","greaterequal","","","","","","","","","","","","","","","","","","","","","","","","","","","","","propersubset","propersuperset","notsubset","","reflexsubset","reflexsuperset","","","","","","","","","","","","","","circleplus","","circlemultiply","","","",
"","","","","","","","","","","perpendicular","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","dotmath","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","house","","","","","","","","","","","","","","revlogicalnot","","","","","","","","","","","","","","","","integraltp","integralbt","","","","","","","","angleleft","angleright","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","SF100000","","SF110000","","","","","","","","","","SF010000","","","","SF030000","","","","SF020000","","","","SF040000","","","","SF080000","","","","","","","","SF090000","","","","","","","","SF060000","","","","","","","","SF070000","","","","","","","","SF050000","","","","","","","","","","","","","","","","","","","","SF430000","SF240000","SF510000","SF520000","SF390000","SF220000","SF210000","SF250000","SF500000","SF490000","SF380000","SF280000","SF270000","SF260000","SF360000","SF370000","SF420000","SF190000","SF200000","SF230000","SF470000","SF480000","SF410000","SF450000","SF460000","SF400000","SF540000","SF530000","SF440000","","","","","","","","","","","","","","","","","","","","upblock","","","","dnblock","","","","block","","","","lfblock","","","","rtblock","ltshade","shade","dkshade","","","","","","",
"","","","","","","filledbox","H22073","","","","","","","","","H18543","H18551","filledrect","","","","","","triagup","","","","","","","","triagrt","","triagdn","","","","","","","","triaglf","","","","","","lozenge","circle","","","","H18533","","","","","","","","","invbullet","invcircle","","","","","","","","","","","","","openbullet","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","smileface","invsmileface","sun","","","","female","","male","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","spade","","","club","","heart","diamond","","","","musicalnote","musicalnotedbl"};









const char* defaultglyphs2[]=
{
"dotlessj","LL","ll","Scedilla","scedilla","commaaccent","afii10063","afii10064","afii10192","afii10831","afii10832","Acute","Caron","Dieresis","DieresisAcute","DieresisGrave","Grave","Hungarumlaut","Macron","cyrBreve","cyrFlex","dblGrave","cyrbreve","cyrflex","dblgrave","dieresisacute","dieresisgrave","copyrightserif","registerserif","trademarkserif","onefitted","rupiah","threequartersemdash","centinferior","centsuperior","commainferior","commasuperior","dollarinferior","dollarsuperior","hypheninferior","hyphensuperior","periodinferior","periodsuperior","asuperior","bsuperior","dsuperior","esuperior","isuperior","lsuperior","msuperior","osuperior","rsuperior","ssuperior","tsuperior","Brevesmall","Caronsmall","Circumflexsmall","Dotaccentsmall","Hungarumlautsmall","Lslashsmall","OEsmall","Ogoneksmall","Ringsmall","Scaronsmall","Tildesmall","Zcaronsmall","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","exclamsmall","","","dollaroldstyle","","ampersandsmall","","","","","","","","","","zerooldstyle","oneoldstyle","twooldstyle","threeoldstyle","fouroldstyle","fiveoldstyle","sixoldstyle","sevenoldstyle","eightoldstyle","nineoldstyle","","","","","","questionsmall","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Gravesmall","Asmall","Bsmall","Csmall","Dsmall","Esmall","Fsmall","Gsmall","Hsmall","Ismall","Jsmall","Ksmall","Lsmall","Msmall","Nsmall","Osmall","Psmall","Qsmall","Rsmall","Ssmall","Tsmall","Usmall","Vsmall","Wsmall","Xsmall","Ysmall","Zsmall","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","exclamdownsmall","centoldstyle","","","","","","Dieresissmall","","","","","","","Macronsmall","","","","","Acutesmall","","","","Cedillasmall","","","","","","","questiondownsmall","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Agravesmall","Aacutesmall","Acircumflexsmall","Atildesmall","Adieresissmall","Aringsmall","AEsmall","Ccedillasmall","Egravesmall","Eacutesmall","Ecircumflexsmall","Edieresissmall","Igravesmall","Iacutesmall","Icircumflexsmall","Idieresissmall","Ethsmall","Ntildesmall","Ogravesmall","Oacutesmall","Ocircumflexsmall","Otildesmall","Odieresissmall","","Oslashsmall","Ugravesmall","Uacutesmall","Ucircumflexsmall","Udieresissmall","Yacutesmall","Thornsmall","Ydieresissmall","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"radicalex","arrowvertex","arrowhorizex","registersans","copyrightsans","trademarksans","parenlefttp","parenleftex","parenleftbt","bracketlefttp","bracketleftex","bracketleftbt","bracelefttp","braceleftmid","braceleftbt","braceex","integralex","parenrighttp","parenrightex","parenrightbt","bracketrighttp","bracketrightex","bracketrightbt","bracerighttp","bracerightmid","bracerightbt","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","ff","fi","fl","ffi","ffl" };

typedef class
{
public:
HK_CHARSIZE individual;
hk_string   glyphname;
} uni2inditype;




const HK_CHARSIZE MAXCHARVALUE=0xFFFF;
class hk_encodingtabprivate
{
 public:
 hk_encodingtabprivate()
  {
   maxnr=0;
   p_has_changed=false;
   for (HK_CHARSIZE k=0;k<MAXCHARVALUE;++k)
   {
    uni2indi[k].individual=0;
   }
  }
 
 
 HK_CHARSIZE maxnr;
 uni2inditype uni2indi[MAXCHARVALUE];
 vector<HK_CHARSIZE> indi2uni;
 bool p_has_changed;
 
 
};


hk_encodingtab::hk_encodingtab()
{
  p_private=new hk_encodingtabprivate;
  register_unicode(0x0A,".notdef");
  register_unicode(0x20,"space");

}

hk_encodingtab::~hk_encodingtab()
{
 delete p_private;
}


void hk_encodingtab::register_unicode(HK_CHARSIZE code,const hk_string& glname)
{ 
  if (code >MAXCHARVALUE)
  {
    cerr <<std::hex<<"unicode"<<code <<">MAXCHARVALUE"<<MAXCHARVALUE<<std::dec<<endl;
   return;
  }
//     cerr <<"Glyph:"<<glyphname<<" Register:" <<std::hex<<code<<std::dec<<" ";
  if (p_private->uni2indi[code].individual==0)
     {
       ++p_private->maxnr;
       if ((p_private->maxnr % 255)==0) 
       /* ignore value 255 as it is needed as an Escape sequence for Composite fonts
       ,0 (not used mark),1 (newline) and 2 (space)
       */
       {
         p_private->maxnr+=4;
       }
       p_private->uni2indi[code].individual=p_private->maxnr;
       p_private->uni2indi[code].glyphname=trim(glname);
       p_private->indi2uni.resize(p_private->maxnr+1);
       p_private->indi2uni[p_private->maxnr]=code;
//             cerr <<"is "<<p_private->maxnr<<endl;
       p_private->p_has_changed=true;
     }
//           else cerr <<"already registered as "<<p_private->uni2indi[code].individual <<endl;
}

void hk_encodingtab::register_unicode(HK_CHARSIZE code)
{ 
  register_unicode(code,"");
}

HK_CHARSIZE hk_encodingtab::max_nr() const
{
  return p_private->maxnr;
}

HK_CHARSIZE hk_encodingtab::local (HK_CHARSIZE u)
{
  if (u >MAXCHARVALUE)
  {
//    cerr <<"hk_encodingtab::local:"<<std::hex<<unicode<<">"<<MAXCHARVALUE<<" thus returning 0"<<endl;
   return 0;
  }
  return p_private->uni2indi[u].individual;

}

hk_string hk_encodingtab::glyphname (HK_CHARSIZE u)
{
  
  // characters from 0xF6BE are not handled yet !


//   cerr <<"glyphname::"<<std::hex<<unicode<<std::dec<<endl;
  
  if (u >MAXCHARVALUE)
   {
    cerr <<std::hex<<"unicode"<<u <<">MAXCHARVALUE"<<MAXCHARVALUE<<std::dec<<endl;
    return "";
   }
  hk_string result=trim(p_private->uni2indi[u].glyphname);
  if (result.size()==0 ||result==".notdef")
    {
       HK_CHARSIZE count=sizeof(defaultglyphs)/sizeof(char*);
       if (u>=count)
       {
//         cerr <<unicode<< ">count"<<count<<endl;
              count=sizeof(defaultglyphs2)/sizeof(char*);
              if (u-0xF6BE <count && u>=0xFBE)
              {
                result=defaultglyphs2[u-0xF6BE];
              
              }
       }
       else
       {
//         cerr <<"defaultsglyphs wird benutzt für:"<<unicode<<" neu:"<<defaultglyphs[unicode]<<endl;
        result=defaultglyphs[u];
//         cerr <<"DEFAULTSGLYPHS:"<<endl<<"==============="<<endl;
//         HK_CHARSIZE c=0;
//         while (c<count)
//         {
//           cerr <<std::hex<<c<<": "<<defaultglyphs[c]<<endl;
//           ++c;
//         }
//         cerr <<std::dec<<endl<<"==============="<<endl;
       }
    }
   if (result.size()==0)result=".notdef";  
  return result;

}

HK_CHARSIZE hk_encodingtab::unicode(HK_CHARSIZE l)
{
  if (l >p_private->indi2uni.size()) return 0;
  return p_private->indi2uni[l];

}

void hk_encodingtab::reset(void)
{
   p_private->maxnr=0;
   for (HK_CHARSIZE k=0;k<MAXCHARVALUE;++k) 
     {
      p_private->uni2indi[k].individual=0;
      p_private->uni2indi[k].glyphname="";
     }
   p_private->indi2uni.clear();
   register_unicode(0x0A,".notdef");
   register_unicode(0x20,"space");
   reset_has_changed();

}

void hk_encodingtab::reset_has_changed(void)
{
p_private->p_has_changed=false;
}

bool hk_encodingtab::has_changed(void) const
{
  return p_private->p_has_changed;
}

