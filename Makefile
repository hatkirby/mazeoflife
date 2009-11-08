PROJECT = mazeoflife
LTARGET = build/$(PROJECT)
WTARGET = build/$(PROJECT).exe
CC	= g++
WINCC	= i586-mingw32msvc-g++
WINDRES = i586-mingw32msvc-windres
FILES 	= $(addprefix build/,$(wildcard *.cpp))
HEADERS = $(wildcard *.h)
MODULES = $(patsubst %.cpp,%,$(FILES))
SOURCES = $(addsuffix .o,$(MODULES))
WINSRC 	= $(addsuffix win,$(SOURCES))
RES	= $(wildcard resources/*)
CRES	= $(patsubst resources/%,build/%,$(addsuffix .o,$(RES)))
LINCCFL = `sdl-config --cflags`
LINLDFL = `sdl-config --libs` -lSDL_ttf -lSDL_net
WINCCFL = `/opt/SDL-1.2.9/bin/i386-mingw32msvc-sdl-config --cflags` -DWINDOWS
WINLDFL = `/opt/SDL-1.2.9/bin/i386-mingw32msvc-sdl-config --libs` -lSDL_ttf -lSDL_net

all: init $(LTARGET) $(WTARGET)
linux: init $(LTARGET)
windows: init $(WTARGET)

init:
	mkdir -p build

clean:
	rm -rdfv build

$(LTARGET): $(SOURCES) $(CRES)
	$(CC) $(SOURCES) $(CRES) -o $(LTARGET) $(LINLDFL)

$(SOURCES): build/%.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@ $(LINCCFL)

$(WTARGET): $(WINSRC) $(CRES) build/winres.o
	$(WINCC) $(WINSRC) $(CRES) build/winres.o -o $(WTARGET) $(WINLDFL)

$(WINSRC): build/%.owin: %.cpp $(HEADERS)
	$(WINCC) -c $< -o $@ $(WINCCFL)

build/winres.o: winres.rc
	$(WINDRES) $? $@

$(CRES): build/%.o: resources/%
	objcopy --input binary --output elf32-i386 -B i386 $? $@
