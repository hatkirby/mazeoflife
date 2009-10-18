#ifndef CHLSTATE_H
#define CHLSTATE_H

class ChooseHighscoreListState : public State {
	public:
		ChooseHighscoreListState();
		void input(SDL_keysym key);
		void render(SDL_Surface* screen);

	private:
		SDL_Surface* background;
		SDL_Surface* pointer;
		int selection;
};

#endif
