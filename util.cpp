#include "util.h"
#include "mazeoflife.h"
#include <iostream>

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

SDL_Texture* loadImage(SDL_Renderer* renderer, std::string file)
{
	SDL_Surface* surface = SDL_LoadBMP(file.c_str());
	if (surface == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		return NULL;
	}
	
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	
	return texture;
}

void applyTexture(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y)
{
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	
	SDL_RenderCopy(renderer, tex, NULL, &pos);
}