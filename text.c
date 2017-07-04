#include "text.h"

#define NUMGLYPHS 255

Typewriter* TypewriterNew(SDL_Renderer* rend, const char* ft_path, const char* dialog_path, int ptsize)
{
	if(!ft_path || ptsize < 1){
		DEBUGMSG(ERRPARAM);
	}

	Typewriter* tw = (Typewriter*) malloc(sizeof(Typewriter));
	if(!tw){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	tw->color.r = 0;
	tw->color.g = 0;
	tw->color.b = 0;
	tw->color.a = 0;

	tw->renderer = rend;

	tw->ptsize = ptsize;
	tw->font = TTF_OpenFont(ft_path, ptsize);
	if(!tw->font){
		DEBUGMSG(TTF_GetError());
		free(tw);
		return NULL;
	}

	tw->dialogtex = TilemapNew(rend, 0, 0, dialog_path);

	tw->metrics = (GlyphMetrics*) calloc(255, sizeof(GlyphMetrics));
	if(!tw->metrics){
		DEBUGMSG(ERRNOMEM);
		TTF_CloseFont(tw->font);
		free(tw);
		return NULL;
	}

	for(int i = 0; i < NUMGLYPHS; i++){
		TTF_GlyphMetrics(tw->font, i,
				&tw->metrics[i].minx,
				&tw->metrics[i].maxx,
				&tw->metrics[i].miny,
				&tw->metrics[i].maxy,
				&tw->metrics[i].advance);
	}

	return tw;
}

void TypewriterFree(Typewriter* tw)
{
	if(!tw){
		DEBUGMSG(ERRPARAM);
		return;
	}

	TTF_CloseFont(tw->font);
	free(tw);
}

void TypewriterWrite(Typewriter* tw, Message* m, int x, int y)
{
	if(!tw || !m){
		DEBUGMSG(ERRPARAM);
		return;
	}

	if(m->numlines){
		SDL_Surface* surf = TTF_RenderText_Solid(tw->font, m->msg, tw->color);
		if(!surf){
			DEBUGMSG(TTF_GetError());
			return;
		}

		SDL_Texture* tex = SDL_CreateTextureFromSurface(tw->renderer, surf);
		if(!tex){
			DEBUGMSG(SDL_GetError());
			SDL_FreeSurface(surf);
			return;
		}

		m->tex_w = surf->w;
		m->tex_h = surf->h;

		SDL_FreeSurface(surf);
		m->tex = tex;
	}
	
	if(m->curr_num != m->len){
		if(m->dt * m->speed >= MSGSPEEDCONST){
			m->dt -= MSGSPEEDCONST/m->speed;
			while(1){
				int curr_glyph = (int) m->msg[++m->curr_num];
				m->curr_pos += tw->metrics[curr_glyph].advance;
				if(curr_glyph != (int) ' ')
					break;
			}
		}
		else
			m->dt += 10;
	}

	SDL_Rect dest = {x, y, m->curr_pos, m->tex_h};
	SDL_Rect src  = {0, 0, m->curr_pos, m->tex_h};

	SDL_RenderCopy(tw->renderer, m->tex, &src, &dest);
}

Message* MessageNew(char* msg, int spd)
{
	if(!msg){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	Message* m = (Message*) malloc(sizeof(Message));
	if(!m){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	m->tex = NULL;
	m->msg = calloc(strlen(msg)+1, sizeof(char));
	strcpy(m->msg, msg);
	m->curr_num = 0;
	m->curr_pos = 0;

	m->len = strlen(m->msg);
	m->speed = spd;
	m->dt = 0;

	return m;
}

void MessageFree(Message* m)
{
	if(!m){
		DEBUGMSG(ERRPARAM);
		return;
	}

	SDL_DestroyTexture(m->tex);
	free(m->msg);
	free(m);
}
