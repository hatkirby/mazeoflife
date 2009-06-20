SOURCES = mazeoflife.cpp titlestate.cpp htpstate.cpp gamestate.cpp
OBJS = $(SOURCES:.cpp=.cpp.o)
IMAGES = title.bmp pointer.bmp htp1.bmp htp2.bmp
CIMAGES = $(IMAGES:.bmp=.bmp.o)
CC = g++
CFLAGS = `pkg-config sdl --cflags`
LIBS = `pkg-config sdl --libs`

mazeoflife: $(OBJS) $(CIMAGES)
	$(CC) $(OBJS) $(CIMAGES) $(LIBS) -o mazeoflife

%.cpp.o: %.cpp
	$(CC) -c $< $(CFLAGS) -o $@

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.bmp.o: %.bmp
	objcopy --input binary --output elf32-i386 -B i386 $< $@

include $(OBJS:.cpp.o=.d)

clean:
	rm -rdfv $(OBJS) $(OBJS:.cpp.o=.d) $(CIMAGES) mazeoflife
