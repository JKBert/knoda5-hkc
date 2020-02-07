INCLUDES= -I../hk_classes

bin_PROGRAMS = hk_actionquery  hk_dbcopy hk_exportcsv hk_exporthtml hk_exportxml \
		hk_importcsv hk_report 
AM_CXXFLAGS= -Wall   -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -I@XMLINCLUDE@ @BITARCHITECTURE@

hk_report_LDADD = ../hk_classes/libhk_classes.la -ldl
hk_report_SOURCES = hk_reportutility.cpp

#hk_webform_LDADD = ../hk_classes/libhk_classes.la -ldl
#hk_webform_SOURCES = hk_webformutility.cpp

hk_actionquery_LDADD = ../hk_classes/libhk_classes.la -ldl
hk_actionquery_SOURCES = hk_actionqueryutility.cpp

hk_exportxml_LDADD = ../hk_classes/libhk_classes.la -ldl
hk_exportxml_SOURCES = hk_exportxmlutility.cpp

hk_exportcsv_LDADD = ../hk_classes/libhk_classes.la -ldl
hk_exportcsv_SOURCES = hk_exportcsvutility.cpp

hk_exporthtml_LDADD = ../hk_classes/libhk_classes.la -ldl
hk_exporthtml_SOURCES = hk_exporthtmlutility.cpp

hk_importcsv_LDADD = ../hk_classes/libhk_classes.la -ldl
hk_importcsv_SOURCES = hk_importcsvutility.cpp

hk_dbcopy_LDFLAGS = -lhk_classes -ldl -L../hk_classes
hk_dbcopy_SOURCES = hk_dbcopyutility.cpp

man_MANS = hk_report.1man hk_actionquery.1man hk_dbcopy.1man hk_exportxml.1man \
	hk_exporthtml.1man hk_exportcsv.1man hk_importcsv.1man

EXTRA_DIST = $(man_MANS)

