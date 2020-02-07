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
#ifndef HK_BUTTON_H
#define HK_BUTTON_H
#include "hk_dsvisible.h"
#include <list>
using namespace std;
class hk_database;
class hk_buttonprivate;
class hk_buttonmodeprivate;
/**
 *
 *@short base class for a button in a form
 *@version $Revision: 1.26 $
 *@author Horst Knorr (hk_classes@knoda.org)
 */
class hk_button :public hk_dsvisible
{
    public:
        hk_button(hk_form* form=NULL);
        virtual ~hk_button(void);
/**
 * when the button is pushed the action will be executed on the object
 *
 *@param action allowed actions:<pre>
   open_form,
   close_form,
   open_table,
   open_query,
   preview_report,
   print_report,
   goto_firstrow,
goto_lastrow,
goto_nextrow,
goto_previousrow,
insert_row,
delete_row,
store_row,
action_query
</pre>
*@param object i.e. a form name (if you want to open a form)
*@param showmaximized if true show the window maximized else show the new window in its origin or default size
*@param registerchange if this button is part of a form and any registerchange is true, the changes will be stored when you close the window
*
*Example: to open the form "test" use
<pre>
set_action("open_form","test");
push_action();
push_action();
</pre>
*/
        void set_action(const hk_string& action,const hk_string& object,bool showmaximized=false,bool registerchange=true);
        void set_action(long action,const hk_string& object,bool showmaximized=false,bool registerchange=true);
virtual bool push_action(void);
        hk_string object (void);
/**
 * @return -1 no action set;
<pre>
   0 open_form;
   1 close_form;
   2 open_table;
   3 open_query;
   4 preview_report;
   5 print_report;
   6 goto_firstrow,
   7 goto_lastrow,
8 goto_nextrow,
9 goto_previousrow,
10 insert_row,
11 delete_row,
12 store_row,
13 quiet_query,
14 close_application,
15 open_view,
</pre>
*/
        int action(void);
        bool show_maximized(void);
        void set_database(hk_database* database);
        hk_database* database(void);

        virtual void loaddata(xmlNodePtr definition);
        virtual void  savedata(ostream& s );

        typedef class
        {
            public:
                hk_string reportfield;
                hk_string condition;
                hk_string formdatasource;
                hk_string formfield;
        } struct_condition;

        void add_condition(const hk_string& reportfield,const hk_string& condition,const hk_string& formdatasource,const hk_string& formfield,bool registerchange=true);
        list<struct_condition>* conditions(void);
        void clear_conditions(bool registerchange=true);

virtual bool load_icon(const hk_string& filename,bool registerchange=true);
struct_raw_data* icon();
virtual void show_icon(){}
virtual void set_is_togglebutton(bool,bool registerchange=true,bool forcesetting=false);
        bool is_togglebutton(void) const;
virtual bool is_pushed() const;
virtual void set_is_pushed(bool,bool registerchange=true,bool forcesetting=false);


    protected:
        virtual bool    widget_specific_open_form(void);
        virtual bool    widget_specific_close_form(void);
        virtual bool    widget_specific_open_table(void);
        virtual bool    widget_specific_open_query(void);
        virtual bool    widget_specific_open_view(void);
        virtual bool    widget_specific_preview_report(void);
        virtual bool    widget_specific_print_report(void);
        virtual bool    widget_specific_close_application(void);
        virtual bool    widget_specific_is_pushed(void);
        hk_string parsed_condition(hk_presentation*);

    private:
        long int p_action;
        hk_string p_object;
        bool p_showmaximized;
        hk_database* p_database;
        list<struct_condition> p_reportconditions;
	hk_buttonmodeprivate* p_designdata;
	hk_buttonmodeprivate* p_viewdata;
	hk_buttonprivate* p_private;

};
#endif
