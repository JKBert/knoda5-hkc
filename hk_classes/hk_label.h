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
#ifndef HK_LABEL
#define HK_LABEL
#include "hk_visible.h"
#include "hk_form.h"
using namespace std;
class hk_labelmodeprivate;
/**
 *
 *@short  base class for labels in forms
 *@version $Revision: 1.11 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *base class for labels in forms
 */
class hk_label:public hk_visible
{
    public:
        hk_label(hk_form* form=NULL);
	virtual ~hk_label();
	/**
 *if true a line should be painted above the data
 *
 *See @ref set_configurefunction  for details
 *
 */
        void    set_topline(int l=1,bool registerchange=true);
/**
 *if true a line should be painted under the data
 *
 *See @ref set_configurefunction  for details
 */
        void    set_bottomline(int l=1,bool registerchange=true);
/**
 *if true a line should be painted left of the data
 *
 *See @ref set_configurefunction  for details
 */

        void    set_leftline(int l=1,bool registerchange=true);
/**
 *if true a line should be painted right the data
 *
 *See @ref set_configurefunction  for details
 */

        void    set_rightline(int l=1,bool registerchange=true);
/**
 *if true a diagonal should be painted from _l_inks _u_nten to _r_echts _o_ben
 * (left down to right up)
 *
 *See @ref set_configurefunction  for details
 */

        void    set_diagonalluro(int l=1,bool registerchange=true);
/**
 *if true a diagonal should be painted from _l_inks _o_ben to _r_echts _u_nten
 * (left top to right down)
 *
 *See @ref set_configurefunction  for details
 */
        void    set_diagonalloru(int l=1,bool registerchange=true);
        int    diagonalloru(void);
        int    topline(void);
        int    bottomline(void);
        int    leftline(void);
        int    rightline(void);
        int    diagonalluro(void);
/**
 *if true a frame should be painted around the text (bottomline + topline + leftline + rightline)
 *
 *See @ref set_configurefunction  for details
 */
        void    set_frame(int l=1,bool registerchange=true);

protected:
	virtual bool	presentationmode_changed(void);
        virtual void savedata(ostream& s );
        virtual void loaddata(xmlNodePtr definition);
	virtual void widget_specific_topline_changed(void){}
	virtual void widget_specific_bottomline_changed(void){}
	virtual void widget_specific_leftline_changed(void){}
	virtual void widget_specific_rightline_changed(void){}
	virtual void widget_specific_diagonalluro_changed(void){}
	virtual void widget_specific_diagonalloru_changed(void){}



private:
 	hk_labelmodeprivate* p_designdata;
 	hk_labelmodeprivate* p_viewdata;
};
#endif
