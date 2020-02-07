libdir=@HK_CLASSESDIR@/drivers
INCLUDES= -I../hk_classes -I@FIREBIRDINCDIR@

AM_CPPFLAGS=-W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings

AM_CXXFLAGS= -Wall -I@XMLINCLUDE@ @BITARCHITECTURE@

libhk_firebirddriver_la_LDFLAGS = -version-info 0:0:0 -L@FIREBIRDLIBDIR@
libhk_firebirddriver_la_LIBADD = -lfbclient ../hk_classes/libhk_classes.la

lib_LTLIBRARIES = 	libhk_firebirddriver.la

noinst_HEADERS = hk_firebirdactionquery.h  hk_firebirdconnection.h hk_firebirddatabase.h \
				 hk_firebirddatasource.h hk_firebirdcolumn.h\
				 hk_firebirdresultquery.h hk_firebirdtable.h hk_firebirdview.h



include_HEADERS =
libhk_firebirddriver_la_SOURCES= 	hk_firebirdactionquery.cpp hk_firebirdconnection.cpp hk_firebirddatabase.cpp hk_firebirddatasource.cpp hk_firebirdcolumn.cpp\
				 hk_firebirdresultquery.cpp hk_firebirdtable.cpp hk_firebirdview.cpp
