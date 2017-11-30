/******************************************************************************
filename    EnemyEncounterHandler.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 28 November 2017

Brief Description:
This file contains an encounter handler that has utility calls to detect whether
a monster is encountered.

******************************************************************************/
#include "EnemyEncounterHandler.h"
#include "../Utilities/Utilities.h"

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
void EnemyEncounterHandler_LinkedInitiallize(EnemyEncounterHandler* Self);
void EnemyEncounterHandler_LinkedExit(EnemyEncounterHandler* Self);
void InitiallizeEnemy(EnemyEncounterHandler* Self, enum Enemies EnemyToLoad, char* FilePath);
///****************************************************************************
// Function Definitions
///****************************************************************************
/* Setup function that initiallizes the DungeonCamera */
void EnemyEncounterHandler_Setup(EnemyEncounterHandler* Self)
{
	// Set up base parameters and handle function pointer linking
	Self->Initiallize = EnemyEncounterHandler_LinkedInitiallize;
	Self->Exit = EnemyEncounterHandler_LinkedExit;
}

/* Linked Internal Functions */
void EnemyEncounterHandler_LinkedInitiallize(EnemyEncounterHandler* Self)
{
	// Set the array to null
	memset(Self->EnemyList, 0, sizeof(Enemy) * Boss_Max);
	memset(Self->EnemyValueChecker, 0, sizeof(short) * Boss_Max);

	// Load all the valid enemy types
	InitiallizeEnemy(Self, Enemy_Bird, "Resources/Enemy/Bird.txt");
	InitiallizeEnemy(Self, Enemy_Ghost, "Resources/Enemy/Ghost.txt");
	InitiallizeEnemy(Self, Enemy_Goblin, "Resources/Enemy/Goblin.txt");
	InitiallizeEnemy(Self, Enemy_Rat, "Resources/Enemy/Rat.txt");
	InitiallizeEnemy(Self, Enemy_Skeleton, "Resources/Enemy/Skeleton.txt");
	InitiallizeEnemy(Self, Enemy_Frog, "Resources/Enemy/Frog.txt");

	InitiallizeEnemy(Self, Boss_DatBoiLv1, "Resources/Enemy/DatBoi1.txt");
	InitiallizeEnemy(Self, Boss_DatBoiLv2, "Resources/Enemy/DatBoi2.txt");
	InitiallizeEnemy(Self, Boss_DatBoiLv3, "Resources/Enemy/DatBoi3.txt");
	InitiallizeEnemy(Self, Boss_DatBoiLv4, "Resources/Enemy/DatBoi4.txt");
	InitiallizeEnemy(Self, Boss_DatBoiLv5, "Resources/Enemy/DatBoi5.txt");

	// Set up a default for testing
	Self->CurrentEnemy = &Self->EnemyList[Enemy_Ghost];
	Self->PreviousSceneWasDungeon = 0;
}

void EnemyEncounterHandler_LinkedExit(EnemyEncounterHandler* Self)
{
	Self->CurrentEnemy = NULL;
	// Free the memory used in the array for enemies
	for (int i = 0; i < Boss_Max; ++i)
		if (Self->EnemyValueChecker[i] > 0)
			FreeEnemy(&Self->EnemyList[i]);
}

// Randomizes an encounter
short EnemyEncounterHandler_RandomizeEncounter(EnemyEncounterHandler* Self, unsigned short EncounterPercent, enum Enemies MinimumThreshold, enum Enemies MaximumThreshold)
{
	// A variable determining if a monster spawned
	short DidMonsterSpawn = 0;
	// Percent check for an encounter
	if (EncounterPercent > IntRandomizeRange(0, 100)){
		// Randomize a monster
		Self->CurrentEnemyType = IntRandomizeRange(MinimumThreshold, MaximumThreshold);
		Self->CurrentEnemy = &Self->EnemyList[Self->CurrentEnemyType];
		DidMonsterSpawn = 1;
	}
	return DidMonsterSpawn;
}

// Sets up the encounter
void EnemyEncounterHandler_SetUpEncounter(EnemyEncounterHandler* Self, enum Enemies MonsterToEncounter)
{
	Self->CurrentEnemy = &Self->EnemyList[MonsterToEncounter];
	Self->CurrentEnemyType = MonsterToEncounter;
}

void InitiallizeEnemy(EnemyEncounterHandler* Self, enum Enemies EnemyToLoad, char* FilePath)
{
	// Malloc an enemy pointer
	Enemy* NewEnemy = malloc(sizeof(Enemy));
	PopulateEnemy(NewEnemy, FilePath);
	// Set the enemy into the array
	Self->EnemyList[EnemyToLoad] = *NewEnemy; 
	Self->EnemyValueChecker[EnemyToLoad] = 1;
	free(NewEnemy);
}