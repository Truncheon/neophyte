#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "core.h"
#include "utils.h"

typedef struct _tilemap_
{
	int 			w, h, num;
	int 			tile_w, tile_h;
	SDL_Texture* 	tileset;
	SDL_Rect* 		mapping;
}
Tilemap;

typedef struct _tile_
{
	SDL_Rect 		dest;
	int 			code;
}
Tile;

typedef struct _map_
{
	int 			w, h;
	Tile* 			tiles;
	Tilemap* 		tilemap;
	SDL_Renderer* 	renderer;
}
Map;

Tilemap* 	TilemapNew  (SDL_Renderer*, int, int, const char*);
int 	 	TilemapLoad (SDL_Renderer*, Map*, const char*, const char*);
void 		TilemapFree (Tilemap*);

Map* 		MapLoad 	(SDL_Renderer*, const char*, const char*);
void 		MapFree 	(Map*);
void 		MapPrint 	(Map*);
void 		MapRender 	(Map*);
