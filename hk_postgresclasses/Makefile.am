libdir=@HK_CLASSESDIR@/drivers
INCLUDES=   -I../hk_classes -I@POSTGRESINCDIR@

AM_CXXFLAGS= -Wall -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -I@XMLINCLUDE@ @BITARCHITECTURE@

libhk_postgresdriver_la_LDFLAGS = -version-info 0:2:0 -L@POSTGRESLIBDIR@
libhk_postgresdriver_la_LIBADD = -lpq ../hk_classes/libhk_classes.la

lib_LTLIBRARIES =libhk_postgresdriver.la

include_HEADERS =
noinst_HEADERS =  	hk_postgresqlcolumn.h hk_postgresqlconnection.h hk_postgresqldatabase.h\
			hk_postgresqldatasource.h hk_postgresqlactionquery.h hk_postgresqlresultquery.h\
			hk_postgresqltable.h hk_postgresqlview.h


libhk_postgresdriver_la_SOURCES= 	hk_postgresqlcolumn.cpp hk_postgresqlconnection.cpp hk_postgresqldatabase.cpp\
				hk_postgresqldatasource.cpp hk_postgresqlactionquery.cpp hk_postgresqlresultquery.cpp\
				hk_postgresqltable.cpp hk_postgresqlview.cpp
