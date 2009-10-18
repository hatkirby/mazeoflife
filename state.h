#ifndef STATE_H
#define STATE_H

class State
{
	public:
		virtual void input(SDL_keysym key) {};
		virtual void tick() {};
		virtual void render(SDL_Surface* screen) {};
};

#endif
