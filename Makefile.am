pkglibexec_PROGRAMS	 = table-procexec

AM_CFLAGS = -I$(top_srcdir)/api/

table_procexec_SOURCES	 = table-procexec.c
table_procexec_SOURCES	+= $(top_srcdir)/api/dict.c
table_procexec_SOURCES	+= $(top_srcdir)/api/log.c
table_procexec_SOURCES	+= $(top_srcdir)/api/table_api.c
table_procexec_SOURCES	+= $(top_srcdir)/api/util.c

LDADD =			$(LIBOBJS)
