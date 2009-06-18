#include "includes.h"

GameState::GameState(SDL_PixelFormat* fmt)
{
	int x,y;
	for (y=0;y<HEIGHT;y++)
	{
		for (x=0;x<WIDTH;x++)
		{
			blocks[x][y] = false;
		}
	}

	on = SDL_MapRGB(fmt, 0, 0, 0);
	off = SDL_MapRGB(fmt, 255, 255, 255);
	player_color = SDL_MapRGB(fmt, 255, 255, 0);

	playerx = 1;
	playery = 1;
}

void GameState::input(SDLKey key)
{
	switch (key)
	{
		case SDLK_LEFT:
			move(playerx-1, playery);

			break;
		case SDLK_RIGHT:
			move(playerx+1, playery);

			break;
		case SDLK_UP:
			move(playerx, playery-1);

			break;
		case SDLK_DOWN:
			move(playerx, playery+1);

			break;
	}
}

void GameState::move(int x, int y)
{
	wrap(&x, &y);

	if (blocks[x][y]) return;

	playerx = x;
	playery = y;
}

void GameState::wrap(int* x, int* y)
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

void GameState::render(SDL_Surface* screen)
{
	SDL_Rect block;
	block.w = 16;
	block.h = 16;

	int x,y;

	for (y=0;y<HEIGHT;y++)
	{
		for (x=0;x<WIDTH;x++)
		{
			block.x = x*16;
			block.y = y*16;

			SDL_FillRect(screen, &block, (blocks[x][y] ? on : off));
		}
	}

	block.x = playerx*16;
	block.y = playery*16;

	SDL_FillRect(screen, &block, player_color);
}
