/*  xbfile.cpp

    Xbase64 project source code

    This file contains logic for the basic Xbase class.

    Copyright (C) 1997,2003,2004  Gary A Kunkel
                                  Sergiy Yakovin    
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


    Contact:
    
     Email:
    
      xbase64-devel@lists.sourceforge.net
      xbase64-users@lists.sourceforge.net
      
      
     Regular Mail:
     
       XBase Support
       149C South Main St
       Keller Texas, 76248     
       USA

*/

#ifdef __GNUG__
  #pragma implementation "xbfile.h"
#endif


#include "xbase64.h"

xbString xbFile::MakeFileName(const char *name)
{
  xbString file=name;
  if (file.isEmpty()) return file;
  int len=strlen(name);
  const char *extLower=GetExtWithDot(true);
  const char *extUpper=GetExtWithDot(false);
  int lenLower=strlen(extLower);
  int lenUpper=strlen(extUpper);
  if (len>lenLower && strcmp(&name[len-lenLower], extLower)==0 ||
       len>lenUpper && strcmp(&name[len-lenUpper], extUpper)==0) return file;
  char lastSymbol=name[len-1];
  file+=GetExtWithDot(lastSymbol<'A' || lastSymbol>'Z');
  return file;
}

