#include "includes.h"

TitleState::TitleState()
{
	background = SDL_LoadBMP("resources/title.bmp");
	pointer = SDL_LoadBMP("resources/pointer.bmp");

	selection = 0;
}

void TitleState::input(SDL_keysym key)
{
	if ((key.sym == SDLK_UP) && (selection != 0))
	{
		selection--;
	} else if ((key.sym == SDLK_DOWN) && (selection != 3))
	{
		selection++;
	} else if (key.sym == SDLK_RETURN)
	{
		switch (selection)
		{
			case 0:
				changeState(new GameState());

				break;
			case 1:
				changeState(new HowToPlayState());

				break;
			case 2:
				changeState(new ChooseHighscoreListState());

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
	pSpace.w = pointer->w;
	pSpace.h = pointer->h;

	SDL_BlitSurface(pointer, NULL, screen, &pSpace);
}
