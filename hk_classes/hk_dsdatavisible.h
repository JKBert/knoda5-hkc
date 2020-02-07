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
#ifndef HK_DSDATAVISIBLE
#define HK_DSDATAVISIBLE
#include "hk_dsvisible.h"
#include <vector>
using namespace std;
class hk_column;
class hk_form;
class hk_dsdatavisiblemodeprivate;
class hk_dsdatavisibleprivate;



/**
 *
 *@short base class for visible widgets with data connection
 *@version $Revision: 1.50 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *This is the base class for visible widgets that provides
 * access to a specific column and its data. To use visible
 * objects you have to define the datasource and the column.
 */
class hk_dsdatavisible: public hk_dsvisible
{
    friend class hk_column;
    public:
        hk_dsdatavisible(hk_presentation* p=NULL);
        virtual ~hk_dsdatavisible();
/**
 *column name in the @ref datasource. If there is no column
 *with this name the widget will not be enabled
 *@param column the wished column
 *@param registerchange: If this object will be displayed on a form you can decide whether changing the column name
 *should mark the form as changed (then it will be saved when closed). Default is "true".
 */
        void set_columnname(const hk_string& c,bool registerchange=true);
        void set_columnname(const hk_string& c, bool registerchange, int coloccurance);
/**
 *@return NULL if there is no column with the defined name.
 *An object of type hk_column if it was found in the datasource
 */
        hk_column* column(void);
        hk_string columnname(void) const;
/**
 * the datasource
 */
        virtual void set_datasource(hk_datasource* d);
        hk_datasource* datasource(void);
        virtual void savedata(ostream& s );
        virtual void loaddata(xmlNodePtr definition);
/**
 *true if the default value will be used while inserting a new row
 */
        bool    use_defaultvalue(void);
/**
 *sets the default value
 *@param def the new default value
 *@param registerchange if true and this widget is part of a form the new default value
 *will be stored when closing the form.
 *<pre>The following variables are available:
 * %NOW% the actual date, time or datetime, depending on the fieldtype. Default ist datetime
 * %NOWDATE% the actual date
 * %NOWTIME% the actual time
 * %TRUE% the driver specific true value.
 * %FALSE% the driver specific false value.
 *</pre>
 */
        void    set_defaultvalue(const hk_string& def,bool registerchange=true);
/**
 * @return the default value as set with @ref set_defaultvalue
 */
        hk_string raw_defaultvalue(void);
/**
 *@return the default value, but variables are replaced by its values
 */
        hk_string defaultvalue(void);
/**
 * resets the default value. After that no default value will be used
 */
        void    reset_default(bool registerchange=true);

        void    set_numberformat(bool use_numberseparator=false, int commadigits=-1,bool registerchange=true);
        void    set_numberformat(bool use_numberseparator, int commadigits,bool registerchange,bool force_setting);
        bool    use_numberseparator(void);
/**
 *@deprecated use @ref precision() instead
 */
        int     commadigits(void);
        int precision(void);
        static void set_defaultnumberformat(bool use_thousandsseparator,int precision);
        static bool defaultuse_numberseparator(void);
        static int defaultprecision(void);
/**
*returns the formatted value of the row 'row'
*/
        virtual hk_string value_at(unsigned long row);
	virtual hk_string value(void);
/**
*sets the display value. If this object has an existing column() the data will be changed
*/
	virtual void set_value(const hk_string&);

        unsigned int find(unsigned int from,unsigned int to,const hk_string& searchtext,bool wholephrase=false,bool casesensitive=false,bool backwards=false);
        unsigned int find(const hk_string& searchtext,bool wholephrase=false,bool casesensitive=false,bool backwards=false);

	void set_on_valuechanged_action(const hk_string&,bool registerchange=true,bool force_setting=true);
	hk_string on_valuechanged_action(void);
        virtual bool action_on_valuechanged(void);
    
    virtual hk_string tag_value(int tagnumber, bool& ok);
bool set_filtervalue(const hk_string&);
 hk_string filtervalue(bool withcolumnnames=false) const;

	static bool parsed_value(hk_string& v,hk_connection* c);
        virtual void update_filter(void){}
    protected:
        /**
        *used from eg. hk_kdememo to allow to signal the datasource that the data has changed without writing it with hk_column::set_asstring
        */
        void set_data_has_changed(void);
        virtual bool datasource_disable(void);
        virtual bool datasource_enable(void);
        virtual void datasource_delete(void);
        virtual bool before_columns_deleted(void);
        virtual void widget_specific_set_column(void);
        virtual void widget_specific_numberformat(void);
        virtual void widget_specific_set_defaultvalue(void){}
        virtual void columndata_has_changed(void){}
        virtual void before_insert_row(void);
        virtual void new_column_pointer_created(void);
	virtual bool	presentationmode_changed(void);
        hk_column* p_column;
        
        hk_string p_filterreplacevalue;
        hk_string p_filtervalueend;

    private:
        void set_column(void);
	bool	       p_virginname;//used to find out whether to automatically set defaultvalues
        static  bool p_defaultthousandsseparator;
        static  int  p_defaultprecision;
	hk_dsdatavisiblemodeprivate* p_designdata;
	hk_dsdatavisiblemodeprivate* p_viewdata;
	hk_dsdatavisibleprivate* p_private;


}


;
#endif
