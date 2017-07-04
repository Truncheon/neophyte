#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "core.h"
#include "sprite.h"
#include "player.h"
#include "map.h"
#include "text.h"
#include "utils.h"

#define SCALE 1
#define SCRWIDTH 1920
#define SCRHEIGHT 1080
#define WAITTIME 33

int main()
{
	SDL_Window* 	window;
	SDL_Renderer* 	renderer;
	SDL_Event 		event;
	int 			exit;
	int* 			keys;
	Player* 		player;
	Map* 			map;
	Typewriter* 	typew;
	Message* 		msg;
	unsigned long 	currentTime = 0, lastTime = 0;
	
	keys 	= CoreSetup(SCRWIDTH, SCRHEIGHT);
	window 	= CoreInit();
	renderer= SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	exit 	= 0;
	
	player 	= PlayerNew(renderer, "resources/skeleripped.png", 100, 100, 31, 42, 2);
	map 	= MapLoad(renderer, "maps/test2.txt", "resources/tiles2.png");
	typew 	= TypewriterNew(renderer, "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
							"resources/ui/dialog_profane.png", 24);
	msg 	= MessageNew("All your base are belong to us", 3);

	do{
		exit = CoreInput(&event, keys);
		PlayerInput(player, keys);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);
	
		MapRender(map);
		PlayerRender(player);
		TypewriterWrite(typew, msg, 10, 10);

		SDL_RenderPresent(renderer);
		
		currentTime = SDL_GetTicks();

		//printf("Frame time: %ld\n", currentTime - lastTime);
		if(WAITTIME > currentTime - lastTime)
			SDL_Delay(WAITTIME - currentTime + lastTime);
		lastTime = currentTime;
	}
	while(!exit);

	TypewriterFree(typew);
	MessageFree(msg);
	PlayerFree(player);
	MapFree(map);
	SDL_DestroyRenderer(renderer);
	CoreShutdown(window, keys);
	return 0;
}

