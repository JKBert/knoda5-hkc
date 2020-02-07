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
//$Revision: 1.83 $
#include "hk_reportutils.h"
#include "hk_reportsectionpair.h"
#include "hk_datasource.h"
#include <nl_types.h>
#include <langinfo.h>
#include <locale.h>

//****************************************************
//*                 HTML Stuff                       *
//****************************************************

hk_reporthtml::hk_reporthtml(void):hk_report()
{
    p_columnbefore="";
    set_recodefunction("Html");
    p_withheader=true;
    p_pagehead="<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">\n<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf8\">\n";
    p_head="";
   hk_reportsectionpair* p_tableheadersectionpair= new_sectionpair();
   hk_reportsectionpair* p_tablesurroundpair= new_sectionpair();

   p_tableheadersection=p_tableheadersectionpair->headersection();
   p_tableheadersection->set_automatic_create_data(true);
   p_beforetablesection=p_tablesurroundpair->headersection();
   p_datasection=datasection();
   p_tableendsection=p_tablesurroundpair->footersection();
   p_beforetabledata=p_beforetablesection->new_data();
   p_aftertabledata=p_tableendsection->new_data();
//   set_fileextension(".html");

   p_pageheaddata=page_header()->new_data();
    configure_page();
    configure_table();


}


hk_reporthtml::~hk_reporthtml(void)
{

}


void hk_reporthtml::configure_page(void)
{
    hk_string buffer=p_pagehead+"\n<HTML>\n <HEAD>\n";
    page_header()->set_sectionbegin(buffer);
    buffer="   ";
    buffer+=p_head;
    if (p_title.size()>0)
    {
        buffer+="   <TITLE>";
        buffer+=p_title+"</TITLE>\n";

    }
    if (p_author.size()>0)
    {
        buffer+="   <meta name=\"author\" content=\"";
        buffer+=p_author+"\">\n";

    }

    p_pageheaddata->set_data(recode_html(buffer,this));
    page_header()->set_sectionend("\n </HEAD>\n <BODY>\n");
    page_footer()->set_sectionend(" </BODY>\n</HTML>\n");
    p_datasection->set_automatic_create_data(true);
}


void hk_reporthtml::configure_table(void)
{
    hk_string buffer;
    hk_string trtag="   <TR ";
    hk_string trendtag="</TR>\n";
    trtag+=p_trtag+">";
    p_beforetablesection->reportsectionpair()->set_columnname(p_groupcolumn);
    p_beforetablesection->set_unique(true);
    p_beforetablesection->set_automatic_create_data(false);
    set_tabletag("  BORDER=1 CELLSPACING=1");

    p_tableheadersection->reportsectionpair()->set_columnname(p_groupcolumn);
    p_tableheadersection->set_unique(true);
    if (p_withheader)
    {
        buffer="<TH ";
        buffer+=p_thtag+">%COLUMNNAME%</TH>";
        p_tableheadersection->set_sectionbegin(trtag);
        p_tableheadersection->set_sectionend(trendtag);

    }
    else                                          //(not withheader
    {
        buffer="";
    }
    p_tableheadersection->set_default_reportdata(buffer);

    buffer="<TD ";
    buffer+=p_tdtag+" ALIGN=\"%ALIGN%\">";
    p_datasection->set_default_beforereportdata(buffer);
    p_datasection->set_default_afterreportdata("</TD>");
    p_datasection->set_default_reportdata("%VALUE%");
    p_datasection->set_sectionbegin(trtag);
    p_datasection->set_sectionend(trendtag);

    p_tableendsection->set_columnname(p_groupcolumn);
    p_tableendsection->set_unique(true,true);
    p_tableendsection->set_automatic_create_data(false);
    p_tableendsection->set_sectionbegin("  </TABLE>\n");
    p_tableendsection->set_new_page_after_section(p_multiplefiles);



    buffer=p_aftervalue;
    if (p_columnafter.size()>0)buffer+="%VALUE%";
    p_aftertabledata->set_data(buffer);
    p_aftertabledata->set_columnname(p_columnafter);

    buffer=p_beforevalue;
    if (p_columnbefore.size()>0)buffer+="%VALUE%";
    p_beforetabledata->set_data(buffer);
    p_beforetabledata->set_columnname(p_columnbefore);

}


void hk_reporthtml::set_tabletag(const hk_string& t)
{
    p_tabletag=t;
    hk_string buffer;
    buffer="  <TABLE ";
    buffer+=p_tabletag+">\n";
    p_beforetablesection->set_sectionend(buffer);

}


void    hk_reporthtml::set_beforetable(const hk_string& value,const hk_string& columnname)
{
    p_columnbefore=columnname;
    p_beforevalue=recode_html(value,this);
    configure_table();
}


void    hk_reporthtml::set_aftertable(const hk_string& value,const hk_string& columnname)
{
    p_columnafter=columnname;
    p_aftervalue=recode_html(value,this);
    configure_table();
}


void hk_reporthtml::set_multiplefiles(bool m)
{
    hk_report::set_multiplefiles(m);
    configure_table();
}


void    hk_reporthtml::set_trtag(const hk_string& t)
{
    p_trtag=t;
    configure_table();
}


void    hk_reporthtml::set_thtag(const hk_string& t)
{
    p_thtag=t;
    configure_table();
}


void    hk_reporthtml::set_tdtag(const hk_string& t)
{
    p_tdtag=t;
    configure_table();
}


void    hk_reporthtml::set_beforehtmltag(const hk_string& b)
{
    p_pagehead=b;
    configure_page();
}


void    hk_reporthtml::set_betweenheadtags(const hk_string& b,const hk_string& colname)
{
    p_head=b;
    p_pageheaddata->set_columnname(colname);
    configure_page();
}


void    hk_reporthtml::set_title(const hk_string& t)
{
    p_title=t;
    configure_page();
}


void    hk_reporthtml::set_author(const hk_string& a)
{
    p_author=a;
    configure_page();
}


void    hk_reporthtml::set_groupcolumn(const hk_string& c)
{
    p_groupcolumn=c;
    configure_table();
}


hk_string  recode_html(const hk_string& where,hk_report*)
{
    
    return smallstringconversion(l2u(where),"","UTF8");

}


//****************************************************
//*             POSTSCRIPT Stuff                     *
//****************************************************

bool set_reporttype_postscript(hk_report* report,bool registerchange)
{
    if (report==NULL) return false;
    report->neutralize_definition(registerchange);
    report->set_fileextension(".ps",registerchange);
    report->set_pagedelimiter("showpage\n",registerchange);
    report->set_recodefunction("Postscript",registerchange);
    report->set_fullpagereplacefunction("Postscript",registerchange);
    report->set_print_full_pages_only(true,registerchange);
    report->set_reportconfigurefunction("Postscript",registerchange);
    report->set_default_reportdata("%VALUE%",registerchange);
    report->set_useencodingtab(true);
    report->configure_page();

    return true;

}







hk_string basefont(const hk_string& f, hk_report* r)
{
   hk_string font=trim(f);
   if (font.size()==0||!r) return "";
   hk_encodingtab* tab=r->encodingtab();
   HK_CHARSIZE max=tab->max_nr();
   unsigned int s=max >> 8;
   
   hk_string buffer;
   hk_string result="12 dict\nbegin\n /FontName /"+font+"base def\n/FontType 0 def\n/WMode 0 def\n/FontMatrix matrix def\n/Encoding [";
   for (HK_CHARSIZE i=0;i<=s;++i)
     {
       result+=ulongint2string(i)+" ";
     }
   result+=" ] def\n/FMapType 3 def\n/EscChar 255 def\n/FDepVector [\n";
   for (HK_CHARSIZE i=0;i<=s;++i)
     {
       result+="/"+font+" /myfont fontencoding"+ulongint2string(i)+" ReEncode /myfont findfont\n";
     }
   
   result+="] def\nFontName\ncurrentdict\nend\ndefinefont\npop\n\n";
   return result;
}


 
hk_string currentencoding(hk_report* r)
{
  if (!r) return "";
  hk_string result;
  
  hk_encodingtab* tab=r->encodingtab();
  
  HK_CHARSIZE c=0;
  int encodingcount=0;
  bool newencoding=true; 
  if (tab->max_nr()==0) return "";
//   cerr <<"tab->max_nr"<<tab->max_nr()<<endl;
  while (c<= tab->max_nr())
  {
    if (c % 256 ==0 &&encodingcount!=0)
    {
     result +=" ] /fontencoding"+longint2string(encodingcount)+" exch def\n\n";
     ++encodingcount;
     if (c<tab->max_nr()-1) 
         newencoding=true;
    }
    if (newencoding) 
      {
       result += "\n[ ";
       newencoding=false;
      }
    result +="/"+tab->glyphname(tab->unicode(c))+" ";
   ++c;
  }
  
  while (c%256) 
   {
     result+="/.notdef ";
     ++c;
   }
  result +=" ] /fontencoding"+longint2string(encodingcount)+" exch def\n\n";
  
//   cerr <<"ENCODING:"<<endl<<result<<endl<<endl<<basefont("ComicSansMS",r)<<endl<<endl;
  return result;
}


hk_string  fullpagereplace_postscript(const hk_string& where,hk_report* report)
{
  hk_report* r=NULL;
  if (report->masterreport()) r=report->masterreport();
  else r=report;
  hk_string result=replace_all("%USEDENCODING%",where,currentencoding(r));
  hk_string fonts;
  list<hk_string>* uf=r->usedpsfonts();
  list<hk_string>::iterator it=uf->begin();
  while (it!=uf->end())
  {
   fonts+=basefont(*it,r)+"\n";
  ++it;
  }
  result=replace_all("%COMPOSITEFONTS%",result,fonts);
  
  
  return result;
}






hk_string  recode_postscript(const hk_string& where,hk_report*r)
{

//  cerr <<"recode:#"<<where<<"#"<<endl;
 
//  hk_string latin1=smallstringconversion(l2u(where),"","ISO8859-15");
 hk_string smuni=smallstringconversion(l2u(where),"UTF8","WCHAR_T");
wstring p_where; p_where.append((wchar_t*)(const_cast<char*> (smuni.data())),smuni.size()/sizeof(wchar_t));

    hk_string result="FF00";
    unsigned int curcompositefont=0;
    for (unsigned int i=0; i<p_where.size();++i)
     {
//        hk_string iso(1,latin1[i]);
//        cerr <<"  "<<i<<": #"<<std::hex<<smallstringconversion(iso,"ISO8859-15","")<<"("<<(int)iso[0]<<")# unicode:"<<p_where[i]<<" results in: #"<<std::dec<<(HK_CHARSIZE)r->encodingtab()->local(p_where[i])
//        <<"# and bin2hex:"<<bin2hex((HK_CHARSIZE)r->encodingtab()->local(p_where[i]))<<endl;
       HK_CHARSIZE curchar=(HK_CHARSIZE)r->encodingtab()->local(p_where[i]);
       unsigned int s=curchar >> 8;
       if (s!=curcompositefont)
       {
         result+="FF"+bin2hex(s);
         curcompositefont=s;
       }
       result+=bin2hex(curchar);
     }
    
//     cerr <<"result: "<<result<<endl<<endl;
    
    return result;
}



unsigned long recount_postscript(hk_reportsection* s)
{
    if (s==NULL) return 0;
    unsigned long c=0;
    unsigned long res=0;
    vector<hk_reportdata*>* dlist=s->datalist();
    if (dlist!=NULL)
    {
        vector<hk_reportdata*>::iterator it=dlist->begin();
        while (it!=dlist->end())
        {
            c=(((*it)->data_fully_printed()&& (*it)->pagescount()==1)?(*it)->y():0)+(*it)->height();
            if (c>res) res=c;
            it++;
        }
    }
    if (s->is_fully_printed())
    res+=s->offset();
    if (s->report()->sizetype()==hk_presentation::relative)
       res=s->relativ2vertical(res);
    return res;
}



hk_string replacepsdatafunction(hk_reportdata* rd,const hk_string& value)
{
return value;
if (!rd) return value; 
hk_string result=replace_all("%BASEFONT%",value,basefont(rd->font().psfontname(),rd->report()));
return result;
}


void   configure_postscriptdata(hk_reportdata* d)
{
    if (d==NULL) return;
//     d->set_datacountfunction("Postscript",false);
    hk_string res;
    res+="%XPOS% zmm %YPOS% zmm %WIDTH% zmm %HEIGHT% zmm (%ALIGN%) %FONTSIZE%  set_variables\n%BGRED% rgb %BGGREEN% rgb %BGBLUE% rgb %FGRED% rgb %FGGREEN% rgb %FGBLUE% rgb paint_background\n<";
    d->set_beforedata(res);
    if (d->linebreak()) res="> /%PSFONT%base BreakIntoLines\n";
    else res="> /%PSFONT%base set_all\n" ;
    if (d->topline())res+="paint_top\n";
    if (d->bottomline())res+="paint_bottom\n";
    if (d->leftline())res+="paint_left\n";
    if (d->rightline())res+="paint_right\n";
    if (d->diagonalluro())res+="paint_diagonal_luro\n";
    if (d->diagonalloru())res+="paint_diagonal_loru\n";
//  if (d->report()->page_header()==d->section()
//      ||d->report()->page_footer()==d->section())res+="grestore\nset_pageboundaries\n";
    d->set_afterdata(res,false);
    int fsize=d->report()->vertical2relativ(zmm2ps(d->font().fontsize()));
    if ((int)d->height()<fsize)
    {
        d->set_height(fsize,false);
    }
    d->set_replacefunction("Postscript",false);
}


void set_reportsection(hk_reportsection*s,const hk_string& p_defaultsctbegin
,const hk_string& p_defaultsctend
,const hk_string& p_defaultbeforedata
,const hk_string& p_defaultafterdata
)
{
//  internal function used by  configure_postscriptreport

    if (s==NULL) return;
    s->set_sectionbegin(p_defaultsctbegin,false);
    s->set_sectionend(p_defaultsctend,false);
    s->set_sectioncountfunction("Postscript",false);
    s->set_default_reportdataconfigurefunction("Postscript",false);
    s->set_default_reportdata(s->report()->default_reportdata(),false);
    s->set_default_beforereportdata(p_defaultbeforedata,false);
    s->set_default_afterreportdata(p_defaultafterdata,false);

    vector<hk_reportdata*>*d=s->datalist();
    if (d!=NULL)
    {
        vector<hk_reportdata*>::iterator data_it=d->begin();
        while(data_it!=d->end())
        {
            (*data_it)->set_configurefunction("Postscript",false);
//             (*data_it)->set_datacountfunction("Postscript",false);
            (*data_it)->set_beforedata(p_defaultbeforedata,false);
            (*data_it)->set_afterdata(p_defaultafterdata,false);
            configure_postscriptdata((*data_it));
            data_it++;
        }

    }
}


bool   configure_postscriptreport(hk_report* report,int /*encoding*/)
{


    if (report==NULL) return false;
    hk_string p_filehead="%!PS-Adobe-3.0\n%%Creator:  HK_CLASSES <c> 2000-2006 Horst Knorr <hk_classes@knoda.org>\n\
%%Pages: (atend)\n%%DocumentNeededResources: (atend)\n%%DocumentSuppliedResources: (atend)\n%BOUNDINGBOX%\n%%Orientation: ";
    p_filehead+=(report->orientation()==hk_report::landscape?"Landscape":"Portrait");
    p_filehead+="\n%%DocumentMedia: Plain ";
    p_filehead+=ulongint2string(zmm2ps(report->designwidth()))+" ";
    p_filehead+=ulongint2string(zmm2ps(report->designheight()));
//      delete[] m;

    p_filehead+=" () ()\n\n%%EndComments\n%%BeginProlog\n%%EndProlog\n%%BeginSetup\n%DEFINITIONS\n%\n\
/set_clippath\n{\nnewpath\n\tx leftborder add  y  moveto\n\twidth 0 rlineto\n\t0 height rlineto\n\
   \twidth neg 0 rlineto\n\tclosepath clip\n} def\n\n \
/position\n{\n\tdup stringwidth pop\n\t/w exch def\n\talign (RIGHT) eq\n\t{/nx x width add leftborder add w sub 4 sub def}\
{\n\talign (CENTER) eq\n\t{/nx x width 2 div add leftborder add w 2 div sub def}\n\t{/nx x leftborder add def\n\t}\
\n\t   	ifelse\n\t}\n\tifelse\n} def\n\n\
/set_textposition\n{\n\tposition\n\tnx 2 add 0 add y fontsize 5 div add moveto\n} def\n\n\
/set_font\n{\n\tfindfont\n\tfontsize scalefont\n\tsetfont\n} def\n\n\
/set_variables\n{\n\t/fontsize exch def\n\t/align exch def\n\t/height exch def\n\t/width exch def\n\t/y exch def\n\t/x exch def\n\t/yrel offset y sub fontsize sub def\n\t/y offset y sub height sub def\n} def\n\n\
/set_all\n{\n\tgsave\n\tset_clippath\n\tset_font\n\tset_textposition\n\tshow\n\tgrestore\n} def\n\n\
/paint_background\n{\n\t/fgblue exch def\n\t/fggreen exch def\n\t/fgred exch def\n\t/bgblue exch def\n\t/bggreen exch def\n\t/bgred exch def\n\t\
\n\tgsave\n\tset_clippath\n\tbgred bggreen bgblue setrgbcolor\n\tfill\n\tgrestore\n\tfgred fggreen fgblue setrgbcolor\n\t} def\n\t\
   \n/set_pageboundaries\n{\n\tgsave\n\t/x 0 def\n\t/y bottomborder def\n\t/width pagewidth leftborder sub rightborder sub def\n\t/height pageheight topborder sub bottomborder sub def\n\
   \t/offset pageheight topborder sub def\n\t%set_clippath\n";

    if (report->topline())p_filehead+="\tpaint_top\n";
    if (report->bottomline())p_filehead+="\tpaint_bottom\n";
    if (report->leftline())p_filehead+="\tpaint_left\n";
    if (report->rightline())p_filehead+="\tpaint_right\n";
    if (report->diagonalluro())p_filehead+="\tpaint_diagonal_luro\n";
    if (report->diagonalloru())p_filehead+="\tpaint_diagonal_loru\n";

    p_filehead+="\n\t} def\n\n\n" ;

    p_filehead+=" /ReEncode\n{\n\t/newencoding exch def\n\t/newfontname exch def\n\
   \t/basefontname exch def\n\t/basefontdict basefontname findfont def\n\
   \t/newfont basefontdict maxlength dict def\n\n\tbasefontdict\n\t{\
    \n\texch dup dup /FID ne exch /Encoding ne and \n\t{ exch newfont 3 1 roll put }\n\t{ pop pop } \
    \n\tifelse\n\t} forall\n\tnewfont /FontName newfontname put\n\tnewfont /Encoding newencoding put \
   \n\tnewfontname newfont definefont pop\n} def\n\n\
   /paint_top\n{\n\tnewpath\n\tx leftborder add   y height add moveto\n\twidth  0 rlineto\n\tstroke\n} def\n\n\n \
   /paint_bottom\n{\n\tnewpath\n\tx leftborder add  y   moveto\n\twidth  0 rlineto\n\tstroke\n} def\n\n\n \
   /paint_left\n{\n\tnewpath\n\tx leftborder add  y   moveto\n\t0  height rlineto\n\tstroke\n} def\n\n\n \
   /paint_right\n{\n\tnewpath\n\tx leftborder add width add y  moveto\n\t0 height rlineto\n\tstroke\n} def\n\n\n \
   /paint_diagonal_loru\n{\n\tnewpath\n\tx leftborder add  y  height add moveto\n\twidth height neg rlineto\n\tstroke\n} def\n\n\n \
   /paint_diagonal_luro\n{\n\tnewpath\n\tx leftborder add  y  moveto\n\twidth height rlineto\n\tstroke\n} def\n\n\n \
   \n/zmm\n{\n\t72 mul 254 div\n} def\n\n\n \
   \n/rgb\n{\n\t 255 div\n} def\n\n\n \
    /set_pagesize\n{\n\t/x 0 def\n\t/y bottomborder def\n\t/height pageheight topborder sub bottomborder sub def\
\n\t/width pagewidth leftborder sub rightborder sub def\n\tgsave\n\tset_clippath\n\t/offset pageheight def\n} def\
   /pagehead\n{\n\t/x 0 def\n\t/y pageheight topborder sub def\n\t/height topborder def\n\n \
   \t/width pagewidth leftborder sub rightborder sub def\n\tset_clippath\n\t/offset pageheight def\n} def\n\n\n \
   /pagefoot\n{\n\t/x 0 def\n\t/y 0 def\n\t/height bottomborder def\n\n \
   \t/width pagewidth leftborder sub rightborder sub def\n\tset_clippath\n\t/offset bottomborder  def\n} def\n\n\
/proc\n\t{\n\tposition\n\tnx 2 add yrel moveto show\n\t/yrel yrel fontsize sub def\n\t} def\n\n\
/wordbreak <02> def\n\n\
/singleBreakIntoLines\n\t{\n\tgsave\n\tset_clippath\n\tset_font\n\t/nextword () def\n\t/textstring exch def\n\ttextstring\
\n\tproc\n\tgrestore\n\t}def\n\
/textvor () def\n\
/EOLchar <01> def\n\
/BreakIntoLines\
{\n/font exch def\
 /textnach exch def\
{\ntextnach\nEOLchar search\
   { %if  Success\n/textvor exch def\n     pop %ignore match\n     /textnach exch def\n}\
   { %else    \n    font singleBreakIntoLines\n     exit\n   }ifelse\ntextvor font\nsingleBreakIntoLines\
} loop\
} def\n";
    hk_string   p_defaultbeforedata="%XPOS% zmm %YPOS% zmm %WIDTH% zmm %HEIGHT% zmm (%ALIGN%) %FONTSIZE%  set_variables\n(";
    hk_string   p_defaultafterdata=") /%PSFONT%base set_all\n";

    hk_string def;
    hk_string defbefore;
    hk_string defafter;
    defbefore="gsave\npagehead\n";
    defbefore+=p_defaultbeforedata;
    report->page_header()->set_default_beforereportdata(defbefore,false);
    hk_string sectionbegin="\n%%Page: %PAGENUMBER% %ABSOLUTEPAGENUMBER% \n";
    if ((report->orientation()==hk_report::landscape)&&report->pageformat()!=hk_report::userdefined)
    {
        sectionbegin+=longint2string(report->designheight());
        sectionbegin+=" zmm 0 translate\n90 rotate\n\n";
    }
    sectionbegin+="/offset pageheight topborder sub def\ngsave\npagehead\n%USEDENCODING%\n%COMPOSITEFONTS%\n";
    report->page_header()->set_sectionbegin(sectionbegin,false);
//   defafter=p_defaultafterdata+"grestore\ngsave\nset_pageboundaries\n";
//   report->page_header()->set_default_afterdata(defafter);
    report->page_header()->set_default_afterreportdata("",false);
    report->page_header()->set_sectionend("grestore\nset_pagesize\n%BGRED% rgb %BGGREEN% rgb %BGBLUE% rgb %FGRED% rgb %FGGREEN% rgb %FGBLUE% rgb paint_background\nset_pageboundaries\n\n%Pagehead Ende\n",false);

    report->page_header()->set_default_reportdata(report->default_reportdata(),false);
    report->page_header()->set_default_reportdataconfigurefunction("Postscript",false);
// set pagehead
    vector<hk_reportdata*>*dit=report->page_header()->datalist();
    if (dit!=NULL)
    {
        vector<hk_reportdata*>::iterator data_it=dit->begin();
        while(data_it!=dit->end())
        {
            (*data_it)->set_configurefunction("Postscript",false);
//             (*data_it)->set_datacountfunction("Postscript",false);
            
            (*data_it)->set_beforedata(defbefore,false);
            (*data_it)->set_afterdata(defafter,false);
            configure_postscriptdata((*data_it));
            data_it++;
        }
    }

    defbefore="";
    defbefore+=p_defaultbeforedata;
    report->page_footer()->set_default_beforereportdata(defbefore,false);
    report->page_footer()->set_default_afterreportdata(defafter,false);
    report->page_footer()->set_default_reportdata(report->default_reportdata(),false);
    report->page_footer()->set_default_reportdataconfigurefunction("Postscript",false);
    report->page_footer()->set_sectionbegin("grestore\ngrestore\ngsave\n\npagefoot\n%Pagefooter Begin\n",false);
    report->page_footer()->set_sectionend("grestore\ngsave\nset_pageboundaries\ngrestore\n%Pagefooter Ende\n",false);

// set pagefoot
    dit=report->page_footer()->datalist();
    if (dit!=NULL)
    {
        vector<hk_reportdata*>::iterator data_it=dit->begin();
        while(data_it!=dit->end())
        {
            (*data_it)->set_configurefunction("Postscript",false);
//             (*data_it)->set_datacountfunction("Postscript",false);
            (*data_it)->set_beforedata(defbefore,false);
            (*data_it)->set_afterdata(defafter,false);
            configure_postscriptdata((*data_it));
            data_it++;
        }
    }

    hk_string h=p_filehead;

    h+="\n/pagewidth ";
    hk_string d=" zmm def\n";
    h+=longint2string(report->designwidth());
    h+=d+"/pageheight ";
    h+=longint2string(report->designheight());
    h+=d+"/leftborder %BORDERLEFT%";

    h+=d+"/rightborder %BORDERRIGHT%";

    h+=d+"/topborder %BORDERTOP%";

    h+=d+"/bottomborder %BORDERBOTTOM%";

    h+=d;
    h+="%%EndSetup\n%PSEMBEDDEDFONTS%\n%PROGRAM START\n \n\n";
    report->set_filebegin(h,false);
    report->set_periodic(report->designheight()-
      ((report->sizetype()==hk_report::relative)?(
      report->relativ2vertical(report->border_bottom()+report->border_top()))
      :(report->border_bottom()+report->border_top())),"",true,false);

    hk_string p_defaultsctend="/offset offset %COUNTS_AS% zmm  sub def\n\n\n";
    report->set_default_beforereportdata(p_defaultbeforedata,false);
    report->set_default_afterreportdata(p_defaultafterdata,false);
    report->set_default_reportsectionbegin("",false);
    report->set_default_reportsectionend(p_defaultsctend,false);
    report->set_default_reportsectioncountfunction("Postscript",false);
    report->set_default_reportdataconfigurefunction("Postscript",false);
    report->set_fileend("%%Trailer\n%%DocumentNeededResources:\n%PSUSEDFONTS%\n%%DocumentSuppliedResources:\n%PSSUPPLIEDFONTS%\n%%Pages: %ABSOLUTEPAGENUMBER%\n%%EOF\n",false);

// set data

    vector<hk_reportsectionpair*>* s=report->sectionpairs();

    if (s!=NULL)
    {
        vector<hk_reportsectionpair*>::iterator section_it=s->begin();
        while(section_it!=s->end())
        {
            set_reportsection((*section_it)->headersection(),"",p_defaultsctend,p_defaultbeforedata,p_defaultafterdata);
            set_reportsection((*section_it)->footersection(),"",p_defaultsctend,p_defaultbeforedata,p_defaultafterdata);
            section_it++;
        }

        if (report->report_header()!=NULL) set_reportsection(report->report_header(),"%reportheadersection\n",p_defaultsctend,p_defaultbeforedata,p_defaultafterdata);
        if (report->report_footer()!=NULL) set_reportsection(report->report_footer(),"%reportfootersection\n",p_defaultsctend,p_defaultbeforedata,p_defaultafterdata);
        if (report->datasection()!=NULL) set_reportsection(report->datasection(),"%reportdatasection\n",p_defaultsctend,p_defaultbeforedata,p_defaultafterdata);

    }
    return true;
}


unsigned long zmm2ps(unsigned long u)
{
    return (unsigned long)(u*72/254+0.5);

}


unsigned long ps2zmm(unsigned long u)
{

    return (unsigned long)(u*254/72+0.5);
}


//****************************************************
//*             CSV Stuff                            *
//****************************************************

hk_reportcsv::hk_reportcsv(void):hk_report()
{
    p_datasection=datasection();
    p_footsection=new_sectionpair()->footersection();;
    p_withheader=true;
    p_delimiter="\"";
    p_between=" , ";
    set_default_use_reportseparator(false);
    set_default_reportprecision(-1);
    set_use_standard_storagepath(false);
    configure_page();
}


hk_reportcsv::~hk_reportcsv(void)
{

}


void hk_reportcsv::set_groupcolumn(const hk_string& c)
{
    p_groupcolumn=c;
    p_footsection->set_columnname(p_groupcolumn);
    p_footsection->set_unique(true,true);
}


void hk_reportcsv::set_multiplefiles(bool m)
{
    hk_report::set_multiplefiles(m);
    p_footsection->set_new_page_after_section(m);
}


void    hk_reportcsv::set_filedefinition(const hk_string& textdel,const hk_string& betweenf,const hk_string& /*rowdel*/,const hk_string& datetimef,const hk_string& datef,const hk_string& timef, const hk_string& charst,const hk_string& l)
{
    p_delimiter=textdel;
    p_between=betweenf;
    set_datetimeformat(datetimef,datef,timef);
    set_filelocale(l);
    set_filecharset(charst);

    configure_page();

}


void hk_reportcsv::set_withheader(bool with)
{

    p_withheader=with;
    configure_page();

}


void hk_reportcsv::configure_page(void)
{
    if (p_withheader)
    {

        hk_string h=p_delimiter+"%COLUMNNAME%";
        h+=p_delimiter;
        page_header()->set_default_reportdata(h);
        page_header()->set_betweendata(p_between);
        page_header()->set_sectionend("\n");
        page_header()->set_automatic_create_data(true);
    }
    else
    {
        page_header()->set_default_reportdata("");
        page_header()->set_betweendata("");
        page_header()->set_sectionend("");
        page_header()->set_automatic_create_data(false);
    }

    hk_string h="%VALUE%";
    p_datasection->set_default_reportprecision(-1);
    p_datasection->set_default_use_reportseparator(false);
    p_datasection->set_default_reportdata(h);
    p_datasection->set_betweendata(p_between);
    p_datasection->set_sectionend("\n");
    p_datasection->set_automatic_create_data(true);
    p_footsection->set_columnname( p_groupcolumn);
    p_footsection->set_unique(true,true);
    p_footsection->set_automatic_create_data(false);

}


void    hk_reportcsv::widget_specific_before_execute(void)
{ 
    setlocale(LC_NUMERIC,filelocale().c_str());
    datasection()->set_automaticcreatedatafunction(&automatic_create_csvfields);
}


void    hk_reportcsv::widget_specific_after_execute(void)
{
    setlocale(LC_NUMERIC,locale().c_str());
}


// void set_cvs(hk_report* e,bool with_header,const hk_string& col_delimiter, const hk_string& betweendata)
// {
//     if (e==NULL)return;
//     hk_string delimiter=col_delimiter;
//     hk_string between=betweendata;
//     if (with_header)
//     {
//
//         hk_reportsectionpair* header=e->new_sectionpair();
//         hk_string h=delimiter+"%COLUMNNAME%";
//         h+=delimiter;
//         header->headersection()->set_default_reportdata(h);
//         header->headersection()->set_betweendata(between);
//         header->headersection()->set_sectionend("\n");
//         header->headersection()->set_unique();
//
//     }
//     hk_reportsection* section=e->datasection();
//     hk_string h=delimiter+"%VALUE%";
//     h+=delimiter;
//     section->set_default_reportdata(h);
//     section->set_betweendata(between);
//     section->set_sectionend("\n");
//
// }
//


hk_string encodefilecharsetfunction(hk_reportdata* rd,const hk_string& value)
{
if (!rd) return value;
return smallstringconversion(value,"",rd->report()->filecharset());
}

void automatic_create_csvfields(hk_reportsection* s)
{
    if (!s) return;
    if (!s->report()->datasource()) return;
    hk_reportcsv *rcsv = dynamic_cast<hk_reportcsv*>(s->report());
    if (!rcsv) return;
    list<hk_column*>* cols=s->report()->datasource()->columns();
    if (cols!=NULL)
    {
        list<hk_column*>::iterator cit=cols->begin();
        while (cit!=cols->end())
        {
            hk_reportdata* dat=s->new_data();
            dat->set_replacefunction("FILECHARACTERSET",false);
            dat->set_datetimeformat(rcsv->datetimeformat(),rcsv->dateformat(),rcsv->timeformat());
            dat->set_columnname((*cit)->name());
            dat->set_data(s->default_reportdata());
            if ((*cit)->columntype()==hk_column::textcolumn || (*cit)->columntype()==hk_column::memocolumn)
            {
                dat->set_beforedata(rcsv->textdelimiter());
                dat->set_afterdata(rcsv->textdelimiter());

            }
            cit++;
        }
    }

}


// XML STUFF

hk_reportxml::hk_reportxml(void):hk_report()
{
    set_recodefunction("UTF8");
    set_default_use_reportseparator(false);
    set_default_reportprecision(0);
    set_use_standard_storagepath(false);
    set_maindocumenttag("table");
    set_rowtag("row");
    set_includetableschema(false);
    p_excelxml=false;
    p_fieldtag=fieldname;
    configure_page();
}


hk_reportxml::~hk_reportxml(void)
{

}


void hk_reportxml::set_fieldname_as_attribute(enum_fieldtag tag)
{
    p_fieldtag=tag;
    configure_page();

}


hk_reportxml::enum_fieldtag hk_reportxml::fieldname_as_attribute(void)
{
    return p_fieldtag;
}

void hk_reportxml::set_excelxml(bool e)
{
p_excelxml=e;
configure_page();
}

bool hk_reportxml::excelxml(void)
{
return p_excelxml;
}

bool hk_reportxml::set_maindocumenttag(const hk_string& t)
{
    if (t.size()==0) return false;
    p_maindocumenttag=t;
    configure_page();
    return true;

}


hk_string hk_reportxml::maindocumenttag(void)
{
    return p_maindocumenttag;
}


bool hk_reportxml::set_rowtag(const hk_string& t)
{
    if (t.size()==0) return false;
    p_rowtag=t;
    configure_page();
    return true;
}


hk_string hk_reportxml::rowtag(void)
{
    return p_rowtag;
}




void hk_reportxml::configure_page(void)
{
    if (excelxml())
    {
      configure_excelpage();
      
      return;
    }
    ;
   set_print_full_pages_only(false,false);
   set_fullpagereplacefunction("None",false);
   set_default_reportdataconfigurefunction("None",false);
    
    hk_string header= "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<!DOCTYPE %DT%>\n<%DT%>\n";
    header=replace_all("%DT%",header,p_maindocumenttag);
    hk_string footer=replace_all("%DT%","</%DT%>\n",p_maindocumenttag);
    hk_reportdata* d=NULL;
    if (page_header()->datalist()->size()==0) d=page_header()->new_data();
    else d=page_header()->data_at(0);
    if (d!=NULL)
    {
        d->set_data(header);
    }

    if (page_footer()->datalist()->size()==0) d=page_footer()->new_data();
    else d=page_footer()->data_at(0);
    if (d!=NULL)
    {
       if (!p_include_tableschema) d->set_data(footer);
       else d->set_data("");
    }

    datasection()->set_default_beforereportdata(p_fieldtag==fieldname?"   <%COLUMNNAME%>":"   <field NAME=\"%COLUMNNAME%\">");
    datasection()->set_default_afterreportdata(p_fieldtag==fieldname?"</%COLUMNNAME%>\n":"</field>\n");
    datasection()->set_sectionbegin(replace_all("%RT%","  <%RT%>\n",p_rowtag));
    datasection()->set_sectionend(replace_all("%RT%","  </%RT%>\n",p_rowtag));
    datasection()->set_automatic_create_data(true);
}



void    hk_reportxml::configure_excelpage(void)
{  
   set_print_full_pages_only(true,false);
   set_fullpagereplacefunction("ExcelXML",false);
   set_default_reportdataconfigurefunction("ExcelXML",false);
   datasection()->set_default_reportdataconfigurefunction("ExcelXML",false);
  hk_string header="<?xml version=\"1.0\"?>\n\
<?mso-application progid=\"Excel.Sheet\"?>\n\
<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\
 xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n\
 xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n\
 xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\
 xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n\
 <DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">\n\
  <Author>HK_CLASSES</Author>\n\
  <LastAuthor>HK_CLASSES</LastAuthor>\n\
  <Created>%DATETIME%</Created>\n\
  <LastSaved>%DATETIME%</LastSaved>\n\
  <Version>11.6360</Version>\n\
 </DocumentProperties>\n\
 <ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\n\
  <WindowHeight>9210</WindowHeight>\n\
  <WindowWidth>11595</WindowWidth>\n\
  <WindowTopX>360</WindowTopX>\n\
  <WindowTopY>60</WindowTopY>\n\
  <ProtectStructure>False</ProtectStructure>\n\
  <ProtectWindows>False</ProtectWindows>\n\
 </ExcelWorkbook>\n\
 <Styles>\n\
  <Style ss:ID=\"Default\" ss:Name=\"Normal\">\n\
   <Alignment ss:Vertical=\"Bottom\"/>\n\
   <Borders/>\n\
   <Font/>\n\
   <Interior/>\n\
   <NumberFormat/>\n\
   <Protection/>\n\
  </Style>\n\
 </Styles>\n<Worksheet ss:Name=\"Tabelle1\">\n\
  <Table ss:ExpandedColumnCount=\"%COLS%\" ss:ExpandedRowCount=\"%ROWS%\" x:FullColumns=\"1\"\
   x:FullRows=\"1\" ss:DefaultColumnWidth=\"60\">\n";
    hk_datetime dt;
    dt.set_now();
    dt.set_datetimeformat("Y-M-DTh:m:sZ");
    header=replace_all("%DATETIME%",header,dt.datetime_asstring());
    hk_string footer="  </Table>\n\
  <WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\">\n\
   <PageSetup>\n\
    <Header x:Margin=\"0.4921259845\"/>\n\
    <Footer x:Margin=\"0.4921259845\"/>\n\
    <PageMargins x:Bottom=\"0.984251969\" x:Left=\"0.78740157499999996\"\n\
     x:Right=\"0.78740157499999996\" x:Top=\"0.984251969\"/>\n\
   </PageSetup>\n\
   <Selected/>\n\
   <Panes>\n\
    <Pane>\n\
     <Number>3</Number>\n\
     <ActiveRow>0</ActiveRow>\n\
     <ActiveCol>0</ActiveCol>\n\
    </Pane>\n\
   </Panes>\n\
   <ProtectObjects>False</ProtectObjects>\n\
   <ProtectScenarios>False</ProtectScenarios>\n\
  </WorksheetOptions>\n\
 </Worksheet>\n\
</Workbook>\n";
    hk_reportdata* d=NULL;
    if (page_header()->datalist()->size()==0) d=page_header()->new_data();
    else d=page_header()->data_at(0);
    if (d!=NULL)
    {
        d->set_data(header);
    }

    if (page_footer()->datalist()->size()==0) d=page_footer()->new_data();
    else d=page_footer()->data_at(0);
    if (d!=NULL)
    {
       d->set_data(footer);
    }

    datasection()->set_default_beforereportdata("<Cell><Data ss:Type=\"%TYPE%\">");
    datasection()->set_default_afterreportdata("</Data></Cell>\n");
    datasection()->set_sectionbegin("<Row>\n");
    datasection()->set_sectionend("</Row>\n");
    datasection()->set_automatic_create_data(true);










}









void    hk_reportxml::widget_specific_before_execute(void)
{
    setlocale(LC_NUMERIC,"C");
}


void    hk_reportxml::widget_specific_after_execute(void)
{
    setlocale(LC_NUMERIC,locale().c_str());
}


hk_string  recode_utf8(const hk_string& where,hk_report*)
{
    return l2u(where);
}


void hk_reportxml::set_includetableschema(bool i)
{
    p_include_tableschema=i;
}


bool hk_reportxml::includetableschema(void)
{
    return p_include_tableschema;

}


void hk_reportxml::widget_specific_before_last_outstream_closed(void)
{
    if (p_include_tableschema) 
      {
        datasource()->save_datasourcedefinition(*outputstream());
        hk_string footer=replace_all("%DT%","\n</%DT%>\n",p_maindocumenttag);
	(*outputstream())<<footer;
      }
}



hk_string  fullpagereplace_excelxml(const hk_string& where,hk_report* report)
{
  hk_report* r=NULL;
  if (report->masterreport()) r=report->masterreport();
  else r=report;
  if (!r || !r->datasource()) return where;
  hk_string cols=format_number(r->datasource()->columns()->size(),false,0);
  hk_string rows=format_number(r->datasource()->max_rows(),false,0);
  hk_string result=replace_all("%COLS%",where,cols);
  
  result=replace_all("%ROWS%",result,rows);
  return result;
}

hk_string replaceexcelxmldatafunction(hk_reportdata* rd,const hk_string& value)
{
if (!rd) return value; 
if (!rd->column()) return value;
hk_string type="String";
if (is_numerictype(rd->column())) type="Number";
hk_string result=replace_all("%TYPE%",value,type);
return result;
}




void   configure_excelxmldata(hk_reportdata* d)
{
    if (d==NULL) return;
    d->set_replacefunction("ExcelXML",false);
}

