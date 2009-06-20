OBJS = mazeoflife.o titlestate.o gamestate.o
CC = g++
CFLAGS = `pkg-config sdl --cflags`
LIBS = `pkg-config sdl --libs`

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
