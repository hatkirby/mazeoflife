#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState : public State {
	private:
		Uint32 player_color;
		int playerx, playery;
		bool newGame;
		bool doneMaking;
		Board board;
		void move(int x, int y);

	public:
		GameState(SDL_PixelFormat* fmt);
		void input(SDLKey key);
		void render(SDL_Surface* screen);
};

#endif
