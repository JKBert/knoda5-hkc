SUBDIRS =  hk_classes  documentation  python @DRIVERSDIR@  @UTILITIESDIR@

EXTRA_DIST = misc/acinclude.m4.hk_classes config.h doxygen.config misc/systemtest.py

AUTOMAKE_OPTIONS= dist-bzip2


doc:
	(doxygen doxygen.config)

check:
	$(PYTHON) misc/systemtest.py

install-data-hook:
	@echo ; echo ; echo "**********************************************"; echo ; \
	if @NODRIVER@ ;then echo "W A R N I N G    !!!!" ;	 \
	echo "No database driver installed! It makes no sense to run " ; echo "hk_classes without a database driver."; echo "Make sure that you have one of the following SQL databases." ; echo ; echo "Existing drivers are: @POSSIBLE_DRIVERS@" ; echo ; echo "Then re-configure and re-make this package." ;\
	echo;echo "If you are sure the database is installed, but you still see this message,";	echo "check your installation. Probably you haven't installed your distribution's ";echo "development package for this database. For Mysql it is called ";echo "mysql-devel (the exact name is distribution specific)";\
	else echo "The following drivers are installed:"; echo "@DRIVERSDIR@" ;fi;echo;  echo "I wish you much fun";echo ; echo "Horst Knorr <hk_classes@knoda.org>";  echo; echo "**********************************************"; echo ;
