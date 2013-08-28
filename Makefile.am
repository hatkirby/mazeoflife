AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = mazeoflife
mazeoflife_SOURCES = highscore.cpp hslist.cpp mazeoflife.cpp util.cpp titlestate.cpp gamestate.cpp
AM_CXXFLAGS = $(SDLTTF_CXXFLAGS) $(SDLNET_CXXFLAGS)
mazeoflife_LDADD = $(SDLTTF_LIBS) $(SDLNET_LIBS)
EXTRA_DIST = resources/chl.bmp resources/hlo_paartm.bmp resources/hlo_passartm.bmp resources/hlo_rtm.bmp resources/htp1.bmp resources/htp2.bmp resources/icon.bmp resources/mono.ttf resources/pointer.bmp resources/title.bmp
