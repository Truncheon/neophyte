#include "player.h"

Player* PlayerNew(SDL_Renderer* rend, Input* input, const char* path, int x, int y, int w, int h, int scale)
{
	if(!rend || !input || !path){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	Player* p = (Player*) malloc(sizeof(Player));
	if(!p) return NULL;

	p->sprite = SpriteNew(rend, path, x, y, w, h, scale);
	p->anim = 0;

	p->A = InputFindKeyWithID(input, "A");
	p->W = InputFindKeyWithID(input, "W");
	p->S = InputFindKeyWithID(input, "S");
	p->D = InputFindKeyWithID(input, "D");

	/* TODO: Remove this testing code and refactor it for the love of god */
	int* a,* b,* c,* d;
	a = (int*) malloc(sizeof(int) * 4);
	b = (int*) malloc(sizeof(int) * 4);
	c = (int*) malloc(sizeof(int) * 4);
	d = (int*) malloc(sizeof(int) * 4);

	a[0] = 0;
	a[1] = 1;
	a[2] = 2;
	a[3] = 1;
	
	b[0] = 0 + 3;
	b[1] = 1 + 3;
	b[2] = 2 + 3;
	b[3] = 1 + 3;
	
	c[0] = 0 + 6;
	c[1] = 1 + 6;
	c[2] = 2 + 6;
	c[3] = 1 + 6;
	
	d[0] = 0 + 9;
	d[1] = 1 + 9;
	d[2] = 2 + 9;
	d[3] = 1 + 9;

	SpriteAddAnimation(p->sprite, a, 4);
	SpriteAddAnimation(p->sprite, b, 4);
	SpriteAddAnimation(p->sprite, c, 4);
	SpriteAddAnimation(p->sprite, d, 4);

	return p;
}

void PlayerFree(Player* p)
{
	SpriteFree(p->sprite);
	free(p);
}

void PlayerRender(Player* p)
{
	SpriteRender(p->sprite);
}

void PlayerUpdate(Player* p)
{
	#define MOVE 3
	#define UPDATE(x, step, anim, val) 	\
			do{ 						\
				x += step; 				\
				if(anim != val) 		\
					anim = val;			\
				return; 				\
			}while(0);

	Key* keys[4] = {p->A, p->W, p->S, p->D};

	while(1){
		int swap = 0;
		for(int i = 0; i < 3; i++){
			if(keys[i]->dt > keys[i+1]->dt){
				swap = 1;
				Key* tmp = keys[i+1];
				keys[i+1] = keys[i];
				keys[i] = tmp;
			}
		}
		if(!swap)
			break;
	}
	for(int i = 0; i < 4; i++){
		if(keys[i]->value == KEY_OFF)
			continue;			
		if(strcmp(keys[i]->id, "A") == 0)
			UPDATE(p->sprite->dest.x, -MOVE, p->sprite->current_anim, 1);
		if(strcmp(keys[i]->id, "W") == 0)
			UPDATE(p->sprite->dest.y, -MOVE, p->sprite->current_anim, 3);
		if(strcmp(keys[i]->id, "S") == 0)
			UPDATE(p->sprite->dest.y, MOVE, p->sprite->current_anim, 0);
		if(strcmp(keys[i]->id, "D") == 0)
			UPDATE(p->sprite->dest.x, MOVE, p->sprite->current_anim, 2);
	}
}
