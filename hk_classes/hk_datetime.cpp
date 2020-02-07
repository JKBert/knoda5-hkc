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
//$Revision: 1.20 $
#include "hk_datetime.h"
#include <stdio.h>
#include <time.h>

static const int months[12] ={31,29,31,30,31,30,31,31,30,31,30,31};
hk_datetime::hk_datetime():hk_class()
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::constructor");
#endif
    p_day=1;
    p_month=1;
    p_year=1900;
    p_hour=0;
    p_minute=0;
    p_second=0;
    p_dateformat=defaultdateformat();
    p_timeformat=defaulttimeformat();
    p_datetimeformat=defaultdatetimeformat();
    buffer="";
    set_now();
}


hk_datetime::hk_datetime(const hk_datetime& d):hk_class()
{
    set_hk_datetime(d);

}


bool hk_datetime::set_date( int d,  int m,  int y)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::set_date( int day,  int month,  int year)");
#endif
    if (is_ok_date(d,m,y))
    {
        p_day=d;
        p_month=m;
        p_year=y;
        return true;
    }
    return false;
}


bool hk_datetime::set_time( int h, int m,  int s)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::set_time( int hour, int minute,  int second)");
#endif
    if (is_ok_time(h,m,s))
    {
        p_hour=h;
        p_minute=m;
        p_second=s;
        return true;
    }
    return false;

}


bool hk_datetime::set_datetime( int d,  int m,  int y,  int h, int mi,  int s)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::set_datetime( int day,  int month,...");
#endif
    if ((set_date(d,m,y))&&(set_time(h,mi,s))) return true;
    return false;
}


bool hk_datetime::is_ok_date( int d,  int m,  int y)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::is_ok_date");
#endif
    if (m<13&& m>0 && y!=-1)
        if (d<=months[m-1]) return true;

    return false;

}


bool hk_datetime::is_ok_time( int h, int m,  int s)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::is_ok_time");
#endif
    if (h<0||m<0||s<0)return false;
    if ((h <( int)24)&&(m<( int)60)&&(s<( int)60))
        return true;
    else  return false;
}


void hk_datetime::set_dateformat(const hk_string& f)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::set_dateformat");
#endif
    p_dateformat=f;
}


void hk_datetime::set_timeformat(const hk_string& f)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::set_timeformat");
#endif
    p_timeformat=f;
}


void hk_datetime::p_dateasstring(void) const
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::p_dateasstring");
#endif
    const int bsize=100;
    char* z2=new char[bsize];
    char* zg=z2;
    if (p_day<10)
    {
        zg[0]='0';
        zg++;
    }
    snprintf(zg,bsize,"%u",p_day);
    hk_string::size_type a= buffer.find("D",0);

    if (a!=hk_string::npos)
    {
        buffer.replace(a,1,z2);
    }

    zg=z2;
    if (p_month<10)
    {
        zg[0]='0';
        zg++;
    }
    snprintf(zg,bsize,"%u",p_month);
    a= buffer.find("M",0);

    if (a!=hk_string::npos)
    {
        buffer.replace(a,1,z2);
    }
    zg=z2;
    if (p_year<10)
    {
        zg[0]='0';
        zg++;
    }
    snprintf(zg,bsize,"%u",p_year);
    a= buffer.find("Y",0);

    if (a!=hk_string::npos)
    {
        buffer.replace(a,1,z2);
    }

    delete[] z2;
}


void hk_datetime::p_timeasstring(void) const
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::p_timeasstring");
#endif
    const int bsize=100;
    char* z2=new char[bsize];
    char* zg=z2;
    if (p_hour<10)
    {
        zg[0]='0';
        zg++;
    }
    snprintf(zg,bsize,"%u",p_hour);
    hk_string::size_type a= buffer.find("h",0);

    if (a!=hk_string::npos)
    {
        buffer.replace(a,1,z2);
    }

    zg=z2;
    if (p_minute<10)
    {
        zg[0]='0';
        zg++;
    }
    snprintf(zg,bsize,"%u",p_minute);
    a= buffer.find("m",0);

    if (a!=hk_string::npos)
    {
        buffer.replace(a,1,z2);
    }

    zg=z2;
    if (p_second<10)
    {
        zg[0]='0';
        zg++;
    }
    snprintf(zg,bsize,"%u",p_second);
    a= buffer.find("s",0);

    if (a!=hk_string::npos)
    {
        buffer.replace(a,1,z2);
    }

    delete[] z2;

}


bool hk_datetime::set_date_asstring(const hk_string& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::set_date_asstring");
#endif
    bool r=true;
    int d,m,y;
    d=m=y=-1;
    int k,l;
    k=l=0;
    while ((k<(int)p_dateformat.size())&&(l<(int)s.size())&&(r==true))
    {
        if (s[l]==p_dateformat[k])
        {
            k++;
            l++;
        }
        else
        {
            switch(p_dateformat[k])
            {
                case 'D' :   d=p_setvalue(l,s);
                if (d<1)r=false;
                break;
                case 'M':    m=p_setvalue(l,s);
                if (m<1)r=false;
                break;
                case 'Y':    y=p_setvalue(l,s,true);
                if (y<0)r=false;
                break;
                default: r=false;
            }
            k++;
        }
    }

    if (r) r=set_date(d,m,y);
    return r;
}


bool hk_datetime::set_time_asstring(const hk_string& ts)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::set_time_asstring");
#endif
    bool r=true;
    int h,m,s;
    h=m=s=-1;
    int k,l;
    k=l=0;
    while ((k<(int)p_timeformat.size())&&(l<(int)ts.size())&&(r==true))
    {
        if (ts[l]==p_timeformat[k])
        {
            k++;
            l++;
        }
        else
        {
            switch(p_timeformat[k])
            {
                case 'h' :   h=p_setvalue(l,ts);
                if (h<0)r=false;
                break;
                case 'm':    m=p_setvalue(l,ts);
                if (m<0)r=false;
                break;
                case 's':    s=p_setvalue(l,ts);
                if (s<0)r=false;
                break;
                default: r=false;
            }
            k++;
        }
    }

    if (r) r=set_time(h,m,s);
    return r;

}


bool hk_datetime::set_datetime_asstring(const hk_string& ts)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::set_datetime_asstring");
#endif
    bool r=true;
    int h,m,s,y,mo,d;
    y=mo=d=h=m=s=-1;
    int k,l;
    k=l=0;
    while ((k<(int)p_datetimeformat.size())&&(l<(int)ts.size())&&(r==true))
    {
        if (ts[l]==p_datetimeformat[k])
        {
            k++;
            l++;
        }
        else
        {
            switch(p_datetimeformat[k])
            {
                case 'h' :   h=p_setvalue(l,ts);
                if (h<0)r=false;
                break;
                case 'm':    m=p_setvalue(l,ts);
                if (m<0)r=false;
                break;
                case 's':    s=p_setvalue(l,ts);
                if (s<0)r=false;
                break;
                case 'D' :   d=p_setvalue(l,ts);
                if (d<1)r=false;
                break;
                case 'M':    mo=p_setvalue(l,ts);
                if (mo<1)r=false;
                break;
                case 'Y':    y=p_setvalue(l,ts,true);
                if (y<0)r=false;
                break;

                default: r=false;
            }
            k++;
        }
    }

    if (r) r=set_datetime(d,mo,y,h,m,s);
    return r;

}


hk_string hk_datetime::datetime_asstring(void) const
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::datetime_asstring");
#endif
    buffer=p_datetimeformat;
    p_dateasstring();
    p_timeasstring();
    return buffer;
}


hk_string hk_datetime::date_asstring(void) const
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::date_asstring");
#endif
    buffer=p_dateformat;
    p_dateasstring();
    return buffer;
}


hk_string hk_datetime::time_asstring(void) const
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::time_asstring");
#endif
    buffer=p_timeformat;
    p_timeasstring();
    return buffer;
}


void hk_datetime::set_datetimeformat(const hk_string& f)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::set_datetimeformat");
#endif
    p_datetimeformat=f;
}


int hk_datetime::p_setvalue(int &l, const hk_string& s,bool is_date)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::p_setvalue");
#endif
    hk_string txt;
    int max=(is_date?4:2);
    int m=l;
    int count=0;
    while ((s[l]>='0') &&( s[l]<='9')&&(l<(int)s.size())&&(count<max))
    {
        l++;
        ++count;
    }
    txt.insert(0,s,m,l-m);
    int res=atoi(txt.c_str());
    return res;
}


void hk_datetime::set_now(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_datetime::set_now");
#endif
    struct tm* time_struct;
    time_t p_time;

    time(&p_time);
    time_struct = localtime(&p_time);
    set_datetime(time_struct->tm_mday,
        time_struct->tm_mon+1,
        time_struct->tm_year+1900,
        time_struct->tm_hour,
        time_struct->tm_min,
        time_struct->tm_sec);

}


double hk_datetime::difference(const hk_datetime& d)
{
    time_t thistime, comparetime;
    struct tm thisstruct, comparestruct;
    thisstruct.tm_mday=p_day;
    thisstruct.tm_mon=p_month-1;
    thisstruct.tm_year=p_year-1900;
    thisstruct.tm_hour=p_hour;
    thisstruct.tm_min=p_minute;
    thisstruct.tm_sec=p_second;

    comparestruct.tm_mday=d.p_day;
    comparestruct.tm_mon=d.p_month-1;
    comparestruct.tm_year=d.p_year-1900;
    comparestruct.tm_hour=d.p_hour;
    comparestruct.tm_min=d.p_minute;
    comparestruct.tm_sec=d.p_second;
    thistime=mktime(&thisstruct);
    comparetime=mktime(&comparestruct);
    return difftime(thistime,comparetime);
}


bool hk_datetime::operator>(const hk_datetime& d)
{
    return (difference(d)>0);
}


bool hk_datetime::operator>=(const hk_datetime& d)
{
    return (difference(d)>=0);

}


bool hk_datetime::operator<(const hk_datetime&d)
{
    return (difference(d)<0);

}


bool hk_datetime::operator<=(const hk_datetime&d)
{
    return (difference(d)<=0);

}


bool hk_datetime::operator=(const hk_datetime&d)
{
    set_hk_datetime(d);
    return true;
}


bool hk_datetime::operator==(const hk_datetime&d)
{
    return is_equal(d);
}


bool hk_datetime::operator!=(const hk_datetime&d)
{
    return !is_equal(d);
}


bool hk_datetime::is_equal(const hk_datetime&d)
{
    return (p_year==d.p_year
        && p_month==d.p_month
        && p_day==d.p_day
        && p_hour==d.p_hour
        && p_minute==d.p_minute
        && p_second==d.p_second);

}


void hk_datetime::set_hk_datetime(const hk_datetime& d)
{
    p_year=d.p_year;
    p_month=d.p_month;
    p_day=d.p_day;
    p_hour=d.p_hour;
    p_minute=d.p_minute;
    p_second=d.p_second;
    p_dateformat=d.p_dateformat;
    p_timeformat=d.p_timeformat;
    p_datetimeformat=d.p_datetimeformat;

}


//************************************************
//************************************************
//************************************************
//************************************************
//************************************************

void set_format(hk_datetime &dt, int  t)
{

    switch (t)
    {
        case 2 :    dt.set_dateformat("Y-M-D");
        dt.set_timeformat("h:m:s");
        dt.set_datetimeformat("Y-M-D h:m:s");
        break;

        default:    dt.set_dateformat("D.M.Y");
        dt.set_timeformat("h:m:s");
        dt.set_datetimeformat("D.M.Y h:m:s");
    }
}


hk_string transfer_date(const hk_string& date,const hk_string& originalformat, const hk_string& targetformat)
{
    hk_datetime d;
    d.set_dateformat(originalformat);
    bool r=d.set_date_asstring(date);
    d.set_dateformat(targetformat);
    if (r==true) return  d.date_asstring();
    else return "";
}


hk_string transfer_time(const hk_string& time,const hk_string& originalformat, const hk_string& targetformat)
{
    hk_datetime d;
    d.set_timeformat(originalformat);
    bool r=d.set_time_asstring(time);
    d.set_timeformat(targetformat);
    if (r==true) return  d.time_asstring();
    else return "";
}


hk_string transfer_datetime(const hk_string& datetime,const hk_string& originalformat, const hk_string& targetformat)
{
    hk_datetime d;
    d.set_datetimeformat(originalformat);
    bool r=d.set_datetime_asstring(datetime);
    d.set_datetimeformat(targetformat);
    if (r==true) return  d.datetime_asstring();
    else return "";
}


ostream& operator<<(ostream&    stream   ,hk_datetime& c)
{
    stream <<c.datetime_asstring();
    return stream;
}


ostream& operator<<(ostream&    stream   ,const hk_datetime& c)
{
    stream <<c.datetime_asstring();
    return stream;
}
