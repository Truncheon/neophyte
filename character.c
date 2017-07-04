#include "character.h"

Character* CharacterNew(char* name, char* desc)
{
	if(!name || !desc){
		DEBUGMSG(ERRPARAM);
		return NULL;
	}
		
	Character* ch = (Character*) malloc(sizeof(Character));
	if(!ch){
		DEBUGMSG(ERRNOMEM);
		return NULL;
	}

	ch->name = name;
	ch->description = desc;
	
	ch->LVL = 1;
	ch->XP = 0;

	ch->GLD = 500;

	ch->ATK = 1;
	ch->HP = 30;
	ch->INT = 2;

	ch->DIV = 5;
	ch->PRO = 5;

	ch->maxlen = 32;

	ch->skills[0] = (char*) calloc(ch->maxlen, sizeof(char));
	strncpy(ch->skills[0], "Scratch", ch->maxlen);
	
	ch->skills[1] = (char*) calloc(ch->maxlen, sizeof(char));
	strncpy(ch->skills[1], "Spirit Bomb", ch->maxlen);
	
	ch->skills[2] = (char*) calloc(ch->maxlen, sizeof(char));
	strncpy(ch->skills[2], "Kamehameha", ch->maxlen);
	
	ch->skills[3] = (char*) calloc(ch->maxlen, sizeof(char));
	strncpy(ch->skills[3], "Judo Move", ch->maxlen);

	return ch;
}

void CharacterFree(Character* ch)
{
	if(!ch){
		DEBUGMSG(ERRPARAM);
		return;
	}

	for(int i = 0; i < num_skills; i++)
		free(ch->skills[i]);

	free(ch->name);
	free(ch->description);
	free(ch);
}
