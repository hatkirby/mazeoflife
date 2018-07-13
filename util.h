#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#ifndef UTIL_H
#define UTIL_H

void wrap(int& x, int& y);
TTF_Font* loadFont(int size);
const char* getDataFile();
SDL_Texture* loadImage(SDL_Renderer* renderer, std::string file);
void applyTexture(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y);

#endif