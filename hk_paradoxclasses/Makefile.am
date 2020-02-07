libdir=@HK_CLASSESDIR@/drivers
INCLUDES= -I../hk_classes -I./pxlib

AM_CPPFLAGS=

AM_CXXFLAGS= -Wall -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -I@XMLINCLUDE@ @BITARCHITECTURE@

PXLIB_BUILD_DATE = 2006-02-28


libhk_paradoxdriver_la_LDFLAGS = -version-info 0:0:0 
libhk_paradoxdriver_la_LIBADD = -lm ../hk_classes/libhk_classes.la

lib_LTLIBRARIES = 	libhk_paradoxdriver.la

noinst_HEADERS = hk_paradoxconnection.h hk_paradoxconnection.h hk_paradoxdatabase.h hk_paradoxdatasource.h \
		 hk_paradoxcolumn.h hk_paradoxresultquery.h hk_paradoxtable.h hk_paradoxactionquery.h \
		 pxlib/fileformat.h   pxlib/paradox.h     pxlib/px_crypt.h   pxlib/px_error.h  pxlib/px_intern.h \
		 pxlib/px_memory.h  pxlib/pxversion.h  pxlib/paradox-gsf.h  pxlib/paradox-mp.h  pxlib/px_encode.h \
		 pxlib/px_head.h   pxlib/px_io.h      pxlib/px_misc.h    pxlib/sdncal.h



include_HEADERS =

libhk_paradoxdriver_la_SOURCES= 	 hk_paradoxconnection.cpp hk_paradoxdatabase.cpp hk_paradoxdatasource.cpp \
				 hk_paradoxcolumn.cpp hk_paradoxresultquery.cpp hk_paradoxtable.cpp hk_paradoxactionquery.cpp \
				 pxlib/gregor.c   pxlib/px_crypt.c   pxlib/px_error.c  pxlib/px_io.c   \
				 pxlib/px_memprof.c pxlib/paradox.c  pxlib/px_encode.c  pxlib/px_head.c  \
				 pxlib/px_memory.c  pxlib/px_misc.c 


EXTRA_DIST =  pxlib/AUTHORS

