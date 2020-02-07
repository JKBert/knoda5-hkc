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
//$Revision: 1.6 $
#include "hk_url.h"

hk_url::hk_url()
{
 init();
}


hk_url::hk_url(const hk_string& s)
{
 init();
 set_url(s);
}

void hk_url::init()
{
  p_isvalid=false;
  p_islocal=true;
  p_extension=p_url=p_filename=p_directory="";
}


bool hk_url::set_url(const hk_string& s)
{
 p_url=trim(s);
 p_isvalid=parse_url();
 return p_isvalid;
}

bool hk_url::operator=(const hk_url& u)
{
  p_isvalid=u.p_isvalid;
  p_islocal=u.p_islocal;
  p_url=u.p_url;
  p_filename=u.p_filename;
  p_directory=u.p_directory;
  p_extension=u.p_extension;
  return true;
}

bool hk_url::operator=(const hk_string& u)
{
  return set_url(u);
}






bool hk_url::parse_url(void)
{
    //return;
    /*
    The following code does not work correctly yet
    */
    if (p_url.size()==0) return false;
    
    hk_string::size_type offset=0;
    hk_string quotedtype;
    //int open_brackets=0;
    enum
    {
	S_START, S_IN_NAME,S_IN_EXTENSION,S_IN_DOUBLEQUOTE,S_IN_QUOTE
    } state=S_START;



//parser begin
int row=1;
int col=1;
hk_string errormessages;

p_filename=p_directory=p_extension="";

while (offset<p_url.size())
{
	hk_string x(1,p_url[offset]);
	switch (state)
	{
	case S_START:   //cerr <<"S_START#"<<x<<"#"<<endl;
			if (isspace(x[0]))
				{
					if (x=="\n")
					{
						++row;col=0;
					}
				  break;
				}
			if (x=="/")
			{
			  p_directory+=p_filename+"/";
			  p_filename="";  
			}
			else
			{
			  p_filename+=x;
			  if (x=="\"")state=S_IN_DOUBLEQUOTE;
			  else
			  if(x=="'") state=S_IN_QUOTE;
			  else 
			  state=S_IN_NAME;
			}  
			break;

	case S_IN_NAME: //cerr <<"S_IN_NAME"<<endl;
			if (x=="/")
			{
			  p_directory+=p_filename+"/";
			  p_filename="";  
			  state=S_START;
			  break;
			}
			if (x=="\"")
			  state=S_IN_DOUBLEQUOTE;
			else
			if (x=="'")
			  state=S_IN_QUOTE;  
			else
			if (x==".")
			{
			  state=S_IN_EXTENSION;
			  break;
			}  
			
			p_filename+=x;
	                 
			break;		
	case S_IN_EXTENSION: //cerr <<"S_IN_EXTENSION"<<endl;
	                 if (x=="/")
			  {
			    state=S_START;
			    p_directory+=p_filename+"."+p_extension;
			    p_filename=p_extension="";
			    break;
			  }
			 p_extension+=x;
			 
			break;		
	case S_IN_QUOTE: //cerr <<"S_IN_QUOTE"<<endl;
	                  p_filename+=x;
			  if (x=="'") state=S_IN_NAME; 
			break;		
	case S_IN_DOUBLEQUOTE: //cerr <<"S_IN_DOUBLEQUOTE"<<endl;
	                  p_filename+=x;
			  if (x=="\"") state=S_IN_NAME; 
			break;		
	}
	++offset;++col;



}
  
  
  /*cerr <<"p_url      =#"<<p_url<<"#"<<endl;
  cerr <<"p_directory=#"<<p_directory<<"#"<<endl;
  cerr <<"p_filename =#"<<p_filename<<"#"<<endl;
  cerr <<"p_extension=#"<<p_extension<<"#"<<endl;*/
  bool result=true;
  switch (state)
  {
   case S_IN_QUOTE :
   case S_IN_DOUBLEQUOTE:
   case S_START:
   			result=false;
			break;
   default: 		result=true;
  }
  //cerr <<"result     ="<<result<<endl;
  return result;

}


