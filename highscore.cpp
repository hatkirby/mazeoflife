#include "highscore.h"

Highscore::Highscore(char* name, int level)
{
	this->name = name;
	this->level = level;
}

char* Highscore::getName()
{
	return name;
}

int Highscore::getLevel()
{
	return level;
}

void Highscore::setRank(int rank)
{
	this->rank = rank;
}

int Highscore::getRank()
{
	return rank;
}
