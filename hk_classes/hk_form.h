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
#ifndef HK_FORM_H
#define HK_FORM_H
#include "hk_presentation.h"
#include <list>
using namespace std;

class hk_dslineedit;
class hk_dsgrid;
class hk_dsboolean;
class hk_dsrowselector;
class hk_dscombobox;
class hk_dsmemo;
class hk_button;
class hk_label;
class hk_dsimage;
class hk_dsdate;
class hk_tabvisible;
class hk_formprivate;
class hk_subform;
class hk_formmodeprivate;
/**
 *
 *@short  base class for forms
 *@version $Revision: 1.53 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 */

/**
 * base class of a form.To use it set a database with set_database() first. To  create a  form it has to be in
 *mode "design".
 */
class hk_form:public hk_presentation
{
    friend class hk_dsvisible;
    friend class hk_visible;
    friend class hk_subform;

    public:

        hk_form(void);
        virtual ~hk_form(void);
/**
 * the form has to be in design mode to use this function! The function will create a new
 * grid in the upper left corner.
 *@return the new created object if successful else NULL
 */
        hk_dsgrid*      new_grid(void);
/**
 * the form has to be in design mode to use this function! The function will create a new
 * lineeidt field in the upper left corner.
 *@return the new created object if successful else NULL
 */
        hk_dslineedit*      new_lineedit(void);
/**
 * the form has to be in design mode to use this function! The function will create a new
 * bool field in the upper left corner.
 *@return the new created object if successful else NULL
 */
        hk_dsboolean*       new_bool(void);
/**
 * the form has to be in design mode to use this function! The function will create a new
 * combobox in the upper left corner.
 *@return the new created object if successful else NULL
 */
        hk_dscombobox*      new_combobox(void);
/**
 * the form has to be in design mode to use this function! The function will create a new
 * button in the upper left corner.
 *@return the new created object if successful else NULL
 */
        hk_button*      new_button(void);
/**
 * the form has to be in design mode to use this function! The function will create a new
 * row selector in the upper left corner.
 *@return the new created object if successful else NULL
 */
        hk_dsrowselector*   new_rowselector(void);
/**
 * the form has to be in design mode to use this function! The function will create a new
 * memofield in the upper left corner.
 *@return the new created object if successful else NULL
 */
        hk_dsmemo*      new_memo(void);
/**
 * the form has to be in design mode to use this function! The function will create a new
 * textlabel in the upper left corner.
 *@return the new created object if successful else NULL
 */
        hk_label*       new_label(void);

        hk_subform*     new_subform(void);
        hk_dsdate*      new_date(void);
        hk_dsimage*     new_image(void);
        hk_tabvisible*  new_tabvisible(void);

/**
 *save the form to a file
 *@param name the name of the file
 *@param ask  if true and the form already exists you will be asked before the old form will be overwritten
 *@return true if successful else false
 */
        bool            save_form(const hk_string& name="",bool ask=true);
/**
 *load the form from a file
 *@param name the name of the file
 *
 */
        bool            load_form(const hk_string& name="");
        virtual void savedata(ostream& s);
        virtual void loaddata(xmlNodePtr definition);

/**
 * sets the real size of the form. If you designed a form in i.e. 800x600 resolution and want to watch it
 * with 1024x768 resolution, set this to 1024x768. Dynamic sized visible objects will be resized. All objects
 * will be positioned new.
 */
        virtual void            set_designsize(unsigned int width, unsigned int height,bool registerchange=true);
        hk_visible* get_visible(long nr);
        hk_visible* get_visible(const hk_string& identifier);
        virtual void bulk_operation(enum_bulkoperation bulkoperation);
        virtual bool            set_mode(enum_mode s);
/**
*if this is a subform it will return master form it belongs to
*/
	hk_form*    masterform(void) const;
/**
 *resets the internal visible objects list and deletes all visible objects
 */
        void clear_visiblelist(void);

/**
*sets the tab order (the focus order) of the widgets.
@param t list of the presentation numbers of the widgets, which should get the focus
*/	void set_taborder(list<int> t,bool registerchange=true, bool forcesetting=true);
	list<int> taborder() const;
	hk_visible* taborder_previous(hk_visible*);
	hk_visible* taborder_next(hk_visible*);
	hk_visible* first_tabobject();
	hk_visible* last_tabobject();
/**
*will move the the focus to the next widget (see @ref set_taborder )
*/
virtual	void goto_taborder_next(){}
/**
*will move the the focus to the previous widget (see @ref set_taborder )
*/
virtual	void goto_taborder_previous(){}
/**
*will move the the focus to the first widget (see @ref set_taborder )
*/
virtual void goto_taborder_first(){}
/**
*will move the the focus to the last widget (see @ref set_taborder )
*/
virtual void goto_taborder_last(){}
/**
*will set the focus to the visible object
*/
virtual void set_focus(hk_visible*){}

        list<hk_visible*>*  visibles(void);
	bool is_subform() const;
virtual bool is_subpresentation(void);
virtual bool show_asdialog(hk_form* parentform=NULL);

virtual int screen_width();
virtual int screen_height();
bool while_load_form(void) const;
virtual bool while_loading(void);

enum  enum_include {include_data,no_data};
hk_string  form2html(enum_include i=no_data);
    
    protected:
        bool set_formbasedfilter();
        void set_masterform(hk_form*);
        virtual hk_dsgrid*      widget_specific_new_grid(void){return NULL;}
        virtual hk_dsrowselector*   widget_specific_new_rowselector(void){return NULL;}
        virtual hk_dslineedit*      widget_specific_new_lineedit(void){return NULL;}
        virtual hk_dsboolean*       widget_specific_new_bool(void){return NULL;}
        virtual hk_button*      widget_specific_new_button(void){return NULL;}
        virtual hk_dscombobox*      widget_specific_new_combobox(void){return NULL;}
        virtual hk_dsmemo*      widget_specific_new_memo(void){return NULL;}
        virtual hk_label*       widget_specific_new_label(void){return NULL;}
        virtual hk_subform*       widget_specific_new_subform(void){return NULL;}
        virtual hk_dsdate*       widget_specific_new_date(void){return NULL;}
        virtual hk_dsimage*      widget_specific_new_image(void){return NULL;}
        virtual hk_tabvisible*   widget_specific_new_tabvisible(void){return NULL;}
        virtual void            widget_specific_fieldresize(hk_visible* v);
        virtual void            widget_specific_after_loadform(void){}
        void remove_visible(hk_visible* v);
        hk_visible* new_object(const hk_string& name);
        void raise_widget(hk_visible*);
        void lower_widget(hk_visible*);
	virtual void sizetype_changed(void);
	void set_subformobject(hk_subform*);
	hk_subform* subformobject() const;
    private:
        void add_visible(hk_visible* v);

	hk_formprivate* p_private;
        hk_formmodeprivate* p_designdata;
	hk_formmodeprivate* p_viewdata;

};
#endif
