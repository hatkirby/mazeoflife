#include "includes.h"

Board::Board()
{
	int x,y;
	for (y=0;y<HEIGHT;y++)
	{
		for (x=0;x<WIDTH;x++)
		{
			if (x > 10 && x < 20 && y > 10 && y < 20)
			{
				blocks[x][y] = rand() % 2;
			} else {
				blocks[x][y] = false;
			}
		}
	}

	on = getColor(0, 0, 0);
	off = getColor(255, 255, 255);
}

bool Board::isObstructed(int x, int y)
{
	return blocks[x][y];
}

void Board::render(SDL_Surface* screen)
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
}
