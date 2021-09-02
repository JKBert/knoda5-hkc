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
//$Revision: 1.52 $
#include "hk_importcsv.h"
#include "hk_database.h"
#include "hk_column.h"
#include "hk_datasource.h"
#include "hk_datetime.h"
#include <iconv.h>
#include <locale.h>
#include <nl_types.h>
#include <langinfo.h>

hk_importcsv::hk_importcsv(void):hk_dsvisible()
{
#ifdef HK_DEBUG
// wanna_debug(true);
    hkclassname("import");
    hkdebug("hk_importcsv::constructor");
#endif
    p_textdelimiter="\"";
    p_rowdelimiter="\n";
    p_betweenfields=",";
    p_filestream=NULL;
    p_firstrow_contains_fieldnames=true;
    p_create_new_table=true;
    p_datamode=true;
    p_cancelimport=true;
    p_overwrite=false;
    p_progressdialog=NULL;
    p_datetimeformat=defaultdatetimeformat();
    p_dateformat=defaultdateformat();
    p_timeformat=defaulttimeformat();
    p_detect_autoinc=true;
}


hk_importcsv::~hk_importcsv(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::destructor");
#endif
    if (p_filestream!=NULL)delete p_filestream;
    p_filestream = NULL;
}


bool hk_importcsv::execute(enum_interaction enuminteractive)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::execute");
#endif
    if (runtime_only())
    {
      show_warningmessage(hk_translate("Cannot import as you run in runtime only mode !"));
      return false;

    }
    if (datasource()==NULL) return false;
    p_datamode=true;
    if ((p_textdelimiter.size()==0)&&(p_betweenfields.size()==0)
        ||p_filename.size()==0)
    {
        reset();
        return false;
    }

    if (!initialize_table(enuminteractive))
    {
        reset();

        return false;
    }
    bool blockbuffer=datasource()->blockvisiblesignals();
    datasource()->set_blockvisiblesignals(true);
    hk_string sbuffer;
    int i=1;
    set_columns();
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::execute while");
#endif
    p_cancelimport=false;
    std::streampos spos=p_filestream->tellg();
    p_filestream->seekg(0,std::ios::end);
    unsigned long maxsize=p_filestream->tellg();
    unsigned long r=0;
    p_filestream->seekg(spos);
    int rowpos=1;
    hk_string errors;
    while (!p_filestream->eof()&&!p_cancelimport)
    {
     if (p_progressdialog)
        p_cancelimport=p_progressdialog(r,maxsize,hk_translate("Importing data ..."));
        get_line(*p_filestream,sbuffer);
        r=p_filestream->tellg();
        create_valuelist(sbuffer);
        unsigned int z=p_columnlist.size();
        if (z>datasource()->columns()->size())z=datasource()->columns()->size();
        datasource()->setmode_insertrow();
//*************************************
//*          store data               *
//*************************************
        vector<colstruct>::iterator col_it=p_columnlist.begin();
        vector<hk_string>::iterator value_it=p_valuelist.begin();
        if ((p_valuelist.size()>0)&&(p_columnlist.size()))
        {

//     for (unsigned int k=0;k<z;k++)
            while (col_it!=p_columnlist.end() && value_it!=p_valuelist.end())
            {
                if ((*col_it).col!=NULL)
                {
                    if (!(is_numerictype((*col_it).col)))
		    (*col_it).col->set_asstring((*value_it),true,true);
		    else
		    if ((*value_it)!="")
		    {
		     // handle numeric_types
		     if (is_integertype((*col_it).col))
		    (*col_it).col->set_asinteger((long int)standardstring2double((*value_it),p_locale));
		    else
		    (*col_it).col->set_asdouble(standardstring2double((*value_it),p_locale));
		    }
                }
                col_it++;
                value_it++;
            }
        }

        i++;
        bool storeresult=datasource()->store_changed_data();
        if (!storeresult)
        { 
        hk_string rown=format_number(rowpos,false,0);
        hk_string msg=hk_translate(replace_all("%2",replace_all("%1","Could not import row number %1\nValue:'%2'\n\nCancel import?",rown),sbuffer));
        errors+=rown+"\n";
          p_cancelimport=show_yesnodialog(msg,true);
          datasource()->set_ignore_changed_data();
        }
        if (!p_cancelimport)
          p_cancelimport=!widget_specific_after_new_row();
    ++rowpos;
    }
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::execute while ENDE");
#endif
datasource()->set_blockvisiblesignals(blockbuffer);
    reset();
    if (p_cancelimport) return false;
    else
    {
    if (errors.size()>0) show_warningmessage(hk_translate("Could not import the following rows:\n")+errors);
    return true;
    }
}


void hk_importcsv::get_line(ifstream&s ,hk_string& b)
{
   char c;
   bool noreturn=true;
   bool inbrackets=false;
   b="";
   while (noreturn)
   {
     // JKB void* r=s.get(c);
     istream& r=s.get(c);

     hk_string buf(1,c);
     if (buf==p_textdelimiter)
      {
        inbrackets=!inbrackets;
      }

     if (buf==p_rowdelimiter)
      {
        if (!inbrackets) noreturn=false;
      }

// JKB if (r==0) noreturn=false;
     if (!r) noreturn=false;
     if (noreturn)   b=b+c;


   }
    hk_string use_locale=(p_charset.size()==0?nl_langinfo(CODESET):p_charset);
   hk_string r=smallstringconversion(b,p_charset,"");
   if (r.size()>0) b=r;
}


bool hk_importcsv::initialize_table(enum_interaction e)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::initialize_table");
#endif
    hk_string sbuffer;
    if (datasource()==NULL)
    {
        show_warningmessage(hk_translate("No datasource defined!"));
        return false;
    }

    if (p_filestream!=NULL) delete p_filestream;
    p_filestream=new ifstream(p_filename.c_str(),ios::in);
    if (!(*p_filestream))
    {
        hk_string w=hk_translate("No such CSV file: ");
        w+=p_filename;
        show_warningmessage(w);
        reset();

        return false;
    }

    if (p_firstrow_contains_fieldnames)
    {
        get_line(*p_filestream,sbuffer);           //erste zeile lesen
        if (p_columnlist.size()==0)
        {
            create_valuelist(sbuffer);
            create_automatic_columns();
        }
    }

    if (!p_create_new_table &&datasource()->database()->table_exists(datasource()->name()))
    {
	create_automatic_columns();
    p_accessmode=datasource()->accessmode();
    if (datasource()->is_enabled()) datasource()->disable();
    datasource()->set_accessmode(hk_datasource::batchwrite);
    if (!datasource()->is_enabled())datasource()->enable();
    if (!datasource()->is_enabled())
    {
        reset();

        show_warningmessage(hk_translate("Datasource could not be enabled"));
        return false;
    }
      return true;
    }

    if (p_create_new_table||(!p_create_new_table &&!datasource()->database()->table_exists(datasource()->name())))
    {
        if (datasource()->database()->table_exists(datasource()->name()))
        {
            if (runtime_only()||(e==noninteractive && ! p_overwrite))
            {
                show_warningmessage(hk_translate("Table already exists"));
                return false;
            }
            hk_datasource* d=datasource();
            if (!datasource()->database()->delete_table(datasource()->name(),
	        e==interactive?hk_database::interactive:hk_database::noninteractive))
	       return false;
            set_datasource(d);
        }
        datasource()->setmode_createtable();
        hk_column* col=NULL;
        get_line(*p_filestream,sbuffer);           //erste Datenzeile einlesen
        if (!p_firstrow_contains_fieldnames &&p_columnlist.size()==0)
        {
            create_valuelist(sbuffer);
            create_automatic_columns();
        }
        p_datamode=false;
	//cerr <<"BEGIN FELDTYP ERMITTLUNG"<<endl;
        create_valuelist(sbuffer);
	//cerr <<"VALUELIST EINLESEN FELDTYP ERMITTLUNG"<<endl;
        p_datamode=true;
#ifdef HK_DEBUG
        hkdebug("hk_importcsv::initialize_table vor while");
#endif
        vector<colstruct>::iterator colit=p_columnlist.begin();
        vector<hk_string>::iterator it=p_valuelist.begin();
        while (it!=p_valuelist.end()&&colit!=p_columnlist.end())
        {
#ifdef HK_DEBUG
            hkdebug("in while begin");
#endif
            col=datasource()->new_column();
            col->set_name((*colit).colname);
#ifdef HK_DEBUG
            hkdebug("angelegte Spalte: ",col->name());
#endif

            col->set_columntype(interpret_columntype((*it)));
            if ((*it).size()>255)col->set_columntype(hk_column::memocolumn);
            else col->set_size(255);
            if (col->columntype()==hk_column::integercolumn)
               (*colit).currentvalue=localestring2int((*it));
            col->set_dateformat(p_dateformat);
            col->set_datetimeformat(p_datetimeformat);
            col->set_timeformat(p_timeformat);
            (*colit).col=col;
#ifdef HK_DEBUG
            hkdebug("initialize_table ++");
#endif

            it++;
//         if (colit!=p_columnlist.end())
            colit++;
        }                                         //end while

        const int maxpreview=50;
        bool eof=false;
        int i=0;
#ifdef HK_DEBUG
            hkdebug("Start type preview");
#endif
     for (i=0;i<maxpreview &&!eof;++i)
     {
        if (!p_filestream->eof())
        {
        get_line(*p_filestream,sbuffer);           //zweite bis maxpreview Datenzeile einlesen
        p_datamode=false;
        create_valuelist(sbuffer);
        p_datamode=true;

        it=p_valuelist.begin();
        colit=p_columnlist.begin();
        while (it!=p_valuelist.end()&&colit!=p_columnlist.end())
        {
#ifdef HK_DEBUG
            hkdebug("in while begin");
#endif
            col=(*colit).col;
            hk_column::enum_columntype ctype=interpret_columntype((*it));
            //cerr <<"INTERPRETIERTER COLUMNTYPE akuteller Wert:"<<*it<<" "<<ctype<<endl;
            if (col && ctype!=col->columntype())
            {
            if (ctype==hk_column::floatingcolumn && col->columntype()==hk_column::integercolumn
            ||ctype==hk_column::integercolumn && col->columntype()==hk_column::floatingcolumn
            )
            col->set_columntype(hk_column::floatingcolumn);
            else
             if(col->columntype()!=hk_column::memocolumn)
             {
               col->set_columntype(hk_column::textcolumn);
#ifdef HK_DEBUG
		hkdebug("Fix data type to textcolumn, column:",(*colit).colname);
#endif
             }



            if ((*it).size()>255)col->set_columntype(hk_column::memocolumn);
            else col->set_size(255);
            } //end different columntypes
#ifdef HK_DEBUG
            hkdebug("initialize_table ++");
#endif
            int v=localestring2int((*it));
            //the following is needed to detect autoinc
            if (col->columntype()==hk_column::integercolumn && v>(*colit).currentvalue &&(*colit).currentvalue>0)
               (*colit).currentvalue=v;
            else
		(*colit).currentvalue=-1;
	    //end detect autoinc part
            it++;
            colit++;
        }                                         //end while
        }
        else eof=true;//eof

    } // end for

         colit=p_columnlist.begin();
         bool has_autoinc=false;
        if (p_detect_autoinc)
        {
        while (colit!=p_columnlist.end())
        {
	    if ((*colit).col->columntype()==hk_column::integercolumn&&(*colit).currentvalue>0&&!has_autoinc
	    && i>2)
	    {
	       (*colit).col->set_columntype(hk_column::auto_inccolumn);
	       has_autoinc=true;
	    }
	    (*colit).col=NULL;
            colit++;
        }                                         //end while
        }


#ifdef HK_DEBUG
        hkdebug("hk_importcsv::initialize_table NACH while");
//datasource()->wanna_debug(true);
#endif
        if(! datasource()->create_table_now())
        {
#ifdef HK_DEBUG
            hkdebug("Tabelle kann nicht angelegt werden");
#endif
            show_warningmessage("Maybe the column names are not valid!");
            reset();
            return false;
        }

    }  //end create table
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::initialize_table nach create table");
#endif

p_datamode=true;
    p_accessmode=datasource()->accessmode();
    if (datasource()->is_enabled()) datasource()->disable();
    datasource()->set_accessmode(hk_datasource::batchwrite);
    if (!datasource()->is_enabled())datasource()->enable();
    if (!datasource()->is_enabled())
    {
        reset();

        show_warningmessage(hk_translate("Datasource could not be enabled"));
        return false;
    }

#ifdef HK_DEBUG
    hkdebug("hk_importcsv::initialize_table nach enable");
#endif

// Datei wieder auf erste Datenzeile stellen
    if (p_filestream!=NULL) delete p_filestream;
    p_filestream=new ifstream(p_filename.c_str(),ios::in);
    if (p_firstrow_contains_fieldnames)
        get_line(*p_filestream,sbuffer);           //erste zeile lesen
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::initialize_table vor return");
#endif

    return true;
}


void    hk_importcsv::set_filedefinition(const hk_string& fielddel,const hk_string& between, const hk_string& rowdel, const hk_string& datetimef, const hk_string& datef, const hk_string& timef,const hk_string& charst,const hk_string& l)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::set_filedefinition");
#endif
    p_textdelimiter=fielddel;
    if (between.size()>0) p_betweenfields=between;
    p_rowdelimiter=rowdel;
    if (datetimef.size()>0)p_datetimeformat=datetimef;
    if (timef.size()>0) p_timeformat=timef;
    if (datef.size()>0) p_dateformat=datef;
    p_charset=charst;
    p_locale=l;
}


void hk_importcsv::set_filename(const hk_string& filename)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::set_filename");
#endif
    p_filename=filename;

}


void hk_importcsv::set_firstrow_contains_fieldnames(bool f)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::set_firstrow_contains_fieldnames");
#endif
    p_firstrow_contains_fieldnames=f;
}


void hk_importcsv::set_append_rows(bool append_rows)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::set_append_rows");
#endif
    p_create_new_table = ! append_rows;
}


/**
 *the parser is based on the KOffice/Kspread import filter <c>1999 David Faure
 */






vector<hk_string>* hk_importcsv::create_valuelist( hk_string& row)
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_importcsv::create_valuelist #"+row+"#");
#endif
    hk_string::size_type returnkey = row.find('\r');
    row.append("\n");
    hk_string delimiters=" ";
    if (returnkey!=hk_string::npos)  row.replace(returnkey,1,"");

    p_valuelist.erase(p_valuelist.begin(),p_valuelist.end());
    if(row.size()<2)
    {
#ifdef HK_DEBUG
        hkdebug("hk_importcsv::create_valuelist return am Beginn der whileschleife");
#endif
        return &p_valuelist;
    }
    hk_string::size_type offset=0;
//  hk_string::size_type beginposition=0;
//  hk_string::size_type endposition=0;
    hk_string field;
#ifdef HK_DEBUG
    hkdebug("Zeile: ",row);
    hkdebug("Gesamtlänge: ",(int)row.size());
    hkdebug("Z: ",row);
    hkdebug("Z: 12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    hkdebug("Z: 0        1         2         3         4         5         6         7         8");
#endif

    enum
    {
        S_START,S_QUOTED_FIELD,S_MAYBE_END_OF_QUOTED_FIELD, S_END_OF_QUOTED_FIELD,
        S_NORMAL_FIELD
    } state=S_START;
//cout <<"SUCHSTRING: "<<row<<endl;
    while (offset<row.size())
    {
        hk_string xc(1,row[offset]);               //row[offset];
        ++offset;
        switch (state)
        {
            case S_START :
                if ( p_textdelimiter==xc)
                {
                    state = S_QUOTED_FIELD;      // cout <<"S_STARTED set S_QUOTED_FIELD"<<endl;
                    if (!p_datamode) field="X";   //to mark that this is a field of type string
                }
                else if (xc == p_betweenfields||xc==p_rowdelimiter)
                {
                                                  //cout <<"LEERZEILTE S_START"<<endl;
                    p_valuelist.insert(p_valuelist.end(),"");//cerr <<"FIELD: "<<""<<endl;
                    field="";
                }
                 else
                {
                    field = xc;
                    state = S_NORMAL_FIELD; //cout <<"S_STARTED MAYBE_NORMAL";
                }
                break;


            case S_QUOTED_FIELD :
                if (xc == p_textdelimiter)
                {
                                                //  cout <<"S_QUOTED_FIELD set S_MAYBE_END_OF_QUOTED_FIELD"<<endl;
                    state = S_MAYBE_END_OF_QUOTED_FIELD;
                }

                else
                {
                    field += xc;
                }
                break;

            case S_MAYBE_END_OF_QUOTED_FIELD :
                if (xc == p_textdelimiter)
                {
                    field += xc;
                    state = S_QUOTED_FIELD;
		   // cerr <<"von S_MAYBE_END_OF_QUOTED_FIELD nach S_QUOTED_FIELD"<<endl;
                }
                else if (xc == p_betweenfields||xc==p_rowdelimiter  )
                {
                                          //        cout <<"S_MAYBE_END_OF_QUOTED_FIELD "<<field<<endl;
                    p_valuelist.insert(p_valuelist.end(),field);//cerr <<"FIELD: "<<field<<endl;
                    field = "";
                    state = S_START;
                }
                else
                {   //cerr <<"von S_MAYBE_END_OF_QUOTED_FIELD nach S_END_OF_QUOTED_FIELD"<<endl;
                    state = S_END_OF_QUOTED_FIELD;
                }
                break;


	    case S_END_OF_QUOTED_FIELD :
                if (xc == p_betweenfields ||xc==p_rowdelimiter)
                {
                                                 // cout <<"S_END_OF_QUOTED_FIELD  nach S_START"<<field<<" x="<<x<<endl;
                    p_valuelist.insert(p_valuelist.end(),field);//cerr <<"FIELD: "<<field<<endl;
                    field = "";
                    state = S_START;
                }
                else
                {
		   //cerr <<"ELSE       S_END_OF_QUOTED_FIELD"<<endl;
                    state = S_END_OF_QUOTED_FIELD;
                }
                break;


	    case S_NORMAL_FIELD :
                if (xc == p_betweenfields ||xc==p_rowdelimiter)
                {
                                              //    cout <<"S_NORMAL_FIELD "<<field<<endl;
                    p_valuelist.insert(p_valuelist.end(),field);//cerr <<"FIELD: "<<field<<endl;
                    field = "";
                    state = S_START;
                }
                else
                {
                    field += xc;
                }
        }
    }
    if (state!=S_START)p_valuelist.push_back(field);
#ifdef HK_DEBUG
  if (wanna_debug())
  {
  vector<hk_string>::iterator it=p_valuelist.begin();
  while (it!=p_valuelist.end())
  {
    cerr <<"Feld:'"<<(*it)<<"'"<<endl;
  ++it;
  }
  }
#endif
return &p_valuelist;
}


void    hk_importcsv::add_columnname(const hk_string& col)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::add_columnname");
#endif
    colstruct p_col;

// remove leading and trailing spaces

    p_col.colname=trim(col);

// replace interior spaces with underscores
    p_col.colname=replace_all(" ",p_col.colname,"_");
    p_col.col=NULL;
    p_col.currentvalue=-1;
    p_columnlist.insert(p_columnlist.end(),p_col);
}


void hk_importcsv::clear_columnlist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::clear_columnlist");
#endif
    p_columnlist.erase(p_columnlist.begin(),p_columnlist.end());
}


void hk_importcsv::set_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::set_columns");
#endif
//cerr <<"set_columns:" <<datasource()<<endl;
    if (datasource()==NULL) return;
    vector<colstruct>::iterator it=p_columnlist.begin();
    hk_string result; cerr <<"p_columnlist:"<<p_columnlist.size()<<endl;
    while (it!=p_columnlist.end())
    {
        (*it).col=datasource()->column_by_name((*it).colname);
//cout <<"Suche nach Column: " << (*it).colname<<" "<< (!(*it).col?"nicht ":"")<<"erfolgreich"<<endl;
	if (!(*it).col) result+=(*it).colname+"\n";
	else
	{
	    (*it).col->set_dateformat(p_dateformat);
            (*it).col->set_datetimeformat(p_datetimeformat);
            (*it).col->set_timeformat(p_timeformat);

	}
        it++;
    }
   if (result.size()>0)
    show_warningmessage(hk_translate("The following columns could not be found in the table:\n")+result);

}


void    hk_importcsv::create_automatic_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::create_automatic_columns");
#endif

    if (!p_create_new_table &&datasource()->database()->table_exists(datasource()->name()))
    {
       list<hk_column*>* col=datasource()->columns();
       list<hk_column*>::iterator it=col->begin();
       while(it!=col->end())
    {
        add_columnname((*it)->name());

        ++it;
    }

      return ;
    }



    hk_string newfieldname;
    clear_columnlist();
    int zaehler=0;
    const int bsize=60;
    char* buf= new char[bsize];

    vector<hk_string>::iterator it=p_valuelist.begin();
    while(it!=p_valuelist.end())
    {
        snprintf(buf,bsize,"%d",zaehler);
        newfieldname=hk_translate("field_")+buf;
        add_columnname(p_firstrow_contains_fieldnames?(*it):newfieldname);

        it++;++zaehler;
    }
    delete[] buf;

}


bool hk_importcsv::before_columns_deleted(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::before_columns_deleted");
#endif
    vector<colstruct>::iterator it=p_columnlist.begin();
    while (it!=p_columnlist.end())
    {
        (*it).col=NULL;
        it++;
    }
//  clear_columnlist();
    return true;
}

hk_column::enum_columntype hk_importcsv::interpret_columntype(hk_string& field)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::interpret_columntype");
#endif
    int num=0;
    int komma=0;
    int alpha=0;
    hk_datetime dt;
    dt.set_datetimeformat(p_datetimeformat);
    dt.set_dateformat(p_dateformat);
    dt.set_timeformat(p_timeformat);
    hk_string uf=string2upper(field);
   if (uf=="TRUE"||uf=="FALSE")
   {
#ifdef HK_DEBUG
    hkdebug("interpreted datatype: boolcolumn");
#endif
    return hk_column::boolcolumn;
   }
   if (dt.set_datetime_asstring(field))
   {
#ifdef HK_DEBUG
    hkdebug("interpreted datatype: datetimecolumn");
#endif
    return hk_column::datetimecolumn;
   }
   if (dt.set_date_asstring(field))
   {
#ifdef HK_DEBUG
    hkdebug("interpreted datatype: datecolumn");
#endif
    return hk_column::datecolumn;
   }
   if (dt.set_time_asstring(field))
   {
#ifdef HK_DEBUG
    hkdebug("interpreted datatype: timecolumn");
#endif
   return hk_column::timecolumn;
   }

    for (unsigned int i=0;i<field.size();i++)
    {

        if (isdigit(field[i])) num++;
        else if (field[i]=='.'||field[i]==',')komma++;
        else alpha++;

    }
//  cout <<"interpretiere feldtyp: "<<field<<endl;
//  cout <<"alpha: "<<alpha<<" komma: "<<komma<<" num: "<<num<<endl;

    if (alpha>0||komma>1)
    {
#ifdef HK_DEBUG
    hkdebug("interpreted datatype: textcolumn, da komma >1");
#endif
    return hk_column::textcolumn;
    }
    if (num>0) if (komma==1)
    {
#ifdef HK_DEBUG
    hkdebug("interpreted datatype: floatingcolumn");
#endif

     return hk_column::floatingcolumn;
    }
    else
    {
#ifdef HK_DEBUG
    hkdebug("interpreted datatype: integercolumn");
#endif
    return hk_column::integercolumn;
    }
#ifdef HK_DEBUG
    hkdebug("interpreted datatype: textcolumn default");
#endif
    return hk_column::textcolumn;

}


void hk_importcsv::reset(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_importcsv::reset");
#endif
    delete p_filestream;
    p_filestream=NULL;
    clear_columnlist();
}


bool hk_importcsv::widget_specific_after_new_row(void)
{
    return true;
}


void hk_importcsv::set_overwrite_table(bool o)
{
    p_overwrite=o;

}

void    hk_importcsv::set_create_new_table(bool n)
{
   p_overwrite=!n;
}

bool hk_importcsv::overwrite_table(void)
{
    return p_overwrite;
}


void hk_importcsv::set_detect_autoinc(bool v)
{
 p_detect_autoinc=v;
}

bool hk_importcsv::detect_autoinc(void) const
{
  return p_detect_autoinc;
}

void hk_importcsv::set_progressdialog(progress_dialogtype* p)
{
p_progressdialog=p;
}

progress_dialogtype* hk_importcsv::progressdialog(void)const
{
  return p_progressdialog;
}


