#include "core.h"

const char TITLE [256] = ".exe";
static int CORE_WIN_WIDTH = 1920;
static int CORE_WIN_HEIGHT = 1080;

int* CoreSetup(int w, int h){
	int* keys;
	
	CORE_WIN_WIDTH = w;
	CORE_WIN_HEIGHT = h;

	keys = (int*) calloc(64, sizeof(int));
	if(!keys) printf("keys not allocated!\n");

	return keys;
}

SDL_Window* CoreInit(){
	SDL_Window* window = NULL;
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("ERRNOINIT: In SDL_Init: \"%s\"\n", SDL_GetError());
		return NULL;
	}
	if(IMG_Init(CORE_IMG_INIT_FLAGS) < 0){
		printf("ERRNOINIT: In SDL_IMG_Init: %s\n", SDL_GetError());
		return NULL;
	}
	window = SDL_CreateWindow(TITLE, CORE_WIN_POS_X, CORE_WIN_POS_Y,
									 CORE_WIN_WIDTH, CORE_WIN_HEIGHT,
									 CORE_FLAGS);
	if(window == NULL){
		printf("ERRSDL: In SDL_CreateWindow: %s\n", SDL_GetError());
		return NULL;
	}
	
	return window;
}

int CoreInput(SDL_Event* ev, int* keys)
{
	while(SDL_PollEvent(ev)){
		switch(ev->type){
			case SDL_QUIT: 		return 1;
			case SDL_KEYDOWN: 	CoreSetKeys(ev, keys, 1); break;
			case SDL_KEYUP: 	CoreSetKeys(ev, keys, 0); break;
			default: break;
		}
		if(keys[4]) return 1;
	}

	return 0;
}

void CoreSetKeys(SDL_Event* ev, int* keys, int mode)
{
	switch(ev->key.keysym.sym){
		case SDLK_a: keys[0] = mode; break;
		case SDLK_w: keys[1] = mode; break;
		case SDLK_s: keys[2] = mode; break;
		case SDLK_d: keys[3] = mode; break;
		case SDLK_ESCAPE: keys[4] = mode; break;
		case SDLK_SPACE: keys[5] = mode; break;
		default: break;
	}

}

void CoreShutdown(SDL_Window* window){
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Surface* CoreLoadSurface(SDL_PixelFormat* fmt, const char* path){
	SDL_Surface* optimized;	
	SDL_Surface* surface = IMG_Load(path);
	if(fmt == NULL){
		surface = surface;
	}
	if(surface == NULL){
		printf("ERRSDL: %s || ERRPATH: %s\n", IMG_GetError(), path);
		return NULL;
	}
	
	optimized = SDL_ConvertSurface(surface, fmt, 0);
	if(optimized == NULL){
		printf("ERRSDL: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
		return NULL;
	}
	SDL_FreeSurface(surface);
	return optimized;
}

