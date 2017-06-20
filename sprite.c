#include "sprite.h"

Sprite* SpriteNew(SDL_Renderer* rend, const char* path, int x, int y, int w, int h)
{
	Sprite* spr;
	SDL_Texture* texture;
	SDL_Surface* tmp;
	int i, c;

	spr = (Sprite*) malloc(sizeof(Sprite));
	if(spr == NULL){
		printf("ERRNOMEM: at SpriteNew\n");
		return NULL;
	}

	tmp = IMG_Load(path);
	if(!tmp){
		return NULL;
	}
	SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 0xFF, 0, 0xFF));

	texture = SDL_CreateTextureFromSurface(rend, tmp);
	//SDL_FreeSurface(tmp);
	
	spr->position.x = x;
	spr->position.y = y;
	spr->position.w = w;
	spr->position.h = h;

	spr->sheet = texture;
	spr->anim = AnimationNew();
	SpriteAddFrame(spr, x, y);

	for(i = 0; i < tmp->w; i+=w)
		for(c = 0; c < tmp->h; c+=h){
			SpriteAddFrame(spr, i, c);
		}

	return spr;
}

void SpriteFree(Sprite* s)
{
	if(s == NULL) return;
	AnimationFree(s->anim);
	SDL_DestroyTexture(s->sheet);
	free(s);
}

void SpriteNextFrame(Sprite* s)
{
	if(s == NULL) return;
	AnimationNext(s->anim);
}

void SpriteRender(Sprite* s, SDL_Renderer* renderer)
{
	if(s == NULL || renderer == NULL) return;
	if(s->anim == NULL) return;

	SDL_RenderCopy(renderer, s->sheet, &s->anim->curr->tilepos, &s->position);
	//AnimationNext(s->anim);
}

void SpriteAddFrame(Sprite* s, int x, int y)
{
	if(s == NULL) return;
	AnimationAddFrame(s->anim, x, y, s->position.w, s->position.h);
}

void SpriteLoadSheet()
{

}

Animation* AnimationNew()
{
	Animation* anim = (Animation*) malloc(sizeof(Animation));
	if(anim == NULL){
		printf("ERROR: ERRNOMEM\nWHERE: AnimationNew\n");
		return NULL;
	}

	anim->size = 0;
	anim->head = anim->curr = NULL;

	return anim;
}

void AnimationFree(Animation* anim)
{
	if(anim == NULL) return;
	
	AnimationFrame* aux;
	while(anim->head != NULL){
		aux = anim->head->next;
		free(anim->head);
		anim->head = aux;
	}
	anim->size = 0;
	free(anim);
}

int AnimationAddFrame(Animation* anim, int x, int y, int w, int h)
{
	if(anim == NULL){
		anim = AnimationNew();
	}

	AnimationFrame* iter;
	AnimationFrame* frame = (AnimationFrame*) malloc(sizeof(AnimationFrame));
	if(frame == NULL){
		printf("ERROR: ERRNOMEM\nWHERE: AnimationAddFrame\n");
		return -1;
	}

	frame->tilepos.x = x;
	frame->tilepos.y = y;
	frame->tilepos.w = w;
	frame->tilepos.h = h;
	frame->next = NULL;
	
	if(anim->size == 0){
		anim->head = frame;
	}
	else{
		for(iter = anim->head; iter->next != NULL; iter = iter->next);
		iter->next = frame;
	}
	anim->size++;

	return 0;
}

void AnimationNext(Animation* anim)
{
	if(anim == NULL) return;
	anim->curr = anim->curr->next;
}

