#include "utils.h"

SDL_Texture* UtilsLoadTexture(SDL_Renderer* renderer, const char* path, int* w, int* h)
{
	SDL_Texture* texture = NULL;	

	SDL_Surface* surface = IMG_Load(path);
	if(surface == NULL){
		DEBUGMSG(IMG_GetError());
		return NULL;
	}
	
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(!texture)
		DEBUGMSG(SDL_GetError());

	if(w) *w = surface->w;
	if(h) *h = surface->h;

	SDL_FreeSurface(surface);
	return texture;
}

SDL_Texture* UtilsLoadText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const char* msg, int* w, int* h)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, msg, color);
	if(!surface){
		DEBUGMSG(TTF_GetError());
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(!texture){
		DEBUGMSG(SDL_GetError());
		SDL_FreeSurface(surface);
		return NULL;
	}

	if(w) *w = surface->w;
	if(h) *h = surface->h;

	SDL_FreeSurface(surface);
	return texture;
}
