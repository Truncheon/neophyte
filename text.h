#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include "utils.h"
#include "map.h"

#define MSGSPEEDCONST 100

typedef struct _glyphMetrics_
{
	int minx, maxx;
	int miny, maxy;
	int advance;
}
GlyphMetrics;

typedef struct _typewriter_
{
	TTF_Font* 		font;
	GlyphMetrics* 	metrics;
	int 			ptsize;
	SDL_Color 		color;
	SDL_Renderer* 	renderer;

	Tilemap* 		dialogtex;
}
Typewriter;

typedef struct _msg_
{
	char* msg;
	int len;
	
	int num_lines;
	SDL_Texture* lines[64];
	int tex_w, tex_h;

	int curr_num;
	int curr_pos;
	
	int speed;
	int dt;
}
Message;

typedef struct _dialog_
{
	int num;
	SDL_Texture* lines[64];
}
Dialog;

Typewriter* TypewriterNew 	(SDL_Renderer*, const char*, const char*, int);
void 		TypewriterWrite (Typewriter*, Message*, int, int);
void 		TypewriterFree 	(Typewriter*);

Message* 	MessageNew 	(char* msg, int spd);
void 		MessageFree (Message* m);
