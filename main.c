#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "sprite.h"

int main(){

	SDL_Window* window = CoreInit();

	SDL_Surface* windowSurf = SDL_GetWindowSurface(window);
	SDL_Renderer* renderer = SDL_CreateRenderer();

	SDL_Event event;
	int running = 1;

	Sprite* spr = SpriteNew(renderer, "resources/thedonald", 100, 100, 1000, 500);
	
	while(running){
		SDL_PollEvent(&event);
		printf("running...\n");
		switch(event.type){
			case SDL_WINDOWEVENT:
				switch(event.window.event){
					case SDL_WINDOWEVENT_CLOSE:
						running = 0;
						break;
					default: break;
				}
			case SDL_KEYUP:
				if(event.key.keysym.sym == SDLK_a){
					running = 0;
				}
			default: break;
		}

		SpriteRender(spr, renderer);
		//SpriteNextFrame(spr);

		SDL_UpdateWindowSurface(window);
		SDL_Delay((int)1000/60.0);
	}
	
	SpriteFree(spr);
	SDL_FreeSurface(sprite);

	CoreShutdown(window);

	return 0;
}
