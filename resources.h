#ifndef RESOURCES_H
#define RESOURCES_H

#ifdef WINDOWS
#define RESNAME(title,part) binary_resources_ ## title ## _ ## part
#else
#define RESNAME(title,part) _binary_resources_ ## title ## _ ## part
#endif

#define DEFRES(title) extern int* RESNAME(title,start); extern int* RESNAME(title,size);
#define LOADIMAGE(var,title) SDL_RWops * title ## _rw = SDL_RWFromMem(&RESNAME(title ## _bmp,start), (int) &RESNAME(title ## _bmp,size)); var = SDL_LoadBMP_RW( title ## _rw, 1);

DEFRES(icon_bmp)
DEFRES(title_bmp)
DEFRES(pointer_bmp)
DEFRES(htp1_bmp)
DEFRES(htp2_bmp)
DEFRES(chl_bmp)
DEFRES(hlo_rtm_bmp)
DEFRES(hlo_paartm_bmp)
DEFRES(hlo_passartm_bmp)
DEFRES(mono_ttf)

#endif
