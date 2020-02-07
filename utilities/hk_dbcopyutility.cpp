#include "hk_database.h"
#include "hk_utilities.h"




void copy_singlefile(hk_database* fromdb, hk_database* todb,const hk_string& name,filetype f)
{
     if (!fromdb ||!todb) return;
     hk_string input=fromdb->load(name,f);
     todb->save(input,name,f,true,true);

}

bool copy_local_files(hk_database* fromdb,hk_database* todb,objecttype f)
{
    if (!fromdb||!todb)return false;

    bool cancel=false;
    vector<hk_string>* namelist=NULL;
    hk_string viewlist;
    switch (f)
    {
        case ot_table:  namelist=fromdb->filelist(ft_table);viewlist=hk_class::hk_translate("Copying tabledefinition: %FILE%");break;
        case ot_query:  namelist=fromdb->querylist();viewlist=hk_class::hk_translate("Copying query: %FILE%");break;
        case ot_form:   namelist=fromdb->formlist();viewlist=hk_class::hk_translate("Copying form: %FILE%");break;
        case ot_report: namelist=fromdb->reportlist();viewlist=hk_class::hk_translate("Copying report: %FILE%");break;
        default: return false;
    }
    if (!namelist) return false;

    vector<hk_string>::iterator it=namelist->begin();
    while (it!=namelist->end())
    {
     copy_singlefile(fromdb,todb,(*it),(filetype)f);
        it++;
    }
    return !cancel;
}








static char doc[]="hk_dbcopy copies database elements between different database backends";
static char args_doc[]="";
static struct argp_option options[] =
{
    DEFAULTPARAMETER,
    {"todriver",13,"DRIVER",0,"target driver name",0},
    {"todatabase",14,"DATABASE",0,"target database",0},
    {"touser",29,"USER",0,"target user name",0},
    {"topassword",15,"PASSWORD",0,"target password",0},
    {"tohost",16,"HOST",0,"target host name",0},
    {"toport",17,"PORT",0,"target port",0},
    
    {"table",18,"TABLE NAME",0,"the name of the table that should be copied",0},
    {"query",19,"QUERY NAME",0,"the name of the query that should be copied",0},
    {"form",20,"FORM NAME",0,"the name of the form that should be copied",0},
    {"report",21,"REPORT NAME",0,"the name of the report",0},
    {"view",22,"VIEW NAME",0,"the name of the view",0},
    
    {"all-tables",23,0,0,"copy all tables",0},
    {"all-queries",24,0,0,"copy all queries",0},
    {"all-views",25,0,0,"copy all views",0},
    {"all-reports",26,0,0,"copy all reports",0},
    {"all-forms",27,0,0,"copy all forms",0},
    {"whole-database",28,0,0,"copy all database objects",0},
{0,0,0,0,0,0}

};

class csvargumentclass :public argumentclass
{
    public:
        csvargumentclass()
                        {
                        alltables=allqueries=allviews=allreports=allforms=wholedatabase=false;
        		}
	hk_string todriver,todatabase,topassword,touser,tohost,toport,table,form,query,report,view;
	bool alltables,allqueries,allviews,allreports,allforms,wholedatabase;
};

static error_t parse_options (int key,char* arg, struct argp_state* state)
{
    csvargumentclass* arguments=(csvargumentclass*)state->input;
    assert(arguments);

    switch (key)
    {

        case 13 : arguments->todriver=arg;
        	  break;
        case 14 : arguments->todatabase=arg;
        	  break;
        case 15 : arguments->topassword=arg;
        	  break;
        case 16 : arguments->tohost=arg;
        	  break;
        case 17 : arguments->toport=arg;
        	  break;
        case 18 : arguments->table=arg;
        	  break;
        case 19 : arguments->query=arg;
        	  break;
        case 20 : arguments->form=arg;
        	  break;
        case 21 : arguments->report=arg;
        	  break;
        case 22 : arguments->view=arg;
        	  break;
        case 23 : arguments->alltables=true;
        	  break;
        case 24 : arguments->allqueries=true;
        	  break;
        case 25 : arguments->allviews=true;
        	  break;
        case 26 : arguments->allreports=true;
        	  break;
        case 27 : arguments->allforms=true;
        	  break;
        case 28 : arguments->wholedatabase=true;
        	break;
        case 29 : arguments->touser=arg;
        	  break;
        break;
        case ARGP_KEY_ARG:
        {
            if (state->arg_num!=0)
            {
                cerr <<"argnum!=0 !!!"<<endl;
                argp_usage(state);
            }

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
//     if (arguments.file.size()==0)
//     {
//         cerr << "No file selected"<<endl;
//         exit(1);
//     }
//     if (arguments.table.size()==0)
//     {
//         cerr << "No table selected"<<endl;
//         exit(1);
// 
//     }

}


int main(int argc, char** argv)
{
     umask(0077);
    hk_drivermanager drivermanager;
   csvargumentclass arguments;
    argp_parse(&argstruct,argc,argv,0,0, &arguments);
    verify_arguments(arguments);
    hk_connection * fromconnection=drivermanager.new_connection(arguments.driver,hk_connection::noninteractive);
    if (!fromconnection)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Error creating connection"));
        exit(1);
    }
    if (arguments.user.size())fromconnection->set_user(arguments.user);
    if (arguments.password.size()) fromconnection->set_password(arguments.password);
    if (arguments.host.size()) fromconnection->set_host(arguments.host);
    if (arguments.port.size())
    {
        fromconnection->set_tcp_port(localestring2uint(arguments.port));
    }
    if (!fromconnection->connect(hk_connection::noninteractive))
    {
    cerr <<"fromconnectoin connect failed"<<endl;
        exit(1);
    }
    if (!fromconnection->database_exists(arguments.database))
    {
        hk_class::show_warningmessage(hk_class::hk_translate("No such database"));
        exit(1);
    }

    hk_database* fromdb=fromconnection->new_database(arguments.database);
    
    hk_connection* toconnection=arguments.todriver.size()>0?drivermanager.new_connection(arguments.todriver,hk_connection::noninteractive):fromconnection;
    if (!toconnection)
    {
    
    cerr <<"targetconnection could not be created"<<endl;
    exit(1);
    }
    if (fromconnection!=toconnection)
    {
    if (arguments.touser.size())toconnection->set_user(arguments.touser);
    if (arguments.topassword.size()) toconnection->set_password(arguments.topassword);
    if (arguments.tohost.size()) toconnection->set_host(arguments.tohost);
    if (arguments.toport.size())
    {
        toconnection->set_tcp_port(localestring2uint(arguments.toport));
    }
    if (!toconnection->connect(hk_connection::noninteractive))
    {  
    cerr<<" no toconnection"<<endl;
        exit(1);
    }
    
    }
    
     hk_database* todb=toconnection->new_database(arguments.todatabase);
    
     if (arguments.wholedatabase)
     {
       toconnection->copy_database(fromdb,true,true);
     }
     else
     {
     if (!toconnection->database_exists(arguments.todatabase))
     {
       cerr <<"targetdatabase does not exist"<<endl;
       exit(1);
     }
     if (arguments.allqueries)
        copy_local_files(fromdb,todb,ot_query);
     else 
      if (arguments.query.size()>0)
         copy_singlefile(fromdb,todb,arguments.query,ft_query);
     
     if (arguments.allforms)
        copy_local_files(fromdb,todb,ot_form);
     else 
      if (arguments.form.size()>0)
         copy_singlefile(fromdb,todb,arguments.form,ft_form);
     
     if (arguments.allreports)
        copy_local_files(fromdb,todb,ot_report);
     else 
      if (arguments.report.size()>0)
         copy_singlefile(fromdb,todb,arguments.report,ft_report);
     
    if (arguments.alltables)
    {
    	vector<hk_string>::iterator it=fromdb->tablelist()->begin();
    	while (it!=fromdb->tablelist()->end())
    	{
          hk_datasource* fds= fromdb->new_table((*it));
	  if (!fds) return false;
          todb->copy_table(fds,true,true,true);
          delete fds;
          it++;
    	}
    	
    	//now copy referential integrity
    if (fromdb->connection()->server_supports(hk_connection::SUPPORTS_REFERENTIALINTEGRITY)
       &&todb->connection()->server_supports(hk_connection::SUPPORTS_REFERENTIALINTEGRITY)
    )
    {
    it=fromdb->tablelist()->begin();
    while (it!=fromdb->tablelist()->end())
    {
        if (todb->table_exists((*it)))
        {
          hk_datasource* fds=fromdb->new_table((*it));
          hk_datasource* tds=todb->new_table((*it));
          if (fds &&tds)
          {
          list <referentialclass>* l=fds->referenceslist();
          list <referentialclass>::iterator rit=l->begin();
          while (rit!=l->end())
          {
            tds->add_reference(*rit);
            ++rit;
          }
          delete fds;
          delete tds;
          }
        }
        it++;
    }
    }


    }
    else if(arguments.table.size()>0)
    {
    	if (!fromdb->table_exists(arguments.table))
    	{
    	 cerr <<"Table does not exist"<<endl;
    	 exit(1);
    	}
    	hk_datasource* fromds=fromdb->new_table(arguments.table);
    	if (!fromds) exit(1);
    	todb->copy_table(fromds,true,false,true);
    	delete fromds;
    }
if (arguments.allviews || arguments.view.size()>0)
{
    if (!fromdb->connection()->server_supports(hk_connection::SUPPORTS_VIEWS)
        ||!todb->connection()->server_supports(hk_connection::SUPPORTS_VIEWS)
        )
        {
         cerr <<"Source database and/or target database does not support views!"<<endl;
         exit(1);
        }

}    
    if (arguments.allviews)
    {
    	vector<hk_string>::iterator it=fromdb->viewlist()->begin();
    	while (it!=fromdb->viewlist()->end())
    	{
          hk_datasource* fds= fromdb->new_view((*it));
	  if (!fds) return false;
          todb->copy_view(fds);
          delete fds;
          it++;
    	}
    }
    else if(arguments.view.size()>0)
    {
    	if (!fromdb->view_exists(arguments.view))
    	{
    	 cerr <<"View does not exist"<<endl;
    	 exit(1);
    	}
    	hk_datasource* fromds=fromdb->new_view(arguments.view);
    	if (!fromds) exit(1);
    	todb->copy_view(fromds);
    	delete fromds;
    }
    
    
    }

}

