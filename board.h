#ifndef BOARD_H
#define BOARD_H

class Board
{
	private:
		bool blocks[WIDTH][HEIGHT];

	public:
		Board();
		Board(Level level);
		bool isObstructed(int x, int y);
		void render(SDL_Surface* screen, Level level);
};

#endif
