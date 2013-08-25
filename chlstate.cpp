#include "includes.h"

ChooseHighscoreListState::ChooseHighscoreListState()
{
	background = SDL_LoadBMP("chl.bmp");
	pointer = SDL_LoadBMP("pointer.bmp");

	selection = 0;
}

void ChooseHighscoreListState::input(SDL_keysym key)
{
	if ((key.sym == SDLK_UP) && (selection != 0))
	{
		selection--;
	} else if ((key.sym == SDLK_DOWN) && (selection != 2))
	{
		selection++;
	} else if (key.sym == SDLK_RETURN)
	{
		switch (selection)
		{
			case 0:
				changeState(new LocalHighscoreListState(false));

				break;
			case 1:
				changeState(new GlobalHighscoreListState());

				break;
			case 2:
				changeState(new TitleState());

				break;
		}
	}
}

void ChooseHighscoreListState::render(SDL_Surface* screen)
{
	SDL_BlitSurface(background, NULL, screen, NULL);

	SDL_Rect pSpace;
	pSpace.x = 127;
	pSpace.y = (selection==0?306:(selection==1?336:396));
	pSpace.w = pointer->w;
	pSpace.h = pointer->h;

	SDL_BlitSurface(pointer, NULL, screen, &pSpace);
}
