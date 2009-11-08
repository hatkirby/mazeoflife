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

#endif
