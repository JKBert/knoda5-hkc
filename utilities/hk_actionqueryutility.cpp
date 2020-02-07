#include "hk_actionquery.h"
#include "hk_utilities.h"

static char doc[]="hk_actionquery executes a data manipulation query (DML)";
static char args_doc[]="QUERY";
static struct argp_option options[] =
{
    DEFAULTPARAMETER,
    {"sql",'s',"SQL",0,"sql",0},
    {0,0,0,0,0,0}

};

class queryargumentclass :public argumentclass
{
    public:
        hk_string sql,query;
};

static error_t parse_options (int key,char* arg, struct argp_state* state)
{
    queryargumentclass* arguments=(queryargumentclass*)state->input;
    assert(arguments);
    switch (key)
    {

        case 's'    :
        {
            arguments->sql=arg;
            break;

        }
        case ARGP_KEY_ARG:
        {
            if (state->arg_num!=0)
            {
                cerr <<"argnum!=0 !!!"<<endl;
                argp_usage(state);
            }

            arguments->query=arg;
            break;
        }
        default :  return parse_generaloptions(key,arg,state);
    }
    return 0;
}


static struct argp argstruct={options,parse_options,args_doc,doc,0,0,0};

void verify_arguments(queryargumentclass& arguments)
{

/*cerr <<"driver:"<<arguments.driver<<endl;
cerr <<"database:"<<arguments.database<<endl;
cerr <<"query:"<<arguments.query<<endl;
cerr <<"sql:"<<arguments.sql<<endl;
cerr <<"port:"<<arguments.port<<endl;
cerr <<"user:"<<arguments.user<<endl;
cerr <<"password:"<<arguments.password<<endl;
cerr <<"host:"<<arguments.host<<endl;
cerr <<"filter:"<<arguments.filter<<endl;
*/
    if (arguments.query.size()==0 && arguments.sql.size()==0)
    {
        cerr << "No query selected"<<endl;
        exit(1);
    }
    if (arguments.query.size()>0 && arguments.sql.size()>0)
    {
        cerr << "Either select a query or a SQL statement, never both"<<endl;
        exit(1);
    }
    if (!verify_generalarguments(arguments))exit(1);

}


int main(int argc, char** argv)
{
    umask(0077);
    hk_drivermanager d;
    queryargumentclass arguments;
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

    hk_actionquery* q=db->new_actionquery();
    if (!q)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("query could not be created"));
        exit(1);
    }

    if (arguments.query.size())
    {
        xmlNodePtr query;
        if (is_localfile(arguments.query))
        {
            xmlDocPtr dp=xmlParseFile(arguments.query.c_str());
            query=xmlDocGetRootElement(dp);
        }
        else
            query=db->xmlload(arguments.query,ft_query);

        if (!query)
        {
            hk_class::show_warningmessage(hk_class::hk_translate("Queryfile couldn't be loaded or is empty"));
            exit(1);
        }
        hk_class::get_tagvalue(query,"SQL",arguments.sql);
    }

    if (arguments.sql.size()) q->set_sql(arguments.sql.c_str(),arguments.sql.size());

    if (!q->execute())
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Query couldn't be executed"));
        exit(1);
    }

}
