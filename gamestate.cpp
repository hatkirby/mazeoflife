#include "gamestate.h"
#include <SDL_ttf.h>
#include "util.h"
#include "mazeoflife.h"
#include "highscore.h"
#include "titlestate.h"

class GameBoard {
	public:
		GameBoard(int level);
		void tick(int playerx, int playery);
		void render(SDL_Renderer* renderer);
		bool isObstructed(int x, int y);
		
	private:
		int level;
		bool blocks[WIDTH][HEIGHT];
};

class LoadGameState : public State {
	public:
		LoadGameState(int level);
		State* operator() (SDL_Renderer* renderer);
		
	private:
		int level;
};

class PlayGameState : public State {
	public:
		PlayGameState(int level, GameBoard* board, int playerx, int playery);
		State* operator() (SDL_Renderer* renderer);
	
	private:
		bool move(int x, int y);
		int level;
		GameBoard* board;
		int playerx;
		int playery;
};

void setRendererAliveColor(SDL_Renderer* renderer, int level)
{
	switch ((level/10)%5)
	{
		case 0: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break; // Black
		case 1: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break; // Red
		case 2: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break; // Green
		case 3: SDL_SetRenderDrawColor(renderer, 85, 85, 85, 255); break; // Dark Gray
		case 4: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); break; // Magenta
	}
}

void setRendererDeadColor(SDL_Renderer* renderer, int level)
{
	switch ((level/10)%5)
	{
		case 0: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break; // White
		case 1: SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255); break; // Pink
		case 2: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break; // Cyan
		case 3: SDL_SetRenderDrawColor(renderer, 170, 170, 170, 255); break; // Light Gray
		case 4: SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255); break; // Orange
	}
}

void incrementIfNeighbor(int x, int y, bool temp[WIDTH][HEIGHT], int* tick, int playerx, int playery)
{
	int nx = x;
	int ny = y;

	wrap(&x, &y);

	if (!((nx!=x)&&(ny!=y)))
	{
		if ((temp[x][y])||((playerx==x)&&(playery==y))||((x==15)&&(y==15)))
		{
			++*tick;
		}
	}
}

State* GameState::operator() (SDL_Renderer* renderer)
{
	return new LoadGameState(0);
}

LoadGameState::LoadGameState(int m_level)
{
	level = m_level;
}

State* LoadGameState::operator() (SDL_Renderer* renderer)
{
	// Randomly place the player in a corner
	int playerx, playery;
	switch (rand()%4)
	{
		case 0: playerx = 1; playery = 1; break;
		case 1: playerx = 1; playery = HEIGHT-2; break;
		case 2: playerx = WIDTH-2; playery = HEIGHT-2; break;
		case 3: playerx = WIDTH-2; playery = 1; break;
	}
	
	// Display the level number
	setRendererDeadColor(renderer, level);
	SDL_RenderClear(renderer);
	
	TTF_Font* font = loadFont(100);
	SDL_Color fontColor = {0, 0, 0, 0};
	char levelnum[8];
	sprintf(levelnum, "%d", level);
	SDL_Surface* dispsurf = TTF_RenderText_Solid(font, levelnum, fontColor);
	SDL_Texture* disptext = SDL_CreateTextureFromSurface(renderer, dispsurf);
	SDL_FreeSurface(dispsurf);
	
	SDL_Rect pos;
	SDL_QueryTexture(disptext, NULL, NULL, &pos.w, &pos.h);
	pos.x = 240-(pos.w/2);
	pos.y = 240-(pos.h/2);
	
	SDL_RenderCopy(renderer, disptext, NULL, &pos);
	SDL_RenderPresent(renderer);
	
	// Do 50 gens of Conway
	GameBoard* board = new GameBoard(level);
	for (int i=0; i<50; i++)
	{
		board->tick(playerx, playery);
	}
	
	// Wait a bit
	SDL_Delay(500);
	
	// Start the level
	return new PlayGameState(level, board, playerx, playery);
}

PlayGameState::PlayGameState(int m_level, GameBoard* m_board, int m_playerx, int m_playery)
{
	level = m_level;
	board = m_board;
	playerx = m_playerx;
	playery = m_playery;
}

State* PlayGameState::operator() (SDL_Renderer* renderer)
{
	SDL_Event e;
	
	for (;;)
	{
		// Tick board
		board->tick(playerx, playery);
		
		// Paint board
		board->render(renderer);
		
		// Paint event
		SDL_Rect block;
		block.w = 16;
		block.h = 16;
		block.x = 15*16;
		block.y = 15*16;
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &block);
		
		// Paint player
		block.x = playerx*16;
		block.y = playery*16;
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderFillRect(renderer, &block);
		
		SDL_RenderPresent(renderer);
		
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return NULL;
			} else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_LEFT:
						if (move(playerx-1, playery))
						{
							return new LoadGameState(level+1);
						} else {
							break;
						}
						
					case SDLK_RIGHT:
						if (move(playerx+1, playery))
						{
							return new LoadGameState(level+1);
						} else {
							break;
						}
						
					case SDLK_UP:
						if (move(playerx, playery-1))
						{
							return new LoadGameState(level+1);
						} else {
							break;
						}
					
					case SDLK_DOWN:
						if (move(playerx, playery+1))
						{
							return new LoadGameState(level+1);
						} else {
							break;
						}
					
					case SDLK_ESCAPE:
						return new TitleState();
				}
			}
		}
		
		SDL_Delay(5);
	}
}

bool PlayGameState::move(int x, int y)
{
	wrap(&x, &y);
	
	// Are we at the event?
	if ((x==15)&&(y==15))
	{
		return true;
	}

	// Can we even go there?
	if (!board->isObstructed(x,y))
	{
		playerx = x;
		playery = y;
	}
	
	return false;
}

GameBoard::GameBoard(int m_level)
{
	level = m_level;
	
	for (int y=0; y<HEIGHT; y++)
	{
		for (int x=0; x<WIDTH; x++)
		{
			blocks[x][y] = false;
			
			switch (level/10+1)
			{
				case 1:
					if ((x>13)&&(x<17)&&(y>13)&&(y<17))
					{
						blocks[x][y] = rand() % 2;
					}
					break;
				case 2:
				case 3:
			        if ((x>12)&&(x<18)&&(y>12)&&(y<18))
					{
						blocks[x][y] = rand() % 2;
					}
					break;
				case 4:
				case 5:
					if ((x>11)&&(x<19)&&(y>11)&&(y<19))
					{
						blocks[x][y] = rand() % 2;
					}
					break;
				default:
					blocks[x][y] = rand() % 2;
			}
		}
	}
	
	blocks[15][15] = false;
}

void GameBoard::tick(int playerx, int playery)
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

			incrementIfNeighbor(x-1,y-1,temp,&neighbors, playerx, playery);
			incrementIfNeighbor(x-1,y,temp,&neighbors, playerx, playery);
			incrementIfNeighbor(x-1,y+1,temp,&neighbors, playerx, playery);
			incrementIfNeighbor(x,y-1,temp,&neighbors, playerx, playery);
			incrementIfNeighbor(x,y+1,temp,&neighbors, playerx, playery);
			incrementIfNeighbor(x+1,y-1,temp,&neighbors, playerx, playery);
			incrementIfNeighbor(x+1,y,temp,&neighbors, playerx, playery);
			incrementIfNeighbor(x+1,y+1,temp,&neighbors, playerx, playery);

			if (temp[x][y])
			{
				blocks[x][y] = ((neighbors >= 1) && (neighbors <= 4));
			} else {
				blocks[x][y] = (neighbors == 3);
			}
		}
	}
}

void GameBoard::render(SDL_Renderer* renderer)
{
	SDL_Rect block;
	block.w = 16;
	block.h = 16;
	
	for (int y=0; y<HEIGHT; y++)
	{
		for (int x=0; x<WIDTH; x++)
		{
			block.x = x*16;
			block.y = y*16;
			
			if (blocks[x][y])
			{
				setRendererAliveColor(renderer, level);
			} else {
				setRendererDeadColor(renderer, level);
			}
			
			SDL_RenderFillRect(renderer, &block);
		}
	}
}

bool GameBoard::isObstructed(int x, int y)
{
	return blocks[x][y];
}
