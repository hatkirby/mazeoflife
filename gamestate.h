#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState : public State {
	public:
		GameState();
		void input(SDLKey key);
		void tick();
		void render(SDL_Surface* screen);

		class Level
		{
			private:
				int level;
				Uint32 alive[5];
				Uint32 dead[5];

			public:
				Level();
				int getLevel();
				int getLevelGroup();
				bool checkSquare(int x, int y);
				Uint32 getAliveColor();
				Uint32 getDeadColor();
		};

		struct Info {
			int playerx, playery;
			Level level;
		};

		class Board
		{
			private:
				bool blocks[WIDTH][HEIGHT];
				void incrementIfNeighbor(int x, int y, bool temp[WIDTH][HEIGHT], int* tick);
				GameState::Info* info;

			public:
				Board();
				Board(GameState::Info* info);
				bool isObstructed(int x, int y);
				void render(SDL_Surface* screen);
				void tick();
		};

	private:
		Uint32 player_color;
		bool newGame;
		bool doneMaking;
		Info info;
		Board board;
		void move(int x, int y);
};

#endif
