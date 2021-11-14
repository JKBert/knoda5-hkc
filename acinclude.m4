dnl    This file is part of the hk_classes libraries/packages
dnl    Copyright (C) 2001-2003 Horst Knorr (hk_classes@knoda.org)

dnl    This file is free software; you can redistribute it and/or
dnl    modify it under the terms of the GNU Library General Public
dnl    License as published by the Free Software Foundation;
dnl    version 2 of the License.

dnl    This library is distributed in the hope that it will be useful,
dnl    but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl    Library General Public License for more details.

dnl    You should have received a copy of the GNU Library General Public License
dnl    along with this library; see the file COPYING.LIB.  If not, write to
dnl    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
dnl    Boston, MA 02111-1307, USA.

dnl 	$Id: acinclude.m4,v 1.75 2006/10/03 10:33:07 knorr Exp $







AC_DEFUN([HK_CLASSESINIT],
[
AC_MSG_CHECKING([HK_CLASSES INIT])
AC_MSG_RESULT(OK)

AC_MSG_CHECKING([whether this is a 64bit system])
AC_TRY_LINK([],[#ifndef __LP64__ 
#error "no 64 bit system" 
#endif
],[x64bit=yes],[x64bit=no])
AC_MSG_RESULT($x64bit)


if test "$prefix" = "NONE"; then
  if test $x64bit = "yes"; then
  HK_CLASSESDIR=/usr/lib64/hk_classes
  BITARCHITECTURE="-m64"
  
    else
  HK_CLASSESDIR=/usr/lib/hk_classes
  BITARCHITECTURE="-m32"
  
  fi
  
 HK_CLASSESINCDIR=/usr/include/hk_classes
else
 HK_CLASSESDIR=$libdir/hk_classes
 HK_CLASSESINCDIR=$includedir/hk_classes
fi
if test "$with_hk_classes_incdir" != ""; then
HK_CLASSESINCDIR=$with_hk_classes_incdir
fi

HKPYTHONDIR=""
UTILITIESDIR=""
AC_SUBST(HK_CLASSESDIR)
AC_SUBST(HK_CLASSESINCDIR)
NODRIVER=true
POSSIBLE_DRIVERS="FIREBIRD,MYSQL,ODBC,PARADOX,POSTGRESQL,SQLITE2,SQLITE3,MDB,XBASE"
dnl AC_DEFINE_UNQUOTED(HKCLASSES, "$HK_CLASSESDIR",[PREPROCESSOR DEFINITION])
AC_SUBST(BITARCHITECTURE)
])




AC_DEFUN([HK_MYSQLTEST],
[
if test "$want_mysql" = "yes"; then
AC_MSG_CHECKING([whether MYSQL is available])
if test "$with_mysql_dir" = ""; then
  if test "$want_64bit" = "yes"; then
   MYSQLLIBDIR=/usr/lib64/mysql
   else
   MYSQLLIBDIR=/usr/lib/mysql
   fi
   MYSQLINCDIR=/usr/include/mysql
else MYSQLLIBDIR=$with_mysql_dir/lib
MYSQLINCDIR=$with_mysql_dir/include
fi

if test "x$with_mysql_incdir" != "x"; then
 MYSQLINCDIR=$with_mysql_incdir
fi

if test "x$with_mysql_libdir" != "x"; then
 MYSQLLIBDIR=$with_mysql_libdir
fi


ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

CXXLIBS="$ac_save_CXXLIBS -L${MYSQLLIBDIR} -lmysqlclient"
LIBS="$ac_save_LIBS -L${MYSQLLIBDIR} -lmysqlclient"
CPPFLAGS="$ac_save_CPPFLAGS -I$MYSQLINCDIR"

AC_TRY_LINK([#include <mysql.h>],[;],[mysql_lib=yes],[mysql_lib=no])

AC_MSG_RESULT($mysql_lib)
if test $mysql_lib = yes ; then
NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_mysqlclasses"
AC_SUBST(MYSQLLIBDIR)
AC_SUBST(MYSQLINCDIR)

CXXLIBS="$ac_save_CXXLIBS "
LIBS="$ac_save_LIBS "
CPPFLAGS="$ac_save_CPPFLAGS "


MYSQLREALESCAPE
fi


LIBS="$ac_save_LIBS"
CPPFLAGS="$ac_save_CPPFLAGS"
CXXLIBS="$ac_save_CXXLIBS"

fi
])

AC_DEFUN([MYSQLREALESCAPE],
[

AC_MSG_CHECKING([MYSQL has mysql_real_escape_string function])


AC_TRY_LINK([#include <mysql.h>],[
MYSQL* m;
char* to;
 mysql_real_escape_string(m,to,"test",4);
],[mysql_real=yes],[mysql_real=no])

AC_MSG_RESULT($mysql_real)
if test $mysql_real = yes ; then
AC_DEFINE(HAVE_MYSQL_REAL_ESCAPE_STRING,1,Defines if Mysql has the mysql_real_escape_string function)
fi


])




AC_DEFUN([HK_POSTGRESTEST],
[
if test "$want_postgres" = "yes"; then
AC_MSG_CHECKING([whether POSTGRES is available])
if test "$with_postgres_dir" = ""; then
  if test "$want_64bit" = "yes"; then
   POSTGRESLIBDIR=/usr/local/pgsql/lib64
   else
   POSTGRESLIBDIR=/usr/local/pgsql/lib
   fi
   POSTGRESINCDIR=/usr/include/pgsql
else
   POSTGRESLIBDIR=$with_postgres_dir/lib
   POSTGRESINCDIR=$with_postgres_dir/include
fi

if test "x$with_postgres_incdir" != "x"; then
 POSTGRESINCDIR=$with_postgres_incdir
fi

if test "x$with_postgres_libdir" != "x"; then
 POSTGRESLIBDIR=$with_postgres_libdir
fi


ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

CXXLIBS="$ac_save_CXXLIBS -L${POSTGRESLIBDIR} -lpq"
LIBS="$ac_save_LIBS -L${POSTGRESLIBDIR} -lpq"
CPPFLAGS="$ac_save_CPPFLAGS -I$POSTGRESINCDIR"

AC_TRY_LINK([#include <libpq-fe.h>],[;],[postgres_lib=yes],[postgres_lib=no])

AC_MSG_RESULT($postgres_lib)
if test $postgres_lib = yes ; then
NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_postgresclasses"
AC_SUBST(POSTGRESLIBDIR)
AC_SUBST(POSTGRESINCDIR)
fi


LIBS="$ac_save_LIBS"
CPPFLAGS="$ac_save_CPPFLAGS"
CXXLIBS="$ac_save_CXXLIBS"
echo "POSTGRES LIBS" $LIBS "CPPFLAGS=" $CPPFLAGS "CXXFLAGS=" $CXXFLAGS
fi


])








AC_DEFUN([HK_ODBCTEST],
[
if test "$want_odbc" = "yes"; then
AC_MSG_CHECKING([whether ODBC is available])
if test "$ODBCDIR" = ""; then
  if test "$want_64bit" = "yes"; then
   ODBCLIBDIR=/usr/local/lib64
   else
   ODBCLIBDIR=/usr/local/lib
   fi
   ODBCINCDIR=/usr/local/include
else ODBCLIBDIR=$ODBCDIR/lib
ODBCLINCDIR=$ODBCDIR/include
fi
ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

if test "$with_odbc_incdir" != ""; then

ODBCINCDIR=$with_odbc_incdir

fi

if test "$with_odbc_libdir" != ""; then

ODBCLIBDIR=$with_odbc_libdir

fi



CXXLIBS="$ac_save_CXXLIBS -L${ODBCLIBDIR} -lodbc"
LIBS="$ac_save_LIBS -L${ODBCLIBDIR} -lodbc"
CPPFLAGS="$ac_save_CPPFLAGS -I$ODBCINCDIR"




AC_TRY_LINK([#include <sql.h>],[;],[odbc_lib=yes],[odbc_lib=no])
if test x$odbc_lib = xno ; then
if test "$ODBCDIR" = ""; then
  if test "$want_64bit" = "yes"; then
   ODBCLIBDIR=/usr/local/lib64
   else
   ODBCLIBDIR=/usr/local/lib
   fi
   ODBCINCDIR=/usr/include/
fi



LIBS="$ac_save_LIBS -L${ODBCLIBDIR} -lodbc"
CPPFLAGS="$ac_save_CPPFLAGS -I$ODBCINCDIR"

AC_TRY_LINK([#include <sql.h>],[;],[odbc_lib=yes],[odbc_lib=no])
fi


if test x$odbc_lib = xyes ; then
ODBCLIBRARY="odbc"
else
LIBS="$ac_save_LIBS -L${ODBCLIBDIR} -liodbc"
CPPFLAGS="$ac_save_CPPFLAGS -I$ODBCINCDIR"
 AC_TRY_LINK([#include <sql.h>],[;],[odbc_lib=yes],[odbc_lib=no])
 if test x$odbc_lib = xyes ; then
 ODBCLIBRARY="iodbc"
 fi
fi

AC_MSG_RESULT($odbc_lib)
if test x$odbc_lib = xyes ; then
NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_odbcclasses"
AC_SUBST(ODBCLIBDIR)
AC_SUBST(ODBCINCDIR)
AC_SUBST(ODBCLIBRARY)

CXXLIBS="$ac_save_CXXLIBS "
LIBS="$ac_save_LIBS "
CPPFLAGS="$ac_save_CPPFLAGS "



AC_DEFINE_UNQUOTED(ODBCLIBRARY,"$ODBCLIBRARY",[DEFINES THE NAME OF THE ODBC LIBRARY])
dnl else
dnl  AC_MSG_ERROR( ODBC not found )

fi
LIBS="$ac_save_LIBS"
CPPFLAGS="$ac_save_CPPFLAGS"
CXXLIBS="$ac_save_CXXLIBS"
fi

])







AC_DEFUN([HK_DEFINEDRIVERS],
[
AC_SUBST(DRIVERSDIR)
AC_SUBST(NODRIVER)
AC_SUBST(POSSIBLE_DRIVERS)
AC_SUBST(HKPYTHONDIR)

])

AC_DEFUN([AC_SSTREAM_EXISTS],
[

AC_MSG_CHECKING([whether <sstream> exists])
AC_TRY_LINK([#include <sstream>],[;],[sstream_exists=yes],[sstream_exists=no])
AC_MSG_RESULT($sstream_exists)
if test $sstream_exists = yes ; then
AC_DEFINE(HAVE_SSTREAM,1,Defines if stdc++ library contains a sstream implementation)
else
AC_DEFINE(HAVE_NO_SSTREAM,1,Defined if stdc++ library does not contain a sstream implementation)
fi

])

AC_DEFUN([AC_IOSTREAM_EXISTS],
[

AC_MSG_CHECKING([whether <iostream> exists])
AC_TRY_LINK([#include <iostream>],[;],[iostream_exists=yes],[iostream_exists=no])
AC_MSG_RESULT($iostream_exists)
if test $iostream_exists = yes ; then
AC_DEFINE(HAVE_IOSTREAM,1,Defines if stdc++ library contains a <iostream> implementation)
else
AC_DEFINE(NO_IOSTREAM,1,Defines that stdc++ library does not have a new <iostream> implementation , using <iostream.h> instead)

fi

])


AC_DEFUN([AC_MAGIC_EXISTS],
[

AC_MSG_CHECKING([whether the <magic> library exists])

ac_save_LIBS="$LIBS"

LIBS="$ac_save_LIBS -lmagic"

AC_TRY_LINK([#include <magic.h>],[magic_set *magic;],[magic_exists=yes],[magic_exists=no])
AC_MSG_RESULT($magic_exists)
if test $magic_exists = no ; then
	   AC_MSG_ERROR([
  Could not link to the 'magic' library
  ============================================================================
   ERROR!
   You probably have to install the development version of the 'magic' package
   for your distribution (don't mix it up with libmagick, that's a different package).  
   The exact name of this package varies among them (some distributions have it in 'file-yxx.rpm').
  ============================================================================
	   ])

fi

LIBS="$ac_save_LIBS "


])




AC_DEFUN([HK_CLEARENV_EXISTS],
[
AC_MSG_CHECKING([whether clearenv() is available])
AC_TRY_LINK([#include <stdlib.h>],[clearenv();],[ac_clearenv=yes],[ac_clearenv=no])
if test $ac_clearenv = yes ; then
AC_DEFINE(HAVE_CLEARENV,1,Defines if stdlib has function clearenv)
fi
AC_MSG_RESULT($ac_clearenv)
])


AC_DEFUN([HK_CHECK_ARGP_EXISTS],
[
dnl argp.h is not available on FreeBSD !
AC_MSG_CHECKING([whether argp.h is available])
AC_TRY_LINK([#include <argp.h>],[;],[ac_argp=yes],[ac_argp=no])
if test $ac_argp = yes ; then
AC_DEFINE(HAVE_ARGP,1,Defines whether argp.h exists)
 UTILITIESDIR="utilities"

fi
AC_SUBST(UTILITIESDIR)
AC_MSG_RESULT($ac_argp)

])


dnl the following makro has been taken from GNU gettext
dnl and modified to fit for HK_CLASSES
dnl 17.02.2002 Horst Knorr


dnl Copyright (C) 2000-2002 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

dnl From Bruno Haible.

AC_DEFUN([AM_ICONV_LINK],
[
  dnl Some systems have iconv in libc, some have it in libiconv (OSF/1 and
  dnl those with the standalone portable GNU libiconv installed).

  dnl Prerequisites of AC_LIB_LINKFLAGS_BODY.
  dnl AC_REQUIRE([AC_LIB_PREPARE_PREFIX])
  dnl AC_REQUIRE([AC_LIB_RPATH])

  dnl Search for libiconv and define LIBICONV, LTLIBICONV and INCICONV
  dnl accordingly.
  dnl AC_LIB_LINKFLAGS_BODY([iconv])

  AC_MSG_CHECKING([if libiconv is present])
  ac_save_LIBS="$LIBS"
  LIBS="$ac_save_LIBS -liconv"
  AC_TRY_LINK(,
    [;],
    LIBICONV="-liconv"
    ac_liconv="yes",
    LIBS="$ac_save_LIBS"
    ac_liconv="no")
  AC_MSG_RESULT($ac_liconv)
  dnl Add $INCICONV to CPPFLAGS before performing the following checks,
  dnl because if the user has installed libiconv and not disabled its use
  dnl via --without-libiconv-prefix, he wants to use it. The first
  dnl AC_TRY_LINK will then fail, the second AC_TRY_LINK will succeed.
  am_save_CPPFLAGS="$CPPFLAGS"
  dnl AC_LIB_APPENDTOVAR([CPPFLAGS], [$INCICONV])

  AC_CACHE_CHECK(for iconv, am_cv_func_iconv, [
    am_cv_func_iconv="no, consider installing GNU libiconv"
    am_cv_lib_iconv=no
    AC_TRY_LINK([#include <stdlib.h>
#include <iconv.h>],
      [iconv_t cd = iconv_open("","");
       iconv(cd,NULL,NULL,NULL,NULL);
       iconv_close(cd);],
      am_cv_func_iconv=yes)
    if test "$am_cv_func_iconv" != yes; then
      am_save_LIBS="$LIBS"
      LIBS="$LIBS $LIBICONV"
      AC_TRY_LINK([#include <stdlib.h>
#include <iconv.h>],
        [iconv_t cd = iconv_open("","");
         iconv(cd,NULL,NULL,NULL,NULL);
         iconv_close(cd);],
        am_cv_lib_iconv=yes
        am_cv_func_iconv=yes)
      LIBS="$am_save_LIBS"
    fi
  ])
  if test "$am_cv_func_iconv" = yes; then
    AC_DEFINE(HAVE_ICONV, 1, [Define if you have the iconv() function.])
  fi
  if test "$am_cv_lib_iconv" = yes; then
    AC_MSG_CHECKING([how to link with libiconv])
    AC_MSG_RESULT([$LIBICONV])
  else
    dnl If $LIBICONV didn't lead to a usable library, we don't need $INCICONV
    dnl either.
    CPPFLAGS="$am_save_CPPFLAGS"
    LIBICONV=
    LTLIBICONV=
  fi
  if test "$am_cv_func_iconv" != "yes" ; then
   if  test "$am_cv_lib_iconv" != "yes" ; then
      AC_MSG_ERROR(
        No iconv installation found. Either you have a very old libc or a system that doesn't support
        this function. Perhaps you should install the library libiconv from Bruno Haible.
       )
   fi
  fi



LIBS="$ac_save_LIBS "


  AC_SUBST(LIBICONV)
  AC_SUBST(LTLIBICONV)
])

AC_DEFUN([AM_ICONV],
[
  AC_REQUIRE([AM_ICONV_LINK])
  if test "$am_cv_func_iconv" = yes; then
    AC_MSG_CHECKING([for iconv declaration])
    AC_CACHE_VAL(am_cv_proto_iconv, [
      AC_TRY_COMPILE([
#include <stdlib.h>
#include <iconv.h>
extern
#ifdef __cplusplus
"C"
#endif
#if defined(__STDC__) || defined(__cplusplus)
size_t iconv (iconv_t cd, char * *inbuf, size_t *inbytesleft, char * *outbuf, size_t *outbytesleft);
#else
size_t iconv();
#endif
], [], am_cv_proto_iconv_arg1="", am_cv_proto_iconv_arg1="const")
      am_cv_proto_iconv="extern size_t iconv (iconv_t cd, $am_cv_proto_iconv_arg1 char * *inbuf, size_t *inbytesleft, char * *outbuf, size_t *outbytesleft);"])
    am_cv_proto_iconv=`echo "[$]am_cv_proto_iconv" | tr -s ' ' | sed -e 's/( /(/'`
    AC_MSG_RESULT([$]{ac_t:-
         }[$]am_cv_proto_iconv)
    AC_DEFINE_UNQUOTED(ICONV_CONST, $am_cv_proto_iconv_arg1,
      [Define as const if the declaration of iconv() needs const.])
  fi
])














AC_DEFUN([HK_SQLITE2TEST],
[
if test "$want_sqlite" = "yes"; then

if test "x$want_sqlite2library" != "xno"; then


AC_MSG_CHECKING([whether SQLITE2 is available])
if test "$with_sqlite_dir" = ""; then
  if test "$want_64bit" = "yes"; then
   SQLITELIBDIR=/usr/local/lib64
  else
   SQLITELIBDIR=/usr/local/lib
  fi
   SQLITEINCDIR=/usr/local/include
else SQLITELIBDIR=$with_sqlite_dir/lib
SQLITEINCDIR=$with_sqlite_dir/include
fi

if test "x$with_sqlite_incdir" != "x"; then
 SQLITEINCDIR=$with_sqlite_incdir
fi

if test "x$with_sqlite_libdir" != "x"; then
 SQLITELIBDIR=$with_sqlite_libdir
fi


ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

CXXLIBS="$ac_save_CXXLIBS -L${SQLITELIBDIR} -lsqlite"
LIBS="$ac_save_LIBS -L${SQLITELIBDIR} -lsqlite"
CPPFLAGS="$ac_save_CPPFLAGS -I$SQLITEINCDIR"

AC_TRY_LINK([#include <sqlite.h>],[char* errormsg=0;
sqlite* mydb=sqlite_open("",0,&errormsg);
sqlite_vm *vm=0;
int result =sqlite_compile(mydb,"",0,&vm,&errormsg);
;],[sqlite_lib=yes],[sqlite_lib=no])

AC_MSG_RESULT($sqlite_lib)
if test $sqlite_lib = yes ; then
NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_sqliteclasses"
AC_SUBST(SQLITELIBDIR)
AC_SUBST(SQLITEINCDIR)


LIBS="$ac_save_LIBS"
CPPFLAGS="$ac_save_CPPFLAGS"
CXXLIBS="$ac_save_CXXLIBS"
fi




else
AC_MSG_CHECKING([whether the SQLITE2 driver will be installed])

NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_sqliteclasses"
AC_MSG_RESULT(yes)
fi

fi
AM_CONDITIONAL(USESQLITE2LIBRARY,test x$sqlite_lib = xyes )

])




















AC_DEFUN([HK_SQLITE3TEST],
[
if test "$want_sqlite3" = "yes"; then

if test "x$want_sqlite3library" != "xno"; then


AC_MSG_CHECKING([whether SQLITE3 is available])
if test "$with_sqlite3_dir" = ""; then
   
  if test "$want_64bit" = "yes"; then
   SQLITE3LIBDIR=/usr/local/lib64
   else
   SQLITE3LIBDIR=/usr/local/lib
   fi
   SQLITE3INCDIR=/usr/local/include
else SQLITE3LIBDIR=$with_sqlite3_dir/lib
SQLITE3INCDIR=$with_sqlite3_dir/include
fi

if test "x$with_sqlite3_incdir" != "x"; then
 SQLITE3INCDIR=$with_sqlite3_incdir
fi

if test "x$with_sqlite3_libdir" != "x"; then
 SQLITE3LIBDIR=$with_sqlite3_libdir
fi


ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

CXXLIBS="$ac_save_CXXLIBS -L${SQLITE3LIBDIR} -lsqlite3"
LIBS="$ac_save_LIBS -L${SQLITE3LIBDIR} -lsqlite3"
CPPFLAGS="$ac_save_CPPFLAGS -I$SQLITE3INCDIR"

AC_TRY_LINK([#include <sqlite3.h>],[
sqlite3* mydb;
sqlite3_open("",&mydb);
sqlite3_stmt *vm=0;
int result =sqlite3_prepare(mydb,"",0,&vm,0);
;],[sqlite3_lib=yes],[sqlite3_lib=no])

AC_MSG_RESULT($sqlite3_lib)
if test $sqlite3_lib = yes ; then
NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_sqlite3classes"
AC_SUBST(SQLITE3LIBDIR)
AC_SUBST(SQLITE3INCDIR)


LIBS="$ac_save_LIBS"
CPPFLAGS="$ac_save_CPPFLAGS"
CXXLIBS="$ac_save_CXXLIBS"
fi




else
AC_MSG_CHECKING([whether the SQLITE3 driver will be installed])

NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_sqlite3classes"
AC_MSG_RESULT(yes)
fi

fi
AM_CONDITIONAL(USESQLITE3LIBRARY,test x$sqlite3_lib = xyes )

])



AC_DEFUN([HK_PGKCONFIG],[
if test "$want_mdb" = "yes"; then

dnl Check for pkg-config
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

if test "$PKG_CONFIG" = "no"; then
  AC_MSG_ERROR([This package requires pkg-config.])
fi

dnl Check for Glib-2.0
# GLIB_CFLAGS:  cflags for compiling glib dependant sources
# GLIB_LIBADD:  glib libraries (-l options)
# GLIB_LDFLAGS: flags containing path to glib libraries (-L options)

GLIB_PACKAGES="glib-2.0"
GLIB_VERSION="2.4"
AC_MSG_CHECKING(whether the MDB/ACCESS driver will be installed)

if $PKG_CONFIG --atleast-pkgconfig-version 0.15 ; then
  if $PKG_CONFIG --atleast-version $GLIB_VERSION $GLIB_PACKAGES >/dev/null 2>&1 ; then
    GLIB_CFLAGS="`$PKG_CONFIG --cflags $GLIB_PACKAGES`"
    GLIB_LIBADD="`$PKG_CONFIG --libs-only-l --libs-only-other $GLIB_PACKAGES`"
    GLIB_LDFLAGS="`$PKG_CONFIG --libs-only-L $GLIB_PACKAGES`"
    AC_MSG_RESULT(yes)
  fi
else
  if $PKG_CONFIG --atleast-version $GLIB_VERSION $GLIB_PACKAGES >/dev/null 2>&1 ; then
    GLIB_CFLAGS="`$PKG_CONFIG --cflags $GLIB_PACKAGES`"
    GLIB_LIBADD="`$PKG_CONFIG --libs-only-l $GLIB_PACKAGES`"
    GLIB_LDFLAGS="`$PKG_CONFIG --libs-only-L $GLIB_PACKAGES`"
  fi
fi

if test -z "$GLIB_LIBADD"; then
  AC_MSG_RESULT( GLib-2.0 (at least $GLIB_VERSION) is missing)
else
CPPFLAGS="$CPPFLAGS $GLIB_CFLAGS"
CFLAGS="$CFLAGS $GLIB_CFLAGS"

AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBADD)
AC_SUBST(GLIB_LDFLAGS)

CXXLIBS="$ac_save_CXXLIBS "
LIBS="$ac_save_LIBS "
CPPFLAGS="$ac_save_CPPFLAGS "

NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_mdbclasses"
AC_MSG_RESULT(yes)

fi
fi

])



AC_DEFUN([HK_MDBTEST],
[
AC_REQUIRE([HK_PGKCONFIG])

])






AC_DEFUN([HK_XBASETEST],
[

if test "$want_xbase" = "yes"; then
AC_MSG_CHECKING([whether XBASE is available])
if test "$with_xbase_dir" = ""; then
  if test "$want_64bit" = "yes"; then
   XBASELIBDIR=/usr/local/lib64
  else
   XBASELIBDIR=/usr/local/lib
  fi
   XBASEINCDIR=/usr/local/include
else XBASELIBDIR=$with_xbase_dir/lib
XBASEINCDIR=$with_xbase_dir/include
fi

if test "x$with_xbase_incdir" != "x"; then
 XBASEINCDIR=$with_xbase_incdir
fi

if test "x$with_xbase_libdir" != "x"; then
 XBASELIBDIR=$with_xbase_libdir
fi

ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

CXXLIBS="$ac_save_CXXLIBS -L${XBASELIBDIR}"
LIBS="$ac_save_LIBS -L${XBASELIBDIR}  -lxbsql "
CPPFLAGS="$ac_save_CPPFLAGS -I .  -I $XBASEINCDIR  "

AC_TRY_LINK([#include <xbsql.h>],[
XBaseSQL xb ("tst");

;],[xbase_lib=yes],[xbase_lib=no])

AC_MSG_RESULT($xbase_lib)
if test $xbase_lib = yes ; then
NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_xbaseclasses"
AC_SUBST(XBASELIBDIR)
AC_SUBST(XBASEINCDIR)
fi


LIBS="$ac_save_LIBS"
CPPFLAGS="$ac_save_CPPFLAGS"
CXXLIBS="$ac_save_CXXLIBS"
fi

])







AC_DEFUN([HK_PARADOXTEST],
[

AC_MSG_CHECKING([whether the PARADOX (pxlib) driver will be installed])
if test "$want_paradox" = "yes"; then
NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_paradoxclasses"
AC_MSG_RESULT(yes)
else
AC_MSG_RESULT(no)
fi

])






AC_DEFUN([HK_DBASETEST],
[

AC_MSG_CHECKING([whether the DBASE driver will be installed])
if test "$want_dbase" = "yes"; then
NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_dbaseclasses"
AC_MSG_RESULT(yes)
else
AC_MSG_RESULT(no)
fi

])






AC_DEFUN([HK_FIREBIRDTEST],
[

if test "$want_firebird" = "yes"; then
AC_MSG_CHECKING([whether FIREBIRD is available])
if test "$with_firebird_dir" = ""; then
  if test "$want_64bit" = "yes"; then
   FIREBIRDLIBDIR=/opt/firebird/lib64
   else
   FIREBIRDLIBDIR=/opt/firebird/lib
   fi
   FIREBIRDINCDIR=/opt/firebird/include
else FIREBIRDLIBDIR=$with_firebird_dir/lib
FIREBIRDINCDIR=$with_firebird_dir/include
fi

if test "x$with_firebird_incdir" != "x"; then
 FIREBIRDINCDIR=$with_firebird_incdir
fi

if test "x$with_firebird_libdir" != "x"; then
 FIREBIRDLIBDIR=$with_firebird_libdir
fi

ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

CXXLIBS="$ac_save_CXXLIBS -L${FIREBIRDLIBDIR}"
LIBS="$ac_save_LIBS -L${FIREBIRDLIBDIR}  -lfbclient "
CPPFLAGS="$ac_save_CPPFLAGS -I .  -I $FIREBIRDINCDIR  "

AC_TRY_LINK([
#include <ibase.h>
],[
isc_db_handle   newdb;
;],[firebird_lib=yes],[firebird_lib=no])

AC_MSG_RESULT($firebird_lib)
if test $firebird_lib = yes ; then
NODRIVER=false
DRIVERSDIR="$DRIVERSDIR hk_firebirdclasses"
AC_SUBST(FIREBIRDLIBDIR)
AC_SUBST(FIREBIRDINCDIR)
fi


LIBS="$ac_save_LIBS"
CPPFLAGS="$ac_save_CPPFLAGS"
CXXLIBS="$ac_save_CXXLIBS"
fi

])



AC_DEFUN([HK_FIREBIRD2TEST],
[

if test "$want_firebird" = "yes"; then
AC_MSG_CHECKING([whether FIREBIRD2 is available])
if test "$with_firebird_dir" = ""; then
  if test "$want_64bit" = "yes"; then
   FIREBIRDLIBDIR=/opt/firebird/lib64
   else
   FIREBIRDLIBDIR=/opt/firebird/lib
   fi
   FIREBIRDINCDIR=/opt/firebird/include
else FIREBIRDLIBDIR=$with_firebird_dir/lib
FIREBIRDINCDIR=$with_firebird_dir/include
fi

if test "x$with_firebird_incdir" != "x"; then
 FIREBIRDINCDIR=$with_firebird_incdir
fi

if test "x$with_firebird_libdir" != "x"; then
 FIREBIRDLIBDIR=$with_firebird_libdir
fi

ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

CXXLIBS="$ac_save_CXXLIBS -L${FIREBIRDLIBDIR}"
LIBS="$ac_save_LIBS -L${FIREBIRDLIBDIR}  -lfbclient "
CPPFLAGS="$ac_save_CPPFLAGS -I .  -I $FIREBIRDINCDIR  "

AC_TRY_LINK([
#include <ibase.h>
],[
isc_db_handle   newdb;
  const ISC_STATUS* pvector;
  char msg[512];
  fb_interpret(msg,512,&pvector);

;],[firebird2_lib=yes],[firebird2_lib=no])

AC_MSG_RESULT($firebird2_lib)
if test $firebird2_lib = yes ; then
AC_DEFINE(HAVE_FIREBIRD2,1,Defines whether Firebird 2.x is installed)
fi


LIBS="$ac_save_LIBS"
CPPFLAGS="$ac_save_CPPFLAGS"
CXXLIBS="$ac_save_CXXLIBS"
fi

])



AC_DEFUN([HK_CHECK_FONTCONFIG],
[
AC_MSG_CHECKING([whether FONTCONFIG is available])
if test "$with_fontconfig_dir" = ""; then
  if test "$want_64bit" = "yes"; then
   FONTCONFIGLIBDIR=/usr/lib64
  else
   FONTCONFIGLIBDIR=/usr/lib
  fi
   FONTCONFIGINCDIR=/usr/include/fontconfig
else FONTCONFIGLIBDIR=$with_fontconfig_dir/lib
FONTCONFIGINCDIR=$with_fontconfig_dir/include
fi

if test "x$with_fontconfig_incdir" != "x"; then
 FONTCONFIGINCDIR=$with_fontconfig_incdir
fi

if test "x$with_fontconfig_libdir" != "x"; then
 FONTCONFIGLIBDIR=$with_fontconfig_libdir
fi

ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

CXXLIBS="$ac_save_CXXLIBS -L${FONTCONFIGLIBDIR}"
LIBS="$ac_save_LIBS -L${FONTCONFIGLIBDIR}  -lfontconfig "
CPPFLAGS="$ac_save_CPPFLAGS -I .  -I $FONTCONFIGINCDIR  "

AC_TRY_LINK([
#include <fontconfig.h>
],[

FcInit();
FcFontSet* fs;
 FcObjectSet *os = 0;
 FcChar8* fontformat;
 FcPattern* pat= FcPatternCreate ();
    
 os = FcObjectSetBuild (FC_FAMILY, FC_STYLE, (char *) 0);
 fs = FcFontList (0, pat, os);
 
;],[fontconfig_lib=yes],[fontconfig_lib=no])

AC_MSG_RESULT($fontconfig_lib)


if test ! "$fontconfig_lib" = yes ; then
  AC_MSG_ERROR(
  fontconfig not available or the version is too old!!!
  ============================================================================
   ERROR!
   You probably have to install the fontconfig-devel.rpm of your distribution.
   The exact name of this rpm is distribution specific.
  ============================================================================

  )
fi
AC_SUBST(FONTCONFIGLIBDIR)
AC_SUBST(FONTCONFIGINCDIR)

CXXLIBS="$ac_save_CXXLIBS "
LIBS="$ac_save_LIBS "
CPPFLAGS="$ac_save_CPPFLAGS "


])





AC_DEFUN([HK_CHECK_FREETYPE],
[
AC_MSG_CHECKING([whether FREETYPE is available])
   FREETYPEINC=`pkg-config freetype2 --cflags`
   FREETYPELIB=`pkg-config freetype2 --libs`


ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

CXXLIBS="$ac_save_CXXLIBS "
LIBS="$ac_save_LIBS   $FREETYPELIB "
CPPFLAGS="$ac_save_CPPFLAGS -I .  $FREETYPEINC  "

AC_TRY_LINK([
#include <ft2build.h>
#include FT_FREETYPE_H
],[
FT_Library  ftlibrary;
;],[freetype_lib=yes],[freetype_lib=no])

AC_MSG_RESULT($freetype_lib)


if test ! "$freetype_lib" = yes ; then
  AC_MSG_ERROR(
  freetype not available!!!
  ============================================================================
   ERROR!
   You probably have to install the freetype-devel.rpm of your distribution.
   The exact name of this rpm is distribution specific.
  ============================================================================

  )
fi
CXXLIBS="$ac_save_CXXLIBS "
LIBS="$ac_save_LIBS "
CPPFLAGS="$ac_save_CPPFLAGS "
AC_SUBST(FREETYPELIB)
AC_SUBST(FREETYPEINC)

])

AC_DEFUN([HK_CHECK_LIBXML2],[
AC_MSG_CHECKING([whether LIBXML2 is available])
XMLLIB=`xml2-config --libs`
XMLINCLUDE=`xml2-config --cflags|cut -b 3-`

ac_save_LIBS="$LIBS"
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_CXXLIBS="$CXXLIBS"

CXXLIBS="$ac_save_CXXLIBS "
LIBS="$ac_save_LIBS   $XMLLIB "
CPPFLAGS="$ac_save_CPPFLAGS -I .  -I$XMLINCLUDE  "


AC_TRY_LINK([
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>],[
xmlDocPtr doc;],[xml2_lib=yes],[xml2_lib=no])

AC_MSG_RESULT($xml2_lib)

if test ! "$xml2_lib" = yes ; then
  AC_MSG_ERROR(
  libxml2 not available!!!
  ============================================================================
   ERROR!
   You probably have to install the libxml2-devel.rpm of your distribution.
   The exact name of this rpm is distribution specific.
  ============================================================================

  )
fi

CXXLIBS="$ac_save_CXXLIBS "
LIBS="$ac_save_LIBS "
CPPFLAGS="$ac_save_CPPFLAGS "

AC_SUBST(XMLLIB)
AC_SUBST(XMLINCLUDE)

])

dnl @synopsis AC_PYTHON_DEVEL([version])
dnl
dnl Checks for Python and tries to get the include path to 'Python.h'.
dnl It provides the $(PYTHON_CPPFLAGS) and $(PYTHON_LDFLAGS) output
dnl variables.
dnl Also exports $(PYTHON_EXTRA_LIBS) and $(PYTHON_EXTRA_LDFLAGS)
dnl for embedding Python in your code.
dnl
dnl You can search for some particular version of Python by passing a
dnl parameter to this macro, for example ">= '2.3.1'", or "== '2.4'".
dnl Please note that you *have* to pass also an operator along
dnl with the version to match, and pay special attention to the 
dnl single quotes surrounding the version number.
dnl
dnl If the user wants to employ a particular version of Python, she can
dnl now pass to configure the PYTHON_VERSION environment variable.
dnl This is only limited by the macro parameter set by the packager.
dnl
dnl This macro should work for all versions of Python >= 2.1.0. You can 
dnl disable the check for the python version by setting the 
dnl PYTHON_NOVERSIONCHECK environment variable to something else
dnl than the empty string.
dnl
dnl If you need to use this macro for an older Python version, please 
dnl contact the authors. We're always open for feedback.
dnl
dnl @category InstalledPackages
dnl @author Sebastian Huber <sebastian-huber@web.de>
dnl @author Alan W. Irwin <irwin@beluga.phys.uvic.ca>
dnl @author Rafael Laboissiere <laboissiere@psy.mpg.de>
dnl @author Andrew Collier <colliera@nu.ac.za>
dnl @author Matteo Settenvini <matteo@member.fsf.org>
dnl @author Horst Knorr <hk_classes@knoda.org> 
dnl @version 2006-02-05
dnl @license GPLWithACException

AC_DEFUN([AC_PYTHON_DEVEL],[
	#
	# Allow the use of a (user set) custom python version
	#
	AC_ARG_VAR([PYTHON_VERSION],[The installed Python
		version to use, for example '2.3'. This string 
		will be appended to the Python interpreter
		canonical name.])

	AC_PATH_PROG([PYTHON],[python[$PYTHON_VERSION]])
	if test -z "$PYTHON"; then
	   AC_MSG_ERROR([Cannot find python$PYTHON_VERSION in your system path])
	fi

	#
	# Check for a version of Python >= 2.1.0
	#
	AC_MSG_CHECKING([for a version of Python >= '2.1.0'])
	ac_supports_python_ver=`$PYTHON -c "import sys, string; \
		ver = string.split(sys.version)[[0]]; \
		print ver >= '2.1.0'"`
	if test "$ac_supports_python_ver" != "True" -a  "$ac_supports_python_ver" != "1"; then
		if test -z "$PYTHON_NOVERSIONCHECK"; then
			AC_MSG_RESULT([no])
			AC_MSG_FAILURE([
This version of the AC@&t@_PYTHON_DEVEL macro
doesn't work properly with versions of Python before
2.1.0. You may need to re-run configure, setting the
variables PYTHON_CPPFLAGS, PYTHON_LDFLAGS, PYTHON_SITE_PKG,
PYTHON_EXTRA_LIBS and PYTHON_EXTRA_LDFLAGS by hand.
Moreover, to disable this check, set PYTHON_NOVERSIONCHECK
to something else than an empty string.
])dnl
		else
			AC_MSG_RESULT([skip at user request])
		fi
	else 
		AC_MSG_RESULT([yes])
	fi
	
	#
	# if the macro parameter ``version'' is set, honour it
	#
	if test -n "$1"; then
		AC_MSG_CHECKING([for a version of Python $1])
		ac_supports_python_ver=`$PYTHON -c "import sys, string; \
			ver = string.split(sys.version)[[0]]; \
			print ver $1"`
		if test "$ac_supports_python_ver" = "True" -a "$ac_supports_python_ver" = "1"; then
	   	   AC_MSG_RESULT([yes])
		else
			AC_MSG_RESULT([no])
			AC_MSG_ERROR([this package requires Python $1. 
If you have it installed, but it isn't the default Python
interpreter in your system path, please pass the PYTHON_VERSION 
variable to configure. See ``configure --help'' for reference.
])dnl
		fi
	fi

	#
	# Check if you have distutils, else fail
	#
	AC_MSG_CHECKING([for the distutils Python package])
	ac_distutils_result=`$PYTHON -c "import distutils" 2>&1`
	if test -z "$ac_distutils_result"; then
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
		AC_MSG_ERROR([cannot import Python module "distutils".
Please check your Python installation. The error was:
$ac_distutils_result])
	fi

	#
	# Check for Python include path
	#
	AC_MSG_CHECKING([for Python include path])
	if test -z "$PYTHON_CPPFLAGS"; then
		python_path=`$PYTHON -c "import distutils.sysconfig; \
           		print distutils.sysconfig.get_python_inc();"`
		if test -n "${python_path}"; then
		   	python_path="-I$python_path"
		fi
		PYTHON_CPPFLAGS=$python_path
	fi
	if test "$with_python_incdir" != ""; then
	PYTHON_CPPFLAGS="-I$with_python_incdir "
	fi
	AC_MSG_RESULT([$PYTHON_CPPFLAGS])
	AC_SUBST([PYTHON_CPPFLAGS])

	#
	# Check for Python library path
	#
	AC_MSG_CHECKING([for Python library path])
	if test -z "$PYTHON_LDFLAGS"; then
		# (makes two attempts to ensure we've got a version number
		# from the interpreter)
		py_version=`$PYTHON -c "from distutils.sysconfig import *; \
			from string import join; \
			print join(get_config_vars('VERSION'))"`
		if test "$py_version" == "[None]"; then
			if test -n "$PYTHON_VERSION"; then
				py_version=$PYTHON_VERSION
			else
				py_version=`$PYTHON -c "import sys; \
					print sys.version[[:3]]"`
			fi
		fi

		PYTHON_LDFLAGS=`$PYTHON -c "from distutils.sysconfig import *; \
			from string import join; \
			print '-L' + get_python_lib(0,1), \
		      	'-lpython';"`$py_version
	fi		
	if test "$with_python_libdir" != ""; then
	PYTHON_LDFLAGS="-L$with_python_libdir -lpython$py_version"
	fi
	AC_MSG_RESULT([$PYTHON_LDFLAGS])
	AC_SUBST([PYTHON_LDFLAGS])

	#
	# Check for site packages
	#
	AC_MSG_CHECKING([for Python site-packages path])
	if test -z "$PYTHON_SITE_PKG"; then
		PYTHON_SITE_PKG=`$PYTHON -c "import distutils.sysconfig; \
		        print distutils.sysconfig.get_python_lib(0,0);"`
	fi
	AC_MSG_RESULT([$PYTHON_SITE_PKG])
	AC_SUBST([PYTHON_SITE_PKG])

	#
	# libraries which must be linked in when embedding
	#
	AC_MSG_CHECKING(python extra libraries)
	if test -z "$PYTHON_EXTRA_LIBS"; then
	   PYTHON_EXTRA_LIBS=`$PYTHON -c "import distutils.sysconfig; \
                conf = distutils.sysconfig.get_config_var; \
                print conf('LOCALMODLIBS'), conf('LIBS')"`
	fi
	AC_MSG_RESULT([$PYTHON_EXTRA_LIBS])
	AC_SUBST(PYTHON_EXTRA_LIBS)

	#
	# linking flags needed when embedding
	#
	AC_MSG_CHECKING(python extra linking flags)
	if test -z "$PYTHON_EXTRA_LDFLAGS"; then
		PYTHON_EXTRA_LDFLAGS=`$PYTHON -c "import distutils.sysconfig; \
			conf = distutils.sysconfig.get_config_var; \
			print conf('LINKFORSHARED')"`
	fi
	AC_MSG_RESULT([$PYTHON_EXTRA_LDFLAGS])
	AC_SUBST(PYTHON_EXTRA_LDFLAGS)

	#	
	# final check to see if everything compiles alright
	#
	AC_MSG_CHECKING([whether collected informations are consistent])
	AC_LANG_PUSH([C])
	# save current global flags
	LIBS="$ac_save_LIBS $PYTHON_LDFLAGS  $PYTHON_EXTRA_LDFLAGS $PYTHON_EXTRA_LIBS "
	CPPFLAGS="$ac_save_CPPFLAGS $PYTHON_CPPFLAGS"
	AC_TRY_LINK([
		#include <Python.h>
	],[
		Py_Initialize();
	],[pythonexists=yes],[pythonexists=no])

	AC_MSG_RESULT([$pythonexists])

        if test ! "$pythonexists" = "yes"; then
	   AC_MSG_ERROR([
  Could not link test program to Python. Maybe the main Python library has been
  installed in some non-standard library path. If so, pass it to configure, 
  via the LDFLAGS environment variable,
  or use the --with-python-incdir and --with-python-libdir directices respectively.
  ============================================================================
   ERROR!
   You probably have to install the development version of the Python package
   for your distribution.  The exact name of this package varies among them.
  ============================================================================
	   ])
	fi
	AC_LANG_POP
	# turn back to default flags
	CPPFLAGS="$ac_save_CPPFLAGS"
	LIBS="$ac_save_LIBS"
	AC_DEFINE(HAVE_PYTHON, 1, [Define if you have the development files for python])

	#
	# all done!
	#
])




