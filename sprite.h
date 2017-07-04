#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>

#include "core.h"
#include "utils.h"
#include "map.h"

typedef struct _animation_
{
	int 	number_frames;
	int* 	frame_sequence;
	int 	current_frame;
	int 	dt_count;
}
Animation;

typedef struct _sprite_
{
	Tilemap*		sheet;
	SDL_Renderer* 	renderer;
	
	int 			current_anim;
	int 			num_anim;
	Animation*		anim[256];

	int 			scale;
	SDL_Rect		dest;
}
Sprite;

Sprite* SpriteNew 			(SDL_Renderer*, const char*, int, int, int, int, int);
void 	SpriteFree 			(Sprite*);
void 	SpriteRender 		(Sprite*);
void 	SpriteNextFrame 	(Sprite*);
void 	SpriteAddAnimation	(Sprite*, int*, int);

#endif
