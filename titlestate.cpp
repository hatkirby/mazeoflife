#include "titlestate.h"
#include "util.h"
#include "gamestate.h"
#include "hslist.h"

State* TitleState::operator() (SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_Texture* background = loadImage(renderer, "resources/title.bmp");
	SDL_Texture* pointer = loadImage(renderer, "resources/pointer.bmp");
	int selection = 0;
	SDL_Event e;

	for (;;)
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, NULL, NULL);
		applyTexture(renderer, pointer, 136, selection==0?316:(selection==1?350:(selection==2?381:417)));
		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return NULL;
			} else if (e.type == SDL_KEYDOWN)
			{
				if ((e.key.keysym.sym == SDLK_UP) && (selection != 0))
				{
					selection--;
				} else if ((e.key.keysym.sym == SDLK_DOWN) && (selection != 3))
				{
					selection++;
				} else if (e.key.keysym.sym == SDLK_RETURN)
				{
					switch (selection)
					{
						case 0: return new GameState();
						case 1: return new HowToPlayState();
						case 2: return new ChooseHighscoreListState();
						case 3:	return NULL;
					}
				}
			}
		}
	}
}

State* HowToPlayState::operator() (SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_Texture* background = loadImage(renderer, "resources/htp1.bmp");
	SDL_Texture* pointer = loadImage(renderer, "resources/pointer.bmp");
	int selection = 0;
	SDL_Event e;

	for (;;)
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, NULL, NULL);
		applyTexture(renderer, pointer, selection==0?74:216, 430);
		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return NULL;
			} else if (e.type == SDL_KEYDOWN)
			{
				if ((e.key.keysym.sym == SDLK_LEFT) && (selection != 0))
				{
					selection--;
				} else if ((e.key.keysym.sym == SDLK_RIGHT) && (selection != 1))
				{
					selection++;
				} else if (e.key.keysym.sym == SDLK_RETURN)
				{
					switch (selection)
					{
						case 0:
							return new HowToPlayPageTwoState();

							break;
						case 1:
							return new TitleState();
					}
				}
			}
		}
	}
}

State* HowToPlayPageTwoState::operator() (SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_Texture* background = loadImage(renderer, "resources/htp2.bmp");
	SDL_Texture* pointer = loadImage(renderer, "resources/pointer.bmp");
	int selection = 0;
	SDL_Event e;

	for (;;)
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, NULL, NULL);
		applyTexture(renderer, pointer, selection==0?45:238, 430);
		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return NULL;
			} else if (e.type == SDL_KEYDOWN)
			{
				if ((e.key.keysym.sym == SDLK_LEFT) && (selection != 0))
				{
					selection--;
				} else if ((e.key.keysym.sym == SDLK_RIGHT) && (selection != 1))
				{
					selection++;
				} else if (e.key.keysym.sym == SDLK_RETURN)
				{
					switch (selection)
					{
						case 0:
							return new HowToPlayState();

							break;
						case 1:
							return new TitleState();
					}
				}
			}
		}
	}
}
