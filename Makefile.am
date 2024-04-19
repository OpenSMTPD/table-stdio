pkglibexec_PROGRAMS	 = table-stdio

AM_CFLAGS = -I$(top_srcdir)/api/ -I$(top_srcdir)/openbsd-compat

table_stdio_SOURCES	 = table-stdio.c
table_stdio_SOURCES	+= $(top_srcdir)/api/dict.c
table_stdio_SOURCES	+= $(top_srcdir)/api/log.c
table_stdio_SOURCES	+= $(top_srcdir)/api/table_api.c
table_stdio_SOURCES	+= $(top_srcdir)/api/util.c

LDADD =			$(LIBOBJS)
