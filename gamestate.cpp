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
}

void GameState::render(SDL_Surface* screen)
{
	int x,y;

	for (y=0;y<HEIGHT;y++)
	{
		for (x=0;x<WIDTH;x++)
		{
			SDL_Rect block;
			block.x = x*16;
			block.y = y*16;
			block.w = 16;
			block.h = 16;

			SDL_FillRect(screen, &block, (blocks[x][y] ? on : off));
		}
	}
}
