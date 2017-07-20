#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include "utils.h"
#include "texture.h"

#define TYPEWRITERPADDINGH 24
#define TYPEWRITERPADDINGV 16

#define MSGSPEEDCONST 100
#define MSGSPEEDSTEP 10
#define MSGMAXLINES 64
#define MSGVPADDING 0
#define MSGTIMEBETWEENLINES 20

#define DIALOGSCROLLTIME 100

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

	Texture* 		backdrop;
}
Typewriter;

typedef struct _msg_
{
	int len;
	char* text;
	Texture* tex;
}
Message;

typedef struct _dialog_
{
	Message* msgs;
	int num_msgs;

	int curr_msg;
	int curr_point;
	int curr_w;

	int speed;
	int dt;

	int over;
	int time;
	int offset;
}
Dialog;

Typewriter* TypewriterNew 	(SDL_Renderer*, const char*, const char*, int);
void 		TypewriterWrite (Typewriter*, Dialog*, int, int);
void 		TypewriterFree 	(Typewriter*);

Dialog* 	DialogNew 	(Typewriter* tw, char* msg, int spd);
int 		DialogFill	(Dialog* dg, Typewriter* tw, const char* msg);
void 		DialogFree 	(Dialog* dg);

void 		MessageFree(Message*);
