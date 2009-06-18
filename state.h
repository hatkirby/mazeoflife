#ifndef STATE_H
#define STATE_H

class State
{
	public:
		virtual void render(SDL_Surface* screen) = 0;
};

#endif
