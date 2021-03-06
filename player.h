#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"
#include <SDL2/SDL.h>

typedef struct _player_
{
	Sprite* sprite;
}
Player;

Player* PlayerNew 	(SDL_Renderer*, const char*, int, int, int, int);
void 	PlayerFree 	(Player*);
void 	PlayerRender(Player*, SDL_Renderer*);
void 	PlayerInput (Player*, int*);

#endif
