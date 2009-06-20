OBJS = mazeoflife.o titlestate.o htpstate.o gamestate.o
CC = g++
CFLAGS = `sdl-config --cflags` -I/usr/local/include/SDL
LIBS = `sdl-config --libs` -lSDL_image

mazeoflife: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o mazeoflife

%.o: %.cpp
	$(CC) -c $< $(CFLAGS) -o $@

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(OBJS:.o=.d)

clean:
	rm -rdfv $(OBJS) $(OBJS:.o=.d) mazeoflife
