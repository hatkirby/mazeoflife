AC_INIT(mazeoflife, version-2.0)
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define foreign])
AC_PROG_CXX
AC_CONFIG_FILES([Makefile])

dnl Check for SDL
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,
    :,
    AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

dnl Check for SDL_ttf
AC_ARG_WITH([sdl-ttf-include-path],
        [AS_HELP_STRING([--with-sdl-ttf-include-path],
                [location of the SDL_ttf headers, defaults to /usr/include/SDL])],
        [SDLTTF_CXXFLAGS="-I$withval"],
        [SDLTTF_CXXFLAGS="-I/usr/include/SDL"])
AC_SUBST([SDLTTF_CXXFLAGS])

AC_ARG_WITH([sdl-ttf-lib-path],
        [AS_HELP_STRING([--with-sdl-ttf-lib-path], [location of the SDL_ttf library])],
        [SDLTTF_LIBS="-L$withval -lsdl_ttf"],
        [SDLTTF_LIBS="-lsdl_ttf"])
AC_SUBST([SDLTTF_LIBS])

dnl Check for SDL_net
AC_ARG_WITH([sdl-net-include-path],
        [AS_HELP_STRING([--with-sdl-net-include-path],
                [location of the SDL_net headers, defaults to /usr/include/SDL])],
        [SDLNET_CXXFLAGS="-I$withval"],
        [SDLNET_CXXFLAGS="-I/usr/include/SDL"])
AC_SUBST([SDLNET_CXXFLAGS])

AC_ARG_WITH([sdl-net-lib-path],
        [AS_HELP_STRING([--with-sdl-net-lib-path], [location of the SDL_net library])],
        [SDLNET_LIBS="-L$withval -lsdl_net"],
        [SDLNET_LIBS="-lsdl_net"])
AC_SUBST([SDLNET_LIBS])

AC_OUTPUT