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
#ifndef HK_PRESENTATION
#define HK_PRESENTATION
#include <list>
#include <exception>
//#include <fstream.h>
#include "hk_dsmodevisible.h"
using namespace std;

#include "hk_datasource.h"
#include "hk_interpreter.h"
class hk_database;
class hk_form;
class hk_presentationprivate;

/**
 *
 *@short  base class for multidatasource based classes like forms and reports
 *@version $Revision: 1.56 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *@internal
 *hk_presentation is the base class for multidatasource based classes like forms and reports. A presentation can
 *be designed when @ref mode is 'design' or the data will be presented when mode is 'view'.
 *
In 'design' mode you can create new datasource by calling the function @ref new_datasource. Each datasource get's
*a unique number (see @ref hk_datasource::presentationnumber) with which you can identifiy each datasource.
*All used datasources will be automatically enabled when the presentation is in 'view'-mode.
*/

class hk_presentation   :public hk_dsmodevisible
{
    friend class hk_datasource;
    friend class hk_database;
    friend class hk_dsvisible;
    friend class hk_visible;
    public:
        hk_presentation(void);
        virtual ~hk_presentation(void);
/**
 *set the database which is base of this presentation
 */
        void            set_database(hk_database* db);
        hk_database*        database(void) const;

        enum            enum_presentationtype {general,form,report,qbe,referentialintegrity};
        enum_presentationtype   presentationtype(void)const {return p_presentationtype;}
/**
 * name of the presentation under which it will be stored
 */
        void            set_name(const hk_string& n);
        hk_string       name(void) const;
       virtual bool            set_mode(enum_mode s);

/**
 *a list of all existing datasources in this presentation
 */
        list<hk_datasource*>*   datasources(void) const;
/**
 * loads an existing datasource "name". if query=false this datasource is a table else a query.
 * @return each presentationdatasource gets a unique number as an identifier.
 */
        long        new_datasource(const hk_string& name,datasourcetype dt=dt_table);
/**
 * returns the datasource with the unique identification number nr
 */
        hk_datasource*  get_datasource(long nr);
        hk_datasource*  get_datasource(const hk_string& description);
        hk_datasource*  get_datasource_by_shortname(const hk_string& description);
/**
returns a unique name for a datasource
looks like : nameofdatasource (uniquenumber) [dependingondatasource] | filter
*/
        hk_string   unique_datasourcename(long nr);
        hk_string   unique_shortdatasourcename(long nr);
        virtual void                savedata(ostream& s);
        virtual void                loaddata(xmlNodePtr definition);
/**
 *resets the internal datasourcelist and deletes all datasourceobjects except of "nodelete"
 */
        void clear_datasourcelist(void);

/**
 * sets the logic size of the form.
 */
        void            set_designsize(unsigned int width, unsigned int height,bool registerchange=true);
        virtual void            set_presentationsize(unsigned int,unsigned int);
        unsigned int    designwidth(void);
        unsigned int    designheight(void);

        unsigned int        horizontal2relativ(unsigned int h);
        unsigned int        vertical2relativ(unsigned int v);
        unsigned int        relativ2horizontal(unsigned int r);
        unsigned int        relativ2vertical(unsigned int r);
        virtual void register_object(hk_visible*);
/**
 * if true (default) all datasources will be automatically enabled when mode changes to viewmode
 * else you have to enable the datasources manually
 */
        void set_automatic_enable_datasources(bool);
        bool automatic_enable_datasources(void);
        enum enum_bulkoperation {bulkfont,bulkforeground,bulkbackground};
        virtual void bulk_operation(enum_bulkoperation bulkoperation);
	enum enum_sizetype {relative,absolute};
	void set_sizetype(enum_sizetype type,bool registerchange=true);
	enum_sizetype sizetype(void);
	static void set_defaultsizetype(enum_sizetype type);
	static enum_sizetype defaultsizetype(void);
	static void set_snap2gridx(int);
	static int snap2gridx(void) ;
	static void set_snap2gridy(int);
	static int snap2gridy(void) ;

	hk_interpreter*  interpreter(void);

	virtual void script_error(hk_visible*, hk_interpreter::enum_action){}
	void set_interpretername(const hk_string&,bool registerchange=true);
	hk_string interpretername(void) const;
        void enable_datasources(bool enable,hk_datasource::enum_accessmode mode=hk_datasource::standard);
        virtual bool is_subpresentation(void);
        virtual bool while_loading(void){return false;}
        virtual bool intervaltasks(void) {return true;}
        
        bool load_module(const hk_string& modulename);

    protected:
        void    remove_datasource(long nr, bool registerchange=true);
        virtual void    widget_specific_presentationresize(void){}
        virtual void    widget_specific_fieldresize(hk_visible* ){}
        virtual void widget_specific_set_name(void){}
        enum_presentationtype p_presentationtype;
        void        database_delete(void);
//will be called from database_delete()
        virtual void    database_has_vanished(void);
        void before_source_vanishes(void);
        bool ask_name(void);
        virtual void raise_widget(hk_visible*){}
        virtual void lower_widget(hk_visible*){}

        unsigned int p_actualwidth;
        unsigned int p_actualheight;
        bool	     p_while_errorhandling;
	virtual void sizetype_changed(void);

    private:
        void remove_datasource( hk_datasource* d,bool registerchange);
        long add_datasource(hk_datasource* d);
	hk_presentationprivate* p_private;
	static enum_sizetype p_defaultsizetype;
	static int p_snap_to_gridx;
	static int p_snap_to_gridy;


};
#endif
