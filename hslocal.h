#ifndef HSLOCAL_H
#define HSLOCAL_H

class LocalHighscoreListState : public State {
	public:
		LocalHighscoreListState(bool fromGame);
		void input(SDL_keysym key);
		void render(SDL_Surface* screen);

	private:
		SDL_Surface* list;
		SDL_Surface* options;
		SDL_Surface* pointer;
		int selection;
		bool fromGame;
};

#endif
