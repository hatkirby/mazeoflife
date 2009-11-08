#include "includes.h"

SubmitHighscoreListState::SubmitHighscoreListState(char* hsname, int level)
{
	LOADIMAGE(pointer,pointer)

	this->hsname = hsname;
	this->level = level;
	this->scoreSent = false;
	this->selection = 0;

	list = SDL_CreateRGBSurface(SDL_SWSURFACE || SDL_SRCCOLORKEY, 480, 480, 32, 0,0,0,0);
	Uint32 bgColor = SDL_MapRGB(list->format, 255, 255, 255);
	SDL_FillRect(list, NULL, bgColor);
	SDL_SetColorKey(list, SDL_SRCCOLORKEY, bgColor);
	TTF_Font* dataFont = loadFont(25);
	SDL_Color fontColor = {0, 0, 0, 0};
	SDL_Surface* text = TTF_RenderText_Blended(dataFont, "Sending highscore....", fontColor);
	SDL_Rect aSpace = {240-(text->w/2), 240-(text->h/2), text->w, text->h};
	SDL_BlitSurface(text, NULL, list, &aSpace);

	SDL_Surface* title = TTF_RenderText_Blended(loadFont(40), "Highscore List", fontColor);
	SDL_Rect tSpace = {240-(title->w/2), 0, title->w, title->h};
	SDL_BlitSurface(title, NULL, list, &tSpace);

	SDL_CreateThread(&LoadHighscoreList, this);
}

int SubmitHighscoreListState::LoadHighscoreList(void* pParam)
{
	SubmitHighscoreList* lhl = new SubmitHighscoreList(((SubmitHighscoreListState*)pParam)->hsname, ((SubmitHighscoreListState*)pParam)->level);
	((SubmitHighscoreListState*)pParam)->list = lhl->render();

	SDL_Color fontColor = {0, 0, 0, 0};
	SDL_Surface* title = TTF_RenderText_Blended(loadFont(40), "Highscore List", fontColor);
	SDL_Rect tSpace = {240-(title->w/2), 0, title->w, title->h};
	SDL_BlitSurface(title, NULL, ((SubmitHighscoreListState*)pParam)->list, &tSpace);

	if (lhl->hasFailed())
	{
		LOADIMAGE(((SubmitHighscoreListState*)pParam)->options,hlo_passartm)
	} else {
		LOADIMAGE(((SubmitHighscoreListState*)pParam)->options,hlo_paartm)
	}

	SDL_Rect oSpace = {0, 440, ((SubmitHighscoreListState*)pParam)->options->w, ((SubmitHighscoreListState*)pParam)->options->h};
	SDL_BlitSurface(((SubmitHighscoreListState*)pParam)->options, NULL, ((SubmitHighscoreListState*)pParam)->list, &oSpace);

	((SubmitHighscoreListState*)pParam)->fail = lhl->hasFailed();
	((SubmitHighscoreListState*)pParam)->newpos = lhl->getNewPos();
	((SubmitHighscoreListState*)pParam)->scoreSent = true;
}

void SubmitHighscoreListState::input(SDL_keysym key)
{
	if (scoreSent)
	{
		if ((key.sym == SDLK_LEFT) && (selection != 0))
		{
		        selection--;
		} else if ((key.sym == SDLK_RIGHT) && (selection != (fail?2:1)))
		{
		        selection++;
		} else if (key.sym == SDLK_RETURN)
		{
			if (fail)
			{
				switch (selection)
				{
					case 0:
						changeState(new GameState());

				                break;
					case 1:
						changeState(new SubmitHighscoreListState(hsname, level));

						break;
					case 2:
						changeState(new TitleState());

						break;
				}
			} else {
				switch (selection)
				{
				        case 0:
				                changeState(new GameState());

				                break;
				        case 1:
				                changeState(new TitleState());

				                break;
				}
			}
		}
	}
}

void SubmitHighscoreListState::render(SDL_Surface* screen)
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

	if (scoreSent)
	{
		SDL_Rect pSpace;
		if (fail)
		{
			pSpace.x = (selection==0?13:(selection==1?138:284));
			pSpace.y = 448;
		} else {
		        pSpace.x = (selection==0?52:225);
		        pSpace.y = 447;
		}
		pSpace.w = pointer->w;
		pSpace.h = pointer->h;

		SDL_BlitSurface(pointer, NULL, screen, &pSpace);

		SDL_Rect eSpace = {0, newpos*40, 480, 40};
		SDL_FillRect(screen, &eSpace, SDL_MapRGB(screen->format, 255, 255, 0));
	}

	SDL_BlitSurface(list, NULL, screen, NULL);
}

SubmitHighscoreListState::SubmitHighscoreList::SubmitHighscoreList(char* hsname, int level)
{
	fail = false;

	try
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

		char body[256];
		sprintf(body, "name=%s&level=%d", hsname, level);
		char headers[256];
		sprintf(headers, "POST /mol/hslist.php?add HTTP/1.1\nHost: other.fourisland.com\nUser-Agent: Maze Of Life v2.0\nAccept: text/plain\nKeep-Alive: 300\nConnection: keep-alive\nContent-Type: application/x-www-form-urlencoded\nContent-Length: %d\n\n%s\n", strlen(body), body);
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

		int rank;
		download.getline(temps, 256);
		if (sscanf(temps, "%d%*c", &rank) != 1)
		{
			printf("Recieved data is of an invalid format: %s\n", temps);
			throw 4;
		}

		this->hslist = getGlobalHighscores();

		if (this->hslist.empty())
		{
			printf("Global Highscore List cannot be empty after adding a score to it.\n");
			throw 5;
		}

		if (rank > 10)
		{
			Highscore temph(hsname, level);
			temph.setRank(rank);

			this->hslist[9] = temph;
			this->newpos = 10;
		} else {
			std::vector<Highscore>::iterator it;
			bool found = false;

			for (it=this->hslist.begin(); it<this->hslist.end(); it++)
			{
				Highscore h = *it;

				if (!found)
				{
					if ((strcmp(h.getName(),hsname) == 0) && (h.getLevel() == level))
					{
						this->newpos = h.getRank();
						found = true;
					}
				}
			}
		}
	} catch (int e)
	{
		fail = true;
	}
}

int SubmitHighscoreListState::SubmitHighscoreList::getNewPos()
{
	return newpos;
}

bool SubmitHighscoreListState::SubmitHighscoreList::hasFailed()
{
	return fail;
}
