#include "includes.h"

bool gameSleep = false;

int main(int argc, char *argv[])
{    
	/* Initialize defaults, Video and Audio */
	if((SDL_Init(SDL_INIT_VIDEO)==-1)) { 
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
		exit(-1);
	}

	/* Clean up on exit */
	atexit(SDL_Quit);

	/*
	* Initialize the display in a 640x480 8-bit palettized mode,
	* requesting a software surface
	*/
	SDL_Surface *screen = SDL_SetVideoMode(WIDTH*16, HEIGHT*16, 8, SDL_DOUBLEBUF);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set %dx%dx8 video mode: %s\n", WIDTH*16, WIDTH*16, SDL_GetError());
		exit(1);
	}

	SDL_WM_SetCaption("Maze Of Life", NULL);

	State* state = new GameState(screen->format);

	SDL_Event anEvent;
	for (;;)
	{
		while (SDL_PollEvent(&anEvent))
		{
			switch (anEvent.type)
			{
				case SDL_ACTIVEEVENT:
					if (anEvent.active.state == SDL_APPINPUTFOCUS)
					{
						//	gameSleep = !anEvent.active.gain;
					}

					break;
				case SDL_QUIT:
					exit(0);

					break;
			}
		}

		state->render(screen);

		SDL_Flip(screen);
	}

	exit(0);
}

