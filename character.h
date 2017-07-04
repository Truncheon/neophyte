#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static const int num_skills = 32;

typedef struct _char_
{
	char* name;
	char* description;

	//Leveling
	int LVL;
	int XP;

	//Monetary
	int GLD;

	//Fighting

	//Basic character traits
	int ATK, HP, INT;
	
	//Divinity - Defense against profane arts
	//Profanity - Defense against divine arts
	int DIV, PRO;
	
	//Skills
	char* skills[4];
	int maxlen;
}
Character;

Character*  CharacterNew 	(char*, char*);
void 		CharacterFree 	(Character*);
