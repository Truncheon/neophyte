#include "texture.h"

Texture* TextureFromSurface(SDL_Renderer* rend, SDL_Surface* surface)
{	
	if(!rend || !surface){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	Texture* t = (Texture*) malloc(sizeof(Texture));
	if(!t){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	//TODO: Investigate
	//SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));
	t->tex = SDL_CreateTextureFromSurface(rend, surface);
	if(!t->tex){
		DEBUGMSG(SDL_GetError());
		free(t);
		return NULL;
	}

	t->rend = rend;
	t->w = surface->w;
	t->h = surface->h;

	//printf("%d, %d, no src\n", t->w, t->h);

	return t;
}

Texture* TextureLoad(SDL_Renderer* rend, const char* path)
{
	if(!rend || !path){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	Texture* t = (Texture*) malloc(sizeof(Texture));
	if(!t){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	SDL_Surface* surface = IMG_Load(path);
	if(surface == NULL){
		DEBUGMSG(IMG_GetError());
		free(t);
		return NULL;
	}
	
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));
	t->tex = SDL_CreateTextureFromSurface(rend, surface);
	if(!t->tex){
		DEBUGMSG(SDL_GetError());
		free(t);
		return NULL;
	}

	t->rend = rend;
	t->w = surface->w;
	t->h = surface->h;
	
	SDL_FreeSurface(surface);
	return t;
}

void TextureRender(Texture* t, SDL_Rect* src, SDL_Rect* dest)
{
	if(!t || !t->rend || !src || !dest){
		DEBUGMSG(ERRPARAM);
		return;
	}

	//printf("src is (%d %d %d %d)\n", src->x, src->y, src->w, src->h);
	//printf("dest is (%d %d %d %d)\n", dest->x, dest->y, dest->w, dest->h);
	SDL_RenderCopy(t->rend, t->tex, src, dest);
}

void TextureRenderAt(Texture* t, int x, int y)
{
	if(!t){
		DEBUGMSG(ERRPARAM);
		return;
	}
	
	SDL_Rect src, dest;

	FILLRECT(src, 0, 0, t->w, t->h);
	FILLRECT(dest, x, y, t->w, t->h);

	TextureRender(t, &src, &dest);
}

void TextureFree(Texture* t)
{
	if(t)
		SDL_DestroyTexture(t->tex);
}

