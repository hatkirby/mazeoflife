#include "hslist.h"
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <sstream>
#include <fstream>
#include "util.h"
#include "titlestate.h"

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

	const char* headers = "GET /mol/hslist.php HTTP/1.1\nHost: other.fourisland.com\nUser-Agent: Maze Of Life v3.0\nAccept: text/plain\nKeep-Alive: 300\nConnection: keep-alive\n\n";
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
			printf("Recieved data is of an invalid format (1-%d): %s\n", i, temps);
			throw 4;
		}

		sprintf(namelens, "%%*d%%%dc", namelen);

		if (sscanf(temps, namelens, name) != 1)
		{
			printf("Recieved data is of an invalid format (2-%d): %s\n", i, temps);
			throw 4;
		}

		sprintf(namelens, "%%*d%%*%dc%%d", namelen);

		if (sscanf(temps, namelens, &score) != 1)
		{
			printf("Recieved data is of an invalid format (3-%d): %s\n", i, temps);
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

State* ChooseHighscoreListState::operator() (SDL_Renderer* renderer)
{
	SDL_Texture* background = loadImage(renderer, "resources/chl.bmp");
	SDL_Texture* pointer = loadImage(renderer, "resources/pointer.bmp");
	int selection = 0;
	SDL_Event e;
	
	for (;;)
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, NULL, NULL);
		applyTexture(renderer, pointer, 127, selection==0?306:(selection==1?336:396));
		SDL_RenderPresent(renderer);
		
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return NULL;
			} else if (e.type == SDL_KEYDOWN)
			{
				if ((e.key.keysym.sym == SDLK_UP) && (selection != 0))
				{
					selection--;
				} else if ((e.key.keysym.sym == SDLK_DOWN) && (selection != 2))
				{
					selection++;
				} else if (e.key.keysym.sym == SDLK_RETURN)
				{
					switch (selection)
					{
						case 0: return new DisplayLocalHighscoreListState();
						case 1: return new DisplayGlobalHighscoreListState();
						case 2: return new TitleState();
					}
				}
			}
		}
	}
}

State* DisplayLocalHighscoreListState::operator() (SDL_Renderer* renderer)
{
	SDL_Texture* pointer = loadImage(renderer, "resources/pointer.bmp");
	
	LocalHighscoreList* lhl = new LocalHighscoreList();
	SDL_Surface* list_s = lhl->render();
	SDL_Color fontColor = {0, 0, 0, 0};
	SDL_Surface* title = TTF_RenderText_Blended(loadFont(40), "Highscore List", fontColor);
	SDL_Rect tSpace = {240-(title->w/2), 0, title->w, title->h};
	SDL_BlitSurface(title, NULL, list_s, &tSpace);
	SDL_FreeSurface(title);

	SDL_Surface* options_s = SDL_LoadBMP("resources/hlo_rtm.bmp");
	SDL_Rect oSpace = {0, 440, options_s->w, options_s->h};
	SDL_BlitSurface(options_s, NULL, list_s, &oSpace);
	SDL_FreeSurface(options_s);
	
	SDL_Texture* list = SDL_CreateTextureFromSurface(renderer, list_s);
	SDL_FreeSurface(list_s);
	
	SDL_Event e;
	
	for (;;)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, list, NULL, NULL);
		applyTexture(renderer, pointer, 137, 449);
		SDL_RenderPresent(renderer);
		
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return NULL;
			} else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_RETURN)
				{
					return new ChooseHighscoreListState();
				}
			}
		}
	}
}

State* DisplayGlobalHighscoreListState::operator() (SDL_Renderer* renderer)
{
	SDL_Texture* pointer = loadImage(renderer, "resources/pointer.bmp");
	
	// Display loading message
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	
	SDL_Surface* list_s = SDL_CreateRGBSurface(0, 480, 480, 32, 0,0,0,0);
	Uint32 bgColor = SDL_MapRGB(list_s->format, 255, 255, 255);
	SDL_FillRect(list_s, NULL, bgColor);
	SDL_SetColorKey(list_s, SDL_TRUE, bgColor);
	TTF_Font* dataFont = loadFont(25);
	SDL_Color fontColor = {0, 0, 0, 0};
	SDL_Surface* text = TTF_RenderText_Blended(dataFont, "Fetching highscores....", fontColor);
	SDL_Rect aSpace = {240-(text->w/2), 240-(text->h/2), text->w, text->h};
	SDL_BlitSurface(text, NULL, list_s, &aSpace);
	SDL_FreeSurface(text);

	SDL_Surface* title = TTF_RenderText_Blended(loadFont(40), "Highscore List", fontColor);
	SDL_Rect tSpace = {240-(title->w/2), 0, title->w, title->h};
	SDL_BlitSurface(title, NULL, list_s, &tSpace);
	SDL_FreeSurface(title);

	SDL_Surface* options_s = SDL_LoadBMP("resources/hlo_rtm.bmp");
	SDL_Rect oSpace = {0, 440, options_s->w, options_s->h};
	SDL_BlitSurface(options_s, NULL, list_s, &oSpace);
	SDL_FreeSurface(options_s);
	
	list = SDL_CreateTextureFromSurface(renderer, list_s);
	SDL_FreeSurface(list_s);
	
	m = SDL_CreateMutex();
	
	// Start downloading scores
	SDL_CreateThread(&LoadHighscoreList, "LoadHighscoreList", this);
	
	// Parse keyboard events
	SDL_Event e;
	
	for (;;)
	{
		if (SDL_LockMutex(m) == 0)
		{
			if (lhl != NULL)
			{
				SDL_Surface* list_s = lhl->render();

				SDL_Color fontColor = {0, 0, 0, 0};
				SDL_Surface* title = TTF_RenderText_Blended(loadFont(40), "Highscore List", fontColor);
				SDL_Rect tSpace = {240-(title->w/2), 0, title->w, title->h};
				SDL_BlitSurface(title, NULL, list_s, &tSpace);
				SDL_FreeSurface(title);

				SDL_Surface* options_s = SDL_LoadBMP("resources/hlo_rtm.bmp");
				SDL_Rect oSpace = {0, 440, options_s->w, options_s->h};
				SDL_BlitSurface(options_s, NULL, list_s, &oSpace);
				SDL_FreeSurface(options_s);
	
				list = SDL_CreateTextureFromSurface(renderer, list_s);
				SDL_FreeSurface(list_s);
			
				lhl = NULL;
			}
			
			SDL_UnlockMutex(m);
		}
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, list, NULL, NULL);
		applyTexture(renderer, pointer, 137, 449);
		SDL_RenderPresent(renderer);
		
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				SDL_DestroyMutex(m);
				
				return NULL;
			} else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_RETURN)
				{
					SDL_DestroyMutex(m);
					
					return new ChooseHighscoreListState();
				}
			}
		}
	}
}

int DisplayGlobalHighscoreListState::LoadHighscoreList(void* pParam)
{
	DisplayGlobalHighscoreListState* parent = ((DisplayGlobalHighscoreListState*)pParam);
	if (SDL_LockMutex(parent->m) == 0)
	{
		parent->lhl = new GlobalHighscoreList();
		
		SDL_UnlockMutex(parent->m);
	} else {
		printf("Couldn't lock mutex: %s\n", SDL_GetError());
	}
}
