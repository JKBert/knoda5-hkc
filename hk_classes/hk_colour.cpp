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
#include "hk_colour.h"
#include "hk_string.h"

hk_colour::hk_colour(void):hk_class()
{
    set_colour(0,0,0);

}


hk_colour::hk_colour(hk_colourtype r, hk_colourtype g, hk_colourtype b):hk_class()
{
    set_colour(r,g,b);

}


hk_colour::hk_colour(const hk_colour& c):hk_class()
{

    set_hk_colour(c);

}


hk_colour::~hk_colour(void)
{

}


bool hk_colour::set_colour(hk_colourtype r, hk_colourtype g, hk_colourtype b)
{
    if (  r < 0   || g <0   ||b <0
        || r > 255 || g >255 || b >255)
        return false;
    p_red=r;
    p_green=g;
    p_blue=b;
    return true;
}


hk_colour::hk_colourtype hk_colour::red(void) const
{
    return p_red;
}


hk_colour::hk_colourtype hk_colour::green(void) const
{
    return p_green;
}


hk_colour::hk_colourtype hk_colour::blue(void) const
{
    return p_blue;
}


void hk_colour::set_hk_colour( const hk_colour& c)
{
    p_red=c.red();
    p_green=c.green();
    p_blue=c.blue();

}


bool hk_colour::operator=(const hk_colour& c)
{
    set_hk_colour(c);
    return true;

}


bool hk_colour::operator==(const hk_colour& c)
{
    return is_equal(c);
}


bool hk_colour::operator!=(const hk_colour& c)
{
    return !is_equal(c);
}


bool hk_colour::is_equal(const hk_colour& c)
{

    return (p_red==c.p_red && p_green==c.p_green && p_blue==c.p_blue);
}


void hk_colour::savedata(ostream& s )
{
    hk_string mtag="HK_COLOUR";
    start_mastertag(s,mtag);
    set_tagvalue(s,"RED",p_red);
    set_tagvalue(s,"GREEN",p_green);
    set_tagvalue(s,"BLUE",p_blue);
    end_mastertag(s,mtag);

}





void hk_colour::loaddata(xmlNodePtr definition)
{
    get_tagvalue(definition,"RED",p_red);
    get_tagvalue(definition,"GREEN",p_green);
    get_tagvalue(definition,"BLUE",p_blue);

}


ostream& operator<<(ostream&    stream   ,hk_colour& c)
{
    stream <<"("<<hk_class::hk_translate("red: ")<<c.red()<<" "<<hk_class::hk_translate("green: ")<<c.green()<<" "<<hk_class::hk_translate("blue: ")<<c.blue()<<")";
    return stream;
}


ostream& operator<<(ostream&    stream   ,const hk_colour& c)
{
    stream <<"("<<hk_class::hk_translate("red: ")<<c.red()<<" "<<hk_class::hk_translate("green: ")<<c.green()<<" "<<hk_class::hk_translate("blue: ")<<c.blue()<<")";
    return stream;
}
