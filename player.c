#include "player.h"

Player* PlayerNew(SDL_Renderer* rend, const char* path, int x, int y, int w, int h)
{
	Player* p = (Player*) malloc(sizeof(Player));
	if(!p) return NULL;

	p->sprite = SpriteNew(rend, path, x, y, w, h);
	return p;
}

void PlayerFree(Player* p)
{
	SpriteFree(p->sprite);
	free(p);
}

void PlayerRender(Player* p, SDL_Renderer* renderer)
{
	SpriteRender(p->sprite, renderer);
}

void PlayerInput(Player* p, int* keys)
{
	#define MOVE 1
	
	if(keys[0]){
		p->sprite->position.x -= MOVE;
		//printf("a\n");
	}
	if(keys[1]){
		p->sprite->position.y -= MOVE;
		//printf("w\n");
	}
	if(keys[2]){
		p->sprite->position.y += MOVE;
		//printf("s\n");
	}
	if(keys[3]){
		p->sprite->position.x += MOVE;
		//printf("d\n");
	}
}
