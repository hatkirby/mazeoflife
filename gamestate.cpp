#include "includes.h"

GameState::GameState()
{
	player_color = getColor(255, 255, 0);
	event_color = getColor(0, 0, 255);

	newGame = false;

	info.playerx = 1;
	info.playery = 1;
	info.level = Level();
	board = Board(&info);

	SDL_WM_SetCaption("Maze Of Life - Level 1", NULL);
}

void GameState::input(SDLKey key)
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
		case SDLK_ESCAPE:
			newGame = false;

			info.playerx = 1;
			info.playery = 1;
			info.level = Level();
			board = Board(&info);

			SDL_WM_SetCaption("Maze Of Life - Level 1", NULL);

			break;
	}
}

void GameState::tick()
{
	if (newGame)
	{
		switch (rand()%4)
		{
			case 0: info.playerx = 1; info.playery = 1; break;
			case 1: info.playerx = 1; info.playery = HEIGHT-2; break;
			case 2: info.playerx = WIDTH-2; info.playery = HEIGHT-2; break;
			case 3: info.playerx = WIDTH-2; info.playery = 1; break;
		}

		info.level.incrementLevel();
		board = Board(&info);
		newGame = false;

		char title[32];
		sprintf(title, "Maze Of Life - Level %d", info.level.getLevel());
		SDL_WM_SetCaption(title, NULL);
	}

	board.tick();
}

void GameState::move(int x, int y)
{
	wrap(&x, &y);

	if (board.isObstructed(x,y)) return;
	if ((x==15)&&(y==15)) newGame = true;

	info.playerx = x;
	info.playery = y;
}

void GameState::render(SDL_Surface* screen)
{
	// Paint maze
	board.render(screen);

	// Paint player
	SDL_Rect block;
	block.x = info.playerx*16;
	block.y = info.playery*16;
	block.w = 16;
	block.h = 16;

	SDL_FillRect(screen, &block, player_color);

	// Paint event
	block.x = 15*16;
	block.y = 15*16;

	SDL_FillRect(screen, &block, event_color);
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

bool GameState::Level::checkSquare(int x, int y)
{
	switch (level/10+1)
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
	return alive[(level/10)%5];
}

Uint32 GameState::Level::getDeadColor()
{
	return dead[(level/10)%5];
}

void GameState::Level::incrementLevel()
{
	level++;
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

	blocks[15][15] = false;
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
			if ((x==15)&&(y==15))
			{
				continue;
			}

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

	if ((temp[x][y])||((info->playerx==x)&&(info->playery==y))||((x==15)&&(y==15)))
	{
		++*tick;
	}
}
