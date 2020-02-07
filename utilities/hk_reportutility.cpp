#include "hk_report.h"
#include "hk_utilities.h"

static char doc[]="hk_report prints a report to stdout";
static char args_doc[]="REPORT";
static struct argp_option options[] =
{
    DEFAULTPARAMETER,
    {"filter",'f',"FILTER",0,"filter",0},
    {0,0,0,0,0,0}

};

class reportargumentclass :public argumentclass
{
    public:
        hk_string report;
};

static error_t parse_options (int key,char* arg, struct argp_state* state)
{
    reportargumentclass* arguments=(reportargumentclass*)state->input;
    assert(arguments);

    switch (key)
    {

        case ARGP_KEY_ARG:
        {
            if (state->arg_num!=0)
            {
                cerr <<"argnum!=0 !!!"<<endl;
                argp_usage(state);
            }

            arguments->report=arg;
            break;
        }
        default :  return parse_generaloptions(key,arg,state);
    }
    return 0;
}


static struct argp argstruct={options,parse_options,args_doc,doc,0,0,0};

void verify_arguments(reportargumentclass& arguments)
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
    if (arguments.report.size()==0)
    {
        cerr << "No report selected"<<endl;
        exit(1);
    }

}


int main(int argc, char** argv)
{
    umask(0077);
    hk_drivermanager d;
    reportargumentclass arguments;
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
    hk_report* r= new hk_report();
    if (!r)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Report couldn't be created"));
        exit(1);
    }
    r->set_database(db);
    if (!is_localfile(arguments.report))
    {
        r->load_report(arguments.report);
    }
    else
    {    xmlDocPtr dp=xmlParseFile(arguments.report.c_str());
        xmlNodePtr report=xmlDocGetRootElement(dp);
        if (!report)
        {
            hk_class::show_warningmessage(hk_class::hk_translate("Reportfile couldn't be loaded"));
            exit(1);
        }
        r->loaddata(report);
    }
    r->set_filename("",false);
    r->set_use_standard_storagepath(false);
    if (r->datasource() && arguments.filter.size())
    {

        r->datasource()->set_temporaryfilter(arguments.filter);
        r->datasource()->set_use_temporaryfilter(true);
    }

    if (!r->execute())
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Report couldn't be executed"));
        exit(1);
    }

}
