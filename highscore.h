#ifndef HIGHSCORE_H
#define HIGHSCORE_H

class Highscore {
	public:
		Highscore(char* name, int level);
		char* getName();
		int getLevel();
		void setRank(int rank);
		int getRank();

	private:
		char* name;
		int level;
		int rank;
};

#endif
