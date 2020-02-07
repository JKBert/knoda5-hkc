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
#ifndef HK_VISIBLE
#define HK_VISIBLE
#include "hk_definitions.h"
#include "hk_class.h"
#include "hk_colour.h"
#include "hk_font.h"
#include "hk_key.h"

using namespace std;

class hk_presentation;
class hk_form;
class hk_report;
class hk_visiblemodeprivate;
class hk_visibleprivate;
class hk_tabvisible;
/**
 *
 *@short base class for visible widgets
 *@version $Revision: 1.70 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *base class for visible widgets.
 */
class hk_visible:public hk_class

{
    friend class hk_presentation;
    friend class hk_form;
    friend class hk_reportdata;
    friend class hk_tabvisible;
    public:
        hk_visible(hk_presentation* p=NULL);
        virtual     ~hk_visible(void);
/**
 * see @ref set_size
 */
        void        set_x(unsigned int px, bool registerchange=true);
/**
 * see @ref set_size
 */
        void        set_y(unsigned int py, bool registerchange=true);
/**
 * see @ref set_size
 */
        void        set_width(unsigned int w, bool registerchange=true);
/**
 * see @ref set_size
 */
        void        set_height(unsigned int h, bool registerchange=true);
        unsigned int    x(void) const;
        unsigned int    y(void) const;
        unsigned int    width(void)const;
        unsigned int    height(void) ;
/**
 *sets the size and the position of the object.  In forms the coordinates are in a range from 0 to 10000 (100.00%)
     */
        void        set_size(unsigned int x, unsigned int y,unsigned int w, unsigned int h, bool registerchange=true);
        void        set_size(unsigned int x, unsigned int y,unsigned int w, unsigned int h, bool registerchange,bool force_setting);
        void        set_size(unsigned int width, unsigned int height, bool registerchange=true);
        void        set_position(unsigned int x, unsigned int y, bool registerchange=true);
/**
 * Every visible object has a (static) label which will be displayed when necessary, i.e. a combobox usually needs
 * a label
 */
        void        set_label(const hk_string& l, bool registerchange=true);
        void        set_label(const hk_string& l, bool registerchange,bool force_setting);
        hk_string       label(void);
/**
*the identifier is a unique name within a presentation (either a form or a report) with which this object can be identified and
*thus located
*/
	bool set_identifier(const hk_string&,bool registerchange=true,enum_interaction=interactive);
	hk_string identifier(void) const;


	enum enum_visibletype {textlabel,button,rowselector,boolean,lineedit,memo,combobox,grid,form,report,reportsection,reportdata,query,subform,image,date,tabvisible,other};
        enum_visibletype  type(void) {return p_visibletype;}
/**
 *sets the font name and size to be used of some reportgenerators
 */
        void       set_font(const hk_string& font,int size,bool registerchange=true);
        void       set_font(const hk_string& font,int size,bool registerchange,bool force_setting);
        void       set_font(const hk_font& font,bool registerchange=true);
        void       set_font(const hk_font& font,bool registerchange,bool force_setting);

        virtual hk_font     font(void);
/**
 *sets the foreground colour. This colour will be used for fonts, frames etc
 */
        void   set_foregroundcolour(const hk_colour& colour,bool registerchange=true);
        void   set_foregroundcolour(const hk_colour& colour,bool registerchange,bool force_setting);
        hk_colour foregroundcolour(void);
/**
 *sets the background colour. This colour will be used to fill the whole background
 */
        void   set_backgroundcolour(const hk_colour& colour,bool registerchange=true);
        void   set_backgroundcolour(const hk_colour& colour,bool registerchange,bool force_setting);
        hk_colour backgroundcolour(void);

/**
 * if a hk_visible object is part of a @ref hk_presentation (i.e. a form) and the size or the label
 * changes the presentation will be informed of this change (and will be stored when closed).
 * If you set n to true the presentation will not be informed
 */
        void set_neverregisterchange(bool n);
        void set_neverregisterchange(bool n,bool force_setting);
        bool neverregisterchange(void);
        virtual void savedata(ostream& s );
        virtual void loaddata(xmlNodePtr definition);
/**
* returns the unique identification number when this object is part of a form or a report
*/
        long  presentationnumber(void) const;
        void set_datetimeformat(const hk_string& datetime,const hk_string& date, const hk_string&  time);
        void set_datetimeformat(const hk_string& datetime,const hk_string& date, const hk_string&  time,bool force_setting);
        hk_string datetimeformat(void);
        hk_string dateformat(void);
        hk_string timeformat(void);
        void set_counts_as(unsigned long int c);
        virtual void set_counts_as(unsigned long int c,bool force_setting);
        virtual unsigned long int counts_as(void);
        virtual hk_string replace(const hk_string& where);
        hk_presentation* presentation(void);

/**
 * only interesting with @ref loaddata is used to copy a visible object in a form or a report.
 *@param l if true the presentationnumber will be loaded (default)
 */
        void set_load_presentationnumber(bool l);
        void set_load_presentationnumber(bool l,bool force_setting);
        bool load_presentationnumber(void);
/**
 *Defines whether the value should be aligned left, right or center.
 */
        enum alignmenttype{alignleft,alignright,aligncenter,aligndefault};
/**
*sets the text alignment
*/
        void set_alignment(alignmenttype a,bool registerchange=true);
        void set_alignment(alignmenttype a,bool registerchange,bool force_setting);
        alignmenttype alignment(void);

/**
 *Defines how text (text are all datatypes except the number types) should be aligned.
 *See also @ref set_defaultnumberalignment
 */
        static void set_defaulttextalignment(alignmenttype a);
        static alignmenttype defaulttextalignment(void);
/**
 *Defines how numbers should be aligned. See also @ref set_defaulttextalignment.
 *Numbers are als integer and realtypes and also auto_inc.
 */
        static void set_defaultnumberalignment(alignmenttype a);
        static alignmenttype defaultnumberalignment(void);
/**
 *defines whether stand alone windows should open maximized or in defined window size
 */
        static void set_open_maximized_windows(bool m);
        static bool open_maximized_windows(void);


/**
 *sets a little help text that will be shown when the mouse moves over the widget
 */
void set_tooltip(const hk_string&,bool registerchange=true,bool force_setting=false);
/**
 *@return the tooltip text
 */
hk_string tooltip(void);



virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);



/**
 * if the widget is part of a presentation this widget will be moved into the background
 */
        virtual void lower_widget(bool registerchange=true);
/**
 * if the widget is part of a presentation this widget will be moved into the front
 */
        virtual void raise_widget(bool registerchange=true);

/**
* sets the script for the on_click action
*/
	void set_on_click_action(const hk_string&,bool registerchange=true,bool force_setting=true);
/**
* @return the script for the on_click action
*/
	hk_string on_click_action(void);
/**
* sets the script for the on_doubleclick action
*/
	void set_on_doubleclick_action(const hk_string&,bool registerchange=true,bool force_setting=true);
/**
* @return the script for the on_doubleclick action
*/
	hk_string on_doubleclick_action(void);
/**
* sets the script for the on_open action
* The on_open action will be executed when the parent form or report will be set into viewmode
*/

	void set_on_open_action(const hk_string&,bool registerchange=true,bool force_setting=true);
/**
* @return the script for the on_open action
*/
	hk_string on_open_action(void);
/**
* sets the script for the on_close action
* The on_close action will be executed when the parent form or report will be set into designmode
*/
	void set_on_close_action(const hk_string&,bool registerchange=true,bool force_setting=true);
/**
* @return the script for the on_close action
*/
	hk_string on_close_action(void);

/**
* sets the script for the on_getfocus action
* The on_getfocus action will be executed when this widget gets the keyboard focus
*/

void set_on_getfocus_action(const hk_string&,bool registerchange=true,bool force_setting=true);
/**
* @return the script for the on_getfocus action
*/

hk_string on_getfocus_action(void);

/**
* sets the script for the on_loosefocus action
* The on_loosefocus action will be executed when this widget looses the keyboard focus
*/

void set_on_loosefocus_action(const hk_string&,bool registerchange=true,bool force_setting=true);
/**
* @return the script for the on_getfocus action
*/

hk_string on_loosefocus_action(void);


void set_on_key_action(const hk_string&,bool registerchange=true,bool force_setting=true);
/**
* @return the script for the on_key action
*/

hk_string on_key_action(void);
void set_key(const hk_key&);
/**
*returns the currently pressed key. See @ref hk_key for details.
*/
hk_key* key() const;

void set_enabled(bool enabled, bool registerchange=true,bool force_setting=false);
bool is_enabled();



// script action called when this visible object is clicked
virtual	bool action_on_click(void);
virtual	bool action_on_doubleclick(void);
virtual bool action_on_close(void);
virtual bool action_on_open(void);
virtual bool action_on_getfocus(void);
virtual bool action_on_loosefocus(void);
virtual bool action_on_key(void);
 bool in_tabvisible(void) const;
void set_buddylabel(long int labelid,bool registerchange=true,bool force_setting=false);
long int buddylabel(void) const;
void set_movebuddylabel(bool);
bool move_buddylabel(void) const;


int tag_number(const hk_string&);
virtual hk_string tag_value(int tagnumber, bool& ok);
virtual hk_string column_value(const hk_string& colname,bool& ok);
hk_string tag_value(const hk_string&, bool& ok);
int register_tag(const hk_string&);

    protected:
        virtual void        widget_specific_form_resizes(void);
        virtual void        widget_specific_font_changed(void);
        virtual void        widget_specific_label_changed(void);
        virtual void        widget_specific_foregroundcolour_changed(const hk_colour& oldcolour);
        virtual void        widget_specific_backgroundcolour_changed(const hk_colour& oldcolour);
        virtual bool        widget_specific_coordinates(unsigned int px,unsigned int py,unsigned int pwidth,unsigned int pheight);
        virtual void        widget_specific_raise_widget(void);
        virtual void        widget_specific_lower_widget(void);
	virtual void        widget_specific_tooltip_changed(void);
	virtual void        widget_specific_enabled_changed(void);
	virtual bool	presentationmode_changed(void);
	virtual void    before_presentationmode_changed(void);
        virtual void widget_specific_alignment(void);
	virtual void sizetype_changed(void);
	virtual void        widget_specific_sizetype_changed(void);

        bool        allow_datachanging(bool force_setting);
	enum enum_has_changed{standard,forcesetting};
        void        has_changed(bool registerchange,enum_has_changed force_setting=standard);
        hk_presentation*    p_presentation;
        enum_visibletype p_visibletype;
        bool        p_setcoordinates;
        bool        p_setwidgetcoordinates;
        bool        while_init;
        void set_tabvisible(hk_tabvisible*);
    private:
        static alignmenttype p_defaulttextalign;
        static alignmenttype p_defaultnumberalign;
        static  bool        p_open_maximized_windows;

	hk_visiblemodeprivate* p_designdata;
	hk_visiblemodeprivate* p_viewdata;
	hk_visibleprivate* p_private;
        long int    p_vupn;                       //visual unique presentation number

};
#endif
