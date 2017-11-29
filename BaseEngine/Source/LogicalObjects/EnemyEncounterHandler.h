/******************************************************************************
filename    EnemyEncounterHandler.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 28 November 2017

Brief Description:
This file contains an encounter handler that has utility calls to detect whether 
a monster is encountered.

******************************************************************************/

/* Header Guards */
#ifndef _ENEMY_ENCOUNTER_HANDLER_H
#define _ENEMY_ENCOUNTER_HANDLER_H

#include "../Enemy/EnemyDataLoader.h"

typedef struct EnemyEncounterHandler
{
	/* Public Parameters */
	enum {
		// Enemies
		Enemy_Bird = 0,
		Enemy_Frog,
		Enemy_Rat,
		Enemy_Goblin,
		Enemy_Ghost,
		Enemy_Skeleton,
		Enemy_Max,
		// Bosses
		Boss_DatBoiLv1,
		Boss_DatBoiLv2,
		Boss_DatBoiLv3,
		Boss_DatBoiLv4,
		Boss_DatBoiLv5,
		Boss_Max,
	}Enemies;

	// The result of what enemy was randomized
	Enemy* CurrentEnemy;
	// An array holding all the enemy objects
	Enemy EnemyList[Boss_Max];
	short EnemyValueChecker[Boss_Max];
	short PreviousSceneWasDungeon;

	/* Public Function Pointers*/
	// This function loads all the enemies into the array
	void(*Initiallize)();
	// This function clears the initiallized data
	void(*Exit)(); 

}EnemyEncounterHandler;

void EnemyEncounterHandler_Setup(EnemyEncounterHandler* Self);
// Randomizes an encounter
short EnemyEncounterHandler_RandomizeEncounter(EnemyEncounterHandler* Self, unsigned short EncounterPercent, enum Enemies MinimumThreshold, enum Enemies MaximumThreshold);
// Sets up the encounter
void EnemyEncounterHandler_SetUpEncounter(EnemyEncounterHandler* Self, enum Enemies MonsterToEncounter);

#endif //_ENEMY_ENCOUNTER_HANDLER_H