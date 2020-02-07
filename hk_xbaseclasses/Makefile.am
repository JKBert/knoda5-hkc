libdir=@HK_CLASSESDIR@/drivers
INCLUDES= -I../hk_classes -I@XBASEINCDIR@ 

AM_CPPFLAGS=

AM_CXXFLAGS=  -Wall -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -I@XMLINCLUDE@ @BITARCHITECTURE@

libhk_xbasedriver_la_LDFLAGS = -version-info 0:0:0 -L@XBASELIBDIR@
libhk_xbasedriver_la_LIBADD = -lxbsql ../hk_classes/libhk_classes.la

lib_LTLIBRARIES = 	libhk_xbasedriver.la

noinst_HEADERS = hk_xbaseconnection.h	 hk_xbaseactionquery.h hk_xbaseconnection.h \
		 hk_xbasedatabase.h hk_xbasedatasource.h hk_xbasecolumn.h \
				 hk_xbaseresultquery.h hk_xbasetable.h



include_HEADERS =	 

libhk_xbasedriver_la_SOURCES= 	 hk_xbaseactionquery.cpp hk_xbaseconnection.cpp hk_xbasedatabase.cpp \
                                 hk_xbasedatasource.cpp hk_xbasecolumn.cpp \
				 hk_xbaseresultquery.cpp hk_xbasetable.cpp
