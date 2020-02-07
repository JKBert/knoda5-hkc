libdir=@HK_CLASSESDIR@/drivers
INCLUDES= -I../hk_classes -I@ODBCINCDIR@ 
AM_CXXFLAGS= -Wall -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -I@XMLINCLUDE@ @BITARCHITECTURE@

libhk_odbcdriver_la_LDFLAGS = -version-info 0:0:0 -L@ODBCLIBDIR@
libhk_odbcdriver_la_LIBADD = -l@ODBCLIBRARY@ ../hk_classes/libhk_classes.la

lib_LTLIBRARIES = 	libhk_odbcdriver.la

noinst_HEADERS = 	hk_odbccolumn.h hk_odbcconnection.h hk_odbcdatabase.h\
			hk_odbcdatasource.h hk_odbcactionquery.h hk_odbcresultquery.h\
			hk_odbctable.h hk_odbcview.h


include_HEADERS =
libhk_odbcdriver_la_SOURCES= 	hk_odbccolumn.cpp hk_odbcconnection.cpp hk_odbcdatabase.cpp\
				hk_odbcdatasource.cpp hk_odbcactionquery.cpp hk_odbcresultquery.cpp\
				hk_odbctable.cpp hk_odbcview.cpp
