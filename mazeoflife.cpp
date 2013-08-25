#include "includes.h"

SDL_Surface *screen;
State* state;

int main(int argc, char *argv[])
{
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
	{ 
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
		exit(-1);
	}

	if (TTF_Init() == -1)
	{
		printf("Could not initialize SDL_ttf: %s.\n", TTF_GetError());
		exit(-1);
	}

	if (SDLNet_Init() == -1)
	{
		printf("Cound not initalize SDL_net: %s.\n", SDLNet_GetError());
		exit(-1);
	}

	/* Clean up on exit */
	atexit(SDL_Quit);
	atexit(TTF_Quit);
	atexit(SDLNet_Quit);

	SDL_WM_SetCaption("Maze Of Life", NULL);

	SDL_Surface* icon;
	icon = SDL_LoadBMP("resources/icon.bmp");
	SDL_WM_SetIcon(icon, NULL);

	/*
	* Initialize the display in a 640x480 8-bit palettized mode,
	* requesting a software surface
	*/
	screen = SDL_SetVideoMode(WIDTH*16, HEIGHT*16, 8, SDL_DOUBLEBUF);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set %dx%dx8 video mode: %s\n", WIDTH*16, WIDTH*16, SDL_GetError());
		exit(1);
	}

	SDL_EnableKeyRepeat(100, 70);

	state = new TitleState();

	SDL_AddTimer(TICKDELAY, *tick, NULL);

	SDL_Event anEvent;
	for (;;)
	{
		while (SDL_PollEvent(&anEvent))
		{
			switch (anEvent.type)
			{
				case SDL_QUIT:
					exit(0);

					break;
				case SDL_KEYDOWN:
					if (anEvent.key.keysym.sym == SDLK_F4)
					{
						SDL_WM_ToggleFullScreen(screen);
					} else {
						state->input(anEvent.key.keysym);
					}

					break;
			}
		}
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

Uint32 tick(Uint32 interval, void *param)
{
	state->tick();
	state->render(screen);

	SDL_Flip(screen);

	return interval;
}

TTF_Font* loadFont(int size)
{
	TTF_Font* tmpfont = TTF_OpenFont("resources/mono.ttf", size);

	if (tmpfont == NULL)
	{
		printf("Unable to load font: %s\n", TTF_GetError());
		exit(1);
	}

	return tmpfont;
}

const char* getDataFile()
{
#ifdef WINDOWS
	char* dir = getenv("USERPROFILE");
#else
	char* dir = getenv("HOME");
#endif

	return (std::string(dir) + "/.molhslist").c_str();
}
