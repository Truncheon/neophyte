#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "core.h"
#include "texture.h"
#include "utils.h"

typedef struct _tilemap_
{
	int 			w, h, num;
	int 			tile_w, tile_h;
	Texture* 		tileset;
	SDL_Rect* 		mapping;
}
Tilemap;

typedef enum _groundType_ {
	NORMAL,
	BLOCKED
}
GroundType;

typedef struct _tile_
{
	SDL_Rect 		dest;
	int 			code;
	GroundType 		coll;
}
Tile;

typedef struct _map_
{
	int 			w, h;
	Tile* 			tiles;
	Tilemap* 		tilemap;
}
Map;

Tilemap* 	TilemapNew  (SDL_Renderer*, int, int, const char*);
int 	 	TilemapLoad (SDL_Renderer*, Map*, const char*, const char*);
void 		TilemapFree (Tilemap*);

/* Creates a map from scratch */
Map* 		MapLoad 		(SDL_Renderer*, const char*, const char*);
/* */
int 		MapFromFile		(Map*, SDL_Renderer*, const char*, const char*);

/* Fill map with the codes */
int 		MapFill 		(Map*, char*, int);

void 		MapFree 		(Map*);
void 		MapPrint 		(Map*);
void 		MapRender 		(Map*);

#endif
