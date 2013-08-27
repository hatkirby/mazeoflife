#include <SDL.h>

#ifndef STATE_H
#define STATE_H

class State
{
	public:
		virtual State* operator() (SDL_Renderer* renderer) {return NULL;};
};

#endif