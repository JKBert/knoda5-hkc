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
#ifndef HK_DEFINITIONS
#define HK_DEFINITIONS
#include "hk_string.h"
#include <list>
#include <stdlib.h>
#include <stdio.h>
/*! \mainpage hk_classes
 *
 * \section intro_sec Introduction
 *
 <em>hk_classes</em> is  C++ library which allows rapid development of database
applications with all features a modern database application should have like forms an reports.
<em>hk_classes</em> is  database and GUI independent. In combination with
<em>hk_kdeclasses</em> (which is part of the <a
href="http://www.knoda.org" TARGET="_blank">knoda package</a>) it supports the rapid development of database
applications for KDE.
<p><em>hk_classes</em> is under heavy development, but it already works pretty
well.
<h2>hk_classes allows you to:</h2>
<ul>
<li>connect to a  database server (
<a href="http://linux.techass.com/projects/xdb/" target="_blank">Dbase/Xbase</a>,
<a href="http://www.firebirdsql.org" target="_blank">Firebird</a>,
<a href="http://mdbtools.sourceforge.net" target="_blank">MS Access/Mdbtools</a>,
<a href="http://www.mysql.com" target="_blank">Mysql</a>,
<a href="http://pxlib.sourceforge.net" target="_blank">Paradox</a>,
<a href="http://www.postgresql.org" target="_blank">Postgresql</a>,
<a href="http://www.sqlite.org" target="_blank">SQLite</a> or
<a href="http://www.unixodbc.org" target="_blank">ODBC</a>); </li>
<li>create and delete databases;</li>
<li>create, alter and delete tables and indices;</li>
<li>add, change and delete data in tables;</li>
<li>define, execute and store sql <strong>queries</strong>;</li>
<li>create, alter and delete <strong>views</strong>;</li>
<li><strong>import</strong> and <strong>export</strong> CSV data; </li>
<li>define and use <strong>forms</strong>; and  </li>
<li>define and print <strong>reports</strong></li>
<li>write your own <strong>scripts</strong> in <a href="http://www.python.org" TARGET="_blank">Python</a></li>
<li>use reports and queries in your own scripts with the command line tools
<ul>
<li>hk_actionquery</li>
<li>hk_exportcsv</li>
<li>hk_exportxml</li>
<li>hk_importcsv</li>
<li>hk_report</li>
</ul>
</li>
</ul>
<br>
<h2>hk_kdeclasses offers you data widgets for KDE:</h2>
<h3>high-level widgets</h3>
<ul>
<li><strong>hk_kdetable</strong><br> display and alter data in a grid and the
possibility to alter the table structure</li>
<li><strong>hk_kdequery</strong><br> allows you to define and execute a
query</li>
<li><strong>hk_kdeform</strong><br> allows you to design and view forms</li>
<li><strong>hk_kdereport</strong><br> allows you to design and preview
reports</li>
</ul>
<h3>low-level widgets</h3>
<UL>
<li><strong>hk_kdelineedit</strong> <br>a data sensitive standard field</li>
<li><strong>hk_kdememo</strong><br>a data sensitive multiline field</li>
<li><strong>hk_kdeboolean</strong><br>a data sensitive bool field</li>
<li><strong>hk_kdecombobox</strong><br>a data sensitive combobox field</li>
<li><strong>hk_kdegrid</strong><br>a data sensitive grid</li>
<li><strong>hk_kderowselector</strong><br>a row selector to browse a table</li>
</ul>

 */
using namespace std;

#define  HK_DEBUG
#define HK_VERSION "084"
/*
** The HK_VERSION_NUMBER is an integer with the value
** (X*100000 + Y*1000 + Z). For example, for version "3.1.2",
** HK_VERSION_NUMBER is set to 3001002. To detect if they are using
** version 0.7.2 or greater at compile time, programs may use the test
** (HK_VERSION_NUMBER>=0007002).
*/
#ifdef HK_VERSION_NUMBER
# undef HK_VERSION_NUMBER
#endif
#define HK_VERSION_NUMBER 0008004


#ifdef HAVE_NO_SSTREAM
#include <strstream.h>
#define STRINGSTREAM  strstream
#else
#include <sstream>
#define STRINGSTREAM stringstream
#endif



/**
 *@version $Revision: 1.68 $
 */
enum filetype{ft_table=1,ft_query=2,ft_form=3,ft_report=4,           ft_view=6,ft_module=7,ft_referentialintegrity=8};
enum listtype{lt_table=1,lt_query=2,lt_form=3,lt_report=4,lt_index=5,lt_view=6,lt_module=7};

enum datasourcetype {dt_table=1,dt_query=2,dt_view=3};
enum objecttype{ot_database=0,ot_table=1,ot_query=2,ot_form=3,ot_report=4,ot_view=6,ot_module=7};

/**
 * used as entry to the raw data
 */

typedef class struct_raw_data
{
    public:
        struct_raw_data(){length=0;data=NULL;}
        unsigned long   length;
        char* data;

} ;

typedef class struct_connectionfields
{
    public:
        struct_connectionfields(){cancel=false;save=false;allowdatabaseselect=true;};
        hk_string host;
        hk_string user;
        hk_string password;
        hk_string sql_delimiter;
        hk_string port;
	hk_string database;
	bool allowdatabaseselect;
        bool cancel;
        bool save;
	bool emulateboolean;
} ;


class struct_commands;

typedef class struct_commandlinefields
{
    public:
        struct_commandlinefields()
	    {
	       runtime_only=false;
	       p_guicommands=NULL;
	    };
	hk_string driver;
	hk_string database;
	hk_string form;

	hk_string host;
        hk_string user;
        hk_string password;
        hk_string port;
	bool runtime_only;
	struct_commands* p_guicommands;
} ;


int hk_random(int min,int max);

class hk_column;
/**
 *returns true if the columntype (not the data) is numeric, either integer or real.
 */
bool is_numerictype(hk_column*);
/**
 *returns true if the columntype (not the data) integer.
 */
bool is_integertype(hk_column*);
/**
 *returns true if the columntype (not the data) floating point type.
 */
bool is_realtype(hk_column*);
double inch2cm(double);
double cm2inch(double);
//the following functions and datatypes are all needed for hk_report
class hk_report;
class hk_reportdata;
class hk_reportsection;

typedef hk_string   recodefunctiontype(const hk_string&,hk_report*);
typedef hk_string   data_replacefunctiontype(hk_reportdata*,const hk_string&);
typedef bool report_configurefunction(hk_report*,int);
typedef bool reporttypefunction(hk_report*,bool);
typedef unsigned long reportsectioncounttype(hk_reportsection*);
typedef void reportsectionautomaticcreatedatatype(hk_reportsection*);
typedef unsigned long reportdatacounttype(hk_reportdata*);
typedef void    data_configurefunctiontype(hk_reportdata*);

typedef void voidfunction(void);

typedef struct_raw_data* psimagedatatype(struct_raw_data* imgdata,const hk_string& file,hk_string& errormsg);
union number
{
    long integer;
    double real;
};

union longnumber
{
    long int integer;
    long double real;
};


typedef class
{
 public:
 hk_string dependingfield;
 hk_string masterfield;
} dependingclass;


typedef class referentialclass
{
public:
referentialclass();
bool operator=(const referentialclass&);


hk_string p_name;
hk_string p_masterdatasource;
list<dependingclass> p_fields;
bool p_deletecascade;
bool p_updatecascade;
} ;


bool load_file(const hk_string& filename, struct_raw_data* result);
bool save_file(const hk_string& filename, const struct_raw_data* data);
bool save_textfile(const hk_string& filename, const hk_string& data);
bool file_exists(const hk_string& filename);
bool data2hex(struct_raw_data* data, hk_string& result);
bool hex2data( const hk_string& data,unsigned long maxbytes, struct_raw_data* result);
const hk_string charsets[]=
{
"UTF8","UTF16","ISO88591","ISO88592","ISO88593","ISO88594","ISO88595","ISO88596","ISO88597","ISO88598",
"ISO88599","ISO885910","ISO885911","ISO885913","ISO885914","ISO885915","KOI8","KOI8R","KOI8U","LATIN1",
"LATIN2","LATIN3","LATIN4","LATIN5","LATIN6","LATIN7","LATIN8","LATIN10","WINDOWS-31J","WINDOWS-874",
"WINDOWS-936","WINDOWS-1250","WINDOWS-1251","WINDOWS-1252","WINDOWS-1253","WINDOWS-1254","WINDOWS-1255",
"WINDOWS-1256","WINDOWS-1257","WINDOWS-1258",
"CP037","CP038","CP273","CP274","CP275","CP278","CP280","CP281","CP282","CP284","CP285","CP290","CP297","CP367",
"CP420","CP423","CP424","CP437","CP500","CP737",
"CP775","CP813","CP819","CP850","CP851","CP852","CP855","CP856","CP857","CP860","CP861","CP862","CP863","CP864","CP865",
"CP866","CP866NAV","CP868","CP869","CP870","CP871","CP874","CP875","CP880","CP891","CP903","CP904","CP905","CP912"
,"CP915","CP916","CP918","CP920","CP922","CP930","CP932","CP933","CP935","CP936","CP937","CP939","CP949","CP950",
"CP1004","CP1026","CP1046","CP1047","CP1070","CP1079","CP1081","CP1084","CP1089","CP1124","CP1125","CP1129","CP1132",
"CP1133","CP1160","CP1161","CP1162","CP1163","CP1164","CP1250","CP1251","CP1252","CP1253","CP1254","CP1255","CP1256",
"CP1257","CP1258","CP1361","CP10007"

};


const hk_string locales[]=
{
"C","aa_DJ","aa_ER","aa_ET","af_ZA","am_ET","an_ES","ar_AE","ar_BH","ar_DZ","ar_EG","ar_IN","ar_IQ","ar_JO","ar_KW",
"ar_LB","ar_LY","ar_MA","ar_OM","ar_QA","ar_SA","ar_SD","ar_SY","ar_TN","ar_YE","be_BY","bg_BG","bn_BD","bn_IN",
"br_FR","bs_BA","byn_ER","ca_ES","cs_CZ","cy_GB","da_DK","de_AT","de_BE","de_CH","de_DE","de_LU","el_GR","en_AU","en_BE","en_BW","en_CA","en_DK","en_GB","en_HK","en_IE","en_IN","en_NZ","en_PH","en_SG",
"en_US","en_ZA","en_ZW","es_AR","es_BO","es_CL","es_CO","es_CR","es_DO","es_EC","es_ES","es_GT",
"es_HN","es_MX","es_NI","es_PA","es_PE","es_PR","es_PY","es_SV","es_US","es_UY","es_VE","et_EE","eu_ES",
"fa_IR","fi_FI","fo_FO","fr_BE","fr_CA","fr_CH","fr_FR","fr_LU",
"ga_IE","gd_GB","gez_ER","gez_ET","gl_ES","gu_IN","gv_GB",
"he_IL","hi_IN","hr_HR","hu_HU","id_ID","is_IS","it_CH","it_IT","iw_IL","ka_GE","kk_KZ","kl_GL","kn_IN",
"kw_GB","lg_UG","lo_LA","lt_LT","lv_LV","mi_NZ","mk_MK","ml_IN","mn_MN","mr_IN","ms_MY","mt_MT","nb_NO","ne_NP",
"nl_BE","nl_NL","nn_NO","no_NO","oc_FR","om_ET","om_KE","pa_IN","pl_PL","POSIX","pt_BR","pt_PT",
"ro_RO","ru_RU","ru_UA","se_NO","sh_YU","sid_ET","sk_SK","sl_SI","so_DJ","so_ET","so_KE","so_SO","sq_AL",
"st_ZA","sv_FI","sv_SE","ta_IN","te_IN","tg_TJ","th_TH","ti_ER","ti_ET","tig_ER","tl_PH","tr_TR","uk_UA",
"ur_PK","uz_UZ","vi_VN","wa_BE","xh_ZA","yi_US","zh_CN","zh_HK","zh_SG","zh_TW","zu_ZA"

};


#endif //HK_DEFINITIONS



