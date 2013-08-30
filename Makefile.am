AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = mazeoflife
mazeoflife_SOURCES = highscore.cpp highscore.h hslist.cpp hslist.h mazeoflife.cpp mazeoflife.h util.cpp util.h titlestate.cpp titlestate.h gamestate.cpp gamestate.h
AM_CXXFLAGS = $(SDLTTF_CXXFLAGS) $(SDLNET_CXXFLAGS)
mazeoflife_LDADD = $(SDLTTF_LIBS) $(SDLNET_LIBS)
EXTRA_DIST = resources/chl.bmp resources/hlo_paartm.bmp resources/hlo_passartm.bmp resources/hlo_rtm.bmp resources/htp1.bmp resources/htp2.bmp resources/icon.bmp resources/mono.ttf resources/pointer.bmp resources/title.bmp
