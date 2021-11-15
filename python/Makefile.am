all:   clean
	@if [ x$(srcdir) != x$(builddir) ]; then \
		ln -sf $(srcdir)/hk_classes_wrap.cxx $(builddir); \
		ln -sf $(srcdir)/hk_classes.py $(builddir); \
	fi
	python setup.py build
clean:
	/bin/rm -f -R ./build/*


swig: hk_classes.i hk_class.i hk_string.i hk_drivermanager.i hk_connection.i hk_database.i hk_datasource.i\
			hk_column.i hk_visible.i hk_dsvisible.i hk_dsdatavisible.i
	swig -c++ -python -I$(srcdir)/../hk_classes  hk_classes.i 

install-data-hook:
	if test -z "$(DESTDIR)"; then \
	   python setup.py install ; \
	else \
 	   python setup.py install --root $(DESTDIR) ; \
	fi
	/bin/rm -f -R ./build/*

uninstall-hook:
	python setup.py uninstall
EXTRA_DIST = hk_classes_wrap.cxx hk_classes.py
