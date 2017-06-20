#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>

#include "core.h"

typedef struct _frame_
{
	SDL_Rect 			tilepos;
	struct _frame_* 	next;
}
AnimationFrame;

typedef struct _animation_
{
	AnimationFrame* 	head;
	AnimationFrame* 	curr;
	int 				size;
	//int frametime;
}
Animation;

typedef struct _sprite_
{
	SDL_Texture* 		sheet;
	Animation* 			anim;
	SDL_Rect 			position;
}
Sprite;

Sprite* SpriteNew 			(SDL_Renderer*, const char*, int, int, int, int);
void 	SpriteFree 			(Sprite*);
void 	SpriteRender 		(Sprite*, SDL_Renderer*);
void 	SpriteNextFrame 	(Sprite*);
void 	SpriteAddFrame 		(Sprite*, int, int);
Sprite*	SpriteFromSheet 	(const char*, int w, int h);

Animation*  AnimationNew 		();
void 		AnimationFree 		(Animation*);
int 		AnimationAddFrame 	(Animation*, int x, int y, int w, int h);
void 		AnimationNext 		(Animation*);

#endif
