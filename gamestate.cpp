#include "includes.h"

GameState::GameState()
{
	player_color = getColor(255, 255, 0);

	newGame = true;
	doneMaking = false;

	level = Level();
}

void GameState::input(SDLKey key)
{
	if (doneMaking)
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
}

void GameState::tick()
{
	if (newGame)
	{
		playerx = 1;
		playery = 1;
		board = Board(level);
		newGame = false;
		doneMaking = true;
	}
}

void GameState::move(int x, int y)
{
	wrap(&x, &y);

	if (board.isObstructed(x,y)) return;

	playerx = x;
	playery = y;
}

void GameState::render(SDL_Surface* screen)
{
	board.render(screen, level);

	SDL_Rect block;
	block.x = playerx*16;
	block.y = playery*16;
	block.w = 16;
	block.h = 16;

	SDL_FillRect(screen, &block, player_color);
}
