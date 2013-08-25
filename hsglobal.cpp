#include "includes.h"

GlobalHighscoreListState::GlobalHighscoreListState()
{
	options = SDL_LoadBMP("hlo_rtm.bmp");
	pointer = SDL_LoadBMP("pointer.bmp");

	list = SDL_CreateRGBSurface(SDL_SWSURFACE || SDL_SRCCOLORKEY, 480, 480, 32, 0,0,0,0);
	Uint32 bgColor = SDL_MapRGB(list->format, 255, 255, 255);
	SDL_FillRect(list, NULL, bgColor);
	SDL_SetColorKey(list, SDL_SRCCOLORKEY, bgColor);
	TTF_Font* dataFont = loadFont(25);
	SDL_Color fontColor = {0, 0, 0, 0};
	SDL_Surface* text = TTF_RenderText_Blended(dataFont, "Fetching highscores....", fontColor);
	SDL_Rect aSpace = {240-(text->w/2), 240-(text->h/2), text->w, text->h};
	SDL_BlitSurface(text, NULL, list, &aSpace);

	SDL_Surface* title = TTF_RenderText_Blended(loadFont(40), "Highscore List", fontColor);
	SDL_Rect tSpace = {240-(title->w/2), 0, title->w, title->h};
	SDL_BlitSurface(title, NULL, list, &tSpace);

	SDL_Rect oSpace = {0, 440, options->w, options->h};
	SDL_BlitSurface(options, NULL, list, &oSpace);

	SDL_CreateThread(&LoadHighscoreList, this);
}

int GlobalHighscoreListState::LoadHighscoreList(void* pParam)
{
	GlobalHighscoreList* lhl = new GlobalHighscoreList();
	((GlobalHighscoreListState*)pParam)->list = lhl->render();

	SDL_Color fontColor = {0, 0, 0, 0};
	SDL_Surface* title = TTF_RenderText_Blended(loadFont(40), "Highscore List", fontColor);
	SDL_Rect tSpace = {240-(title->w/2), 0, title->w, title->h};
	SDL_BlitSurface(title, NULL, ((GlobalHighscoreListState*)pParam)->list, &tSpace);

	SDL_Rect oSpace = {0, 440, ((GlobalHighscoreListState*)pParam)->options->w, ((GlobalHighscoreListState*)pParam)->options->h};
	SDL_BlitSurface(((GlobalHighscoreListState*)pParam)->options, NULL, ((GlobalHighscoreListState*)pParam)->list, &oSpace);
}

void GlobalHighscoreListState::input(SDL_keysym key)
{
	if (key.sym == SDLK_RETURN)
	{
		changeState(new ChooseHighscoreListState());
	}
}

void GlobalHighscoreListState::render(SDL_Surface* screen)
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

	SDL_BlitSurface(list, NULL, screen, NULL);

	SDL_Rect pSpace;
	pSpace.x = 137;
	pSpace.y = 449;
	pSpace.w = pointer->w;
	pSpace.h = pointer->h;

	SDL_BlitSurface(pointer, NULL, screen, &pSpace);
}
