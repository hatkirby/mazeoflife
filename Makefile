OBJS = mazeoflife.o gamestate.o board.o
CC = g++
CFLAGS = `pkg-config sdl --cflags`
LIBS = `pkg-config sdl --libs`

mazeoflife: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o mazeoflife

%.o: %.cpp
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rdfv $(OBJS) mazeoflife
