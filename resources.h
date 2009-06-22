#ifndef RESOURCES_H
#define RESOURCES_H

#ifdef WINDOWS
#define LOADIMAGE(var,title) SDL_RWops * title ## _rw = SDL_RWFromMem(&binary_ ## title ## _bmp_start, (int) &binary_ ## title ## _bmp_size); var = SDL_LoadBMP_RW( title ## _rw, 1);
#define DEFIMAGE(title) extern int* binary_ ## title ## _bmp_start; extern int* binary_ ## title ## _bmp_size;
#else
#define LOADIMAGE(var,title) SDL_RWops * title ## _rw = SDL_RWFromMem(&_binary_ ## title ## _bmp_start, (int) &_binary_ ## title ## _bmp_size); var = SDL_LoadBMP_RW( title ## _rw, 1);
#define DEFIMAGE(title) extern int* _binary_ ## title ## _bmp_start; extern int* _binary_ ## title ## _bmp_size;
#endif

DEFIMAGE(icon)
DEFIMAGE(title)
DEFIMAGE(pointer)
DEFIMAGE(htp1)
DEFIMAGE(htp2)
DEFIMAGE(chl)

#endif
