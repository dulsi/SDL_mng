AC_INIT(SDL_mng, 0.2.4)

VERSION=0.2.4

dnl Detect the canonical host and target build environment
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

dnl Setup for automake
AM_INIT_AUTOMAKE(foreign)
AM_MAINTAINER_MODE

dnl Check for tools
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_INSTALL

dnl Check for libSDL
SDL_VERSION=2.0.0
AM_PATH_SDL($SDL_VERSION,
            :,
	    AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)

dnl Check for libpng
AC_CHECK_LIB(png, png_create_read_struct, have_libpng=yes, , -lz)
if test x$have_libpng != xyes; then
    AC_MSG_ERROR([*** Unable to find PNG library!])
fi

dnl Check for libz
AC_CHECK_LIB(z, uncompress, have_libz=yes)
if test x$have_libz != xyes; then
    AC_MSG_ERROR([*** Unable to find Zlib library])
fi

if test x$have_libpng = xyes -a x$have_libz = xyes; then
    CFLAGS="$CFLAGS -DLOAD_PNG"
    LIBS="$LIBS -lpng -lz"
fi

CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

AC_OUTPUT([
Makefile
SDL_mng.pc
])
