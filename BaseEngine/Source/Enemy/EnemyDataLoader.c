/******************************************************************************
filename	EnemyDataLoader.c
author      Keith Cheng
DP email	keith.cheng@digipen.edu

Brief Description:
A set of functions used to help populate the variables of an enemy

******************************************************************************/

#include "EnemyDataLoader.h"
#include "../Utilities/TextDataLoader.h"

#include <stdlib.h>
#include <string.h>

void PopulateEnemy(Enemy* target, char* filePath)
{
	TextDataLoader loader;
	TextDataLoader_Setup(&loader);

	loader.LoadResource(&loader, filePath);

	for (int i = 0; i < 6; ++i)
	{
		char *token = strrchr(loader.TextData[i], ':') + 1;
		switch (i)
		{
		case 0:
			//target->name = (char*)malloc(strlen(token) * sizeof(char));
			//strcpy(target->name, token);
			for (int j = 0; j < (int)strlen(token); ++j)
			{
				target->name[j] = token[j];
			}
			target->name[strlen(token) - 1] = '\0';
			break;
		case 1:
			sscanf_s(token,"%d",&target->hp);
			break;
		case 2:
			sscanf_s(token, "%d", &target->atk);
			break;
		case 3:
			sscanf_s(token, "%d", &target->lvl);
			break;
		case 4:
			sscanf_s(token, "%d", &target->spd);
			break;
		case 5:
			{
				token[strlen(token) - 1] = 0;
				TextDataLoader tempLoader;
				TextDataLoader_Setup(&tempLoader);
				tempLoader.LoadResource(&tempLoader, token);
				target->sprite = tempLoader.TextData;
				target->spriteRows = tempLoader.NumberOfRows;
				target->spriteColumns = tempLoader.NumberOfColumns;
			}
			break;
		}
	}

	loader.Exit(&loader);
}

void FreeEnemy(Enemy* target)
{
	//free(target->name);
	for (int i = 0; i < target->spriteRows; ++i)
		free(target->sprite[i]);
	free(target->sprite);
}