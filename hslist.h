#include <SDL.h>
#include <vector>
#include "highscore.h"
#include "state.h"

#ifndef HSLIST_H
#define HSLIST_H

class HighscoreList
{
	public:
		SDL_Surface* render();

	protected:
		std::vector<Highscore> getLocalHighscores();
		std::vector<Highscore> getGlobalHighscores();

		std::vector<Highscore> hslist;
};

class LocalHighscoreList : public HighscoreList {
	public:
		LocalHighscoreList();
};

class GlobalHighscoreList : public HighscoreList {
	public:
		GlobalHighscoreList();
		SDL_Surface* render();

	private:
		typedef HighscoreList super;

	protected:
		bool fail;
};

class ChooseHighscoreListState : public State {
	public:
		State* operator() (SDL_Renderer* renderer);
};

class DisplayLocalHighscoreListState : public State {
	public:
		State* operator() (SDL_Renderer* renderer);
};

class DisplayGlobalHighscoreListState : public State {
	public:
		State* operator() (SDL_Renderer* renderer);
		
	protected:
		SDL_Surface* list_s;
		SDL_Texture* list;
		GlobalHighscoreList* lhl;
		SDL_mutex* m;
		
	private:
		static int LoadHighscoreList(void* pParam);
};

#endif
