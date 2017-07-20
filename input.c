#include "input.h"

Input* InputNew()
{
	Input* i = (Input*) calloc(1, sizeof(Input));
	if(!i){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	return i;
}

void InputFree(Input* i)
{
	if(!i){
		DEBUGMSG(ERRPARAM);
		return;
	}

	if(i->keys)
		free(i->keys);
	free(i);
}

void InputAddKey(Input* i, const char* id, int code)
{
	if(!i || !id){
		DEBUGMSG(ERRPARAM);
		return;
	}

	if(InputFindKeyWithID(i, id))
		return;
	if(InputFindKeyWithCode(i, code))
		return;

	i->num_keys++;
	
	Key* tmp = realloc(i->keys, sizeof(Key) * i->num_keys);
	if(!tmp){
		DEBUGMSG(ERRNOMEM);
		return;
	}
	i->keys = tmp;

	i->keys[i->num_keys - 1].value = KEY_OFF;
	i->keys[i->num_keys - 1].code = code;
	i->keys[i->num_keys - 1].dt = 0;
	i->keys[i->num_keys - 1].id = strdup(id);
}

void InputPoll(Input* i, SDL_Event* ev)
{
	if(!i || !ev){
		DEBUGMSG(ERRPARAM);
		return;
	}

	for(int x = 0; x < i->num_keys; x++){
		i->keys[x].dt++;
	}

	while(SDL_PollEvent(ev)){
		switch(ev->type){
			case SDL_QUIT:
				i->exit = 1;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				KeySet(InputFindKeyWithCode(i, ev->key.keysym.sym),
					   ev->type == SDL_KEYDOWN);
				if(InputFindKeyWithCode(i, SDLK_ESCAPE)->value == 1)
					i->exit = 1;
				break;
			default:
				break;
		}
	}
	for(int x = 0; x < i->num_keys; x++){
		printf("Key '%s' (%d)\n", i->keys[x].id, i->keys[x].dt);
	}

}

Key* InputFindKeyWithID(Input* i, const char* id)
{
	if(!i || !id){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	for(int x = 0; x < i->num_keys; x++){
		if(strcmp(id, i->keys[x].id) == 0){
			return &i->keys[x];
		}
	}

	return NULL;
}

Key* InputFindKeyWithCode (Input* i, int code)
{
	if(!i){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}

	for(int x = 0; x < i->num_keys; x++){
		if(i->keys[x].code == code){
			return &i->keys[x];
		}
	}

	return NULL;
}

void KeySet(Key* k, int value)
{
	if(!k || !(value == KEY_OFF || value == KEY_ON)){
		DEBUGMSG(ERRPARAM);
		return;
	}

	k->dt = 0;
	k->value = value;
}

int KeyGet(Key* k)
{
	if(!k){
		DEBUGMSG(ERRPARAM);
		return -1;
	}
	
	return k->value;
}
