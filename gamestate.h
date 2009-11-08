#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState : public State {
	public:
		GameState();
		void input(SDL_keysym key);
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
				void incrementLevel();
		};

		struct Info {
			int playerx, playery;
			Level level;
			bool doneMaking;
			int gens;
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
		Uint32 event_color;
		bool newGame;
		Info* info;
		Board board;
		void move(int x, int y);
};

#endif
