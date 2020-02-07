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
#include <Python.h>
#include "hk_interpreter.h"
#include "hk_button.h"
#include "hk_dslineedit.h"
#include "hk_dsgrid.h"
#include "hk_dsboolean.h"
#include "hk_dscombobox.h"
#include "hk_dsimage.h"
#include "hk_dsdate.h"
#include "hk_module.h"
#include "hk_visible.h"
#include "hk_dsvisible.h"
#include "hk_presentation.h"
#include "hk_dsgridcolumn.h"
#include "hk_dsmemo.h"
#include "hk_form.h"
#include "hk_subform.h"
#include "hk_report.h"
#include "hk_reportdata.h"


class hk_pythoninterpreterprivate
{
public:
 hk_pythoninterpreterprivate()
 {
 p_while_executing=false;
 }
PyObject* p_globals;
bool p_while_executing;

};



static hk_visible* p_currentobject=NULL;

hk_interpreter::hk_interpreter(hk_presentation* p)
{
	p_presentation=p;
	p_error_occured=false;
	p_error_rownumber=-1;
	p_block_execution=false;
}

void hk_interpreter::set_block_execution(bool b)
{
p_block_execution=b;
}

bool hk_interpreter::block_execution(void) const
{
   return p_block_execution;
}




hk_no_interpreter::hk_no_interpreter(hk_presentation*p):hk_interpreter(p)
{
p_error_occured=true;
warning_already_displayed=false;
p_errormessage=hk_translate("No interpreter installed");
}



bool hk_no_interpreter::before_row_change(hk_dsvisible*)
{
warning();
return false;

}

bool hk_no_interpreter::after_row_change(hk_dsvisible*)
{
warning();
return false;

}

bool hk_no_interpreter::before_update(hk_dsvisible*)
{
warning();
return false;

}
bool hk_no_interpreter::after_update(hk_dsvisible*)
{
warning();
return false;

}

bool hk_no_interpreter::before_delete(hk_dsvisible*)
{
warning();
return false;

}
bool hk_no_interpreter::after_delete(hk_dsvisible*)
{
warning();
return false;

}

bool hk_no_interpreter::on_valuechanged(hk_dsdatavisible*)
{
warning();
return false;

}

bool hk_no_interpreter::before_insert(hk_dsvisible*)
{
warning();
return false;

}
bool hk_no_interpreter::after_insert(hk_dsvisible*)
{
warning();
return false;

}

bool hk_no_interpreter::on_click(hk_visible*)
{
warning();
return false;
}

bool hk_no_interpreter::on_doubleclick(hk_visible*)
{
warning();
return false;
}

bool hk_no_interpreter::on_close(hk_visible*)
{
warning();
return false;
}

bool hk_no_interpreter::on_open(hk_visible*)
{
warning();
return false;
}


bool hk_no_interpreter::on_print_data(hk_reportdata*)
{
warning();
return false;
}



bool hk_no_interpreter::on_print_new_page(hk_report*)
{
warning();
return false;
}

bool hk_no_interpreter::on_getfocus(hk_visible*)
{
warning();
return false;
}

bool hk_no_interpreter::on_loosefocus(hk_visible*)
{
warning();
return false;
}

bool hk_no_interpreter::on_key(hk_visible*)
{
warning();
return false;
}

bool hk_no_interpreter::on_select(hk_dscombobox*)
{
warning();
return false;
}

bool hk_no_interpreter::on_select(hk_dsgridcolumn*)
{
warning();
return false;
}

bool hk_no_interpreter::load_module(const hk_string&)
{
warning();
return false;
}



void hk_no_interpreter::warning(void)
{
  if (warning_already_displayed) return;
  show_warningmessage(hk_translate("No interpreter language installed!")+" "+p_presentation->interpretername());
 warning_already_displayed=true;
}




//********************************************************************************************
//********************************* PYTHON SUPPORT *********************************
//********************************************************************************************




int hk_pythoninterpreter::p_referencecounting=0;

hk_pythoninterpreter::hk_pythoninterpreter(hk_presentation* p):hk_interpreter(p)
{
++p_referencecounting;
p_privatdata= new hk_pythoninterpreterprivate;
if (!Py_IsInitialized())Py_Initialize();
//PyRun_SimpleString((char*)"class _hkstdout:\n def write(self,stuff):\n   hk_class.show_warningmessage(stuff)\nimport sys\nsys.stdout=_hkstdout()\n");
p_privatdata->p_globals=NULL;
init();
}







hk_pythoninterpreter::~hk_pythoninterpreter()
{
--p_referencecounting;
Py_DECREF(p_privatdata->p_globals);
delete p_privatdata;
if (p_referencecounting==0)
  {
//    cerr <<"~hk_pythoninterpreter() Py_Finalize()"<<endl;
   //Py_Finalize();
  }
}




void hk_pythoninterpreter::init(void)
{
PyObject* module=PyImport_AddModule(const_cast<char*>("__main__"));
PyObject* main_namespace=PyModule_GetDict(module);
PyRun_SimpleString(const_cast<char*>("import sys\nfrom hk_classes import *\n"));

if (p_privatdata->p_globals)
	{
	  Py_DECREF(p_privatdata->p_globals);
	  p_privatdata->p_globals=NULL;
	}

p_privatdata->p_globals=PyDict_Copy(main_namespace);

}


bool hk_pythoninterpreter::on_click(hk_visible* visible)
{
if (!visible)return false;
p_currentobject=visible;
return execute_script(visible->on_click_action(),a_click);
}


bool hk_pythoninterpreter::on_doubleclick(hk_visible* visible)
{
if (!visible)return false;
p_currentobject=visible;
return execute_script(visible->on_doubleclick_action(),a_doubleclick);
}


bool hk_pythoninterpreter::on_getfocus(hk_visible* visible)
{
if (!visible)return false;
p_currentobject=visible;
return execute_script(visible->on_getfocus_action(),a_on_getfocus);
}

bool hk_pythoninterpreter::on_loosefocus(hk_visible* visible)
{
if (!visible)return false;
p_currentobject=visible;
return execute_script(visible->on_loosefocus_action(),a_on_loosefocus);
}


bool hk_pythoninterpreter::on_key(hk_visible* visible)
{
if (!visible)return false;
p_currentobject=visible;
return execute_script(visible->on_key_action(),a_on_key);
}


bool hk_pythoninterpreter::before_update(hk_dsvisible* ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->before_update_action(),a_before_update);

}





bool hk_pythoninterpreter::after_update(hk_dsvisible* ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->after_update_action(),a_after_update);

}

bool hk_pythoninterpreter::before_delete(hk_dsvisible* ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->before_delete_action(),a_before_delete);

}

bool hk_pythoninterpreter::after_delete(hk_dsvisible* ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->after_delete_action(),a_after_delete);

}


bool hk_pythoninterpreter::before_insert(hk_dsvisible* ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->before_insert_action(),a_before_insert);

}

bool hk_pythoninterpreter::after_insert(hk_dsvisible* ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->after_insert_action(),a_after_insert);

}






bool hk_pythoninterpreter::before_row_change(hk_dsvisible* ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->before_row_change_action(),a_before_row_change);

}





bool hk_pythoninterpreter::after_row_change(hk_dsvisible* ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->after_row_change_action(),a_after_row_change);

}

bool hk_pythoninterpreter::on_close(hk_visible* ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->on_close_action(),a_on_close);

}

bool hk_pythoninterpreter::on_open(hk_visible*ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->on_open_action(),a_on_open);

}

bool hk_pythoninterpreter::on_valuechanged(hk_dsdatavisible*ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->on_valuechanged_action(),a_on_valuechanged);

}

bool hk_pythoninterpreter::on_print_data(hk_reportdata*ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->on_print_action(),a_on_print_data);

}
bool hk_pythoninterpreter::on_print_new_page(hk_report*ds)
{

if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->on_new_page_action(),a_on_print_new_page);

}


bool hk_pythoninterpreter::on_select(hk_dscombobox* ds)
{
if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->on_select_action(),a_on_select);

}

bool hk_pythoninterpreter::on_select(hk_dsgridcolumn* ds)
{
if (!ds)return false;
p_currentobject=ds;
return execute_script(ds->on_select_action(),a_on_select);

}



bool hk_pythoninterpreter::load_module(const hk_string& module)
{
if (!p_presentation) return false;
hk_string script;
hk_module m;
m.set_database(p_presentation->database());
if (!m.load_module(module)) return false;
script=m.script();
if (script.size()>0) 
  {
   hk_string errormsg=replace_all("%2",hk_translate("Error in line %1 while loading module '%2'\\nError message:'%3'"),module);
   
   script=" "+replace_all("\n",script,"\n "); // indent for the try block;
   script="import sys\nfrom traceback import *\ntry:\n"+script+
   "\nexcept Exception,e:\n _a,_b,_c= sys.exc_info()\n show_warningmessage(replace_all(\"%3\",replace_all(\"%1\",\""+errormsg+"\",str(_c.tb_lineno-3)),str(_b)))";
  }
  
p_currentobject=p_presentation;
return execute_script(script,a_on_loadmodule);
}



hk_string hk_pythoninterpreter::pystatement(void)
{
if (currentlineedit()) return "currentlineedit()";
else
if (currentbutton()) return "currentbutton()";
else
if (currentgrid()) return "currentgrid()";
else
if (currentgridcolumn()) return "currentgridcolumn()";
else
if (currentcombobox()) return "currentcombobox()";
else
if (currentdsimage()) return "currentdsimage()";
else
if (currentdsdate()) return "currentdsdate()";
else
if (currentboolean()) return "currentboolean()";
else
if (currentmemo()) return "currentmemo()";
else
if (currentreportdata()) return "currentreportdata()";
else
if (currentform()) return "currentform()";
else
if (currentreport()) return "currentreport()";
else
if (currentdsvisible()) return "currentdsvisible()";
else

return "currentvisible()";

}


bool hk_pythoninterpreter::execute_script(const hk_string& script, enum_action action)
{
// if (p_privatdata->p_while_executing) return true;
if (block_execution()) return true;
if (p_presentation&&p_presentation->mode()==hk_presentation::designmode) return true;
p_error_occured=false;
if (script.size()==0) return true;
//cerr <<"action: "<<action<<"  script: "<<script<<endl<<endl;
p_privatdata->p_while_executing=true;
hk_string hk="hk_this="+pystatement();
PyObject*result=PyRun_String(const_cast<char*>(hk.c_str()),Py_file_input,p_privatdata->p_globals,p_privatdata->p_globals);
hk_string errormsg=hk_translate("\nThis is due to an installation error. If you did install hk_classes from and .rpm or .deb package you should check if there is also a hk_classes-python package");
if (!result)
   {
      show_warningmessage("error while loading hk_this: "+hk+errormsg);
      error_occured(action);
     return false;
   }
   hk_form* f=dynamic_cast<hk_form*>(p_presentation);
   if (f)
   {
     hk="hk_thisform=cast_form(hk_this.presentation())";
    result=PyRun_String(const_cast<char*>(hk.c_str()),Py_file_input,p_privatdata->p_globals,p_privatdata->p_globals);
    if (!result)
     {
      show_warningmessage("error while loading hk_thisform: "+hk+errormsg);
      error_occured(action);
     return false;
     }
   }
   else
   {
    hk_report* r=dynamic_cast<hk_report*>(p_presentation);
    if (r)
    {
      hk="hk_thisreport=cast_report(hk_this.presentation())";
      result=PyRun_String(const_cast<char*>(hk.c_str()),Py_file_input,p_privatdata->p_globals,p_privatdata->p_globals);
      if (!result)
       {
        show_warningmessage("error while loading hk_thisreport: "+hk+errormsg);
        p_privatdata->p_while_executing=false;
        return false;
       }
   }
  }
  hk_string sc=script+"\n";
result=PyRun_String(const_cast<char*>(sc.c_str()),Py_file_input,p_privatdata->p_globals,p_privatdata->p_globals);
if (!result)
{ //an error occured
   error_occured(action);
   return false;
}
Py_XDECREF(result);
p_privatdata->p_while_executing=false;
return true;


}



void hk_pythoninterpreter::error_occured(enum_action action)
{
p_error_occured=true;
p_privatdata->p_while_executing=false;

PyObject* type,*value,*traceback;
  PyErr_Fetch(&type,&value,&traceback);
  PyErr_NormalizeException(&type,&value,&traceback);

//checking for line number of error
PyObject* ln=NULL;
if (traceback)
   {
    ln=PyObject_GetAttrString(traceback,const_cast<char*>("tb_lineno"));
   }
   else
   {
     ln=PyObject_GetAttrString(value,const_cast<char*>("lineno"));cerr <<"no traceback object"<<endl;
   }

int linenumber=-1;
if (ln)
  {
  linenumber=PyInt_AsLong(ln);
  Py_DECREF(ln);
  }
//now checking of error message


  ln=PyObject_Str(value);

  hk_string errormsg="UNKNOWN ERROR";
char* msg=NULL;
if (ln)
  {
  msg=PyString_AsString(ln);
   if (msg)   errormsg=msg;
  Py_DECREF(ln);
  }

/*PyErr_Restore(type,value,traceback);
PyErr_Print();*/

Py_XDECREF(type);
Py_XDECREF(value);
Py_XDECREF(traceback);
p_error_rownumber=linenumber;
p_errormessage=errormsg;
if (p_presentation) p_presentation->script_error(p_currentobject,action);
p_error_occured=false;
}







hk_button* currentbutton(void)
{
 return cast_button(p_currentobject);
}

hk_dslineedit* currentlineedit(void)
{
 return cast_dslineedit(p_currentobject);
}

hk_dsvisible* currentdsvisible(void)
{
  return cast_dsvisible(p_currentobject);
}

hk_visible* currentvisible(void)
{
  return p_currentobject;
}

 hk_dsgrid* currentgrid(void)
{
  return cast_dsgrid(p_currentobject);
}
 hk_dsgridcolumn* currentgridcolumn(void)
{
  return cast_dsgridcolumn(p_currentobject);
}


hk_dsimage* currentdsimage(void)
{
 return cast_dsimage(p_currentobject);
}

hk_dsdate* currentdsdate(void)
{
 return cast_dsdate(p_currentobject);
}

 hk_dscombobox* currentcombobox(void)
{
  return cast_dscombobox (p_currentobject);
}
 hk_dsboolean*  currentboolean(void)
{
  return cast_dsboolean(p_currentobject);
}

 hk_dsmemo*  currentmemo(void)
{
  return cast_dsmemo(p_currentobject);
}

 hk_form* currentform(void)
 {
 return cast_form(p_currentobject);
 }

 hk_report* currentreport(void)
 {
 return cast_report(p_currentobject);

 }

 hk_reportdata* currentreportdata(void)
 {
 return cast_reportdata(p_currentobject);

 }



 hk_button *cast_button(hk_visible* v)
 {
  return dynamic_cast<hk_button*>(v);
 }
 hk_dslineedit *cast_dslineedit(hk_visible*v)
 {
  return dynamic_cast<hk_dslineedit*>(v);
 }

 hk_dsmemo *cast_dsmemo(hk_visible*v)
 {
  return dynamic_cast<hk_dsmemo*>(v);
 }

 hk_dsgrid *cast_dsgrid(hk_visible*v)
 {
  return dynamic_cast<hk_dsgrid*>(v);
 }

 hk_dsgridcolumn *cast_dsgridcolumn(hk_visible*v)
 {
  return dynamic_cast<hk_dsgridcolumn*>(v);
 }


hk_dscombobox *cast_dscombobox(hk_visible*v)
 {
  return dynamic_cast<hk_dscombobox*>(v);
 }

 hk_dsboolean *cast_dsboolean(hk_visible*v)
 {
  return dynamic_cast<hk_dsboolean*>(v);
 }

 hk_dsvisible *cast_dsvisible(hk_visible*v)
 {
  return dynamic_cast<hk_dsvisible*>(v);
 }

 hk_form *cast_form(hk_visible*v)
 {
  return dynamic_cast<hk_form*>(v);
 }
hk_report *cast_report(hk_visible*v)
{
  return dynamic_cast<hk_report*>(v);
 }


hk_reportdata *cast_reportdata(hk_visible*v)
{
  return dynamic_cast<hk_reportdata*>(v);
 }

hk_subform *cast_subform(hk_visible*v)
{
  return dynamic_cast<hk_subform*>(v);
 }


hk_dsdate *cast_dsdate(hk_visible*v)
{
  return dynamic_cast<hk_dsdate*>(v);
 }


hk_dsimage *cast_dsimage(hk_visible*v)
{
  return dynamic_cast<hk_dsimage*>(v);
 }




