libdir=@HK_CLASSESDIR@/drivers
INCLUDES= -I../hk_classes -I ./mdbtools 

AM_CPPFLAGS=$(GLIB_CFLAGS) 

AM_CXXFLAGS= -Wall -I@XMLINCLUDE@ @BITARCHITECTURE@

libhk_mdbdriver_la_LDFLAGS = -version-info 0:0:0  $(GLIB_LIBADD)  $(GLIB_LDFLAGS)
libhk_mdbdriver_la_LIBADD =   ../hk_classes/libhk_classes.la $(GLIB_LIBADD) $(GLIB_LDFLAGS)

lib_LTLIBRARIES = 	libhk_mdbdriver.la

noinst_HEADERS = hk_mdbconnection.h hk_mdbdatabase.h hk_mdbdatasource.h hk_mdbcolumn.h\
				 hk_mdbresultquery.h hk_mdbtable.h mdbtools/mdbtools.h



include_HEADERS =
libhk_mdbdriver_la_SOURCES= 	 hk_mdbconnection.cpp hk_mdbdatabase.cpp hk_mdbdatasource.cpp hk_mdbcolumn.cpp\
				 hk_mdbresultquery.cpp hk_mdbtable.cpp \
				 mdbtools/catalog.c mdbtools/mem.c mdbtools/file.c mdbtools/kkd.c mdbtools/table.c mdbtools/data.c mdbtools/dump.c mdbtools/backend.c mdbtools/money.c mdbtools/sargs.c mdbtools/index.c mdbtools/like.c mdbtools/write.c mdbtools/stats.c mdbtools/map.c mdbtools/props.c mdbtools/worktable.c mdbtools/options.c mdbtools/iconv.c

EXTRA_DIST =  mdbtools/AUTHORS
