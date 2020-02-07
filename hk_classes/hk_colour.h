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
#ifndef HK_COLOUR_H
#define HK_COLOUR_H
#include "hk_class.h"
#include <iostream>
/**
 *
 *@short represents a RGB-colour
 *@version $Revision: 1.8 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *represents a RGB-colour
 */

class hk_colour: public hk_class
{
    friend istream& operator>>(istream&,hk_colour&);
    friend ostream& operator<<(ostream&,hk_colour&);
    public:

        typedef int hk_colourtype ;

        hk_colour(void);
        hk_colour(hk_colourtype red, hk_colourtype green, hk_colourtype blue);
        hk_colour(const hk_colour&);
        virtual ~hk_colour();
/**
 *sets the RGB colour. valid values are in the range 0 to 255.
 *@return true if successful, false if the colour could not be set (out of range).
 */
        bool set_colour(hk_colourtype red, hk_colourtype green, hk_colourtype blue);
        hk_colourtype red(void) const;
        hk_colourtype green(void) const;
        hk_colourtype blue(void) const;
        bool operator=(const hk_colour&);
        bool operator==(const hk_colour&);
        bool operator!=(const hk_colour&);

        virtual void savedata(ostream& s );
virtual void loaddata(xmlNodePtr definition);

    protected:

    private:
        void set_hk_colour( const hk_colour&);
        bool is_equal(const hk_colour&);
        hk_colourtype p_red;
        hk_colourtype p_green;
        hk_colourtype p_blue;

};

ostream& operator<<(ostream&,hk_colour&);
ostream& operator<<(ostream&,const hk_colour&);

const hk_colour hk_aqua(0,255,255);
const hk_colour hk_beige(0xF5,0xF5,0xDC);
const hk_colour hk_black(0,0,0);
const hk_colour hk_blue(0,0,255);
const hk_colour hk_brown(0xA5,0x2A,0x2A);
const hk_colour hk_fuchsia(255,0,255);
const hk_colour hk_darkgrey(0xA9,0xA9,0xA9);
const hk_colour hk_darkyellow(0xEE,0xCA,0x16);
const hk_colour hk_green(0,128,0);
const hk_colour hk_grey(128,128,128);
const hk_colour hk_lightgrey(0xD3,0xD3,0xD3);
const hk_colour hk_lime(0,255,0);
const hk_colour hk_maroon(128,0,0);
const hk_colour hk_navy(0,0,128);
const hk_colour hk_olive(128,128,0);
const hk_colour hk_orange(0xFF,0xA5,0);
const hk_colour hk_pink(0xFF,0xC0,0xCB);
const hk_colour hk_purple(128,0,128);
const hk_colour hk_red(255,0,0);
const hk_colour hk_silver(0,0xc0,0xc0);
const hk_colour hk_teal(0,128,128);               //aquamarin
const hk_colour hk_white(255,255,255);
const hk_colour hk_yellow(255,255,0);
#endif                                            //HK_COLOUR_H
