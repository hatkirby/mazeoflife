#include <SDL.h>
#include <vector>
#include "highscore.h"
#include "state.h"

#ifndef HSLIST_H
#define HSLIST_H

typedef std::vector<Highscore*> hslist_t;

void resetRanks(hslist_t in);

class HighscoreList
{
	public:
		SDL_Surface* render();

	protected:
		hslist_t getLocalHighscores();
		hslist_t getGlobalHighscores();

		hslist_t hslist;
};

class LocalHighscoreList : public HighscoreList {
	public:
		LocalHighscoreList();
		int addHighscore(Highscore* h);
		void writeHighscores();
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

class DisplayAndReturnLocalHighscoreListState : public State {
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

class EnterHighscoreState : public State {
	public:
		EnterHighscoreState(int level);
		State* operator() (SDL_Renderer* renderer);
		
	private:
		int level;
		int lp;
		char* hsname;
		SDL_Texture* newName;
};

class NewHighscoreState : public State {
	public:
		NewHighscoreState(Highscore* h);
		State* operator() (SDL_Renderer* renderer);
		
	private:
		Highscore* h;
};

#endif
