#include "includes.h"

NewHighscoreState::NewHighscoreState(int level)
{
	this->level = level;

	options = SDL_LoadBMP("resources/hlo_passartm.bmp");
	pointer = SDL_LoadBMP("resources/pointer.bmp");

	lhl = new NewHighscoreList(level);
	newpos = lhl->getNewPos();
	list = lhl->render();

	SDL_Color fontColor = {0, 0, 0, 0};
	SDL_Surface* title = TTF_RenderText_Blended(loadFont(40), "New Highscore!", fontColor);
	SDL_Rect tSpace = {240-(title->w/2), 0, title->w, title->h};
	SDL_BlitSurface(title, NULL, list, &tSpace);

	this->enterName = true;
	this->lp = 0;
	this->hsname = (char*) calloc(25, sizeof(char));

	SDL_Surface* text = TTF_RenderText_Blended(loadFont(25), "Enter Your Name", fontColor);
	SDL_Rect oSpace = {240-(text->w/2), 440, text->w, text->h};
	SDL_BlitSurface(text, NULL, list, &oSpace);

	selection = 0;

	int posw, posh;
        char pos[3]; // 2 max characters in rank plus the colon at the end
	sprintf(pos, "%d:", newpos);
	char name[26]; // 25 max characters in username plus the space at the beginning
	sprintf(name, " %s", hsname);
	newName = TTF_RenderText_Blended(loadFont(25), name, fontColor);
	TTF_SizeText(loadFont(40), pos, &posw, &posh);
	rntSpace.x = posw;
	rntSpace.y = newpos*40+((posh/2)-(newName->h/2));
	rntSpace.w = newName->w;
	rntSpace.h = newName->h;

	SDL_EnableUNICODE(1);
}

void NewHighscoreState::input(SDL_keysym key)
{
	if (enterName)
	{
		if ((key.sym == SDLK_BACKSPACE) && (lp > 0))
		{
			hsname[--lp] = 0;

			SDL_Color fontColor = {0, 0, 0, 0};
			char name[26]; // 25 max characters in username plus the space at the beginning
			sprintf(name, " %s", hsname);
			newName = TTF_RenderText_Blended(loadFont(25), name, fontColor);
			rntSpace.w = newName->w;
			rntSpace.h = newName->h;
		} else if ((key.sym == SDLK_RETURN) && (hsname[0] != 0))
		{
			SDL_EnableUNICODE(0);
			enterName = false;

			lhl->addToList(hsname);
			LocalHighscoreList* lhl2 = new LocalHighscoreList();
			list = lhl2->render();

			SDL_Color fontColor = {0, 0, 0, 0};
			SDL_Surface* title = TTF_RenderText_Blended(loadFont(40), "New Highscore!", fontColor);
			SDL_Rect tSpace = {240-(title->w/2), 0, title->w, title->h};
			SDL_BlitSurface(title, NULL, list, &tSpace);

			SDL_Rect oSpace = {0, 440, options->w, options->h};
			SDL_BlitSurface(options, NULL, list, &oSpace);
		} else if (((key.unicode & 0xFF80) == 0) && (key.unicode >= 32 && key.unicode < 127) && (lp < 25))
		{
			hsname[lp++] = key.unicode & 0x7f;
			hsname[lp] = 0;

			SDL_Color fontColor = {0, 0, 0, 0};
			char name[26]; // 25 max characters in username plus the space at the beginning
			sprintf(name, " %s", hsname);
			newName = TTF_RenderText_Blended(loadFont(25), name, fontColor);
			rntSpace.w = newName->w;
			rntSpace.h = newName->h;
		}
	} else {
		if ((key.sym == SDLK_LEFT) && (selection != 0))
		{
			selection--;
		} else if ((key.sym == SDLK_RIGHT) && (selection != 2))
		{
			selection++;
		} else if (key.sym == SDLK_RETURN)
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
		}
	}
}

void NewHighscoreState::render(SDL_Surface* screen)
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

	SDL_Rect eSpace = {0, newpos*40, 480, 40};
	SDL_FillRect(screen, &eSpace, SDL_MapRGB(screen->format, 255, 255, 0));

	SDL_BlitSurface(list, NULL, screen, NULL);

	if (enterName)
	{
		SDL_BlitSurface(newName, NULL, screen, &rntSpace);
	} else {
		SDL_Rect pSpace;
		pSpace.x = (selection==0?13:(selection==1?138:284));
		pSpace.y = 448;
		pSpace.w = pointer->w;
		pSpace.h = pointer->h;

		SDL_BlitSurface(pointer, NULL, screen, &pSpace);
	}
}

NewHighscoreState::NewHighscoreList::NewHighscoreList(int level)
{
	this->hslist = getLocalHighscores();

	if (this->hslist.empty())
	{
		Highscore n = Highscore("", level);
		n.setRank(1);

		this->hslist.push_back(n);
		this->newpos = n.getRank();
	} else {
		std::vector<Highscore>::iterator it;
		bool found = false;
		int lastrank;
		int i=0;

		for (it=this->hslist.begin(); it<this->hslist.end(); it++)
		{
			Highscore h = *it;
			lastrank = h.getRank();

			if (!found)
			{
				if (h.getLevel() < level)
				{
					Highscore n = Highscore("", level);
					n.setRank(h.getRank());

					this->hslist.insert(it, n);
					this->newpos = n.getRank();

					if (this->hslist.size() > 10)
					{
						this->hslist.pop_back();
					}

					found = true;
				}
			} else {
				//this->hslist[i].setRank(h.getRank()+1);
			}

			i++;
		}

		if (!found)
		{
			Highscore n = Highscore("", level);
			n.setRank(lastrank+1);

			this->hslist.push_back(n);
			this->newpos = n.getRank();
		}
	}
}

int NewHighscoreState::NewHighscoreList::getNewPos()
{
	return newpos;
}

void NewHighscoreState::NewHighscoreList::addToList(char* name)
{
	FILE* hsfile = fopen(getDataFile(), "w");
	fprintf(hsfile, "%d ", this->hslist.size());

	std::vector<Highscore>::iterator it;

	for (it=this->hslist.begin(); it<this->hslist.end(); it++)
	{
		Highscore h = *it;

		if (h.getName() == "")
		{
			h = Highscore(name, h.getLevel());
			h.setRank(newpos);
		}

		fprintf(hsfile, "%d%s%d ", strlen(h.getName()), h.getName(), h.getLevel());
	}

	fclose(hsfile);
}
