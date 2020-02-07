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
//$Revision: 1.82 $
#include "hk_connection.h"
#include "hk_database.h"
#include "hk_datasource.h"
#include "hk_drivermanager.h"
#include "hk_actionquery.h"
#include "hk_url.h"

#include <time.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

password_dialogtype* hk_connection::p_passworddialog=NULL;
newpassword_dialogtype* hk_connection::p_newpassworddialog=NULL;

const char* sqltokens[]=
{
 "ABSOLUTE","ACTION","ADD","ALL","ALLOCATE","ALTER","AND","ANY",
 "ARE","AS","ASC","ASSERTION","AT","AUTHORIZATION","AVG","BEGIN",
 "BETWEEN","BIT","BIT_LENGTH","BOTH","BY","CASCADE","CASCADED","CASE",
 "CAST","CATALOG","CHAR","CHARACTER","CHAR_LENGTH","CHARACTER_LENGTH","CHECK","CLOSE",
 "COALESCE","COLLATE","COLLATION","COLUMN","COMMIT","CONNECT","CONNECTION","CONSTRAINT",
 "CONSTRAINTS","CONTINUE","CONVERT","CORRESPONDING","COUNT","CREATE","CROSS","CURRENT",
 "CURRENT_DATE","CURRENT_TIME","CURRENT_TIMESTAMP","CURRENT_USER","CURSOR","DATE","DAY","DEALLOCATE",
 "DEC","DECIMAL","DECLARE","DEFAULT","DEFERRABLE","DEFERRED","DELETE","DESC",
 "DESCRIBE","DESCRIPTOR","DIAGNOSTICS","DISCONNECT","DISTINCT","DOMAIN","DOUBLE","DROP",
 "ELSE","END","END-EXEC","ESCAPE","EXCEPT","EXCEPTION","EXEC","EXECUTE",
 "EXISTS","EXTERNAL","EXTRACT","FALSE","FETCH","FIRST","FLOAT","FOR",
 "FOREIGN","FOUND","FROM","FULL","GET","GLOBAL","GO","GOTO",
 "GRANT","GROUP","HAVING","HOUR","IDENTITY","IMMEDIATE","IN","INDICATOR",
 "INITIALLY","INNER","INPUT","INSENSITIVE","INSERT","INT","INTEGER","INTERSECT",
 "INTERVAL","INTO","IS","ISOLATION","JOIN","KEY","LANGUAGE","LAST",
 "LEADING","LEFT","LEVEL","LIKE","LOCAL","LOWER","MATCH","MAX",
 "MIN","MINUTE","MODULE","MONTH","NAMES","NATIONAL","NATURAL","NCHAR",
 "NEXT","NO","NOT","NULL","NULLIF","NUMERIC","OCTET_LENGTH","OF",
 "ON","ONLY","OPEN","OPTION","OR","ORDER","OUTER","OUTPUT",
 "OVERLAPS","PAD","PARTIAL","POSITION","PRECISION","PREPARE","PRESERVE","PRIMARY",
 "PRIOR","PRIVILEGES","PROCEDURE","PUBLIC","READ","REAL","REFERENCES","RELATIVE",
 "RESTRICT","REVOKE","RIGHT","ROLLBACK","ROWS","SCHEMA","SCROLL","SECOND",
 "SECTION","SELECT","SESSION","SESSION_USER","SET","SIZE","SMALLINT","SOME",
 "SPACE","SQL","SQLCODE","SQLERROR","SQLSTATE","SUBSTRING","SUM","SYSTEMUSER",
 "TABLE","TEMPORARY","THEN","TIME","TIMESTAMP","TIMEZONE_HOUR","TIMEZONE_MINUTE","TO",
 "TRAILING","TRANSACTION","TRANSLATE","TRANSLATION","TRIM","TRUE","UNION","UNIQUE",
 "UNKNOWN","UPDATE","UPPER","USAGE","USER","USING","VALUE","VALUES",
 "VARCHAR","VARYING","VIEW","WHEN","WHENEVER","WHERE","WITH","WORK",
 "WRITE","YEAR","ZONE"
};
class hk_connectionprivate
{
public:
        hk_string   p_host,p_user,p_password,p_defaultdatabase;
        unsigned int    p_tcp_port;
        hk_string p_sqldelimiter;
        hk_string p_classespath;
        hk_string p_databasepath;
	bool p_booleanemulation;

};


hk_connection::hk_connection(hk_drivermanager* c)
{
#ifdef HK_DEBUG
    hkdebug("hk_connection::hk_connection");
#endif
    p_private=new hk_connectionprivate;
    set_tcp_port(default_tcp_port());
    p_connected=false;
    p_database=NULL;
    p_debug_counter=0;
    p_private->p_host="localhost";
    p_private->p_user="root";
    srand(time(NULL));
    char* h= getenv("HOME");
    p_private->p_classespath=(h==NULL?"/tmp":h);
    p_private->p_classespath +="/.hk_classes";
    p_private->p_databasepath=p_private->p_classespath+"/";p_private->p_databasepath+=p_private->p_host.size()>0?p_private->p_host:"localhost";
    p_drivermanager=c;
    p_private->p_booleanemulation=true;
   
   for (unsigned int i=0;i<sizeof(sqltokens)/sizeof(const char*);++i)
    {
    p_sqlstatements.push_back(sqltokens[i]);
    }


}


hk_connection::~hk_connection()
{
#ifdef HK_DEBUG
    hkdebug("hk_connection:destructor");
#endif
    disconnect();
    if (p_database!=NULL)
    {
        delete p_database;
    }
    if (p_drivermanager!=NULL) p_drivermanager->connection_remove(this);
    delete p_private;
#ifdef HK_DEBUG
    hkdebug("hk_connection::~hk_connection ENDE");
#endif
}


void hk_connection::set_password(const hk_string& p)
{
    p_private->p_password=p;
}


hk_string hk_connection::password(void)
{
    return p_private->p_password.c_str();
}


void hk_connection::set_user(const hk_string& u)
{
    p_private->p_user=u;
}


hk_string hk_connection::user(void)
{
    return  p_private->p_user;
}


void hk_connection::set_host(const hk_string& h)
{
    p_private->p_host=h;
        p_private->p_databasepath=p_private->p_classespath+"/";p_private->p_databasepath+=p_private->p_host.size()>0?p_private->p_host:"localhost";
        mkdir (p_private->p_databasepath.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);

}


hk_string hk_connection::host(void)
{
    return  p_private->p_host;
}


void hk_connection::set_tcp_port(unsigned int t)
{
    p_private->p_tcp_port=t;
    ;
}


unsigned int hk_connection::tcp_port(void)
{

    return p_private->p_tcp_port;
}

hk_string hk_connection::sqldelimiter(void)
{
	return p_private->p_sqldelimiter;
}

hk_string hk_connection::databasepath(void)
 {
 return p_private->p_databasepath;
 }

bool  hk_connection::connect(enum_interaction c)
{
#ifdef HK_DEBUG
    hkdebug("hk_connection::connect");
#endif
    if  (p_connected)return true;
    bool cancel=false;
    int logincounter=0;
    while ((!is_connected())&&(!cancel)&&logincounter<4)
    {
        if (!cancel) driver_specific_connect();
        if (!is_connected())
            {
	      if ( c==noninteractive) cancel=true;
              else
	      {
	        if (logincounter>0)
                  show_warningmessage(hk_translate("Servermessage: ")+last_servermessage());
		cancel=!show_passworddialog();
		++logincounter;
	      }
	    }
    }

    if (is_connected())
    {
        if (p_database!=NULL) p_database->select_db(p_database->name());
        return true;
    }
    else
    {
        show_warningmessage(hk_translate("Unable to connect to the SQL-Server!"));
        return false;
    }

}


void hk_connection::set_passworddialog(password_dialogtype* d)
{
    p_passworddialog=d;
}


bool hk_connection::show_passworddialog(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_connection::show_passworddialog");
#endif
    struct_connectionfields fields;
    if (!server_needs(NEEDS_LOGIN)) return true;
    if (p_passworddialog!=NULL)
    {
        fields.host=host();
        fields.user=user();
        fields.save=false;
	fields.database=(p_database?p_database->name():defaultdatabase());
	fields.allowdatabaseselect=!(p_database);
        fields.port=longint2string(p_private->p_tcp_port);
        fields.password=password();
	fields.emulateboolean=booleanemulation();
        p_passworddialog(this,&fields);
        if (!fields.cancel)
        {
            if (server_needs(NEEDS_HOST))
                set_host(fields.host);
            if (server_needs(NEEDS_USERNAME))
                set_user(fields.user);
            if (server_needs(NEEDS_PASSWORD))
                set_password(fields.password);
            if (server_needs(NEEDS_DATABASENAME))
                set_defaultdatabase(fields.database);
            if (server_needs(NEEDS_SQLDELIMITER))
                p_private->p_sqldelimiter=fields.sql_delimiter;
            if (server_needs(NEEDS_PORT))
                p_private->p_tcp_port=atoi(fields.port.c_str());
            if (server_needs(NEEDS_BOOLEANEMULATION))
                set_booleanemulation(fields.emulateboolean);
            if (fields.save) save_configuration();
        }
        return !(fields.cancel);
    }
    else
    {
        cerr <<hk_translate("Please give me the following information:")<<endl;
        cerr <<"========================================="<<endl;

        if (server_needs(NEEDS_HOST))
        {
            cerr <<hk_translate("Host: ");
            cin  >> fields.host;
	    set_host(fields.host);
        }
        if (server_needs(NEEDS_USERNAME))
        {
            cerr <<hk_translate("User: ");
            cin >> fields.user;
            set_user(fields.user);
        }
        if (server_needs(NEEDS_DATABASENAME))
        {
            cerr <<hk_translate("Database name: ");
            cin >> fields.database;
            set_defaultdatabase(fields.database);
        }
        if (server_needs(NEEDS_PASSWORD))
        {
            cerr <<hk_translate("Password (will not be displayed): ");
            struct termios initialsettings,newsettings ;
            tcgetattr(fileno(stdin),&initialsettings);
            newsettings=initialsettings;
            newsettings.c_lflag &=~ECHO;
            tcsetattr(fileno(stdin),TCSAFLUSH,&newsettings);
            cin >> fields.password;
            tcsetattr(fileno(stdin),TCSAFLUSH,&initialsettings);
            cerr<<endl;
            set_password(fields.password);
        }

        if (server_needs(NEEDS_PORT))
        {
            cerr <<hk_translate("TCP Port: ");
            cin >> fields.port;
            set_tcp_port(atoi(fields.port.c_str()));
        }

        if (server_needs(NEEDS_SQLDELIMITER))
        {
            cerr <<hk_translate("SQL Delimiter: ");
            cin >> fields.sql_delimiter;
        }

	if (server_needs(NEEDS_BOOLEANEMULATION))
        {
            cerr <<hk_translate("Emulate Boolean: ");
            cerr <<hk_translate("n=No            all other keys = yes")<<endl;
            hk_string i;
	    cin >>i;
		if (i=="n"||i=="N") fields.emulateboolean=false;
				else fields.emulateboolean=true;
        }

        if (server_needs(NEEDS_HOST))
            set_host(fields.host);
        if (server_needs(NEEDS_USERNAME))
            set_user(fields.user);
        if (server_needs(NEEDS_PASSWORD))
            set_password(fields.password);
        if (server_needs(NEEDS_SQLDELIMITER))
            p_private->p_sqldelimiter=fields.sql_delimiter;
        if (server_needs(NEEDS_PORT))
            p_private->p_tcp_port=atoi(fields.port.c_str());
        if (server_needs(NEEDS_BOOLEANEMULATION))
                set_booleanemulation(fields.emulateboolean);

        return true;                              //not cancel

    }

    return true;
}


bool hk_connection::disconnect()
{
    inform_databases_disconnect();
    return driver_specific_disconnect();
}


vector<hk_string>* hk_connection::dblist(void)
{
    vector<hk_string>* l= driver_specific_dblist();
    if (l) sort(l->begin(),l->end());

    return l;

}


void hk_connection::db_remove(hk_database* d)
{
    if (p_database==d) p_database=NULL;
}


hk_database* hk_connection::new_database(const hk_string& name)
{
#ifdef HK_DEBUG
    hkdebug("hk_connection::new_database");
#endif
    if (name.size()>0&& !database_exists(name)) return NULL;



    if (p_database!=NULL)
    {
        p_database->inform_datasources_before_closing();
        delete p_database;
    }
    p_database=driver_specific_new_database();
    if (name.size()>0)p_database->set_name(name);
    make_databasedir(name);
    return p_database;
}


void hk_connection::make_databasedir(const hk_string& dbname)
{
    if (dbname.size()==0)return ;
    hk_string p=p_private->p_databasepath+"/"; p+=dbname;
    mkdir (p.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);

}


bool hk_connection::is_connected(void)
{
    return p_connected;
}


void hk_connection::inform_databases_connect()
{
    if (p_database!=NULL) p_database->connection_connected();
}


void hk_connection::inform_databases_disconnect()
{
    if (p_database!=NULL) p_database->connection_disconnected();
}


bool hk_connection::create_database(const hk_string& dbase)
{
#ifdef HK_DEBUG
    hkdebug("hk_connection::create_database");
#endif


    if (!server_supports(SUPPORTS_NEW_DATABASE))
      {
       show_warningmessage(hk_translate("The driver does not support creating new databases!"));
       return false;
      }
    if (p_database==NULL) new_database();
    if (p_database==NULL||!is_connected())
    {
      show_warningmessage(hk_translate("Not connected to the server"));
      return false;
    }
    hk_actionquery* q=p_database->new_actionquery();
    if (!q)
    	{
               show_warningmessage(hk_translate("Bug: Actionquery could not be created"));
		return false;
	}
    hk_string sql="CREATE DATABASE ";
    sql+=q->identifierdelimiter()+dbase+q->identifierdelimiter();
    sql+=p_private->p_sqldelimiter;

    bool result=false;
    if (q!=NULL)
    {
        q->set_sql(sql.c_str(),sql.size());
        result=q->execute();
        delete  q;
    }
    if (!result)
    {
        hk_string reason=hk_translate("Database could not be created!")+"\n"+hk_translate("Servermessage: ")+last_servermessage();
        show_warningmessage(reason);
    }
    return result;
}


bool hk_connection::delete_database(const hk_string& dbase,enum_interaction c)
{
    if (p_database==NULL) new_database();
    if (p_database==NULL||!is_connected()) return false;
    bool result=false;
    hk_string warning=hk_translate("Delete the database \"%DBNAME%\"?");
    warning=replace_all("%DBNAME%",warning,dbase);
    if(c==noninteractive || show_yesnodialog(warning,true))
    {
       result=driver_specific_delete_database(dbase);
    }

    if (result)
    {
        hk_string n=dbase;
        if (p_database!=NULL) if (p_database->name()==n)
        {
            delete  p_database;
            p_database=NULL;
        }
        delete_databasedirectory(dbase);
    }
    else
    {
        hk_string reason=hk_translate("Database could not be deleted!")+"\n"+hk_translate("Servermessage: ")+last_servermessage();
        show_warningmessage(reason);
    }
    return result;

}



bool    hk_connection::driver_specific_delete_database(const hk_string& dbase)
{
    bool result=false;
    if (p_database==NULL) new_database();
    hk_actionquery* q=p_database->new_actionquery();
    if (!q)
      {
        show_warningmessage("hk_connection::driver_specific_delete_database() Could not create actionquery!");
	return false;
      }
    hk_string sql="DROP DATABASE ";
    sql+=q->identifierdelimiter()+dbase+q->identifierdelimiter();
    sql+=p_private->p_sqldelimiter;
    if (q!=NULL)
        {
            q->set_sql(sql.c_str(),sql.size());
            result=q->execute();
            delete  q;
        }
    else return false;

  return result;
}


void hk_connection::savedata(ostream& s)
{
    hk_string mt="DBCONFIGURATION";
    s<<"<?xml version=\"1.0\" ?>"<<endl;
    start_mastertag(s,mt);
    set_tagvalue(s,"HOST",p_private->p_host);
    set_tagvalue(s,"USER",p_private->p_user);
    set_tagvalue(s,"TCP-PORT",(long unsigned int)p_private->p_tcp_port);
    set_tagvalue(s,"BOOLEANEMULATION",p_private->p_booleanemulation);
    set_tagvalue(s,"DATABASE",p_private->p_defaultdatabase);
    set_tagvalue(s,"PASSWORD",p_private->p_password);
    end_mastertag(s,mt);

}


void hk_connection::save_configuration(void)
{
    mkdir (p_private->p_classespath.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
    hk_string p_filename =p_private->p_classespath+"/";
    p_filename+="driver.conf";
    ofstream* stream= new ofstream(p_filename.c_str());
    if (stream==NULL)return;
    savedata(*stream);

    delete stream;
    chmod(p_filename.c_str(),S_IRUSR|S_IWUSR);
}


void  hk_connection::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_connection::loaddata");
#endif
    hk_string buffer;
    long unsigned int intbuffer;
                                                  //cout <<"host: "<<p_host<<endl;
    if (get_tagvalue(definition,"HOST",buffer)) set_host(buffer) ;
                                                  //;cout <<"user: "<<p_user<<endl;
    if (get_tagvalue(definition,"USER",buffer)) set_user(buffer);
                                                  //;cout <<"tcp: "<<p_tcp_port<<endl;
    if (get_tagvalue(definition,"TCP-PORT",intbuffer))set_tcp_port(intbuffer);
    get_tagvalue(definition,"BOOLEANEMULATION",p_private->p_booleanemulation);
    get_tagvalue(definition,"DATABASE",p_private->p_defaultdatabase);
}


void hk_connection::load_configuration(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_connection::load_configuration");
#endif
    hk_string n=p_private->p_classespath+"/driver.conf";
    if (!file_exists(n)) return;
    xmlDocPtr doc=xmlParseFile(n.c_str());
    xmlNodePtr result=NULL;
    if (doc) result=xmlDocGetRootElement(doc);
    if (doc && result)
    {
        loaddata(result);

	struct stat statbuf;
	if (lstat(n.c_str(),&statbuf)<0)
		    return;
	bool is_safe=!(statbuf.st_mode&(S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH|S_IXOTH));
	if (is_safe)
		{
		 hk_string buffer;
		 if (get_tagvalue(result,"PASSWORD",buffer))
		 	{
				set_password(buffer);
			}
		}
    }


}


void hk_connection::set_classespath(hk_string& p)
{
    p_private->p_classespath=p;
    p_private->p_databasepath=p+"/";p_private->p_databasepath+=p_private->p_host.size()>0?p_private->p_host:"localhost";
    mkdir (p_private->p_databasepath.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
    load_configuration();
}


bool hk_connection::set_newpassword(const hk_string& newpassword)
{
#ifdef HK_DEBUG
    hkdebug("hk_connection::set_newpassword");
#endif
    if (driver_specific_new_password(newpassword))
    {
        set_password(newpassword);
        return true;
    }
    else
    {
        hk_string reason=hk_translate("Password could not be changed!")+"\n"+hk_translate("Servermessage: ")+last_servermessage();
        show_warningmessage(reason);
    }
    return false;
}


void hk_connection::show_newpassworddialog(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_connection::show_newpassworddialog");
#endif
    if (!is_connected())
    {
        connect();
        if (!is_connected()) return;
    }

    hk_string newpasswd;
    if (p_newpassworddialog!=NULL)
    {
        if (!p_newpassworddialog(newpasswd)) return;
    }
    else
    {
//command line new password dialog
        hk_string np1,np2;
        struct termios initialsettings,newsettings ;
        tcgetattr(fileno(stdin),&initialsettings);
        cerr <<hk_translate("New password (will not be displayed): ");
        newsettings=initialsettings;
        newsettings.c_lflag &=~ECHO;
        tcsetattr(fileno(stdin),TCSAFLUSH,&newsettings);
        cin >> np1;
        cerr << endl<<hk_translate("Reenter new password:");
        cin >>np2;
        tcsetattr(fileno(stdin),TCSAFLUSH,&initialsettings);
        cerr<<endl;
        if (np1!=np2)
        {
            show_warningmessage("Passwords do not match!");
            return;
        }
        newpasswd=np1;

    }
    set_newpassword(newpasswd);
}


void hk_connection::set_newpassworddialog(newpassword_dialogtype* d)
{
    p_newpassworddialog=d;

}


hk_string hk_connection::drivername(void) const
{

    return hk_translate("Unknown driver");
}


hk_drivermanager* hk_connection::drivermanager(void)
{
    return p_drivermanager;

}


bool    hk_connection::database_exists(const hk_string& databasename)
{

#ifdef HK_DEBUG
    hkdebug("hk_connection::database_exists");
#endif
  if (server_supports(SUPPORTS_LOCAL_FILEFORMAT))
  {
    if (server_needs(NEEDS_DIRECTORY_AS_DATABASE))
    {
     DIR* d=opendir(databasename.c_str());
     if (d){
     closedir(d);
     return true;
     }
    }
    else
    {
    hk_url url=databasename;
    if (url.directory().size()>0)
     {
      ifstream ifs(databasename.c_str(),ios::in);
      if (ifs )
         return true;
     }
    }
  }



    vector<hk_string>* liste=dblist();
    if (!liste) return false;
    vector<hk_string>::iterator it= liste->begin();
    while (it!=liste->end())
    {
        if ((*it)==databasename)return true;

        it++;
    }
    return false;

}


bool hk_connection::copy_database(hk_database* fromdatabase,bool schema_and_data,bool cpy_lcl_files,progress_dialogtype* progressdialog)
{
    bool cancel=false;
    if (!fromdatabase)return false;
   cerr <<check_capabilities(fromdatabase->connection(),this)<<endl;
   hk_url url=fromdatabase->name();
   hk_string targetname=url.filename();
    bool dbexists=database_exists(targetname);

    if (dbexists && runtime_only())
     {
       return false;
     }
    hk_database* newdb=driver_specific_new_database();
    if (dbexists)
    {

        hk_string nm=ask_dbname();
        if (nm.size()==0|| database_exists(nm))
        {
            delete newdb;
            return false;
        } else
        {
            if (!create_database(nm))return false;
            newdb->set_name(nm);

        }

    } else
    {
        if (!create_database(targetname))return false;
        newdb->set_name(targetname);
    }

//now copy all tables

    vector<hk_string>::iterator it=fromdatabase->tablelist()->begin();
    while (it!=fromdatabase->tablelist()->end())
    {
        hk_datasource* fds= fromdatabase->new_table((*it));
	if (!fds) return false;
        cancel=!newdb->copy_table(fds,schema_and_data,true,true,progressdialog);
        delete fds;
        it++;
    }
//now copy all views
    if (fromdatabase->connection()->server_supports(SUPPORTS_VIEWS)
       &&server_supports(SUPPORTS_VIEWS)
    )
    {
    it=fromdatabase->viewlist()->begin();
    while (it!=fromdatabase->viewlist()->end())
    {
        hk_datasource* fds= fromdatabase->new_view((*it));
        cancel=!newdb->copy_view(fds,progressdialog);
        delete fds;
        it++;
    }
    }


//now copy referential integrity
    if (fromdatabase->connection()->server_supports(SUPPORTS_REFERENTIALINTEGRITY)
       &&server_supports(SUPPORTS_REFERENTIALINTEGRITY)
    )
    {
    it=fromdatabase->tablelist()->begin();
    while (it!=fromdatabase->tablelist()->end())
    {
        if (newdb->table_exists((*it)))
        {
          hk_datasource* fds=fromdatabase->new_table((*it));
          hk_datasource* tds=newdb->new_table((*it));
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


    if (cpy_lcl_files )
    {                                             //now copy all queries,forms and reports

        copy_local_files(fromdatabase,newdb,ot_table,progressdialog);
        copy_local_files(fromdatabase,newdb,ot_query,progressdialog);
        copy_local_files(fromdatabase,newdb,ot_form,progressdialog);
        copy_local_files(fromdatabase,newdb,ot_report,progressdialog);
    }                                             //end copy_local_files
    delete newdb;
    return !cancel;

}


bool hk_connection::copy_local_files(hk_database* fromdb,hk_database* todb,objecttype f,progress_dialogtype* progressdialog)
{
    if (!fromdb||!todb)return false;

    bool cancel=false;
    vector<hk_string>* namelist=NULL;
    hk_string viewlist;
    switch (f)
    {
        case ot_table:  namelist=fromdb->filelist(ft_table);viewlist=hk_translate("Copying tabledefinition: %FILE%");break;
        case ot_query:  namelist=fromdb->querylist();viewlist=hk_translate("Copying query: %FILE%");break;
        case ot_form:   namelist=fromdb->formlist();viewlist=hk_translate("Copying form: %FILE%");break;
        case ot_report: namelist=fromdb->reportlist();viewlist=hk_translate("Copying report: %FILE%");break;
        default: return false;
    }
    if (!namelist) return false;

    vector<hk_string>::iterator it=namelist->begin();
    int r=1;
    while (it!=namelist->end())
    {
     hk_string input=fromdb->load((*it),(filetype)f);
     todb->save(input,(*it),(filetype)f,true,true);
     if (progressdialog) cancel=progressdialog(r,namelist->size(),replace_all("%FILE%",viewlist,(*it)));
        it++;r++;
    }
    return !cancel;
}


hk_string hk_connection::ask_dbname(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::ask_name");
#endif
    hk_string res=show_stringvaluedialog(hk_translate("Please enter the name of the database:"));

    return res;

}


void hk_connection::delete_databasedirectory(const hk_string& db)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::delete_databasedirectory");
#endif
    hk_url url=db;
    hk_string dir=(url.directory().size()>0?db:databasepath()+"/"+db);
    delete_directory(dir+"/output");

    delete_directory(dir);

}


void hk_connection::delete_directory(const hk_string& d)
{
#ifdef HK_DEBUG
    hkdebug("hk_database::delete_directory");
#endif
    DIR *dp;
    struct dirent *entry;
    hk_string filename;

    if ((dp=opendir(d.c_str()))!=NULL)
    {

        while ((entry = readdir(dp))!=NULL)
        {
            filename=entry->d_name;
            if ((filename != ".") && (filename != ".."))
            {
                filename=d+"/"+filename;
                unlink(filename.c_str());
            }
        }
        closedir(dp);
        rmdir (d.c_str());

    }

}


unsigned int    hk_connection::default_tcp_port(void) const
{
    return 0;
}


void hk_connection::set_booleanemulation(bool emulate)
{

 p_private->p_booleanemulation=emulate;
}

bool hk_connection::booleanemulation(void) const
{
  return p_private->p_booleanemulation;
}

bool hk_connection::rename_database(const hk_string& /*originalname*/, const hk_string& /*new_name*/)
{
return false;
}



void hk_connection::set_last_servermessage(const hk_string& m)
{
   p_lastservermessage=m; //cerr <<"set_last_servermessage= "<<m<<endl;
}

void hk_connection::set_defaultdatabase(const hk_string& db)
{
 p_private->p_defaultdatabase=db;
}

hk_string hk_connection::defaultdatabase(void) const
{
  return p_private->p_defaultdatabase;
}



hk_string hk_connection::check_capabilities(hk_connection* from,hk_connection* to)
{
 hk_string result;
 if ((!from||!to)||(from==to)) return result;
 if (!to->server_supports(SUPPORTS_NEW_DATABASE)) result+="* "+hk_translate("Can't create a new target database")+"\n";
 if (!to->server_supports(SUPPORTS_NEW_TABLE)) result+="* "+hk_translate("Can't create new tables")+"\n";
 if (to->maxfieldnamesize()<from->maxfieldnamesize())result+="* "+replace_all("%1",hk_translate("Field names will be truncated to %1 characters"),ulongint2string(to->maxfieldnamesize()))+"\n";
 if (from->server_supports(SUPPORTS_PROPRIETARYCOLUMN)) result+="* "+hk_translate("Source database may contain proprietary field types")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_NONALPHANUM_FIELDNAMES)) result+="* "+hk_translate("Field names with other characters than [A-Za-z0-9] are not allowed")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_NONASCII_FIELDNAMES)) result+="* "+hk_translate("Field names with non-ASCII characters are not supported")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_SPACE_FIELDNAMES)) result+="* "+hk_translate("Field names with space characters are not supported")+"\n";


 if (!internal_copy_check(from,to,SUPPORTS_TEXTCOLUMN)) result+="* "+hk_translate("No 'char' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_INTEGERCOLUMN)) result+="* "+hk_translate("No 'integer' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_SMALLINTEGERCOLUMN)) result+="* "+hk_translate("No 'smallinteger' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_FLOATINGCOLUMN)) result+="* "+hk_translate("No 'floating' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_SMALLFLOATINGCOLUMN)) result+="* "+hk_translate("No 'smallfloating' field type")+"\n";

 if (!internal_copy_check(from,to,SUPPORTS_AUTOINCCOLUMN)) result+="* "+hk_translate("No 'autoincrement' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_BOOLCOLUMN)) result+="* "+hk_translate("No 'boolean' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_DATECOLUMN)) result+="* "+hk_translate("No 'date' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_TIMECOLUMN)) result+="* "+hk_translate("No 'time' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_DATETIMECOLUMN)) result+="* "+hk_translate("No 'datetime' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_BINARYCOLUMN)) result+="* "+hk_translate("No 'binary' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_MEMOCOLUMN)) result+="* "+hk_translate("No 'memo' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_TIMESTAMPCOLUMN)) result+="* "+hk_translate("No 'timestamp' field type")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_SQL)) result+="* "+hk_translate("Target does not support SQL")+"\n";
 else
 {
  if (!internal_copy_check(from,to,SUPPORTS_SQL_GROUP_BY)) result+="* "+hk_translate("SQL does not support 'GROUP BY' statement")+"\n";
  if (!internal_copy_check(from,to,SUPPORTS_SQL_ORDER_BY)) result+="* "+hk_translate("SQL does not support 'ORDER BY' statement")+"\n";
  if (!internal_copy_check(from,to,SUPPORTS_SQL_HAVING)) result+="* "+hk_translate("SQL does not support 'HAVING' statement")+"\n";
  if (!internal_copy_check(from,to,SUPPORTS_SQL_WHERE)) result+="* "+hk_translate("SQL does not support 'WHERE' statement")+"\n";
  if (!internal_copy_check(from,to,SUPPORTS_SQL_ALIAS)) result+="* "+hk_translate("SQL does not support 'ALIAS' statement")+"\n";
 }
 if (!internal_copy_check(from,to,SUPPORTS_REFERENTIALINTEGRITY)) result+="* "+hk_translate("Referential integrity not supported")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_CREATE_INDEX)) result+="* "+hk_translate("Indices are not supported")+"\n";
 if (!internal_copy_check(from,to,SUPPORTS_VIEWS)
    ||!internal_copy_check(from,to,SUPPORTS_NEW_VIEW)
    ) result+="* "+hk_translate("Views are not supported")+"\n";

 if (result.size()>0)
 result=replace_all("%2",replace_all("%1",hk_translate("Copying data from '%1' to '%2':\n"),from->drivername()),to->drivername())+result;


 return result;

}

bool hk_connection::internal_copy_check(hk_connection* from, hk_connection* to,support_enum feature)
{
  if (!from||!to) return true;
  if (from->server_supports(feature)&&!to->server_supports(feature)) return false;
return true;
}


bool hk_connection::is_sqlstatement(const hk_string& value)
{
 hk_string v=string2upper(value);
 return (find(p_sqlstatements.begin(),p_sqlstatements.end(),v)!=p_sqlstatements.end());
}


