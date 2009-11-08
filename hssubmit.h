#ifndef HSSUBMIT_H
#define HSSUBMIT_H

class SubmitHighscoreListState : public State {
	public:
		SubmitHighscoreListState(char* hsname, int level);
		void input(SDL_keysym key);
		void render(SDL_Surface* screen);

	private:
		static int LoadHighscoreList(void* pParam);

		class SubmitHighscoreList : public GlobalHighscoreList {
			public:
				SubmitHighscoreList(char* hsname, int level);
				int getNewPos();
				bool hasFailed();

			private:
				int newpos;
		};

		SDL_Surface* list;
		SDL_Surface* options;
		SDL_Surface* pointer;
		char* hsname;
		int level;
		bool scoreSent;
		int selection;
		int newpos;
		bool fail;
};

#endif
