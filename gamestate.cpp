#include "gamestate.h"
#include <SDL_ttf.h>
#include "util.h"
#include "mazeoflife.h"
#include "highscore.h"
#include "titlestate.h"
#include <fstream>
#include "hslist.h"
#include <set>
#include <bitset>
#include <tuple>
#include <algorithm>
#include <vector>
#include <deque>
#include <iostream>
#include <sstream>
#include <list>
#include <unordered_map>
#include <unordered_set>

class GameBoard {
	public:
		GameBoard(int level, int playerx, int playery);
		void tick(int playerx, int playery);
		void render(SDL_Renderer* renderer, int level) const;
		bool isObstructed(int x, int y) const;
    bool operator<(const GameBoard& other) const;

    using coord = std::tuple<int, int>;

	private:
    void initialize(int level);
    bool solve(int playerx, int playery) const;
    std::string dump() const;

    using board_type = std::bitset<WIDTH*HEIGHT>;

    void incrementIfNeighbor(
      int x,
      int y,
      const board_type& temp,
      int playerx,
      int playery,
      int& tick) const;

    bool applyNeighbors(
      int x,
      int y,
      const board_type& temp,
      int playerx,
      int playery) const;

    board_type blocks;
    board_type updateable;
    int oldx;
    int oldy;
};

class LoadGameState : public State {
	public:
		LoadGameState(int level);
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);

	private:
		int level;
};

class PlayGameState : public State {
	public:
		PlayGameState(int level, GameBoard* board, int playerx, int playery);
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);

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

void GameBoard::incrementIfNeighbor(
  int x,
  int y,
  const board_type& temp,
  int playerx,
  int playery,
  int& tick) const
{
	int nx = x;
	int ny = y;

	wrap(x, y);

	if (!((nx!=x)&&(ny!=y)))
	{
		if ((temp[x+y*WIDTH])||((playerx==x)&&(playery==y))||((x==15)&&(y==15)))
		{
			++tick;
		}
	}
}

State* GameState::operator() (SDL_Window* window, SDL_Renderer* renderer)
{
	return new LoadGameState(0);
}

LoadGameState::LoadGameState(int m_level)
{
	level = m_level;
}

State* LoadGameState::operator() (SDL_Window* window, SDL_Renderer* renderer)
{
	char* wintitle = new char[50];
	sprintf(wintitle, "Maze Of Life - Level %d", level);
	SDL_SetWindowTitle(window, wintitle);

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
	GameBoard* board = new GameBoard(level, playerx, playery);

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

State* PlayGameState::operator() (SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_Event e;

	for (;;)
	{
		// Tick board
		board->tick(playerx, playery);

		// Paint board
		board->render(renderer, level);

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
						SDL_SetWindowTitle(window, "");

						std::ifstream exists(getDataFile());
						if (exists)
						{
							FILE* hslist = fopen(getDataFile(), "r");
							int scores;
							Highscore* h;

							fscanf(hslist, "%d%*c", &scores);

							if (scores < 10)
							{
								fclose(hslist);

								return new EnterHighscoreState(level);
							} else {
								for (int i=0; i<scores; i++)
								{
									int namelen;
									char namelens[4];
									char* name = (char*) calloc(25, sizeof(char));
									int score;

									fscanf(hslist, "%d", &namelen);
									sprintf(namelens, "%%%dc", namelen);
									fscanf(hslist, namelens, name);
									fscanf(hslist, "%d%*c", &score);

									h = new Highscore(name, score);
								}

								fclose(hslist);

								if (h->getLevel() < level)
								{
									return new EnterHighscoreState(level);
								} else {
									return new DisplayAndReturnLocalHighscoreListState();
								}
							}
						} else {
							return new EnterHighscoreState(level);
						}
				}
			}
		}

		SDL_Delay(5);
	}
}

bool PlayGameState::move(int x, int y)
{
	wrap(x, y);

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

GameBoard::GameBoard(int level, int playerx, int playery)
{
	for (;;)
  {
    initialize(level);
    updateable.set();
    oldx = playerx;
    oldy = playery;

    for (int i=0; i<50; i++)
    {
      tick(playerx, playery);
    }

    if (solve(playerx, playery))
    {
      break;
    } else {
      std::cout << "Impossible board: " << playerx << "," << playery << "," << dump() << std::endl;
    }
  }
}

void GameBoard::tick(int playerx, int playery)
{
  board_type temp {blocks};
  board_type tempdateable {updateable};
  if ((playerx != oldx) || (playery != oldy))
  {
    for (int dy = -1; dy <= 1; dy++)
    {
      for (int dx = -1; dx <=1; dx++)
      {
        int tdx = oldx+dx;
        int tdy = oldy+dy;
        wrap(tdx, tdy);
        tempdateable.set(tdx+tdy*WIDTH);

        tdx = playerx+dx;
        tdy = playery+dy;
        wrap(tdx, tdy);
        tempdateable.set(tdx+tdy*WIDTH);
      }
    }
  }

  oldx = playerx;
  oldy = playery;

  updateable.reset();

	for (int y=0;y<HEIGHT;y++)
	{
		for (int x=0;x<WIDTH;x++)
		{
			if (((x==15)&&(y==15)) || (!tempdateable[x+y*WIDTH]))
			{
				continue;
			}

      blocks[x+y*WIDTH] = applyNeighbors(x, y, temp, playerx, playery);

      if (temp[x+y*WIDTH] != blocks[x+y*WIDTH])
      {
        for (int dy = -1; dy <= 1; dy++)
        {
          for (int dx = -1; dx <=1; dx++)
          {
            int tdx = x+dx;
            int tdy = y+dy;
            wrap(tdx, tdy);
            updateable.set(tdx+tdy*WIDTH);
          }
        }
      }
		}
	}
}

bool GameBoard::applyNeighbors(
  int x,
  int y,
  const board_type& temp,
  int playerx,
  int playery) const
{
	int neighbors = 0;

	incrementIfNeighbor(x-1, y-1, temp, playerx, playery, neighbors);
	incrementIfNeighbor(x-1, y  , temp, playerx, playery, neighbors);
	incrementIfNeighbor(x-1, y+1, temp, playerx, playery, neighbors);
	incrementIfNeighbor(x  , y-1, temp, playerx, playery, neighbors);
	incrementIfNeighbor(x  , y+1, temp, playerx, playery, neighbors);
	incrementIfNeighbor(x+1, y-1, temp, playerx, playery, neighbors);
	incrementIfNeighbor(x+1, y  , temp, playerx, playery, neighbors);
	incrementIfNeighbor(x+1, y+1, temp, playerx, playery, neighbors);

	if (temp[x+y*WIDTH])
	{
		return ((neighbors >= 1) && (neighbors <= 4));
	} else {
		return (neighbors == 3);
	}
}

void GameBoard::render(SDL_Renderer* renderer, int level) const
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

			if (blocks[x+y*WIDTH])
			{
				setRendererAliveColor(renderer, level);
			} else {
				setRendererDeadColor(renderer, level);
			}

			SDL_RenderFillRect(renderer, &block);
		}
	}
}

bool GameBoard::isObstructed(int x, int y) const
{
	return blocks[x+y*WIDTH] || (x == 15 && y == 15);
}

void GameBoard::initialize(int level)
{
	for (int y=0; y<HEIGHT; y++)
	{
		for (int x=0; x<WIDTH; x++)
		{
			blocks[x+y*WIDTH] = false;

			switch (level/10+1)
			{
				case 1:
					if ((x>13)&&(x<17)&&(y>13)&&(y<17))
					{
						blocks[x+y*WIDTH] = rand() % 2;
					}
					break;
				case 2:
				case 3:
			     if ((x>12)&&(x<18)&&(y>12)&&(y<18))
					{
						blocks[x+y*WIDTH] = rand() % 2;
					}
					break;
				case 4:
				case 5:
					if ((x>11)&&(x<19)&&(y>11)&&(y<19))
					{
						blocks[x+y*WIDTH] = rand() % 2;
					}
					break;
				default:
					blocks[x+y*WIDTH] = rand() % 2;
			}
		}
	}

	blocks[15+15*WIDTH] = false;
}

bool GameBoard::operator<(const GameBoard& other) const
{
  for (int i = WIDTH*HEIGHT-1; i >= 0; i--)
  {
    if (blocks[i] ^ other.blocks[i])
    {
      return other.blocks[i];
    }
  }

  return false;
}

bool GameBoard::solve(int playerx, int playery) const
{
  std::deque<std::tuple<GameBoard, coord, int>> search;
  std::unordered_map<board_type, board_type> done;

  // Assume that the player will not move while the board is changing, so tick
  // the board until it either stops changing, or it reaches a state that it has
  // already been in (in the case of alternating systems).
  {
    GameBoard original = *this;

    std::unordered_set<board_type> pastStates;
    pastStates.insert(original.blocks);

    while (original.updateable.any())
    {
      original.tick(playerx, playery);

      if (pastStates.count(original.blocks))
      {
        break;
      }

      pastStates.insert(original.blocks);
    }

    search.emplace_front(std::move(original), coord{playerx, playery}, 0);
  }

  // Use breadth first search to find a solution.
  bool exists = false;
  while (!search.empty())
  {
    auto cur = std::move(search.front());
    search.pop_front();

    GameBoard& cbr = std::get<0>(cur);
    coord& cpl = std::get<1>(cur);
    int cns = std::get<2>(cur);

    // If it has been over 100 generations, give up.
    if (cns > 100)
    {
      continue;
    }

    int cplx = std::get<0>(cpl);
    int cply = std::get<1>(cpl);

    // If this section of this board state has already been checked, skip it.
    if (done.count(cbr.blocks) && done.at(cbr.blocks)[cplx+cply*WIDTH])
    {
      continue;
    }

    // Use a flood fill to find a set of positions accessible to the player
    // without modifying the board state, as well as a set of positions adjacent
    // to the flood that /will/ modify the board state.
    board_type flood;
    std::deque<coord> front;
    front.push_front(cpl);
    flood[cplx+cply*WIDTH] = true;

    std::set<coord> edges;

    while (!front.empty())
    {
      coord frontLoc = std::move(front.front());
      front.pop_front();

      // Iterate over the positions 4-adjacent to the current one.
      for (coord& fc : std::list<coord>{
        {std::get<0>(frontLoc) - 1, std::get<1>(frontLoc)    },
        {std::get<0>(frontLoc) + 1, std::get<1>(frontLoc)    },
        {std::get<0>(frontLoc)    , std::get<1>(frontLoc) - 1},
        {std::get<0>(frontLoc)    , std::get<1>(frontLoc) + 1},
      })
      {
        wrap(std::get<0>(fc), std::get<1>(fc));
        int fcx = std::get<0>(fc);
        int fcy = std::get<1>(fc);

        // If this position is already in the flood, skip it.
        if (flood[fcx+fcy*WIDTH])
        {
          continue;
        }

        // If the player could not move into this position, skip it.
        if (cbr.isObstructed(fcx, fcy))
        {
          continue;
        }

        // If this position is adjacent to the event, then the board is
        // solvable.
        if (((fcx == 15) && ((fcy == 14) || (fcy == 16))) ||
            ((fcy == 15) && ((fcx == 14) || (fcx == 16))))
        {
          exists = true;
          break;
        }

        // Check if the player moving would cause any positions 8-adjacent to
        // the start or end positions to change. This is more efficient than
        // copying the board state and then running tick.
        bool changed = false;
        for (int dy = -1; dy <= 1; dy++)
        {
          for (int dx = -1; dx <=1; dx++)
          {
            if (dx == 0 && dy == 0)
            {
              continue;
            }

            int cpldx = cplx + dx;
            int cpldy = cply + dy;
            wrap(cpldx, cpldy);

            if (cbr.isObstructed(cpldx, cpldy) !=
              applyNeighbors(
                cpldx,
                cpldy,
                cbr.blocks,
                fcx,
                fcy))
            {
              changed = true;
              break;
            }

            int fcxdx = fcx + dx;
            int fcydy = fcy + dy;
            wrap(fcxdx, fcydy);

            if (cbr.isObstructed(fcxdx, fcydy) !=
              applyNeighbors(
                fcxdx,
                fcydy,
                cbr.blocks,
                fcx,
                fcy))
            {
              changed = true;
              break;
            }
          }

          if (changed)
          {
            break;
          }
        }

        // If moving to this position would change the board state, add it to
        // the set of edges; otherwise, add it to the flood and the flood front.
        if (changed)
        {
          edges.insert(fc);
        } else {
          flood[fcx+fcy*WIDTH] = true;
          front.push_back(fc);
        }
      }

      if (exists)
      {
        break;
      }
    }

    if (exists)
    {
      break;
    }

    // Add the flood to the set of checked positions for this board state.
    done[cbr.blocks] |= flood;

    // Add the edges to the search queue.
    for (const coord& newLoc : edges)
    {
      GameBoard nextState1 = cbr;
      nextState1.tick(std::get<0>(newLoc), std::get<1>(newLoc));

      // Assume that the player will not move while the board is changing, so
      // tick the board until it either stops changing, or it reaches a state
      // that it has already been in (in the case of alternating systems).
      std::unordered_set<board_type> pastStates;
      pastStates.insert(nextState1.blocks);

      while (nextState1.updateable.any())
      {
        nextState1.tick(std::get<0>(newLoc), std::get<1>(newLoc));

        if (pastStates.count(nextState1.blocks))
        {
          break;
        }

        pastStates.insert(nextState1.blocks);
      }

      if (!done.count(nextState1.blocks) ||
          !done.at(nextState1.blocks)
            [std::get<0>(newLoc) + std::get<1>(newLoc)*WIDTH])
      {
        search.emplace_back(std::move(nextState1), newLoc, cns+1);
      }
    }
  }

  return exists;
}

std::string GameBoard::dump() const
{
  std::stringstream output;
  output << std::hex;
  for (int i=0; i<WIDTH*HEIGHT/4; i++)
  {
    int chunk = (8 * blocks[i*4]) + (4 * blocks[i*4+1]) + (2 * blocks[i*4+2]) + blocks[i*4+3];
    output << chunk;
  }

  return output.str();
}
