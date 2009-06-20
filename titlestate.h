#ifndef TITLESTATE_H
#define TITLESTATE_H

class TitleState : public State {
	public:
		TitleState();
		void input(SDLKey key);
		void render(SDL_Surface* screen);

	private:
		SDL_Surface* background;
		SDL_Surface* pointer;
		int selection;
};

#endif
