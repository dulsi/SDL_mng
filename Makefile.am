lib_LTLIBRARIES = libSDL_mng.la

libSDL_mngincludedir = $(includedir)/SDL2
libSDL_mnginclude_HEADERS =	\
	SDL_mng.h

libSDL_mng_la_SOURCES =	\
	SDL_mng.c

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = SDL_mng.pc

noinst_PROGRAMS = mngtest

mngtest_SOURCES = mngtest.c
mngtest_LDADD = libSDL_mng.la
