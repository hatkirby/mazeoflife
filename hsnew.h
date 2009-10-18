#ifndef HSNEW_H
#define HSNEW_H

class NewHighscoreState : public State {
	public:
		NewHighscoreState(int level);
		void input(SDL_keysym key);
		void render(SDL_Surface* screen);

	private:
		class NewHighscoreList : public HighscoreList {
			public:
				NewHighscoreList(int level);
				int getNewPos();
				void addToList(char* name);

			private:
				int newpos;
		};

		SDL_Surface* list;
		SDL_Surface* options;
		SDL_Surface* pointer;
		int selection;
		int level;
		int newpos;
		int lp;
		char* hsname;
		bool enterName;
		SDL_Rect rntSpace;
		SDL_Surface* newName;
		NewHighscoreList* lhl;
};

#endif
