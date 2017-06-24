#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "core.h"
#include "sprite.h"
#include "player.h"
#include "map.h"
#include "utils.h"

#define SCALE 1
#define SCRWIDTH 640
#define SCRHEIGHT 480

int main()
{
	SDL_Window* 	window;
	SDL_Renderer* 	renderer;
	SDL_Event 		event;
	int 			exit;
	int* 			keys;
	Player* 		player;
	Map* 			map;
	unsigned long 	currentTime = 0, lastTime = 0;
	
	keys 	= CoreSetup(SCRWIDTH, SCRHEIGHT);
	window 	= CoreInit();
	renderer= SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	exit 	= 0;
	
	player 	= PlayerNew(renderer, "resources/skeleripped.png", 100, 100, 31, 42);
	map 	= MapLoad(renderer, "maps/test2.txt", "resources/tiles2.png");

	do{
		exit = CoreInput(&event, keys);
		PlayerInput(player, keys);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);
	
		MapRender(map);
		PlayerRender(player);

		SDL_RenderPresent(renderer);
		
		currentTime = SDL_GetTicks();

		printf("Frame time: %ld\n", currentTime - lastTime);
		if(33 > currentTime - lastTime)
			SDL_Delay(33 - currentTime + lastTime);
		lastTime = currentTime;
	}
	while(!exit);

	PlayerFree(player);
	MapFree(map);
	SDL_DestroyRenderer(renderer);
	CoreShutdown(window, keys);
	return 0;
}

