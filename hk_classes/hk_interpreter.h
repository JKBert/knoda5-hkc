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
#ifndef HK_INTERPRETER
#define HK_INTERPRETER
#include "hk_class.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

class hk_button;
class hk_dslineedit;
class hk_dsgrid;
class hk_dsgridcolumn;
class hk_dsboolean;
class hk_dscombobox;
class hk_dsmemo;
class hk_dsimage;
class hk_dsdate;
class hk_dsgrid;
class hk_visible;
class hk_dsvisible;
class hk_dsdatavisible;
class hk_presentation;
class hk_form;
class hk_report;
class hk_reportdata;
class hk_subform;

/**
 * hk_interpreter is the basic class for script handling
 * @short basic class for script handling
 *@version $Revision: 1.26 $
 * @author Horst Knorr (hk_classes@knoda.org)
 */

class hk_interpreter:public hk_class
{
public:
hk_interpreter(hk_presentation* p);
virtual ~hk_interpreter(){}
virtual void init(){}
/**
* executes the on_click action
*/
virtual bool on_click(hk_visible*)=0;
/**
* executes the on_doubleclick action
*/
virtual bool on_doubleclick(hk_visible*)=0;
/**
* executes the before_row_change action
*/

virtual bool before_row_change(hk_dsvisible*)=0;
/**
* executes the after_row_change action
*/
virtual bool after_row_change(hk_dsvisible*)=0;
/**
* executes the before_update action
*/
virtual bool before_update(hk_dsvisible*)=0;
/**
* executes the after_update action
*/

virtual bool after_update(hk_dsvisible*)=0;
/**
* executes the before_delete action
*/
virtual bool before_delete(hk_dsvisible*)=0;
/**
* executes the after_delete action
*/
virtual bool after_delete(hk_dsvisible*)=0;
/**
* executes the before_insert action
*/
virtual bool before_insert(hk_dsvisible*)=0;
/**
* executes the after_insert action
*/
virtual bool after_insert(hk_dsvisible*)=0;
/**
* executes the on_close action
*/
virtual bool on_close(hk_visible*)=0;
/**
* executes the on_open action
*/
virtual bool on_open(hk_visible*)=0;
/**
* executes the on_getfocus action
*/
virtual bool on_getfocus(hk_visible*)=0;
/**
* executes the on_loosefocus action
*/
virtual bool on_loosefocus(hk_visible*)=0;

/**
* executes the on_key action
*/
virtual bool on_key(hk_visible*)=0;

/**
* executes the on_valuechanged action
*/
virtual bool on_valuechanged(hk_dsdatavisible*)=0;

/**
* executes the on_print_data action
*/
virtual bool on_print_data(hk_reportdata*)=0;
/**
* executes the on_print_new_page action
*/
virtual bool on_print_new_page(hk_report*)=0;

virtual bool on_select(hk_dscombobox*)=0;
virtual bool on_select(hk_dsgridcolumn*)=0;
virtual bool load_module(const hk_string&)=0;

void set_block_execution(bool);
bool block_execution(void) const;

/**
* @return true if the last executed script failed due to an error
*/
bool scripterror(void){return p_error_occured;}
/**
* @return the script language specific errormessage
*/
hk_string errormessage(void){return p_errormessage;}
/**
* @return the row number  of the script in which the error occured
*/
int error_rownumber(void){return p_error_rownumber;}
/**
* @return the name of the scripting language
*/
virtual hk_string interpretername(void) const {return "unknown";}
enum enum_action{	a_before_row_change,
				a_after_row_change,
				a_click,
				a_doubleclick,
				a_on_open,
				a_on_close,
				a_on_getfocus,
				a_on_loosefocus,
				a_on_key,
				a_before_update,
				a_after_update,
				a_before_delete,
				a_after_delete,
				a_before_insert,
				a_after_insert,
				a_on_print_data,
				a_on_print_new_page,
				a_on_select,
				a_on_valuechanged,
				a_on_loadmodule
				};//when gets updated don't forget to update hk_kdesimpleform/hk_kdesimplereport::script_error !!!

protected:
hk_presentation* p_presentation;
bool p_error_occured;
hk_string p_errormessage;
int p_error_rownumber;

private:
bool p_block_execution;
};

/**
 * hk_nointerpreter always returns a warning message
 * @short basic dummy interpreter class that returns a warning message
 *@version $Revision: 1.26 $
 *@internal
 * @author Horst Knorr (hk_classes@knoda.org)
 */

class hk_no_interpreter:public hk_interpreter
{
public:
hk_no_interpreter(hk_presentation*p);
virtual bool on_click(hk_visible*);
virtual bool on_doubleclick(hk_visible*);
virtual bool before_row_change(hk_dsvisible*);
virtual bool after_row_change(hk_dsvisible*);
virtual bool before_update(hk_dsvisible*);
virtual bool after_update(hk_dsvisible*);
virtual bool on_close(hk_visible*);
virtual bool on_open(hk_visible*);
virtual bool before_delete(hk_dsvisible*);
virtual bool after_delete(hk_dsvisible*);
virtual bool before_insert(hk_dsvisible*);
virtual bool after_insert(hk_dsvisible*);
virtual bool on_print_data(hk_reportdata*);
virtual bool on_print_new_page(hk_report*);
virtual bool on_getfocus(hk_visible*);
virtual bool on_loosefocus(hk_visible*);
virtual bool on_key(hk_visible*);
virtual bool on_select(hk_dscombobox*);
virtual bool on_select(hk_dsgridcolumn*);
virtual bool on_valuechanged(hk_dsdatavisible*);
virtual bool load_module(const hk_string&);
private:
void warning(void);
 bool warning_already_displayed;
};



class hk_pythoninterpreterprivate;
/**
 * hk_pythoninterpreter is the basic class for Python script handling
 * @short basic class for Python script handling
 *@version $Revision: 1.26 $
 *@internal
 * @author Horst Knorr (hk_classes@knoda.org)
 */

class hk_pythoninterpreter:public hk_interpreter
{
public:
hk_pythoninterpreter(hk_presentation* );
virtual ~hk_pythoninterpreter();
virtual void init();

virtual bool on_click(hk_visible*);
virtual bool on_doubleclick(hk_visible*);
virtual bool before_row_change(hk_dsvisible*);
virtual bool after_row_change(hk_dsvisible*);
virtual bool before_update(hk_dsvisible*);
virtual bool after_update(hk_dsvisible*);
virtual bool on_close(hk_visible*);
virtual bool on_open(hk_visible*);
virtual hk_string interpretername(void) const {return "python";}
virtual bool before_delete(hk_dsvisible*);
virtual bool after_delete(hk_dsvisible*);
virtual bool before_insert(hk_dsvisible*);
virtual bool after_insert(hk_dsvisible*);
virtual bool on_print_data(hk_reportdata*);
virtual bool on_print_new_page(hk_report*);
virtual bool on_getfocus(hk_visible*);
virtual bool on_loosefocus(hk_visible*);
virtual bool on_key(hk_visible*);
virtual bool on_select(hk_dscombobox*);
virtual bool on_select(hk_dsgridcolumn*);
virtual bool on_valuechanged(hk_dsdatavisible*);
virtual bool load_module(const hk_string&);
protected:


private:
void error_occured(enum_action action);
bool execute_script(const hk_string& script,enum_action action);
hk_string pystatement(void);
hk_pythoninterpreterprivate* p_privatdata;

static int p_referencecounting;


};


 hk_button* currentbutton(void);
 hk_dslineedit* currentlineedit(void);
 hk_dsmemo* currentmemo(void);
 hk_dsgrid* currentgrid(void);
 hk_dsgridcolumn* currentgridcolumn(void);
 hk_dscombobox* currentcombobox(void);
 hk_dsboolean*  currentboolean(void);
 hk_form* currentform(void);
 hk_report* currentreport(void);
 hk_reportdata* currentreportdata(void);
 hk_dsvisible* currentdsvisible(void);
 hk_visible*  currentvisible(void);
 hk_dsimage* currentdsimage(void);
 hk_dsdate*  currentdsdate(void);
 hk_button *cast_button(hk_visible*);
 hk_dslineedit *cast_dslineedit(hk_visible*);
 hk_dsmemo *cast_dsmemo(hk_visible*);
 hk_dsgrid *cast_dsgrid(hk_visible*);
 hk_dsgridcolumn *cast_dsgridcolumn(hk_visible*);
 hk_dscombobox *cast_dscombobox(hk_visible*);
 hk_dsboolean *cast_dsboolean(hk_visible*);
 hk_dsimage *cast_dsimage(hk_visible*);
 hk_dsdate *cast_dsdate(hk_visible*);
 hk_dsvisible *cast_dsvisible(hk_visible*);
 hk_form *cast_form(hk_visible*);
 hk_report *cast_report(hk_visible*);
 hk_reportdata *cast_reportdata(hk_visible*);
 hk_subform *cast_subform(hk_visible*);

#endif //HK_INTERPRETER

