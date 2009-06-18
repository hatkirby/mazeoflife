#ifndef LEVEL_H
#define LEVEL_H

class Level
{
	private:
		int level;
		Uint32 alive[5];
		Uint32 dead[5];

	public:
		Level();
		int getLevel();
		int getLevelGroup();
		bool checkSquare(int x, int y);
		Uint32 getAliveColor();
		Uint32 getDeadColor();
};

#endif
