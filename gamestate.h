#include <SDL.h>
#include "state.h"

#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState : public State {
	public:
		State* operator() (SDL_Renderer* renderer);
};

#endif