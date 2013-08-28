#include "mazeoflife.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "state.h"
#include "titlestate.h"

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
	
	SDL_Window* window = SDL_CreateWindow("Maze of Life", 100, 100, 480, 480, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	SDL_Surface* icon = SDL_LoadBMP("resources/icon.bmp");
	SDL_SetWindowIcon(window, icon);
	
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	State* state = new TitleState();
	while (state != NULL)
	{
		state = (*state)(renderer);
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDLNet_Quit();
	TTF_Quit();
	SDL_Quit();
}