#include "hslist.h"
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <sstream>
#include <fstream>
#include "util.h"

SDL_Surface* HighscoreList::render()
{
	SDL_Surface* tmp = SDL_CreateRGBSurface(0, 480, 480, 32, 0,0,0,0);
	Uint32 bgColor = SDL_MapRGB(tmp->format, 255, 255, 255);
	SDL_FillRect(tmp, NULL, bgColor);
	SDL_SetColorKey(tmp, SDL_TRUE, bgColor);
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
	IPaddress ipaddress;

	if (SDLNet_ResolveHost(&ipaddress, "other.fourisland.com", 80) == -1)
	{
		printf("Could not resolve host \"other.fourisland.com\": %s\n", SDLNet_GetError());
		throw 1;
	}

	TCPsocket tcpsock = SDLNet_TCP_Open(&ipaddress);
	if (!tcpsock)
	{
		printf("Could not connect to host \"other.fourisland.com\": %s\n", SDLNet_GetError());
		throw 2;
	}

	char* headers = "GET /mol/hslist.php HTTP/1.1\nHost: other.fourisland.com\nUser-Agent: Maze Of Life v2.0\nAccept: text/plain\nKeep-Alive: 300\nConnection: keep-alive\n\n";
	if (SDLNet_TCP_Send(tcpsock, headers, strlen(headers)+1) < strlen(headers))
	{
		printf("Connection closed by peer: %s\n", SDLNet_GetError());
		throw 3;
	}

	std::stringstream download(std::stringstream::in | std::stringstream::out);
	char hslist[1024];
	SDLNet_TCP_Recv(tcpsock, hslist, 1024);
	download << hslist;
	SDLNet_TCP_Close(tcpsock);

	char temps[256];
	download.getline(temps,256);
	while (strlen(temps) != 1)
	{
		download.getline(temps,256);
	}

	std::vector<Highscore> temp = std::vector<Highscore>();
	int scores;
	download.getline(temps, 256);
	if (sscanf(temps, "%d%*c", &scores) != 1)
	{
		printf("Recieved data is of an invalid format: %s\n", temps);
		throw 4;
	}

	for (int i=0; i<scores; i++)
	{
		int namelen;
		char namelens[13];
		char* name = (char*) calloc(25, sizeof(char));
		int score;
		download.getline(temps, 256);

		if (sscanf(temps, "%d", &namelen) != 1)
		{
			printf("Recieved data is of an invalid format: %s\n", temps);
			throw 4;
		}

		sprintf(namelens, "%%*d%%%dc", namelen);

		if (sscanf(temps, namelens, name) != 1)
		{
			printf("Recieved data is of an invalid format: %s\n", temps);
			throw 4;
		}

		sprintf(namelens, "%%*d%%*%dc%%d%%*c", namelen);

		if (sscanf(temps, namelens, &score) != 1)
		{
			printf("Recieved data is of an invalid format: %s\n", temps);
			throw 4;
		}

		Highscore h = Highscore(name, score);
		h.setRank(i+1);

		temp.push_back(h);
	}

	return temp;
}

LocalHighscoreList::LocalHighscoreList()
{
	this->hslist = getLocalHighscores();
}

GlobalHighscoreList::GlobalHighscoreList()
{
	fail = false;

	try
	{
		this->hslist = getGlobalHighscores();
	} catch (int e)
	{
		fail = true;
	}
}

SDL_Surface* GlobalHighscoreList::render()
{
	if (fail)
	{
		SDL_Surface* tmp = SDL_CreateRGBSurface(0, 480, 480, 32, 0,0,0,0);
		Uint32 bgColor = SDL_MapRGB(tmp->format, 255, 255, 255);
		SDL_FillRect(tmp, NULL, bgColor);
		SDL_SetColorKey(tmp, SDL_TRUE, bgColor);
		TTF_Font* dataFont = loadFont(25);
		SDL_Color fontColor = {0, 0, 0, 0};
		SDL_Surface* text = TTF_RenderText_Blended(dataFont, "Error retrieving highscores", fontColor);
		SDL_Rect tSpace = {240-(text->w/2), 240-(text->h/2), text->w, text->h};
		SDL_BlitSurface(text, NULL, tmp, &tSpace);

		return tmp;
	} else {
		return super::render();
	}
}
