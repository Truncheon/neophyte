#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "core.h"
#include "sprite.h"
#include "player.h"
#include "map.h"
#include "text.h"
#include "utils.h"
#include "input.h"

#define SCALE 1
#define SCRWIDTH 1920
#define SCRHEIGHT 1080
#define WAITTIME 33

#define MSG "All your base are belong to us, no use to run now because you're most certainly doomed!"

#define FONTPATH "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf"

int main()
{
	SDL_Window* 	window;
	SDL_Renderer* 	renderer;
	SDL_Event 		event;
	Input* 			input;
	Player* 		player;
	Map* 			map;
	Typewriter* 	typew;
	Dialog* 		dialog;
	unsigned long 	currentTime = 0, lastTime = 0;
	
	CoreSetup(SCRWIDTH, SCRHEIGHT);
	
	window 	 = CoreInit();
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	input = InputNew();

	InputAddKey(input, "A", 	SDLK_a);
	InputAddKey(input, "W", 	SDLK_w);
	InputAddKey(input, "S", 	SDLK_s);
	InputAddKey(input, "D", 	SDLK_d);
	InputAddKey(input, "ESC", 	SDLK_ESCAPE);

	player 	= PlayerNew(renderer, input, "resources/skeleripped.png", 100, 100, 31, 42, 2);
	map 	= MapLoad(renderer, "maps/test2.txt", "resources/tiles2.png");
	typew 	= TypewriterNew(renderer, FONTPATH, "resources/ui/dialog_test.png", 24);
	dialog 	= DialogNew(typew, MSG, 20);

	do{
		InputPoll(input, &event);
		PlayerUpdate(player);
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);
	
		MapRender(map);
		PlayerRender(player);
		TypewriterWrite(typew, dialog, 100, 800);

		SDL_RenderPresent(renderer);
		
		currentTime = SDL_GetTicks();
		//printf("Frame time: %ld\n", currentTime - lastTime);
		if(WAITTIME > currentTime - lastTime)
			SDL_Delay(WAITTIME - currentTime + lastTime);
		lastTime = currentTime;
	}
	while(!input->exit);

	TypewriterFree(typew);
	DialogFree(dialog);
	PlayerFree(player);
	MapFree(map);
	SDL_DestroyRenderer(renderer);
	CoreShutdown(window);

	return 0;
}

