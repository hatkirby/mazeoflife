#include "includes.h"

HowToPlayState::HowToPlayState()
{
	LOADIMAGE(background1,htp1)
	LOADIMAGE(background2,htp2)
	LOADIMAGE(pointer,pointer)

	secondPage = false;
	selection = 0;

	SDL_WM_SetCaption("Maze Of Life - How To Play", NULL);
}

void HowToPlayState::input(SDLKey key)
{
	if ((key == SDLK_LEFT) && (selection != 0))
	{
		selection--;
	} else if ((key == SDLK_RIGHT) && (selection != 1))
	{
		selection++;
	} else if (key == SDLK_RETURN)
	{
		switch (selection)
		{
			case 0:
				secondPage = !secondPage;

				break;
			case 1:
				changeState(new TitleState());

				break;
		}
	}
}

void HowToPlayState::render(SDL_Surface* screen)
{
	SDL_Rect pSpace;

	if (!secondPage)
	{
		SDL_BlitSurface(background1, NULL, screen, NULL);
			
		pSpace.x = (selection==0?74:216);
	} else {
		SDL_BlitSurface(background2, NULL, screen, NULL);

		pSpace.x = (selection==0?45:238);
	}

	pSpace.y = 430;
	pSpace.w = pointer->w;
	pSpace.h = pointer->h;

	SDL_BlitSurface(pointer, NULL, screen, &pSpace);
}
