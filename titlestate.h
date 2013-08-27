#include <SDL.h>
#include "state.h"

#ifndef TITLESTATE_H
#define TITLESTATE_H

class TitleState : public State {
	public:
		State* operator() (SDL_Renderer* renderer);
};

#endif