#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState : public State {
	private:
		Uint32 player_color;
		int playerx, playery;
		bool newGame;
		bool doneMaking;
		Level level;
		Board board;
		void move(int x, int y);

	public:
		GameState();
		void input(SDLKey key);
		void tick();
		void render(SDL_Surface* screen);
};

#endif
