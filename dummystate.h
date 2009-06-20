#ifndef DUMMYSTATE_H
#define DUMMYSTATE_H

class DummyState : public State
{
	public:
		DummyState() {};
		void input(SDLKey key) {};
		void render(SDL_Surface* screen) {};
};

#endif
