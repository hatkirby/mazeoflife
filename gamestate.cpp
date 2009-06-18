#include "includes.h"

GameState::GameState()
{
	player_color = getColor(255, 255, 0);

	newGame = true;
	doneMaking = false;

	info.level = Level();
}

void GameState::input(SDLKey key)
{
	if (doneMaking)
	{
		switch (key)
		{
			case SDLK_LEFT:
				move(info.playerx-1, info.playery);

				break;
			case SDLK_RIGHT:
				move(info.playerx+1, info.playery);

				break;
			case SDLK_UP:
				move(info.playerx, info.playery-1);

				break;
			case SDLK_DOWN:
				move(info.playerx, info.playery+1);

				break;
		}
	}
}

void GameState::tick()
{
	if (newGame)
	{
		info.playerx = 1;
		info.playery = 1;
		board = Board(&info);
		newGame = false;
		doneMaking = true;
	}

	board.tick();
}

void GameState::move(int x, int y)
{
	wrap(&x, &y);

	if (board.isObstructed(x,y)) return;

	info.playerx = x;
	info.playery = y;
}

void GameState::render(SDL_Surface* screen)
{
	board.render(screen);

	SDL_Rect block;
	block.x = info.playerx*16;
	block.y = info.playery*16;
	block.w = 16;
	block.h = 16;

	SDL_FillRect(screen, &block, player_color);
}

GameState::Level::Level()
{
	level = 1;

	alive[0] = getColor(0, 0, 0); // Black
	alive[1] = getColor(255, 0, 0); // Red
	alive[2] = getColor(0, 255, 0); // Green
	alive[3] = getColor(85, 85, 85); // Dark Gray
	alive[4] = getColor(255, 0, 255); // Magenta

	dead[0] = getColor(255, 255, 255); // White
	dead[1] = getColor(255, 192, 203); // Pink
	dead[2] = getColor(0, 255, 255); // Cyan
	dead[3] = getColor(170, 170, 170); // Light Gray
	dead[4] = getColor(255, 128, 0); // Orange
}

int GameState::Level::getLevel()
{
	return level;
}

int GameState::Level::getLevelGroup()
{
	return (level/10)+1;
}

bool GameState::Level::checkSquare(int x, int y)
{
	switch (getLevelGroup())
	{
		case 1:
	                return ((x>13)&&(x<16)&&(y>13)&&(y<16));
		case 2:
	                return ((x>13)&&(x<17)&&(y>13)&&(y<17));
		case 3:
		case 4:
	                return ((x>12)&&(x<18)&&(y>12)&&(y<18));
		case 5:
		case 6:
			return ((x>11)&&(x<19)&&(y>11)&&(y<19));
		default:
			return true;
	}
}

Uint32 GameState::Level::getAliveColor()
{
	return alive[(getLevelGroup()-1)%5];
}

Uint32 GameState::Level::getDeadColor()
{
	return dead[(getLevelGroup()-1)%5];
}

GameState::Board::Board()
{
	GameState::Board::Board(new GameState::Info());
}

GameState::Board::Board(GameState::Info* info)
{
	this->info = info;

	int x,y;
	for (y=0;y<HEIGHT;y++)
	{
		for (x=0;x<WIDTH;x++)
		{
			if (info->level.checkSquare(x, y))
			{
				blocks[x][y] = rand() % 2;
			} else {
				blocks[x][y] = false;
			}
		}
	}
}

bool GameState::Board::isObstructed(int x, int y)
{
	return blocks[x][y];
}

void GameState::Board::render(SDL_Surface* screen)
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

			SDL_FillRect(screen, &block, (blocks[x][y] ? info->level.getAliveColor() : info->level.getDeadColor()));
		}
	}
}

void GameState::Board::tick()
{
	bool temp[WIDTH][HEIGHT];
	int x,y;
	for (x=0;x<WIDTH;x++)
	{
		for (y=0;y<HEIGHT;y++)
		{
			temp[x][y] = blocks[x][y];
		}
	}

	for (x=0;x<WIDTH;x++)
	{
		for (y=0;y<HEIGHT;y++)
		{
			int neighbors = 0;

			if ((x>0)&&(y>0)) incrementIfNeighbor(x-1,y-1,temp,&neighbors);
			if ((x>0)) incrementIfNeighbor(x-1,y,temp,&neighbors);
			if ((x>0)&&(y<HEIGHT-1)) incrementIfNeighbor(x-1,y+1,temp,&neighbors);
			if ((y>0)) incrementIfNeighbor(x,y-1,temp,&neighbors);
			if ((y<HEIGHT-1)) incrementIfNeighbor(x,y+1,temp,&neighbors);
			if ((x<WIDTH-1)&&(y>0)) incrementIfNeighbor(x+1,y-1,temp,&neighbors);
			if ((x<WIDTH-1)) incrementIfNeighbor(x+1,y,temp,&neighbors);
			if ((x<WIDTH-1)&&(y<HEIGHT-1)) incrementIfNeighbor(x+1,y+1,temp,&neighbors);

			if (temp[x][y])
			{
				blocks[x][y] = ((neighbors >= 1) && (neighbors <= 4));
			} else {
				blocks[x][y] = (neighbors == 3);
			}
		}
	}
}

void GameState::Board::incrementIfNeighbor(int x, int y, bool temp[WIDTH][HEIGHT], int* tick)
{
	wrap(&x, &y);

	if ((blocks[x][y])||((info->playerx==x)&&(info->playery==y)))
	{
		++*tick;
	}
}
