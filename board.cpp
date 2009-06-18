#include "includes.h"

Board::Board()
{
	Board(Level());
}

Board::Board(Level level)
{
	int x,y;
	for (y=0;y<HEIGHT;y++)
	{
		for (x=0;x<WIDTH;x++)
		{
			if (level.checkSquare(x, y))
			{
				blocks[x][y] = rand() % 2;
			} else {
				blocks[x][y] = false;
			}
		}
	}
}

bool Board::isObstructed(int x, int y)
{
	return blocks[x][y];
}

void Board::render(SDL_Surface* screen, Level level)
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

			SDL_FillRect(screen, &block, (blocks[x][y] ? level.getAliveColor() : level.getDeadColor()));
		}
	}
}
