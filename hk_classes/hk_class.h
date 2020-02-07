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
#ifndef HK_CLASS
#define HK_CLASS
#include "hk_definitions.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef NO_IOSTREAM
#include <iostream.h>
#include <fstream.h>
#else
#include <fstream>
#include <iostream>
#endif                                            //HAVE_IOSTREAM

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


using namespace std;
class hk_connection;
class hk_presentation;
class hk_interpreter;

typedef void  messagewindowtype(const hk_string&);
typedef bool  yesno_dialogtype(const hk_string&, bool );
typedef hk_string translatefunctiontype(const hk_string&);
typedef void password_dialogtype(hk_connection*,struct_connectionfields*);
typedef bool newpassword_dialogtype(hk_string&);
typedef hk_string stringvalue_dialogtype(const hk_string&);
enum enum_operationmode{file_open,file_save};
typedef hk_string file_dialogtype(const hk_string&,enum_operationmode);
//to display the progress of an action,returns true if cancel is pressed progress(currentposition,max);
typedef bool progress_dialogtype(long int,long int,const hk_string&);
/**
 * hk_class is the basic class for all other defined classes. It has some methods to support
 * a simple debugging.
 * @short basic class for all other defined classes.
 *@version $Revision: 1.49 $
 * @author Horst Knorr (hk_classes@knoda.org)
 * @internal
 */
class hk_class
{
    public:
/**
*this enum will be used from other class members if their behaviour should be different e.g. if called
*from the command line in a script (then they should be noninteractive) or as a GUI application, where the
*user should interactively enter a password when the previous login failed.
*/
enum enum_interaction {noninteractive,interactive};
#ifdef  HK_DEBUG
/**
 *The classname is for internal debugging use only
 *It will be shown in front of all debugging messages.
 */
        void hkclassname(const hk_string& n);
        hk_string hkclassname(void)const ;
/**
 *if @ref wanna_debug is true hkdebug prints on standard output the classname and the message d.
 *@param d is the message that will be printed.
 */
        void hkdebug(const hk_string& d)const ;
        void hkdebug(const hk_string& d, const hk_string& e)const ;
        void hkdebug(const hk_string& d, int i)const ;
        void hkdebug(const hk_string& d, double i)const ;
/**
 *@param d if true hk_debug will print messages of the class  else there is no output.
 */
        void wanna_debug(bool d);
        bool wanna_debug(void)const ;
/**
 *@param d if true  hk_debug will print messages of ALL classes even if wanna_debug
 *is false for a particular class.
 */
        static  void set_generaldebug(bool d);
        bool generaldebug(void){return p_generaldebug;}
#endif
/**
 *tries to translate the message t by using the translatingfunction set with @ref set_translatefunction.
 *@return the translated message. If translating was not possible it will return the parameter t unchanged.
 */
        static  hk_string hk_translate(const hk_string& t);
/**
 *prints a warning message by using a function set with @ref set_warningmessage. If none is set the message
 *will be print to standard error (cerr).
 */
        static  void show_warningmessage(const hk_string& m);
/**
 *you can write your own GUI dependend function to print warning messages by defining a function of
 *messagewindowtype
 */
        static  void set_warningmessage(messagewindowtype* m);
/**
 *prints a question by using a function set with @ref set_yesnomessage. If none is set the question
 *will be print to standard error (cerr). The user has either to answer the question with yes or no.
 *@param m the shown question
 *@param default_value if  @ref set_show_pedantic == false the default_value will be returned without
 *asking.
 */
        static  bool show_yesnodialog(const hk_string& m, bool default_value);
/**
 *you can write your own GUI dependend function to print yes_no message by defining a function of
 *dialogtype
 */
        static  void set_yesnodialog(yesno_dialogtype* d);
/**
 *prints a question by using a function set with @ref set_stringvaluedialog. If none is set the question
 *will be print to standard error (cerr). The user has to answer the question by entering a string (i.e. a filename).
 *@param t the shown question
 *@return the string the user gave as an answer
 */

        static  hk_string show_stringvaluedialog(const hk_string& t);
/**
 *you can write your own GUI dependend function to print a string value dialog by defining a function of
 *stringvalue_dialogtype
 */
        static  void set_stringvaluedialog(stringvalue_dialogtype* t);
        static  void set_translatefunction(translatefunctiontype* t);

        static  void set_filedialog(file_dialogtype* f);
	static  hk_string show_filedialog(const hk_string& f="",enum_operationmode m=file_open);

        static  void set_directorydialog(stringvalue_dialogtype* f);
	static  hk_string show_directorydialog(const hk_string& f="");


/**
 * if true all dialogs and messages will be shown. if false
 * only important messages(errors and warnings) will be shown.
 * @param s default = true
 */
        static  void set_showpedantic(bool s);
        static  bool    showpedantic(void){return p_showpedantic;}
	enum enum_measuresystem{cm,inch};
	static void set_measuresystem(enum_measuresystem);
	static enum_measuresystem measuresystem(void);
/**
 *stores the object's values to stream s. Especially used to store queries, forms and reports
 */
        virtual void  savedata(ostream& s);
/**
 *loads the object's values out of the definition string. Especially used to load queries, forms and reports
 */
//         virtual void  loaddata(const hk_string& definition);
        virtual void  loaddata(xmlNodePtr definition);
	enum enum_tagtype{normaltag,mastertag};
/**
 *called from loaddata  to load a variable value originally stored with @ref set_tagvalue
 *@param where a text where the wished value has to be filtered out
 *@param tag the tag name. See @ref set_tagvalue for details
 *@param value the found value will be returned in this parameter
 *@param position if there are more than one values for a variable (i.e. a list of columnnames),first position is 1
 *give the wished position  of the value.
@return true if the tag was found and the value could be set else false
*/




        static  xmlNodePtr get_tagvalue(xmlNodePtr where, const hk_string &tag, hk_string &value, int position=1,enum_tagtype tagtype=normaltag);
        static  xmlNodePtr get_tagvalue(xmlNodePtr where, const hk_string &tag, unsigned long &value, int position=1);
        static  xmlNodePtr get_tagvalue(xmlNodePtr where, const hk_string &tag, unsigned int &value, int position=1);
        static  xmlNodePtr get_tagvalue(xmlNodePtr where, const hk_string &tag, long &value, int position=1);
        static  xmlNodePtr get_tagvalue(xmlNodePtr where, const hk_string &tag, int &value, int position=1);
        static  xmlNodePtr get_tagvalue(xmlNodePtr where, const hk_string &tag, bool &value, int position=1);
        static  xmlNodePtr get_tagvalue(xmlNodePtr &where, const hk_string &tag);


/**
 * Each value will be stored between delimiting tags in the form &lt;tag>value&lt;/tag>\n
 *@param stream a stream to which the value will be written
 *@param tag the tag name
 *@param value the value to be stored
 */
        static  void set_tagvalue(ostream& stream,const hk_string& tag,const hk_string& value);
        static  void set_tagvalue(ostream& stream,const hk_string& tag,unsigned long value);
        static  void set_tagvalue(ostream& stream,const hk_string& tag,unsigned int value);
        static  void set_tagvalue(ostream& stream,const hk_string& tag,long value);
        static  void set_tagvalue(ostream& stream,const hk_string& tag,int value);
        static  void set_tagvalue(ostream& stream,const hk_string& tag,bool value);
/**
 *just writes &lt;tag> to a stream. Use this function if you want to begin a section.
 *Don't forget to call @ref end_mastertag
 *
 */
        static  void start_mastertag(ostream& stream,const hk_string& tag);
/**
 *just writes &lt;/tag> to a stream. Use this function if you want to end a section
 */
        static  void end_mastertag(ostream& stream,const hk_string& tag);
        static  void set_defaultdatetimeformat(const hk_string& timeformat,const hk_string& dateformat,const hk_string&datetimeformat);
        static  hk_string defaultdateformat(void);
        static  hk_string defaulttimeformat(void);
        static  hk_string defaultdatetimeformat(void);

/**
 *This function stores all preferences of the hk_classes library in the file ~/.hk_classes/preferences
 *like the @ref defaultdateformat, and @ref hk_report::printcommand
 */
        static  void save_preferences(void);
/**
 *true if widgets can't get in design mode
 */
        static  bool runtime_only(void);
/**
 *sets the locale which is used for the regional formats (e.g. dataformat). Default is ""
 *that means that the environment variable is queried and set
 */
        static void set_locale(const hk_string& locale);
        static hk_string locale(void);

        static void set_defaultdriver(const hk_string& d);
        static hk_string defaultdriver(void) ;
        
        static void set_default_automatic_data_update(bool);
        static bool default_automatic_data_update(void);

    protected:

        hk_class(void);
        virtual ~hk_class(void);
 hk_interpreter* new_interpreter(const hk_string& interpreter,hk_presentation*);
        static  hk_string p_defaulttimeformat;
        static  hk_string p_defaultdateformat;
        static  hk_string p_defaultdatetimeformat;
        static  bool        p_runtime_only;

    private:
        static  void set_tag(const hk_string& tag);
        static  void set_levelspace(ostream& stream);
        hk_string p_classname;
#ifdef HK_DEBUG
        bool p_debug;
#endif
        static  yesno_dialogtype*   p_yesno;
        static  messagewindowtype*  p_warning;
        static  stringvalue_dialogtype* p_stringvalue;
        static  translatefunctiontype* p_translate;
	static  file_dialogtype* p_filedialog;
	static  stringvalue_dialogtype* p_directorydialog;
        static  hk_string p_begintag_begin;
        static  hk_string p_begintag_end;
        static  hk_string p_endtag_begin;
        static  hk_string p_endtag_end;
        static  hk_string p_begintag;
        static  hk_string p_endtag;
	static  hk_string p_emptytag;
	static  hk_string p_emptytag_begin;
	static  hk_string p_emptytag_end;
        static unsigned int p_taglevel;

        static  bool    p_showpedantic;
        static  bool    p_generaldebug;
        static  hk_string p_locale;
        static  hk_string p_defaultdriver;
        static  bool p_default_automatic_data_update;
	static  enum_measuresystem p_defaultmeasuresystem;

};

  void show_warningmessage(const hk_string& m);
  bool show_yesnodialog(const hk_string& m, bool default_value);
  hk_string show_filedialog(const hk_string& f="",enum_operationmode m=file_open);
  hk_string show_directorydialog(const hk_string& t);
  hk_string show_stringvaluedialog(const hk_string& t);



#endif
