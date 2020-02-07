#include "hk_importcsv.h"
#include "hk_utilities.h"

static char doc[]="hk_importcsv imports a datasource to an csv file";
static char args_doc[]="CSV-FILE";
static struct argp_option options[] =
{
    DEFAULTPARAMETER,
    {"filter",'f',"FILTER",0,"filter",0},
    {"table",'t',"TABLE NAME",0,"the name of the table",0},
    {"noheader",13,0,0,"if set the column names will not be included in the first row",0},
    {"between",11,"BETWEEN",0,"column separator, default is ','",0},
    {"delimiter",12,"TEXTDELIMITER",0,"the text delimiter, default is '\"'",0},
    {"overwrite",'w',0,0,"overwrites the table if it already exists",0},
    {"append",'a',0,0,"appends data to the table if it already exists",0},

    {"charset",'c',"CHARSET",0,"charset of the import file",0},
    {"locale",'o',"LOCALE",0,"locale of the import file, influences the number appearance",0},
    {"tf",14,"TIMEFORMAT",0,"time format",0},
    {"df",15,"DATEFORMAT",0,"date format",0},
    {"dtf",16,"DATETIMEFORMAT",0,"datetimeformat",0},
    {"noautoincdetection",17,0,0,"avoids detection of auto_inc data type",0},
    {0,0,0,0,0,0}

};

class csvargumentclass :public argumentclass
{
    public:
        csvargumentclass()
                        {
        		noheader=false;
        		delimiter="\"";
        		between=",";
        		overwrite=false;
        		append=false;
        		autoincdetection=true;
        		}
        hk_string file,table;
        hk_string delimiter, between;
        bool noheader;
	bool overwrite;
	bool append;
	bool autoincdetection;
	hk_string timeformat,dateformat,datetimeformat,charset,locale;
};

static error_t parse_options (int key,char* arg, struct argp_state* state)
{
    csvargumentclass* arguments=(csvargumentclass*)state->input;
    assert(arguments);

    switch (key)
    {

        case 't'    : arguments->table=arg;
        break;
	case 'w': arguments->overwrite=true;
	break;
	case 'a': arguments->append=true;
	break;
        case 'c' : arguments->charset=arg;
        break;
        case 'o' : arguments->locale=arg;
        break;
        case 13 : arguments->noheader=true;
        break;
        case 11 : arguments->between=arg;
           	  if (arguments->between=="\\t")
           	  	arguments->between="\t"; 
        break;
        case 12 : arguments->delimiter=arg;
        break;
        case 14 : arguments->timeformat=arg;
        break;
        case 15 : arguments->dateformat=arg;
        break;
        case 16 : arguments->datetimeformat=arg;
        break;
        case 17 : arguments->autoincdetection=false;
        break;
        case ARGP_KEY_ARG:
        {
            if (state->arg_num!=0)
            {
                cerr <<"argnum!=0 !!!"<<endl;
                argp_usage(state);
            }

            arguments->file=arg;
            break;
        }
        default :  return parse_generaloptions(key,arg,state);
    }
    return 0;
}


static struct argp argstruct={options,parse_options,args_doc,doc,0,0,0};

void verify_arguments(csvargumentclass& arguments)
{

/*  cerr <<"driver:"<<arguments.driver<<endl;
  cerr <<"database:"<<arguments.database<<endl;
  cerr <<"report:"<<arguments.report<<endl;
  cerr <<"port:"<<arguments.port<<endl;
  cerr <<"user:"<<arguments.user<<endl;
  cerr <<"password:"<<arguments.password<<endl;
  cerr <<"host:"<<arguments.host<<endl;
  cerr <<"filter:"<<arguments.filter<<endl;*/
    if (!verify_generalarguments(arguments))exit(1);
    if (arguments.file.size()==0)
    {
        cerr << "No file selected"<<endl;
        exit(1);
    }
    if (arguments.table.size()==0)
    {
        cerr << "No table selected"<<endl;
        exit(1);

    }

}


int main(int argc, char** argv)
{
     umask(0077);
    hk_drivermanager d;
   csvargumentclass arguments;
    argp_parse(&argstruct,argc,argv,0,0, &arguments);
    verify_arguments(arguments);
    hk_connection * c=d.new_connection(arguments.driver,hk_connection::noninteractive);
    if (!c)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Error creating connection"));
        exit(1);
    }
    if (arguments.user.size())c->set_user(arguments.user);
    if (arguments.password.size()) c->set_password(arguments.password);
    if (arguments.host.size()) c->set_host(arguments.host);
    if (arguments.port.size())
    {
        c->set_tcp_port(localestring2uint(arguments.port));
    }
    if (!c->connect(hk_connection::noninteractive))
    {
        exit(1);
    }
    if (!c->database_exists(arguments.database))
    {
        hk_class::show_warningmessage(hk_class::hk_translate("No such database"));
        exit(1);
    }

    hk_database* db=c->new_database(arguments.database);
    hk_importcsv* r= new hk_importcsv();
    if (!r)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("IMPORTCSV couldn't be created"));
        exit(1);
    }
    hk_datasource *ds=db->new_table();
    if (!ds)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Datasource couldn't be created"));
        exit(1);
    }
    ds->set_name(arguments.table);
    r->set_datasource(ds);
    r->set_detect_autoinc(arguments.autoincdetection);
    if (arguments.noheader) r->set_firstrow_contains_fieldnames(false);
    r->set_filedefinition(arguments.delimiter,arguments.between,"\n",arguments.datetimeformat,arguments.dateformat,arguments.timeformat,arguments.charset,arguments.locale);
    r->set_filename(arguments.file);
    r->set_overwrite_table(arguments.overwrite);
    r->set_append_rows(arguments.append);
    if (!r->execute(hk_importcsv::noninteractive))
    {
        hk_class::show_warningmessage(hk_class::hk_translate("csv couldn't be executed"));
        exit(1);
    }
  
}

