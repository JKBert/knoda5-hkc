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
#ifndef HK_UTILITIES
#define HK_UTILITIES
#ifdef HAVE_ARGP
#include <argp.h>
#endif
#include "hk_drivermanager.h"
#include "hk_connection.h"
#include "hk_database.h"
#include "hk_string.h"
#include "config.h"
#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEFAULTPARAMETER {"driver",'d',"DRIVERNAME",0,"Name of the database driver",0},{"list",'l',0,1,"List available database drivers",0},{"host",'h',"HOST",0,"IP-number or name of hosts",0},{"port",'r',"PORT",0,"TCP-port on host",0},{"user",'u',"USERNAME",0,"user name",0},{"password",'p',"PASSWORD",0,"user password",0},{"database",'b',"DATABASE",0,"database name",0},{"version",10,0,1,"version and author of hk_classes",0}

/**
 *@internal
 *@short used from hk_utilities
 */

class argumentclass
{
    public:
        hk_string driver,host,port,user,password,database,filter;
};
#ifdef HAVE_ARGP
/**
 *Default command line arguments
 */
inline static  error_t parse_generaloptions (int key,char* arg, struct argp_state* state)
{
    struct argumentclass* arguments=(argumentclass*)state->input;
    assert(arguments);
    switch (key)
    {

        case 10     :                             //version
            cout <<"Version "<<VERSION<<"\nAuthor: Horst Knorr <hk_classes@knoda.org>\nhttp://hk-classes.sourceforge.net"<<endl;
            exit(0);
            break;
        case 'l'        :
        {
            vector<hk_string>* l = hk_drivermanager::driverlist();
            vector<hk_string>::iterator it=l->begin();
            while (it!=l->end())
            {
                cout <<*it<<endl;
                ++it;
            }
            exit(0);
        }

        break;
        case 'd'    : arguments->driver=arg;
        break;
        case 'h'    : arguments->host=arg;
        break;
        case 'r'    : arguments->port=arg;
        break;
        case 'u'    : arguments->user=arg;
        break;
        case 'p'    : arguments->password=arg;
        break;
        case 'f'    : arguments->filter=arg;
        break;
        case 'b'    : arguments->database=arg;
        break;
        default :  return ARGP_ERR_UNKNOWN;
    }
    return 0;
}


bool verify_generalarguments(argumentclass& arguments)
{
    if (arguments.driver.size()==0)
    {
        cerr << "No driver selected"<<endl;
        return false;
    }
    if (arguments.database.size()==0)
    {
        cerr << "No database selected"<<endl;
        return false;
    }

    return true;
}
#endif //HAVE_ARGP

bool is_localfile(const hk_string& file)
{
    return (file.find('/')!=hk_string::npos);
}


bool load_file(const hk_string &name,hk_string &file)
{
    if (name.size()==0) return false;
    ifstream ifs(name.c_str(),ios::in);
    if (ifs)
    {
        char c;
        file="";
        while (ifs.get(c))
            file+=c;
    }
    else return false;
    return true;

}
#endif
