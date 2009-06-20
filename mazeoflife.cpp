#include "includes.h"

SDL_Surface *screen;
bool gameSleep = false;
State* state = new DummyState();

int main(int argc, char *argv[])
{
	srand(time(NULL));

	/* Initialize defaults, Video and Audio */
	if((SDL_Init(SDL_INIT_VIDEO)==-1)) { 
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
		exit(-1);
	}

	/* Clean up on exit */
	atexit(SDL_Quit);

	SDL_WM_SetCaption("Maze Of Life", NULL);

	/*
	* Initialize the display in a 640x480 8-bit palettized mode,
	* requesting a software surface
	*/
	screen = SDL_SetVideoMode(WIDTH*16, HEIGHT*16, 8, SDL_DOUBLEBUF);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set %dx%dx8 video mode: %s\n", WIDTH*16, WIDTH*16, SDL_GetError());
		exit(1);
	}

	SDL_EnableKeyRepeat(100, 50);

	state = new TitleState();

	SDL_Event anEvent;
	for (;;)
	{
		state->tick();

		while (SDL_PollEvent(&anEvent))
		{
			switch (anEvent.type)
			{
				case SDL_ACTIVEEVENT:
					if (anEvent.active.state == SDL_APPINPUTFOCUS)
					{
						gameSleep = !anEvent.active.gain;
					}

					break;
				case SDL_QUIT:
					exit(0);

					break;
				case SDL_KEYDOWN:
					state->input(anEvent.key.keysym.sym);

					break;
			}
		}

		state->render(screen);

		SDL_Flip(screen);
	}

	exit(0);
}

void wrap(int* x, int* y)
{
	if (*x < 0)
	{
		*x = WIDTH-(0-*x);
	} else if (*y < 0)
	{
		*y = HEIGHT-(0-*y);
	} else if (*x >= WIDTH)
	{
		*x = *x-WIDTH;
	} else if (*y >= HEIGHT)
	{
		*y = *y-HEIGHT;
	}
}

Uint32 getColor(int r, int g, int b)
{
	return SDL_MapRGB(screen->format, r, g, b);
}

void changeState(State* nState)
{
	state = nState;
}
