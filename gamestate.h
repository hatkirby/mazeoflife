#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState : public State {
	private:
		bool blocks[WIDTH][HEIGHT];
		Uint32 on;
		Uint32 off;

	public:
		GameState(SDL_PixelFormat* fmt);
		void render(SDL_Surface* screen);
};

#endif
