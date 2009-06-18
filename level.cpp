#include "includes.h"

Level::Level()
{
	level = 1;

	alive[0] = getColor(0, 0, 0); // Black
	alive[1] = getColor(255, 0, 0); // Red
	alive[2] = getColor(0, 255, 0); // Green
	alive[3] = getColor(85, 85, 85); // Dark Gray
	alive[4] = getColor(255, 0, 255); // Magenta

	dead[0] = getColor(255, 255, 255); // White
	dead[1] = getColor(255, 192, 203); // Pink
	dead[2] = getColor(0, 255, 255); // Cyan
	dead[3] = getColor(170, 170, 170); // Light Gray
	dead[4] = getColor(255, 128, 0); // Orange
}

int Level::getLevel()
{
	return level;
}

int Level::getLevelGroup()
{
	return (level/10)+1;
}

bool Level::checkSquare(int x, int y)
{
	switch (getLevelGroup())
	{
		case 1:
	                return ((x>13)&&(x<16)&&(y>13)&&(y<16));
		case 2:
	                return ((x>13)&&(x<17)&&(y>13)&&(y<17));
		case 3:
		case 4:
	                return ((x>12)&&(x<18)&&(y>12)&&(y<18));
		case 5:
		case 6:
			return ((x>11)&&(x<19)&&(y>11)&&(y<19));
		default:
			return true;
	}
}

Uint32 Level::getAliveColor()
{
	return alive[(getLevelGroup()-1)%5];
}

Uint32 Level::getDeadColor()
{
	return dead[(getLevelGroup()-1)%5];
}
