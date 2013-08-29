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
		GlobalHighscoreList(Highscore* h);
		SDL_Surface* render();
		bool didFail();

	private:
		typedef HighscoreList super;

	protected:
		bool fail;
};

class ChooseHighscoreListState : public State {
	public:
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);
};

class DisplayLocalHighscoreListState : public State {
	public:
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);
};

class DisplayAndReturnLocalHighscoreListState : public State {
	public:
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);
};

class DisplayGlobalHighscoreListState : public State {
	public:
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);
		
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
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);
		
	private:
		int level;
		int lp;
		char* hsname;
		SDL_Texture* newName;
};

class NewHighscoreState : public State {
	public:
		NewHighscoreState(Highscore* h);
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);
		
	private:
		Highscore* h;
};

class SubmitHighscoreState : public State {
	public:
		SubmitHighscoreState(Highscore* h);
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);
		
	protected:
		Highscore* h;
		SDL_mutex* m;
		GlobalHighscoreList* lhl;
		
	private:
		static int SubmitHighscore(void* pParam);
};

class FailedSubmittingHighscoreState : public State {
	public:
		FailedSubmittingHighscoreState(Highscore* h);
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);
		
	private:
		Highscore* h;
};

class SubmittedHighscoreState : public State {
	public:
		SubmittedHighscoreState(GlobalHighscoreList* lhl, Highscore* h);
		State* operator() (SDL_Window* window, SDL_Renderer* renderer);
		
	private:
		GlobalHighscoreList* lhl;
		Highscore* h;
};

#endif
