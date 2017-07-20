#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "sprite.h"
#include "utils.h"
#include "input.h"

typedef struct _player_
{
	Sprite* 	sprite;
	Key* 		A;
	Key* 		W;
	Key* 		S;
	Key* 		D;
	int 		anim;
}
Player;

Player* PlayerNew 		(SDL_Renderer*, Input*, const char*, int, int, int, int, int);
void 	PlayerFree 		(Player*);
void 	PlayerRender 	(Player*);
void 	PlayerUpdate 	(Player*);

#endif
