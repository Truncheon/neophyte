#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

#include "core.h"

#define RENDERERFLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

#define GRIDW 24
#define GRIDH 13
#define GRIDSTEPX (WIN_WIDTH/GRIDW)
#define GRIDSTEPY (WIN_HEIGHT/GRIDH)

#define SNAKESTARTX 4
#define SNAKESTARTY 4

static const float FPS = 60;

typedef struct rectlist {
	SDL_Rect* rect;
	struct rectlist* next;
} RectList;

typedef struct snake {
	RectList* body;
	int size;
	int dirx;
	int diry;
	int frames;
	SDL_Texture* tex;
} Snake;

Snake* SnakeNew();
void SnakeAddSeg(Snake*, int, int);
void SnakeDelete(Snake*);
void SnakeInput(Snake*, SDL_Keycode);
void SnakeMove(Snake*);
void Render(SDL_Renderer*, Snake*);

int main()
{
	int running = 1;
	SDL_Window* wd = CoreInit();
	SDL_Renderer* rend = SDL_CreateRenderer(wd, -1, RENDERERFLAGS);
	SDL_Surface* surf = CoreLoadSurface(NULL, "resources/idle.png");
	Snake* snek = SnakeNew();
	snek->tex = SDL_CreateTextureFromSurface(rend, surf);

	printf("all set!\n");

	Uint32 startTime = 0;
	Uint32 endTime;

	while(1) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				printf("hey\n");
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE: running = 0; break;
						default: SnakeInput(snek, event.key.keysym.sym);
								 break;
					}
				default: break;		
			}
		}
		if(!running) break;

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		Render(rend, snek);
		SDL_RenderPresent(rend);
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		endTime = SDL_GetTicks();
		
		if(endTime - startTime > 10000/FPS) {
			//SnakeMove(snek);
			printf("one s\n");
			startTime += 1000/FPS;
			SDL_Delay(1000/FPS);
		}
	}
	//TODO: Double free in SnakeDelete
	//SnakeDelete(snek);
	SDL_DestroyRenderer(rend);
	CoreShutdown(wd);
	return 0;
}

void Render(SDL_Renderer* rend, Snake* snek)
{
	int i;
	RectList* curr;
	SDL_Rect xd;

	xd.x = 0;
	xd.y = 0;
	xd.w = 80;
	xd.h = 80;
	
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	
	for(i = 0; i < GRIDW; i++) {
		SDL_RenderDrawLine(rend, i * GRIDSTEPX, 0, i * GRIDSTEPX, WIN_HEIGHT);
	}
	for(i = 0; i < GRIDH; i++) {
		SDL_RenderDrawLine(rend, 0, i * GRIDSTEPY, WIN_WIDTH, i * GRIDSTEPY);
	}

	curr = snek->body;
	SDL_SetRenderDrawColor(rend, 255, 255, 0, 255);
	if(curr == NULL) exit(1);
	for(int j = 0; j < snek->size; j++) {
		SDL_RenderCopy(rend, snek->tex, &xd, curr->rect);
		curr = curr->next;
	}
	snek->
}

Snake* SnakeNew()
{
	Snake* snek = (Snake*) malloc(sizeof(Snake));
	snek->dirx = 1;
	snek->diry = 0;
	snek->size = 0;
	SnakeAddSeg(snek, SNAKESTARTX, SNAKESTARTY);

	return snek;
}

void SnakeDelete(Snake* snek)
{
	RectList* curr,* tmp;
	
	curr = snek->body;
	while(curr != NULL) {
		tmp = curr->next;
		free(curr->rect);
		free(curr);
		curr = tmp;
	}
	free(snek);
}

void SnakeAddSeg(Snake* snek, int x, int y)
{
	RectList* curr;
	SDL_Rect* rect;

	if(x < 0 || x >= GRIDW || y < 0 || y >= GRIDH) {
		printf("wrong coords!\n");
		return;
	}

	curr = (RectList*) malloc(sizeof(RectList));
	if(curr == NULL) {
		printf("ERRNOMEM\n");
		return;
	}
	rect = (SDL_Rect*) malloc(sizeof(SDL_Rect));
	if(rect == NULL) {
		printf("ERRNOMEM\n");
		free(curr);
		return;
	}
	rect->x = x * GRIDSTEPX;
	rect->y = y * GRIDSTEPY;
	rect->w = GRIDSTEPX;
	rect->h = GRIDSTEPY;
	
	curr->rect = rect;
	curr->next = snek->body;

	snek->body = curr;
	
	snek->size++;
}

void SnakeInput(Snake* snek, SDL_Keycode keycode){
	switch(keycode) {
		case SDLK_UP:
			if(snek->diry == 0) {
				snek->dirx = 0;
				snek->diry = -1;
			}
			break;
		case SDLK_DOWN:
			if(snek->diry == 0) {
				snek->dirx = 0;
				snek->diry = 1;
			}
			break;
		case SDLK_LEFT:
			if(snek->dirx == 0) {
				snek->dirx = -1;
				snek->diry = 0;
			}
			break;
		case SDLK_RIGHT:
			if(snek->dirx == 0) {
				snek->dirx = 1;
				snek->diry = 0;
			}
			break;
	}
}

void SnakeMove(Snake* snek){
	RectList* rect;
	int x, y;

	if(snek->dirx != 0 && snek->diry != 0) {
		printf("direction error in snake\n");
	}

	x = snek->body->rect->x;
	y = snek->body->rect->y;

	if(snek->dirx != 0) {
		x += snek->dirx * GRIDSTEPX;
	} else {
		y += snek->diry * GRIDSTEPY;
	}

	if(x < 0) {
		x = WIN_WIDTH-GRIDSTEPX;
	} else if(x > WIN_WIDTH) {
		x = 0;
	}
		
	if(y < 0) {
		y = WIN_HEIGHT-GRIDSTEPY;
	} else if(y > WIN_HEIGHT) {
		y = 0;
	}


	//printf("from %d, %d to %d, %d, with dir(%d, %d)\n", snek->body->rect->x, snek->body->rect->y, x, y, snek->dirx, snek->diry);
	
	rect = snek->body;
		
	for(int i = 0; i < snek->size-1; i++) {
		rect = rect->next;
	}

	rect->rect->x = x;
	rect->rect->y = y;
}
