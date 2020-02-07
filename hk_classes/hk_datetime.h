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
#ifndef HK_DATETIME
#define HK_DATETIME
#include "hk_class.h"
using namespace std;

/**
 *@short translates date and/or time values in different formats
 *@author Horst Knorr <hk_classes@knoda.org>
 *@version $Revision: 1.13 $
 *@internal
 *
 *This class transforms date and/or time formats to any other format
 *
 */
class hk_datetime: public hk_class
{
    public:
        hk_datetime(void);
        hk_datetime(const hk_datetime&);
        virtual ~hk_datetime(void){}
/**
 *sets the date
 */
        bool set_date( int day,  int month,  int year);
/**
 *sets the time
 */
        bool set_time( int hour, int minute,  int second);
/**
 *sets date and time
 */
        bool set_datetime( int day,  int month,  int year,  int hour, int minute,  int second);
/**
 * sets the date.
 *@param s contains the date as a hk_string in the format as specified with @ref set_dateformat
 */
        bool set_date_asstring(const hk_string& s);
/**
 *@param s contains the time as a hk_string in the format as specified with @ref set_timeformat
 */
        bool set_time_asstring(const hk_string& s);
/**
 *@param s contains the date and time as a hk_string in the format as specified with @ref set_datetimeformat
 */
        bool set_datetime_asstring(const hk_string& s);
/**
 *@return the date as a hk_string in the format as specified with @ref set_dateformat
 */
        hk_string date_asstring(void) const;
/**
 *@return the time as a hk_string in the format as specified with @ref set_timeformat
 */
        hk_string time_asstring(void)const;
/**
 *@return the date and the time as a hk_string in the format as specified with @ref set_datetimeformat
 */
        hk_string datetime_asstring(void) const;
/**
 *Any hk_string is valid. The values have to be separated with at least one character. The variables are
 * D for the day
 * M for the month
 * Y for the year
 * example : "D.M.Y"  shows the 4th of November 2001 as 04.11.2001 and "Y-M-D" as 2001-11-04
 */
        void set_dateformat(const hk_string& f);
/**
 *Any hk_string is valid. The values have to be separated with at least one character. The variables are
 * h for the hour
 * m for the minute
 * s for the second
 * example : "h:m:s"  shows the noon as 12:00:00 and "h/m" as 12/00
 */
        void set_timeformat(const hk_string& f);
/**
 *Any hk_string is valid. The values have to be separated with at least one character. The variables are
 * D for the day
 * M for the month
 * Y for the year
 * h for the hour
 * m for the minute
 * s for the second
 * example : "D.M.Y h:m:s"  shows the 4th of November 2001 at noon as 04.11.2001 12:00:00 and "Y-M-D h/m" as 2001-11-04 12/00
 */
        void set_datetimeformat(const hk_string& f);

        int day() const {return p_day;}
        int month()const{return p_month;}
        int year()const{return p_year;}
        int hour()const{return p_hour;}
        int minute()const{return p_minute;}
        int second()const {return p_second;}
/**
 *sets the current date and time
 */
        void set_now(void);

        bool operator=(const hk_datetime&);
        bool operator==(const hk_datetime&);
        bool operator!=(const hk_datetime&);
        bool operator>(const hk_datetime&);
        bool operator>=(const hk_datetime&);
        bool operator<(const hk_datetime&);
        bool operator<=(const hk_datetime&);

/**
 * returns the difference in seconds,negative values say that this object is contains a time before d;
 */
        double difference(const hk_datetime& d);
    protected:

        bool is_ok_date( int day,  int month,  int year);
        bool is_ok_time( int hour, int minute,  int second);

    private:
        void p_dateasstring(void)const ;
        void p_timeasstring(void) const;
        int  p_setvalue(int &l,const hk_string& s,bool is_date=false);
        int p_year,p_month,p_day;
        int p_second,p_minute,p_hour;
        hk_string p_dateformat;
        hk_string p_timeformat;
        hk_string p_datetimeformat;
        mutable hk_string buffer;
        void set_hk_datetime(const hk_datetime&);
        bool is_equal(const hk_datetime&);
};

//void set_format(hk_datetime &dt,int t);
/**
 *convinience function that returns the date formated as defined in targetformat, where date originally was formatted as
 *defined in originalformat
 */
hk_string transfer_date(const hk_string& date,const hk_string& originalformat, const hk_string& targetformat);
/**
 *convinience function that returns the time formated as defined in targetformat, where time originally was formatted as
 *defined in originalformat
 */
hk_string transfer_time(const hk_string& time,const hk_string& originalformat, const hk_string& targetformat);
/**
 *convinience function that returns the datetime formated as defined in targetformat, where datetime originally was formatted as
 *defined in originalformat
 */
hk_string transfer_datetime(const hk_string& datetime,const hk_string& originalformat, const hk_string& targetformat);

ostream& operator<<(ostream&,const hk_datetime&);
ostream& operator<<(ostream&, hk_datetime&);
#endif
