AC_INIT([table-stdio], [0.1], [bugs@opensmtpd.org])
AC_CONFIG_AUX_DIR(etc)
AC_CONFIG_LIBOBJ_DIR(openbsd-compat)
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS

AC_REPLACE_FUNCS([ \
	freezero \
	getdtablecount \
	getprogname \
	recallocarray \
	strlcat \
	strlcpy \
])

AC_SEARCH_LIBS([imsg_init], [util], [
		AC_DEFINE([HAVE_IMSG], 1,
			[Define to 1 if you have imsg functions (-lutil or -limsg).])
	], [
		AC_LIBOBJ(imsg)
		AC_LIBOBJ(imsg-buffer)
		CFLAGS="$CFLAGS -I$srcdir/openbsd-compat/imsg"
	]
)

AC_CHECK_HEADER([sys/queue.h], [], [
	CFLAGS="$CFLAGS -I$srcdir/openbsd-compat/tree"
])

AC_CHECK_HEADER([sys/tree.h], [], [
	CFLAGS="$CFLAGS -I$srcdir/openbsd-compat/tree"
])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
])
AC_OUTPUT
