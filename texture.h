#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include "utils.h"

typedef struct _texture_
{
	SDL_Texture*  	tex;
	SDL_Renderer* 	rend;
	int 			w;
	int 			h;
}
Texture;

Texture* 	TextureLoad 		(SDL_Renderer* rend, const char* path);
Texture* 	TextureFromSurface 	(SDL_Renderer* rend, SDL_Surface* surface);
void 		TextureRender 		(Texture* t, SDL_Rect* src, SDL_Rect* dest);
void 		TextureRenderAt 	(Texture* t, int x, int y);
void 		TextureFree 		(Texture* t);

#endif
