libdir=@HK_CLASSESDIR@
includedir=@HK_CLASSESINCDIR@

libhk_classes_la_LDFLAGS = -version-info 15:0:0 -lfontconfig @LTLIBICONV@
AM_CXXFLAGS= -Wall  @PYTHON_CPPFLAGS@  -fPIC -I@XMLINCLUDE@ -I@FONTCONFIGINCDIR@ @FREETYPEINC@ @BITARCHITECTURE@

SUBDIRS = .


lib_LTLIBRARIES=libhk_classes.la




include_HEADERS = hk_string.h hk_database.h  hk_connection.h hk_datasource.h hk_column.h hk_class.h hk_definitions.h\
		hk_storagedatasource.h hk_storagecolumn.h \
		hk_data.h hk_visible.h hk_dsvisible.h hk_dsdatavisible.h hk_dsmodevisible.h hk_actionquery.h hk_dsgrid.h \
		hk_dsgridcolumn.h  hk_dsquery.h hk_datetime.h hk_classes.h hk_label.h\
		hk_referentialintegrity.h hk_report.h hk_reportsectionpair.h hk_reportsection.h hk_reportdata.h hk_reportutils.h \
		hk_importcsv.h hk_form.h  hk_presentation.h hk_dslineedit.h \
		hk_dsboolean.h hk_dscombobox.h hk_dsmemo.h hk_button.h hk_dsrowselector.h hk_dstable.h hk_dbvisible.h \
		hk_drivermanager.h hk_colour.h hk_font.h hk_interpreter.h hk_qbe.h hk_subform.h  hk_url.h hk_key.h \
		hk_dsdate.h hk_tabvisible.h hk_dsimage.h hk_mimetype.h hk_module.h

noinst_HEADERS = hk_utilities.h hk_stdint.h

libhk_classes_la_LIBADD =$(PYTHON_LDFLAGS) $(PYTHON_EXTRA_LDFLAGS) $(PYTHON_EXTRA_LIBS) -L$(FONTCONFIGLIBDIR) $(FREETYPELIB) @XMLLIB@

libhk_classes_la_CPPFLAGS= -DHKCLASSES=\"$(HK_CLASSESDIR)\" -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings


libhk_classes_la_SOURCES= \
	hk_actionquery.cpp  \
	hk_button.cpp \
	hk_class.cpp hk_colour.cpp hk_column.cpp hk_connection.cpp\
	hk_data.cpp hk_database.cpp hk_datasource.cpp hk_datetime.cpp  \
	hk_dbvisible.cpp hk_definitions.cpp hk_drivermanager.cpp\
	hk_dsboolean.cpp hk_dscombobox.cpp hk_dsdatavisible.cpp hk_dsdate.cpp hk_dsgrid.cpp hk_dsgridcolumn.cpp \
	hk_dsimage.cpp hk_dslineedit.cpp  hk_dsmemo.cpp hk_dsmodevisible.cpp hk_dsquery.cpp hk_dstable.cpp\
	hk_dsrowselector.cpp hk_dsvisible.cpp\
	hk_font.cpp hk_form.cpp\
	hk_importcsv.cpp hk_interpreter.cpp \
	hk_key.cpp hk_label.cpp  hk_mimetype.cpp hk_module.cpp\
	hk_presentation.cpp hk_qbe.cpp\
	hk_referentialintegrity.cpp hk_report.cpp hk_reportdata.cpp hk_reportsectionpair.cpp hk_reportsection.cpp \
	hk_reportutils.cpp\
	hk_subform.cpp hk_storagedatasource.cpp hk_storagecolumn.cpp hk_string.cpp\
	hk_tabvisible.cpp hk_url.cpp hk_visible.cpp

