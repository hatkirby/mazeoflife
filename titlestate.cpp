#include "includes.h"

TitleState::TitleState()
{
	background = SDL_LoadBMP("title.bmp");
	pointer = SDL_LoadBMP("pointer.bmp");
	selection = 0;
}

void TitleState::input(SDLKey key)
{
	if ((key == SDLK_UP) && (selection != 0))
	{
		selection--;
	} else if ((key == SDLK_DOWN) && (selection != 3))
	{
		selection++;
	} else if (key == SDLK_RETURN)
	{
		switch (selection)
		{
			case 0:
				changeState(new GameState());

				break;
			case 1: // Add How To Play
				break;
			case 2: // Add choose highscore list
				break;
			case 3:
				exit(0);
		}
	}
}

void TitleState::render(SDL_Surface* screen)
{
	SDL_BlitSurface(background, NULL, screen, NULL);

	SDL_Rect pSpace;
	pSpace.x = 136;
	pSpace.y = (selection==0?316:(selection==1?350:(selection==2?381:417)));
	pSpace.w = screen->w;
	pSpace.h = screen->h;

	SDL_BlitSurface(pointer, NULL, screen, &pSpace);
}