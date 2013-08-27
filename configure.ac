AC_INIT(mazeoflife, version-3.0)
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define foreign])
AC_PROG_CXX
AC_CONFIG_FILES([Makefile])

dnl Check for SDL
SDL_VERSION=2.0.0
AM_PATH_SDL2($SDL_VERSION,
    :,
    AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

dnl Check for SDL2_ttf
AC_ARG_WITH([sdl-ttf2-include-path],
        [AS_HELP_STRING([--with-sdl2-ttf-include-path],
                [location of the SDL2_ttf headers, defaults to /usr/include/SDL2])],
        [SDLTTF_CXXFLAGS="-I$withval"],
        [SDLTTF_CXXFLAGS="-I/usr/include/SDL2"])
AC_SUBST([SDLTTF_CXXFLAGS])

AC_ARG_WITH([sdl2-ttf-lib-path],
        [AS_HELP_STRING([--with-sdl2-ttf-lib-path], [location of the SDL2_ttf library])],
        [SDLTTF_LIBS="-L$withval -lsdl2_ttf"],
        [SDLTTF_LIBS="-lsdl2_ttf"])
AC_SUBST([SDLTTF_LIBS])

dnl Check for SDL2_net
AC_ARG_WITH([sdl2-net-include-path],
        [AS_HELP_STRING([--with-sdl2-net-include-path],
                [location of the SDL2_net headers, defaults to /usr/include/SDL2])],
        [SDLNET_CXXFLAGS="-I$withval"],
        [SDLNET_CXXFLAGS="-I/usr/include/SDL2"])
AC_SUBST([SDLNET_CXXFLAGS])

AC_ARG_WITH([sdl2-net-lib-path],
        [AS_HELP_STRING([--with-sdl2-net-lib-path], [location of the SDL2_net library])],
        [SDLNET_LIBS="-L$withval -lsdl2_net"],
        [SDLNET_LIBS="-lsdl2_net"])
AC_SUBST([SDLNET_LIBS])

AC_OUTPUT