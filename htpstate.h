#ifndef HTPSTATE_H
#define HTPSTATE_H

class HowToPlayState : public State {
	public:
		HowToPlayState();
		void input(SDLKey key);
		void render(SDL_Surface* screen);

	private:
		SDL_Surface* background1;
		SDL_Surface* background2;
		SDL_Surface* pointer;
		bool secondPage;
		int selection;
};

#endif
