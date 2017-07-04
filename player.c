#include "player.h"

Player* PlayerNew(SDL_Renderer* rend, const char* path, int x, int y, int w, int h, int scale)
{
	Player* p = (Player*) malloc(sizeof(Player));
	if(!p) return NULL;

	p->sprite = SpriteNew(rend, path, x, y, w, h, scale);
	p->anim = 0;

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

void PlayerInput(Player* p, int* keys)
{
	#define MOVE 3
	
	//Walk left 1
	if(keys[0]){
		p->sprite->dest.x -= MOVE;
		if(p->sprite->current_anim != 1){
			p->sprite->current_anim = 1;
		}
	}
	//Walk up 3
	if(keys[1]){
		p->sprite->dest.y -= MOVE;
		if(p->sprite->current_anim != 3){
			p->sprite->current_anim = 3;
		}
	}
	//Walk down 0
	if(keys[2]){
		p->sprite->dest.y += MOVE;
		if(p->sprite->current_anim != 0){
			p->sprite->current_anim = 0;
		}
	}
	//Walk right 2
	if(keys[3]){
		p->sprite->dest.x += MOVE;
		if(p->sprite->current_anim != 2){
			p->sprite->current_anim = 2;
		}
	}
}
