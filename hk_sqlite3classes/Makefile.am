
libdir=@HK_CLASSESDIR@/drivers

AM_CXXFLAGS= -Wall -DNO_TCL=1 -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -I@XMLINCLUDE@ @BITARCHITECTURE@
AM_CFLAGS= -DNO_TCL=1

libhk_sqlite3driver_la_LIBADD =  ../hk_classes/libhk_classes.la

lib_LTLIBRARIES = 	libhk_sqlite3driver.la

noinst_HEADERS = 	hk_sqlite3column.h hk_sqlite3connection.h hk_sqlite3database.h\
			hk_sqlite3datasource.h hk_sqlite3actionquery.h hk_sqlite3resultquery.h\
			hk_sqlite3table.h hk_sqlite3view.h \
			sqlite/sqlite3.h sqlite/parse.h sqlite/opcodes.h sqlite/keywordhash.h \
			sqlite/btree.h  sqlite/hash.h  sqlite/os_common.h  sqlite/os.h  sqlite/pager.h \
			sqlite/sqliteInt.h  sqlite/vdbe.h  sqlite/vdbeInt.h




include_HEADERS =


normal_src = 	hk_sqlite3column.cpp hk_sqlite3connection.cpp hk_sqlite3database.cpp\
				hk_sqlite3datasource.cpp hk_sqlite3actionquery.cpp hk_sqlite3resultquery.cpp\
				hk_sqlite3table.cpp hk_sqlite3view.cpp 

extra_src = 			sqlite/alter.c    sqlite/auth.c   sqlite/callback.c  sqlite/delete.c  sqlite/hash.c   \
				sqlite/main.c     sqlite/os_win.c  sqlite/prepare.c  sqlite/select.c  sqlite/tclsqlite.c \
				sqlite/update.c  sqlite/vacuum.c   sqlite/vdbe.c      sqlite/where.c \
				sqlite/analyze.c  sqlite/btree.c  sqlite/complete.c  sqlite/expr.c    sqlite/insert.c \
				sqlite/os.c       sqlite/pager.c   sqlite/printf.c   sqlite/shell.c   sqlite/tokenize.c \
				sqlite/utf.c     sqlite/vdbeapi.c  sqlite/vdbefifo.c \
				sqlite/attach.c   sqlite/build.c  sqlite/date.c      sqlite/func.c    sqlite/legacy.c \
				sqlite/os_unix.c  sqlite/pragma.c  sqlite/random.c   sqlite/table.c   sqlite/trigger.c \
				sqlite/util.c    sqlite/vdbeaux.c  sqlite/vdbemem.c sqlite/parse.c   sqlite/opcodes.c

if USESQLITE3LIBRARY
 INCLUDES= -I../hk_classes -I@SQLITE3INCDIR@
 libhk_sqlite3driver_la_LDFLAGS = -version-info 0:0:0 -lsqlite3 -L../hk_classes -L@SQLITE3LIBDIR@ -lhk_classes
 libhk_sqlite3driver_la_SOURCES= $(normal_src)
else
 INCLUDES= -I../hk_classes -I ./sqlite 
 libhk_sqlite3driver_la_LDFLAGS = -version-info 0:0:0
 libhk_sqlite3driver_la_SOURCES= $(normal_src)  $(extra_src)
endif
