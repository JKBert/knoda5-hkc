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
//$Revision: 1.23 $
#include "hk_definitions.h"
#include "hk_class.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <locale.h>
#include <vector>
#include "config.h"

#include "hk_column.h"

int hk_random(int min,int max)
{
    int l;
    double e;
    e=(double) min+(rand()/(RAND_MAX+0.5))*(max-min+1);
    l=(int)(e);
    return l;
}


bool is_numerictype(hk_column* c)
{
    if (c==NULL) return false;

    return (is_integertype(c)||is_realtype(c));

}


bool is_integertype(hk_column*c)
{
    if (c==NULL) return false;

    switch (c->columntype())
    {
        case hk_column::auto_inccolumn :;
        case hk_column::smallintegercolumn:;
        case hk_column::integercolumn:return true;

        default: return false;
    }

}


bool is_realtype(hk_column*c)
{
    if (c==NULL) return false;

    switch (c->columntype())
    {
        case hk_column::smallfloatingcolumn :;
        case hk_column::floatingcolumn :return true;

        default: return false;
    }

}

double inch2cm(double i)
{
 return 2.54*i;
}
double cm2inch(double c)
{
  return c/2.54;
}




referentialclass::referentialclass()
{
p_deletecascade=false;
p_updatecascade=false;
}

bool referentialclass::operator=(const referentialclass& r)
{
   p_deletecascade=r.p_deletecascade;
   p_updatecascade=r.p_updatecascade;
   p_name=r.p_name;
   p_masterdatasource=r.p_masterdatasource;
   p_fields.erase(p_fields.begin(),p_fields.end());
   copy(r.p_fields.begin(),r.p_fields.end(),back_inserter(p_fields));

   return true;
}



bool load_file(const hk_string& filename, struct_raw_data* result)
{
    if (!result) return false;
    FILE *in;
    in=fopen(filename.c_str(),"r");
    if (in==NULL)
      {
      show_warningmessage(replace_all("%1",hk_class::hk_translate("File '%1' does not exist"),filename));
      return false;
      }
    fseek(in,0,SEEK_END);
    long size=ftell(in);
    fseek(in,0,SEEK_SET);
    if (result->data) delete[] result->data;
    result->data=new char[size];
    result->length=size;
    fread(result->data,size,1,in);

    return true;

}

bool save_file(const hk_string& filename,const struct_raw_data* data)
{
  if (file_exists(filename))
    if (!show_yesnodialog(replace_all("%1",hk_class::hk_translate("Overwrite '%1'?"),filename),true)) return false;

    if (!data)return false;
    FILE* out=fopen(filename.c_str(),"w");

    int r=fwrite(data->data,data->length,1,out);
    fclose(out);
    if (r<(signed)data->length)return false;
    else
    return true;

}


bool data2hex(struct_raw_data* data, hk_string& result)
{
  if (!data) return false;
  unsigned long s=0;
  while (s<data->length)
  {

   if (s>0)
   {
   result+=(s%20?" ":"\n");
   }
   result+=bin2hex(data->data[s]);
   ++s;
  }
  return true;
}


bool hex2data( const hk_string& data,unsigned long maxbytes, struct_raw_data* result)
{
  if (!result||maxbytes==0) return false;
  if (result->data) delete[] result->data;
  result->data=new char[maxbytes];
  result->length=maxbytes;
  hk_string::size_type i=0;
  const char* const delimiters=" \t\n";
  const hk_string::size_type len=data.length();
  unsigned long pos=0;
  while ( pos < maxbytes && i<len )
    {
        // eat leading whitespace
        i = data.find_first_not_of (delimiters, i);
        if (i == string::npos)
            return true;   // nothing left but white space

        // find the end of the token
        hk_string::size_type j = data.find_first_of (delimiters, i);

        // push token
        if (j == string::npos) {
            result->data[pos]=hex2bin (data.substr(i));
            return true;
        } else
            result->data[pos]=hex2bin(data.substr(i, j-i));

        // set up for next loop
        i = j + 1;
        ++pos;
    }


  return true;
}


bool save_textfile(const hk_string& filename, const hk_string& data)
{
  if (file_exists(filename))
    if (!show_yesnodialog(replace_all("%1",hk_class::hk_translate("Overwrite '%1'?"),filename),true)) return false;
bool result=true;
    ofstream* stream= new ofstream(filename.c_str());
    if (stream!=NULL &&!stream->fail())
	{
        (*stream) <<data;
	}
	else result=false;
    delete stream;
   return result;

}

bool file_exists(const hk_string& filename)
{
 ifstream ifs(filename.c_str(),ios::in);
// JKB
 return !(!ifs);
}
