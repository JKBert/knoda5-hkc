libdir=@HK_CLASSESDIR@/drivers

AM_CXXFLAGS= -Wall -DNO_TCL=1  -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -I@XMLINCLUDE@ @BITARCHITECTURE@
AM_CFLAGS= -DNO_TCL=1


libhk_sqlite2driver_la_LIBADD =  ../hk_classes/libhk_classes.la

lib_LTLIBRARIES = 	libhk_sqlite2driver.la

noinst_HEADERS = 	hk_sqlitecolumn.h hk_sqliteconnection.h hk_sqlitedatabase.h\
			hk_sqlitedatasource.h hk_sqliteactionquery.h hk_sqliteresultquery.h\
			hk_sqlitetable.h hk_sqliteview.h \
			sqlite/btree.h    sqlite/os.h      sqlite/sqliteInt.h \
			sqlite/config.h   sqlite/pager.h   sqlite/vdbe.h \
			sqlite/hash.h     sqlite/parse.h   sqlite/vdbeInt.h \
			sqlite/opcodes.h  sqlite/sqlite.h



normal_src = 	hk_sqlitecolumn.cpp hk_sqliteconnection.cpp hk_sqlitedatabase.cpp\
				hk_sqlitedatasource.cpp hk_sqliteactionquery.cpp hk_sqliteresultquery.cpp\
				hk_sqlitetable.cpp hk_sqliteview.cpp

extra_src = sqlite/attach.c     \
				sqlite/auth.c              \
				sqlite/btree.c   sqlite/main.c           \
				sqlite/btree_rb.c  sqlite/os.c          \
				sqlite/build.c     sqlite/pager.c      sqlite/tokenize.c \
				sqlite/copy.c      sqlite/parse.c      sqlite/trigger.c \
				sqlite/date.c      sqlite/pragma.c     sqlite/update.c \
				sqlite/delete.c    sqlite/printf.c     sqlite/util.c \
				sqlite/encode.c    sqlite/random.c     sqlite/vacuum.c \
				sqlite/expr.c      sqlite/select.c     sqlite/vdbeaux.c \
				sqlite/func.c      sqlite/shell.c      sqlite/vdbe.c \
				sqlite/hash.c      sqlite/table.c      sqlite/where.c \
				sqlite/insert.c    sqlite/tclsqlite.c  sqlite/opcodes.c

if USESQLITE2LIBRARY
 INCLUDES= -I../hk_classes -I@SQLITEINCDIR@
 libhk_sqlite2driver_la_LDFLAGS = -version-info 0:0:0 -lsqlite -L../hk_classes -L@SQLITELIBDIR@ -lhk_classes
 libhk_sqlite2driver_la_SOURCES= $(normal_src)
else
 INCLUDES= -I../hk_classes -I ./sqlite 
 libhk_sqlite2driver_la_LDFLAGS = -version-info 0:0:0
 libhk_sqlite2driver_la_SOURCES= $(normal_src)  $(extra_src)
endif
