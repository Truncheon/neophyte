#include "sprite.h"

Sprite* SpriteNew(SDL_Renderer* rend, const char* path, int x, int y, int t_w, int t_h, int scale)
{
	Sprite* s;
	SDL_Surface* surface;
	int i, j;

	if(!rend || !path || (scale < 1)){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	s = (Sprite*) malloc(sizeof(Sprite));
	if(!s){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	s->sheet = TilemapNew(rend, t_w, t_h, path);
	s->renderer = rend;	
	surface = IMG_Load(path);
	if(!surface){
		DEBUGMSG(IMG_GetError());
		SpriteFree(s);
		return NULL;
	}
		
	FILLRECT(s->dest, x, y, t_w * scale, t_h * scale);

	s->num_anim = 0;
	s->current_anim = 0;
	s->scale = scale;

	//printf("%d %d\n", s->sheet->w, s->sheet->h);
	for(j = 0; j < s->sheet->h; j++)
		for(i = 0; i < s->sheet->w; i++){
			FILLRECT(s->sheet->mapping[i + j * s->sheet->w], i * t_w, j * t_h, t_w, t_h);
			//printf("%p -> %d %d %d %d\n", &s->sheet->mapping[i + j * s->sheet->w], i * t_w, j * t_h, t_w, t_h);
		}

	return s;
}

void SpriteFree(Sprite* s)
{
	if(!s) return;
	if(s->sheet) TilemapFree(s->sheet);
	free(s);
}

void SpriteNextFrame(Sprite* s)
{
	if(!s) return;
}

void SpriteRender(Sprite* s)
{
	Animation* an;
	if(!s) return;

	an = s->anim[s->current_anim];

	SDL_RenderCopy(s->renderer, s->sheet->tileset,
		&s->sheet->mapping[an->frame_sequence[an->current_frame]],
		&s->dest);

	an->dt_count++;
	if(an->dt_count > 10){
		an->dt_count = 0;
		an->current_frame++;
		if(an->current_frame == an->number_frames)
			an->current_frame = 0;
	}
}

void SpriteAddAnimation(Sprite* s, int* sequence, int n)
{
	if(!s) return;

	Animation* an = (Animation*) malloc(sizeof(Animation));
	if(!an) return;

	an->number_frames = n;
	an->frame_sequence = sequence;
	an->current_frame = 0;
	an->dt_count = 10; /* TODO: Make it a parameter or something? */

	/* TODO: Remove hard limit on animations */
	s->anim[s->num_anim++] = an;
}
