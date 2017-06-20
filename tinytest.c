#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "deps/tmx/tmx.h"

#include "core.h"
#include "sprite.h"
#include "player.h"
#include "map.h"

#define SCALE 1
#define SCRWIDTH 640
#define SCRHEIGHT 480

int main()
{
	SDL_Window* 	window;
	SDL_Renderer* 	renderer;
	Player* 		player;
	SDL_Event 		event;
	int 			exit;
	int* 			keys;
	Map* 			map;
	
	
	keys 	= CoreSetup(SCRWIDTH, SCRHEIGHT);
	exit 	= 0;
	window 	= CoreInit();
	renderer= SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	player 	= PlayerNew(renderer, "resources/skeleripped.png", 32, 50, 48*SCALE, 96*SCALE);

	map = NULL;
	map = MapLoad(renderer, "maps/test.txt", "resources/tiles2.png");
	if(map == NULL) return -1;

	//MapPrint(map);

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
	CoreShutdown(window);
	return 0;
}

