#ifndef CORE_H
#define CORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

#define CORE_FLAGS 0 //SDL_WINDOW_FULLSCREEN

#define CORE_IMG_INIT_FLAGS IMG_INIT_PNG
#define CORE_WIN_POS_X SDL_WINDOWPOS_CENTERED
#define CORE_WIN_POS_Y SDL_WINDOWPOS_CENTERED

int* 			CoreSetup 		(int w, int h);
SDL_Window* 	CoreInit 		();
int 			CoreInput 		(SDL_Event*, int* keys);
void 			CoreSetKeys 	(SDL_Event*, int* keys, int mode);
void 			CoreShutdown 	(SDL_Window*, int*);
SDL_Surface* 	CoreLoadSurface (SDL_PixelFormat*, const char*);

#endif
