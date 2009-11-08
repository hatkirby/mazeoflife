#ifndef HSGLOBAL_H
#define HSGLOBAL_H

class GlobalHighscoreListState : public State {
	public:
		GlobalHighscoreListState();
		void input(SDL_keysym key);
		void render(SDL_Surface* screen);

	private:
		static int LoadHighscoreList(void* pParam);

		SDL_Surface* list;
		SDL_Surface* options;
		SDL_Surface* pointer;
};

#endif
