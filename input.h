#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "utils.h"

#define KEY_OFF 0
#define KEY_ON 1
#define KEY_UNDEF -1

typedef struct _key_
{
	int value;
	int code;
	int dt;
	char* id;
}
Key;

typedef struct _input_
{
	Key* keys;
	int num_keys;
	int exit;
}
Input;

Input* 	InputNew 				();
void	InputFree 				(Input* i);
void 	InputAddKey 			(Input* i, const char* id, int code);

void 	InputPoll 				(Input* i, SDL_Event* ev);


Key* 	InputFindKeyWithID 		(Input* i, const char* id);
Key* 	InputFindKeyWithCode 	(Input* i, int code);
void 	KeySet 					(Key* k, int value);
int 	KeyGet 					(Key* k);

#endif
