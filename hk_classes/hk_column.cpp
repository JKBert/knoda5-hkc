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
//$Revision: 1.67 $
#include "hk_column.h"
#include "hk_datasource.h"
#include "hk_dsdatavisible.h"
#include "hk_datetime.h"
#include <stdio.h>
#include <string.h>

hk_column::hk_column(hk_datasource* ds, const hk_string& lTRUE,const hk_string& lFALSE)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::hk_column");
//   wanna_debug(true);
#endif
    p_datasource=ds;
    p_size=2;
    p_columntype=integercolumn;
    p_driver_specific_delimit_bool=false;
    p_sqltextdelimiter=defaulttextdelimiter;
    p_sqlnulldelimiter=" ";
    p_primary_index=false;
    p_notnull=false;
    p_table_origin_already_set=false;
    set_has_not_changed();
    p_readonly=false;
    p_binary.length=0;
    p_binary.data=NULL;
    p_driver_specific_data=NULL;
    p_original_new_data=NULL;
    p_driver_specific_data_size=0;
    p_original_new_data_size=0;
    set_boolvalues(lTRUE,lFALSE);
    p_dateformat=p_defaultdateformat;
    p_timeformat=p_defaulttimeformat;
    p_datetimeformat=p_defaultdatetimeformat;
    p_driverspecific_dateformat="Y-M-D";
    p_driverspecific_timeformat="h:m:s";
    p_driverspecific_datetimeformat="Y-M-D h:m:s";
    p_driverspecific_timestampformat="Y-M-D h:m:s";
    p_driverspecific_locale="C";
    p_driverspecific_digits=8;
    p_allow_autoincwrite=false;
    p_definitionmode=true;

}


hk_column::~hk_column()
{
#ifdef HK_DEBUG
    hkdebug("hk_column::~hk_column");
#endif
    if (p_datasource)p_datasource->column_remove(this);
    if (p_driver_specific_data!=NULL) delete[] p_driver_specific_data ;
    p_driver_specific_data=NULL;
    if (p_original_new_data!=NULL) delete[] p_original_new_data;
    p_original_new_data=NULL;
}


void hk_column::set_name(const hk_string& n)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_name");
#endif

    if (!in_definitionmode())
    {
        show_warningmessage("hk_column::set_name tried to change the column name outside definitionmode");
        return;
    }
    p_columnname=n;
}


hk_string hk_column::name(void) const
{
    return p_columnname;
}


bool hk_column::is_readonly(void) const
{
    if (p_datasource->is_readonly()) return true;
    if (p_columntype==auto_inccolumn&&!p_allow_autoincwrite) return true;
    return p_readonly;
}

hk_string    hk_column::get_delimiter(void)
{
	if (is_numerictype(this)|| (!p_driver_specific_delimit_bool&&p_columntype==boolcolumn))
	{
		return p_sqlnulldelimiter;
	}
	else {
		return p_sqltextdelimiter;
	}
}


void    hk_column::set_allow_autoincwrite(bool r)
{
    p_allow_autoincwrite=r;
}


bool    hk_column::allow_autoincwrite(void)
{
    return p_allow_autoincwrite;

}


void hk_column::set_readonly(bool r)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_readonly");
#endif
    if (in_definitionmode()    )
        p_readonly=r;
    else show_warningmessage("hk_column::set_readonly outside definitionmode");
}


void    hk_column::set_columntype(enum_columntype f)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_columntype");
#endif
    if (in_definitionmode()  )
    {
        p_columntype=f;
    }
    else show_warningmessage("hk_column::set_columntype outside definitionmode");
}


hk_column::enum_columntype  hk_column::columntype( void) const
{
    return p_columntype;
}


bool hk_column::is_primary(void)
{
    return p_primary_index;

}


bool hk_column::set_primary(bool i)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_primary");
#endif

    if (in_definitionmode())
    {
        p_primary_index=i;
        return true;
    }
    else
    {

        show_warningmessage("hk_column::set_primary outside definitionmode");

    }

    return false;
}


bool hk_column::is_notnull(void)
{
    return p_notnull;

}


bool hk_column::set_notnull(bool i)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_notnull");
#endif
    if (in_definitionmode())
    {
        p_notnull=i;
        return true;
    }
    else
    {
        show_warningmessage("hk_column::set_notnull outside definitionmode");
    }

    return false;
}


long hk_column::size(void)
{

    return p_size;

}


void hk_column::set_size(long s)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_size");
#endif
    if (in_definitionmode())
        p_size=s;
    else show_warningmessage("hk_column::set_size outside definitionmode");

}


hk_string hk_column::asstring(bool is_locale)
{
    if(p_datasource->is_enabled()&&(p_datasource->max_rows()>0))
    {
        return asstring_at(p_datasource->row_position(),is_locale);
    }
    else return "";
}


hk_string hk_column::changed_data_asstring(bool is_locale)
{
    if (p_has_changed)
    {
        if (is_locale)
        {
            if (p_columntype==datecolumn)
                return transfer_date(p_new_data_asstring,p_driverspecific_dateformat,p_dateformat);
            if (p_columntype==timecolumn)
                return transfer_time(p_new_data_asstring,p_driverspecific_timeformat,p_timeformat);
            if (p_columntype==datetimecolumn)
                return transfer_datetime(p_new_data_asstring,p_driverspecific_datetimeformat,p_datetimeformat);
            if (p_columntype==timestampcolumn)
                return transfer_datetime(p_new_data_asstring,p_driverspecific_timestampformat,p_datetimeformat);
            if (is_numerictype(this)&&!changed_data_is_nullvalue())
                return format_number(p_new_data_asstring,false,false,(is_integertype(this)?0:p_driverspecific_digits),locale());

        }
        return p_new_data_asstring;
    }
    else return "";
}


hk_string hk_column::columntype_name()
{
    switch(p_columntype)
    {
        case floatingcolumn :      return   hk_translate("Float");
        case smallfloatingcolumn : return   hk_translate("Small Float");
        case integercolumn  :        return     hk_translate("Integer");
        case smallintegercolumn  :    return    hk_translate("Small Integer");
        case textcolumn     :        return     hk_translate("Text");
        case datetimecolumn :      return   hk_translate("Datetime");
        case timestampcolumn :      return      hk_translate("Timestamp");
        case timecolumn     :      return   hk_translate("Time");
        case datecolumn     :      return       hk_translate("Date");
        case boolcolumn     :      return       hk_translate("Bool");
        case binarycolumn     :      return     hk_translate("Binary");
        case memocolumn     :      return       hk_translate("Memo");
        case auto_inccolumn :      return       hk_translate("Auto Increment");
        default           :      return     hk_translate("Unknown columntype");

    }

}


bool hk_column::has_changed(void)
{
    return p_has_changed;
}


void hk_column::set_asstring(const hk_string& s, bool registerchange)
{
    set_asstring(s,registerchange,true);

}


void hk_column::set_asstring(const hk_string& s,bool registerchange, bool is_locale)
{

#ifdef HK_DEBUG
    hkdebug("hk_column::set_asstring(hk_string s)",s);
#endif
    if (is_readonly())return;
    hk_string buf=s;
    if (is_locale)
    {
        if (p_columntype==datecolumn) buf=transfer_date(buf,p_dateformat,p_driverspecific_dateformat);
        if (p_columntype==timecolumn) buf=transfer_time(buf,p_timeformat,p_driverspecific_timeformat);
        if (p_columntype==datetimecolumn) buf=transfer_datetime(buf,p_datetimeformat,p_driverspecific_datetimeformat);

        if (p_columntype==timestampcolumn) buf=transfer_datetime(buf,p_datetimeformat,p_driverspecific_timestampformat);
        if (is_numerictype(this))
        {
            buf=format_standard_number(buf,false,(is_integertype(this)?0:p_driverspecific_digits),p_driverspecific_locale);
        }
    }
    if (p_columntype==boolcolumn) buf= (string2upper(s)=="TRUE"?p_true:p_false);
    if (p_driver_specific_data!=NULL)
    {
        delete[] p_driver_specific_data;
        p_driver_specific_data=NULL;
    }
    if (p_datasource->is_enabled())
    {
        if(driver_specific_asstring(buf))
        {
            if (registerchange) set_has_changed();
            p_new_data_asstring=buf;

#ifdef HK_DEBUG
            hkdebug("column: data marked as changed");
#endif

        }
        else
        {
#ifdef HK_DEBUG
            hkdebug("column: data NOT marked as changed");
#endif
        }

#ifdef HK_DEBUG
        hkdebug("datasource enabled => data update");
        if (p_has_changed)hkdebug("p_datachanged = true"); else hkdebug("p_datachanged = false");
        hkdebug(s);
#endif

    }
    else
    {
#ifdef HK_DEBUG
        hkdebug("datasource not enabled => no data update");
#endif
    }
}


hk_string hk_column::asstring_at(unsigned long a,bool is_locale)
{
    if (p_columntype==binarycolumn)return hk_translate("Binary");
    if (!(p_datasource->is_enabled()&&(p_datasource->max_rows()>0)&&(a<p_datasource->max_rows())))
        return "";

    if (is_locale)
    {
        if (p_columntype==datecolumn)
            return transfer_date(driver_specific_asstring_at(a),p_driverspecific_dateformat,p_dateformat);
        if (p_columntype==timecolumn)
            return transfer_time(driver_specific_asstring_at(a),p_driverspecific_timeformat,p_timeformat);
        if (p_columntype==datetimecolumn)
            return transfer_datetime(driver_specific_asstring_at(a),p_driverspecific_datetimeformat,p_datetimeformat);
        if (p_columntype==timestampcolumn)
        {
            return transfer_datetime(driver_specific_asstring_at(a),p_driverspecific_timestampformat,p_datetimeformat);
        }
        if (is_numerictype(this)&& !is_nullvalue_at(a))
            return format_number(driver_specific_asstring_at(a),false,false,(is_integertype(this)?0:p_driverspecific_digits),locale());

    }
    if (columntype()==boolcolumn)    return (driver_specific_asstring_at(a)==p_true?"TRUE":"FALSE");

    return driver_specific_asstring_at(a);
}


const char* hk_column::transformed_asstring(void)
{
    return driver_specific_transformed_asstring_at(p_datasource->row_position());
}


const char* hk_column::transformed_asstring_at(unsigned long position)
{
    return driver_specific_transformed_asstring_at(position);
}


const struct_raw_data* hk_column::asbinary(void)
{
    return asbinary_at(p_datasource->row_position());
}


void hk_column::set_asbinary(const struct_raw_data* rd)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_asbinary");
#endif
    if (is_readonly())return;

    if (rd==NULL)return;
    if (p_original_new_data!=NULL)
    {
        delete[] p_original_new_data;
        p_original_new_data=NULL;
    }
    p_original_new_data=new char[rd->length];
    p_original_new_data_size=rd->length;
    memcpy(p_original_new_data,rd->data,rd->length);
    driver_specific_asbinary(p_original_new_data);
    set_has_changed();

}


const struct_raw_data* hk_column::asbinary_at(unsigned long a)
{
    return driver_specific_asbinary_at(a);
}


const struct_raw_data* hk_column::transformed_changed_data(void)
{
    p_transformed_changed_data_struct.length=p_driver_specific_data_size;
    p_transformed_changed_data_struct.data=p_driver_specific_data;
    return &p_transformed_changed_data_struct;
}


const struct_raw_data* hk_column::changed_data(void)
{
    p_changed_data_struct.length=p_original_new_data_size;
    p_changed_data_struct.data=p_original_new_data;
    return &p_changed_data_struct;

}


unsigned int hk_column::fieldnumber(void)
{
    return p_fieldnr;
}


void hk_column::reset_changed_data(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::reset_changed_data");
#endif
    set_has_not_changed();
    p_original_new_data_size=0;
    if (p_original_new_data!=NULL)delete[] p_original_new_data;
    p_original_new_data=NULL;
    p_driver_specific_data_size=0;
    if (p_driver_specific_data!=NULL)delete[] p_driver_specific_data;
    p_driver_specific_data=NULL;

}


void hk_column::set_asbool(bool b)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_asbool");
#endif
    if (is_readonly())return;

    driver_specific_asbool(b);
    set_has_changed();

}


bool hk_column::asbool(void)
{
    return driver_specific_asbool_at(p_datasource->row_position());
}


bool hk_column::asbool_at(unsigned long a)
{
    return driver_specific_asbool_at(a);
}


bool hk_column::changed_data_asbool(void)
{
    if (is_numerictype(this))
    {
        return (format_number(p_new_data_asstring,false,false,0,locale())==p_true);
    }
    return (p_new_data_asstring==p_true);

}


void hk_column::set_boolvalues(const hk_string& aTRUE,const hk_string& aFALSE)
{
    if (aTRUE==aFALSE)return;
    p_true=aTRUE;
    p_false=aFALSE;
}


hk_string hk_column::bool_truevalue(void)
{
    return p_true;
}


hk_string hk_column::bool_falsevalue(void)
{
    return p_false;
}


unsigned int hk_column::find(unsigned int from,unsigned int to,const hk_string& searchtext,bool wholephrase,bool casesensitive,bool backwards)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::find(unsigned int from,unsigned int to,const hk_string& searchtext",searchtext);
#endif
    unsigned int p_from, p_to, p_buffer;
    if (p_datasource->max_rows()==0)return 1;
    if (from <p_datasource->max_rows())p_from=from; else p_from=0;
    if (to <p_datasource->max_rows()) p_to=to;else p_to=p_datasource->max_rows()-1;
    if (p_from>p_to)
    {
        p_buffer=p_from;
        p_from=p_to;
        p_to=p_buffer;
    }
    if (backwards)
    {
        p_buffer=p_from;
        p_from=p_to;
        p_to=p_buffer;
    }
    for (unsigned int k=p_from;(backwards?((k>=p_to)&&(k<=p_from)):k<=p_to);(backwards?k--:k++))
    {
        if (is_findstring(k,searchtext,wholephrase,casesensitive))return k;
    }
    return p_datasource->max_rows()+1;
}


bool hk_column::is_findstring(unsigned int pos,const hk_string& searchtext,bool wholephrase,bool casesensitive)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::is_findstring");
#endif
    hk_string search=searchtext;
    hk_string where=is_numerictype(this)?format_number(asstring_at(pos,true),true,false,(is_integertype(this)?0:p_driverspecific_digits),locale()):asstring_at(pos,true);
    if (!casesensitive)
    {
        for (unsigned int tt=0;tt<where.size();tt++)
        {
            where[tt]=toupper(where[tt]);
        }
        for (unsigned int tt=0;tt<search.size();tt++)
        {
            search[tt]=toupper(search[tt]);
        }
    }
    if (wholephrase)
    {
        if (where==search)return true;
    }
    else
    {
        hk_string::size_type a=where.find(search);
        if (a!=hk_string::npos) return true;
    }

    return false;

}


unsigned int hk_column::find(const hk_string& searchtext,bool wholephrase,bool casesensitive,bool backwards)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::find(const hk_string& searchtext)",searchtext);
#endif
    return find(0,p_datasource->max_rows()-1,searchtext,wholephrase,casesensitive,backwards);
}


bool hk_column::load_from_file(const hk_string& filename)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::load_from_file");
#endif
    long fsize;
    FILE *in;
    in=fopen(filename.c_str(),"r");
    if (in==NULL)return false;
    fseek(in,0,SEEK_END);
    fsize=ftell(in);
    fseek(in,0,SEEK_SET);
    if (p_original_new_data!=NULL)
    {
        delete[] p_original_new_data;
        p_original_new_data=NULL;
    }
    p_original_new_data=new char[fsize];
    p_original_new_data_size=fsize;
    fread(p_original_new_data,fsize,1,in);
    driver_specific_asbinary(p_original_new_data);
    set_has_changed();

    return true;
}


bool hk_column::save_to_file(const hk_string& filename)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::save_to_file");
#endif

   if (columntype()==binarycolumn)
   {
    const struct_raw_data* res=asbinary();
    if (res==NULL)return false;
    return save_file(filename,res);
   }
   else return save_textfile(filename,asstring());
}


bool    hk_column::driver_specific_asstring(const hk_string& )
{
    return true;
}


hk_string hk_column::driver_specific_asstring_at(unsigned long )
{
    return "";
}


const struct_raw_data* hk_column::driver_specific_asbinary_at(unsigned long )
{
    return NULL;
}


bool        hk_column::driver_specific_asbinary(const char* )
{
    return false;
}


void        hk_column::driver_specific_asbool(bool )
{
}


bool hk_column::driver_specific_asbool_at(unsigned long )
{
    return false;
}


void hk_column::datavisible_add(hk_dsdatavisible* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::datavisible_add");
#endif
    p_datavisibles.insert(p_datavisibles.end(),v);
}


void hk_column::datavisible_remove(hk_dsdatavisible* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::datavisible_remove");
#endif
    p_datavisibles.remove(v);
}


void hk_column::data_has_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::data_has_changed");
#endif
    list<hk_dsdatavisible*>::iterator it=p_datavisibles.begin();
    while (it!=p_datavisibles.end())
    {
        (*it)->columndata_has_changed();
        it++;
    }
}


void hk_column::set_has_changed()
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_has_changed");
#endif
    p_has_changed=true;
    p_datasource->set_has_changed();
    data_has_changed();
}


void hk_column::set_has_not_changed()
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_has_not_changed");
#endif
    p_has_changed=false;
}


void hk_column::save_columndefinition(ostream& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::savestructure");
#endif

    hk_string dtag="COLUMNDEFINITION";
    hk_string buffer;
    switch (p_columntype)
    {
        case textcolumn : buffer="TEXTFIELD";break;
        case auto_inccolumn : buffer="AUTOINCFIELD";break;
        case smallintegercolumn : buffer="SMALLINTFIELD";break;
        case integercolumn : buffer="INTFIELD";break;
        case smallfloatingcolumn : buffer="SMALLFLOATFIELD";break;
        case floatingcolumn : buffer="FLOATFIELD";break;
        case datecolumn : buffer="DATEFIELD";break;
        case datetimecolumn : buffer="DATETIMEFIELD";break;
        case timestampcolumn : buffer="TIMESTAMPFIELD";break;
        case timecolumn : buffer="TIMEFIELD";break;
        case binarycolumn : buffer="BINARYFIELD";break;
        case memocolumn : buffer="MEMOFIELD";break;
        case boolcolumn : buffer="BOOLFIELD";break;
        default: buffer="OTHERFIELD";
    }

    start_mastertag(s,dtag);
    set_tagvalue(s,"COLUMNNAME",p_columnname);
    set_tagvalue(s,"COLUMNTYPE",buffer);
    set_tagvalue(s,"COLUMNSIZE",p_size);
    set_tagvalue(s,"COLUMNPRIMARYINDEX",p_primary_index);
    set_tagvalue(s,"COLUMNNOTNULL",p_notnull);
    end_mastertag(s,dtag);

}


void hk_column::load_columndefinition(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::loadstructure");
#endif
    enum_columntype f=othercolumn;
    hk_string buffer;

    get_tagvalue(definition,"COLUMNNAME",p_columnname);
    p_columnname=p_datasource->systemcolumnname(p_columnname);
    get_tagvalue(definition,"COLUMNTYPE",buffer);

    if ( buffer=="TEXTFIELD")f=textcolumn ;
    else if (buffer=="AUTOINCFIELD")f=auto_inccolumn;
    else if (buffer=="SMALLINTFIELD")f=smallintegercolumn;
    else if (buffer=="INTFIELD")f=integercolumn;
    else if (buffer=="SMALLFLOATFIELD")f=smallfloatingcolumn;
    else if (buffer=="FLOATFIELD")f=floatingcolumn;
    else if (buffer=="DATEFIELD")f=datecolumn;
    else if (buffer=="DATETIMEFIELD")f=datetimecolumn;
    else if (buffer=="TIMESTAMPFIELD")f=timestampcolumn;
    else if (buffer=="TIMEFIELD")f=timecolumn;
    else if (buffer=="BINARYFIELD")f=binarycolumn;
    else if (buffer=="MEMOFIELD")f=memocolumn;
    else if (buffer=="BOOLFIELD")f=boolcolumn;
    set_columntype(f);
    get_tagvalue(definition,"COLUMNSIZE",p_size);
    get_tagvalue(definition,"COLUMNPRIMARYINDEX",p_primary_index);
    get_tagvalue(definition,"COLUMNNOTNULL",p_notnull);

}


hk_datasource* hk_column::datasource(void)
{

    return p_datasource;
}


bool hk_column::is_nullvalue(void)
{
    return is_nullvalue_at(p_datasource->row_position());
}


bool hk_column::is_nullvalue_at(unsigned long a)
{
    if(p_datasource->is_enabled()&&(p_datasource->max_rows()>0))
    {
        return driver_specific_is_nullvalue_at(a);
    }
    else return false;
}


bool        hk_column::driver_specific_is_nullvalue_at(unsigned long)
{
    return false;
}


void hk_column::set_asnullvalue(bool registerchange)
{
#ifdef HK_DEBUG
    hkdebug("hk_column::set_asnullvalue");
#endif
    if (is_readonly()||!p_datasource->is_enabled())return;
    if (p_driver_specific_data!=NULL)
    {
        delete[] p_driver_specific_data;
        p_driver_specific_data=NULL;
    }
    if (p_original_new_data!=NULL)
    {
        delete[] p_original_new_data;
        p_original_new_data=NULL;
    }
    p_original_new_data_size=0;

    if (registerchange) set_has_changed();
    p_new_data_asstring="";

}


bool hk_column::changed_data_is_nullvalue(void)
{
  if (!has_changed()) return false;
  return p_original_new_data==NULL;
}



void hk_column::set_definitionmode(bool def)
{
    p_definitionmode=def;

}


bool hk_column::in_definitionmode(void)
{
    if ((p_datasource->mode()==hk_datasource::mode_createtable)||
        (p_datasource->mode()==hk_datasource::mode_altertable)) return true;
    if (!p_definitionmode)
        cout <<"not in definitionmode"<<endl;

    return p_definitionmode;

}

void hk_column::before_alter_table()
{
    if (
                p_columnname==p_originalcolumnname&&
                p_columntype==p_originalcolumntype&&
                p_size==p_originalsize&&
                p_primary_index==p_originalprimary_index&&
                p_notnull==p_originalnotnull
        )  return;

p_datasource->alter_column(p_originalcolumnname,
				 p_columnname==p_originalcolumnname?NULL:&p_columnname,
                p_columntype==p_originalcolumntype?NULL:&p_columntype,
		p_size==p_originalsize?NULL:&p_size,
		NULL,
		p_primary_index==p_originalprimary_index?NULL:&p_primary_index,
                p_notnull==p_originalnotnull?NULL:&p_notnull);


}





void hk_column::set_asinteger(long int z, bool registerchange)
{
   set_asstring(longint2string(z),registerchange,false);
}

long int hk_column::asinteger(void)
{
    if(p_datasource->is_enabled()&&(p_datasource->max_rows()>0))
    {
        return asinteger_at(p_datasource->row_position());
    }
    else return 0;

}

long int hk_column::asinteger_at(unsigned long  a)
{
     if (columntype()==boolcolumn)    return (driver_specific_asstring_at(a)==p_true?1:0);
     return(long int) standardstring2double(driver_specific_asstring_at(a));
}


long int hk_column::changed_data_asinteger(void)
{
     if (columntype()==boolcolumn)    return (p_new_data_asstring==p_true?1:0);
    return (long int) standardstring2double(p_new_data_asstring);
}


void hk_column::set_asdouble(double z, bool registerchange)
{

   set_asstring(format_number(z,true,p_driverspecific_digits),registerchange);
}

double hk_column::asdouble(void)
{
    if(p_datasource->is_enabled()&&(p_datasource->max_rows()>0))
    {
        return asdouble_at(p_datasource->row_position());
    }
    else return 0.0;

}

double hk_column::asdouble_at(unsigned long  a)
{
    return standardstring2double(driver_specific_asstring_at(a));
}

double hk_column::changed_data_asdouble(void)
{
    return standardstring2double(p_new_data_asstring);
}




hk_string hk_column::curval_asstring(void)
    {
      if (has_changed()) return changed_data_asstring();
      else return asstring();
    }


double hk_column::curval_asdouble(void)
    {
      if (has_changed()) return changed_data_asdouble();
      else return asdouble();
    }

long int hk_column::curval_asinteger(void)
    {
      if (has_changed() )return changed_data_asinteger();
      else return asinteger();
    }

bool hk_column::curval_asbool(void)
    {
      if (has_changed() )return changed_data_asbool();
      else return asbool();
    }

long int hk_column::count(unsigned int from,unsigned int to)
{
 long int result=0;
 unsigned int c=from;
 while (c<=to && c<datasource()->max_rows())
 {
   if (!is_nullvalue_at(c))
     ++result;
   ++c;
 }
 return result;
}

double hk_column::sum(unsigned int from,unsigned int to)
{
 from=(from<to?from:to);
 to=(from>to?from:to);
 double result=0;
 unsigned int c=from;
 while (c<=to && c<datasource()->max_rows())
 {
   if (!is_nullvalue_at(c))
     result+=asdouble_at(c);
   ++c;
 }

 return result;

}

long int hk_column::count()
{
   return count(0,p_datasource->max_rows()-1);
}

double hk_column::sum()
{
   return sum(0,p_datasource->max_rows()-1);

}


hk_string hk_column::tableorigin()
{
  if (datasource()->type()==hk_datasource::ds_table) return datasource()->name();
  else
  {
  if (!p_table_origin_already_set)
   datasource()->set_columns_tableorigin();
  return p_tableorigin;
  }
}


