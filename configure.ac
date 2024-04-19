AC_INIT([table-stdio], [0.1], [bugs@opensmtpd.org])
AC_CONFIG_AUX_DIR(etc)
AC_CONFIG_LIBOBJ_DIR(openbsd-compat)
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS

AC_ARG_WITH([libbsd],
	AS_HELP_STRING([--with-libbsd],
		[Build with libbsd library (default: disabled)]))

AS_IF([test "x$with_libbsd" = "xyes"], [
	PKG_CHECK_MODULES([libbsd], [libbsd-overlay libbsd-ctor], [
		CFLAGS="$libbsd_CFLAGS $CFLAGS"
		LIBS="$libbsd_LIBS $LIBS"
	])
])

AC_REPLACE_FUNCS([
	getdtablecount \
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

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
])
AC_OUTPUT
