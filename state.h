#ifndef STATE_H
#define STATE_H

class State
{
	public:
		virtual void input(SDLKey key) = 0;
		virtual void tick() {};
		virtual void render(SDL_Surface* screen) = 0;
};

#endif
