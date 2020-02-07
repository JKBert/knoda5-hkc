#include "hk_reportutils.h"
#include "hk_utilities.h"

static char doc[]="hk_exporthtml exports a datasource to an HTML file";
static char args_doc[]="DATASOURCE";
static struct argp_option options[] =
{
    DEFAULTPARAMETER,
    {"filter",'f',"FILTER",0,"filter",0},
    {"query",'q',0,0,"if set the datasource is a query, else it is a table",0},
    {"beforetable",11,"BEFORETABLETEXT",0,"text displayed before the table",0},
    {"aftertable",12,"AFTERTABLETEXT",0,"text displayed after the table",0},
    {"headsection",13,"HEADSECTION",0,"meta data in the <HEAD></HEAD> section",0},
    {"groupcolumn",14,"COLUMNNAME",0,"for each group will be created an extra table",0},
    {"tabletags",'t',"TABLETAGS",0,"The tags of the <table> statement, e.g.  BORDER=1 CELLSPACING=1",0},
    {0,0,0,0,0,0}

};

class htmlargumentclass :public argumentclass
{
    public:
        htmlargumentclass(){dstype=dt_table;fieldname_asattribute=false;}
        hk_string datasource;
        hk_string beforetable,aftertable,head;
	hk_string groupcolumn;
	hk_string tabletag;
        datasourcetype dstype;
        bool fieldname_asattribute;
};

static error_t parse_options (int key,char* arg, struct argp_state* state)
{
    htmlargumentclass* arguments=(htmlargumentclass*)state->input;
    assert(arguments);

    switch (key)
    {

        case 'q'    : arguments->dstype=dt_query;
        break;
        case 11 : arguments->beforetable=arg;
        break;
        case 12 : arguments->aftertable=arg;
        break;
        case 13 : arguments->head=arg;
        break;
        case 14 : arguments->groupcolumn=arg;
        break;
        case 't' : arguments->tabletag=arg;
        break;
        case ARGP_KEY_ARG:
        {
            if (state->arg_num!=0)
            {
                cerr <<"argnum!=0 !!!"<<endl;
                argp_usage(state);
            }

            arguments->datasource=arg;
            break;
        }
        default :  return parse_generaloptions(key,arg,state);
    }
    return 0;
}


static struct argp argstruct={options,parse_options,args_doc,doc,0,0,0};

void verify_arguments(htmlargumentclass& arguments)
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
    if (arguments.datasource.size()==0)
    {
        cerr << "No datasource selected"<<endl;
        exit(1);
    }

}


int main(int argc, char** argv)
{
    umask(0077);
    hk_drivermanager d;
    htmlargumentclass arguments;
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
    hk_reporthtml* r= new hk_reporthtml();
    if (!r)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Report couldn't be created"));
        exit(1);
    }
    r->set_database(db);
    long ds=r->new_datasource(arguments.datasource,arguments.dstype);
    r->set_presentationdatasource(ds);
/*    if (arguments.rowtag.size()) r->set_rowtag(arguments.rowtag);
    if (arguments.maindocumenttag.size()) r->set_maindocumenttag(arguments.maindocumenttag);
    if (arguments.fieldname_asattribute) r->set_fieldname_as_attribute(hk_reporthtml::fieldnameattribute);
*/
    r->set_beforetable(arguments.beforetable);
    r->set_aftertable(arguments.aftertable);
    r->set_betweenheadtags(arguments.head);
    r->set_groupcolumn(arguments.groupcolumn);//cerr <<"commandlinegroupcolumn: "<<arguments.groupcolumn <<endl;
    r->set_filename("",false);
    r->set_use_standard_storagepath(false);
    if (arguments.tabletag.size()==0) arguments.tabletag="BORDER=1 CELLSPACING=1";
    r->set_tabletag(arguments.tabletag);

    if (r->datasource() && arguments.filter.size())
    {

        r->datasource()->set_temporaryfilter(arguments.filter);
        r->datasource()->set_use_temporaryfilter(true);
    }
    if (!r->execute())
    {
        hk_class::show_warningmessage(hk_class::hk_translate("HTML couldn't be executed"));
        exit(1);
    }

}
