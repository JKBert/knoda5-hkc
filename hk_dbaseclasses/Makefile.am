libdir=@HK_CLASSESDIR@/drivers
INCLUDES= -I$(top_srcdir)/hk_classes 

AM_CPPFLAGS=

AM_CXXFLAGS= -Wall -W  -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -I@XMLINCLUDE@ @BITARCHITECTURE@

libhk_dbasedriver_la_LDFLAGS = -version-info 0:0:0  -L../hk_classes -lhk_classes 

lib_LTLIBRARIES = 	libhk_dbasedriver.la

noinst_HEADERS = hk_dbaseactionquery.h hk_dbaseconnection.h	  hk_dbaseconnection.h hk_dbasedatabase.h hk_dbasedatasource.h hk_dbasecolumn.h\
				 hk_dbaseresultquery.h hk_dbasetable.h \
				 xbase/xbase64.h    xbase/xbdbf.h  xbase/xbfile.h    xbase/xbindex.h  xbase/xbmindex.h  xbase/xbnode.h \
 				 xbase/xbretcod.h  xbase/xbtypes.h xbase/xbcdx.h    xbase/xbdate.h    xbase/xbexp.h  xbase/xbfilter.h  xbase/xblock.h  \
 				 xbase/xbndx.h     xbase/xbntx.h   xbase/xbstring.h  


libhk_dbasedriver_la_SOURCES= 	 hk_dbaseactionquery.cpp hk_dbaseconnection.cpp hk_dbasedatabase.cpp hk_dbasedatasource.cpp hk_dbasecolumn.cpp\
				 hk_dbaseresultquery.cpp hk_dbasetable.cpp \
				 xbase/xbase64.cpp  xbase/xbdbf.cpp     xbase/xbexpprc.cpp  xbase/xbfilter.cpp  xbase/xbmemo.cpp  xbase/xbntx.cpp \
 				 xbase/xbcdx.cpp    xbase/xbexp.cpp     xbase/xbfields.cpp  xbase/xbindex.cpp   xbase/xbndx.cpp   xbase/xbstring.cpp \
 				xbase/xbdate.cpp   xbase/xbexpfnc.cpp  xbase/xbfile.cpp    xbase/xblock.cpp    xbase/xbnode.cpp

