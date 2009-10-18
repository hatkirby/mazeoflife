#ifndef MAZEOFLIFE_H
#define MAZEOFLIFE_H

const int WIDTH = 30;
const int HEIGHT = 30;
const int TICKDELAY = 10;

void wrap(int* x, int* y);
Uint32 getColor(int r, int g, int b);
void changeState(State* nState);
Uint32 tick(Uint32 interval, void *param);
TTF_Font* loadFont(int size);
const char* getDataFile();

#endif
