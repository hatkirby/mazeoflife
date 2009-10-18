#include "includes.h"

SDL_Surface* HighscoreList::render()
{
	SDL_Surface* tmp = SDL_CreateRGBSurface(SDL_SWSURFACE || SDL_SRCCOLORKEY, 480, 480, 32, 0,0,0,0);
	Uint32 bgColor = SDL_MapRGB(tmp->format, 255, 255, 255);
	SDL_FillRect(tmp, NULL, bgColor);
	SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, bgColor);
	TTF_Font* posFont = loadFont(40);
	TTF_Font* dataFont = loadFont(25);
	SDL_Color fontColor = {0, 0, 0, 0};

	for (int i=0; i<hslist.size(); i++)
	{
		Highscore h = hslist[i];

		int posw, posh;
                char pos[3]; // 2 max characters in rank plus the colon at the end
                sprintf(pos, "%d:", h.getRank());
		TTF_SizeText(posFont, pos, &posw, &posh);
		SDL_Rect posSpace = {0, (i+1)*40, posw, posh};
		SDL_BlitSurface(TTF_RenderText_Blended(posFont, pos, fontColor), NULL, tmp, &posSpace);

		int namew, nameh;
		char name[26]; // 25 max characters in username plus the space at the beginning
		sprintf(name, " %s", h.getName());
		TTF_SizeText(dataFont, name, &namew, &nameh);
		SDL_Rect nameSpace = {posw, (i+1)*40+((posh/2)-(nameh/2)), namew, nameh};
		SDL_BlitSurface(TTF_RenderText_Blended(dataFont, name, fontColor), NULL, tmp, &nameSpace);

		int lvlw, lvlh;
		char lvl[10]; // 10 max characters in level (based off the fact that 2^32-1 is 10 characters long, and is the highest int)
		sprintf(lvl, "%d", h.getLevel());
		TTF_SizeText(dataFont, lvl, &lvlw, &lvlh);
		SDL_Rect lvlSpace = {480-lvlw, (i+1)*40+((posh/2)-(nameh/2)), lvlw, lvlh};
		SDL_BlitSurface(TTF_RenderText_Blended(dataFont, lvl, fontColor), NULL, tmp, &lvlSpace);
	}

	return tmp;
}

std::vector<Highscore> HighscoreList::getLocalHighscores()
{
	std::vector<Highscore> temp = std::vector<Highscore>();

	std::ifstream exists(getDataFile());
	if (exists)
	{
		FILE* hslist = fopen(getDataFile(), "r");
		int scores;
		fscanf(hslist, "%d%*c", &scores);

		for (int i=0; i<scores; i++)
		{
			int namelen;
			char namelens[4];
			char* name = (char*) calloc(25, sizeof(char));
			int score;

			fscanf(hslist, "%d", &namelen);
			sprintf(namelens, "%%%dc", namelen);
			fscanf(hslist, namelens, name);
			fscanf(hslist, "%d%*c", &score);

			Highscore h = Highscore(name, score);
			h.setRank(i+1);

			temp.push_back(h);
		}

		fclose(hslist);
	}

	return temp;
}

std::vector<Highscore> HighscoreList::getGlobalHighscores()
{

}

LocalHighscoreList::LocalHighscoreList()
{
	this->hslist = getLocalHighscores();
}
