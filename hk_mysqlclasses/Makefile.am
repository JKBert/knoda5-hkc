libdir=@HK_CLASSESDIR@/drivers
INCLUDES= -I../hk_classes -I@MYSQLINCDIR@

AM_CXXFLAGS= -Wall -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -I@XMLINCLUDE@ @BITARCHITECTURE@

libhk_mysqldriver_la_LDFLAGS = -version-info 3:4:0 -L@MYSQLLIBDIR@
libhk_mysqldriver_la_LIBADD = -lmysqlclient ../hk_classes/libhk_classes.la

lib_LTLIBRARIES = 	libhk_mysqldriver.la

noinst_HEADERS = 	hk_mysqlcolumn.h hk_mysqlconnection.h hk_mysqldatabase.h\
			hk_mysqldatasource.h hk_mysqlactionquery.h hk_mysqlresultquery.h\
			hk_mysqltable.h hk_mysqlview.h


include_HEADERS =
libhk_mysqldriver_la_SOURCES= 	hk_mysqlcolumn.cpp hk_mysqlconnection.cpp hk_mysqldatabase.cpp\
				hk_mysqldatasource.cpp hk_mysqlactionquery.cpp hk_mysqlresultquery.cpp\
				hk_mysqltable.cpp hk_mysqlview.cpp
