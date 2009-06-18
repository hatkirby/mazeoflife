#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState : public State {
	private:
		bool blocks[WIDTH][HEIGHT];
		Uint32 on;
		Uint32 off;
		Uint32 player_color;
		int playerx, playery;
		void move(int x, int y);
		void wrap(int* x, int* y);

	public:
		GameState(SDL_PixelFormat* fmt);
		void input(SDLKey key);
		void render(SDL_Surface* screen);
};

#endif
