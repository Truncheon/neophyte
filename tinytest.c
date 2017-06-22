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
	
	
	keys 	= CoreSetup(SCRWIDTH, SCRHEIGHT);
	window 	= CoreInit();
	renderer= SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	exit 	= 0;
	
	player 	= PlayerNew(renderer, "resources/skeleripped.png", 32, 50, 48*SCALE, 96*SCALE);
	map = MapLoad(renderer, "maps/test2.txt", "resources/tiles2.png");

	do{
		exit = CoreInput(&event, keys);
		PlayerInput(player, keys);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);
	
		MapRender(map);
		PlayerRender(player, renderer);

		SDL_RenderPresent(renderer);
	}
	while(!exit);

	PlayerFree(player);
	MapFree(map);
	SDL_DestroyRenderer(renderer);
	CoreShutdown(window, keys);
	return 0;
}

