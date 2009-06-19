#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Maze Of Life], [2.0], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/mazeoflife.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([SDL], [sdl >= 1.2])
CXXFLAGS="$SDL_CFLAGS $CXXFLAGS"
LDFLAGS="$SDL_LIBS"

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([atexit])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
