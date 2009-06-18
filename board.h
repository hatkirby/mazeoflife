#ifndef BOARD_H
#define BOARD_H

class Board
{
	private:
		Uint32 on;
		Uint32 off;
		bool blocks[WIDTH][HEIGHT];

	public:
		Board();
		bool isObstructed(int x, int y);
		void render(SDL_Surface* screen);
};

#endif
