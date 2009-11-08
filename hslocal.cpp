#include "includes.h"

LocalHighscoreListState::LocalHighscoreListState(bool fromGame)
{
	this->fromGame = fromGame;

	if (fromGame)
	{
		LOADIMAGE(options,hlo_paartm)
	} else {
		LOADIMAGE(options,hlo_rtm)
	}

	LOADIMAGE(pointer,pointer)

	LocalHighscoreList* lhl = new LocalHighscoreList();
	list = lhl->render();

	SDL_Color fontColor = {0, 0, 0, 0};
	SDL_Surface* title = TTF_RenderText_Blended(loadFont(40), "Highscore List", fontColor);
	SDL_Rect tSpace = {240-(title->w/2), 0, title->w, title->h};
	SDL_BlitSurface(title, NULL, list, &tSpace);

	SDL_Rect oSpace = {0, 440, options->w, options->h};
	SDL_BlitSurface(options, NULL, list, &oSpace);

	selection = 0;
}

void LocalHighscoreListState::input(SDL_keysym key)
{
	if (fromGame)
	{
		if ((key.sym == SDLK_LEFT) && (selection != 0))
		{
			selection--;
		} else if ((key.sym == SDLK_RIGHT) && (selection != 1))
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
					changeState(new TitleState());

					break;
			}
		}
	} else {
		if (key.sym == SDLK_RETURN)
		{
			changeState(new ChooseHighscoreListState());
		}
	}
}

void LocalHighscoreListState::render(SDL_Surface* screen)
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

	SDL_BlitSurface(list, NULL, screen, NULL);

	SDL_Rect pSpace;
	pSpace.w = pointer->w;
	pSpace.h = pointer->h;

	if (fromGame)
	{
		pSpace.x = (selection==0?52:225);
		pSpace.y = 447;
	} else {
		pSpace.x = 137;
		pSpace.y = 449;
	}

	SDL_BlitSurface(pointer, NULL, screen, &pSpace);
}
