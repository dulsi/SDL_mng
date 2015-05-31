lib_LTLIBRARIES = libSDL_mng.la

libSDL_mngincludedir = $(includedir)/SDL
libSDL_mnginclude_HEADERS =	\
	SDL_mng.h

libSDL_mng_la_SOURCES =	\
	SDL_mng.c

noinst_PROGRAMS = mngtest

mngtest_SOURCES = mngtest.c
mngtest_LDADD = libSDL_mng.la
