AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = mazeoflife
mazeoflife_SOURCES = chlstate.cpp gamestate.cpp highscore.cpp hsglobal.cpp hslist.cpp hslocal.cpp hsnew.cpp hssubmit.cpp htpstate.cpp mazeoflife.cpp titlestate.cpp
AM_CXXFLAGS = $(SDLTTF_CXXFLAGS) $(SDLNET_CXXFLAGS)
mazeoflife_LDADD = $(SDLTTF_LIBS) $(SDLNET_LIBS)
