#include "sprite.h"

Sprite* SpriteNew(SDL_Renderer* rend, const char* path, int x, int y, int w, int h)
{
	Sprite* spr;
	SDL_Texture* texture;
	SDL_Surface* tmp;
	int i, c;

	spr = (Sprite*) malloc(sizeof(Sprite));
	if(spr == NULL){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	tmp = IMG_Load(path);
	if(!tmp){
		return NULL;
	}
	SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 0xFF, 0, 0xFF));
	texture = SDL_CreateTextureFromSurface(rend, tmp);
	
	FILLRECT(spr->position, x, y, w, h);

	spr->sheet = texture;
	spr->anim = AnimationNew();
	SpriteAddFrame(spr, x, y);

	for(i = 0; i < tmp->w; i+=w)
		for(c = 0; c < tmp->h; c+=h){
			SpriteAddFrame(spr, i, c);
		}

	SDL_FreeSurface(tmp);
	return spr;
}

void SpriteFree(Sprite* s)
{
	if(s == NULL) return;
	AnimationFree(s->anim);
	if(s->sheet) SDL_DestroyTexture(s->sheet);
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
		DEBUGMSG(ERRNOMEM);
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
		DEBUGMSG(ERRNOMEM);
		return -1;
	}

	FILLRECT(frame->tilepos, x, y, w, h);
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

