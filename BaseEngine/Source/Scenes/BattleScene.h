/******************************************************************************
filename    BattleScene.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Created on 10 November 2017

Brief Description:
The battle scene of the game. Utilizes the base state manager to allow logic to
be tied in together with the scene.

******************************************************************************/

/* Header Guards */
#ifndef _BATTLE_SCENE_H
#define _BATTLE_SCENE_H
	
#include "../StateManager/BaseStateManager.h"

/* Public Struct & Functions */
typedef struct BattleScene
{
	/* Public Parameters */
	enum {
		BS_Loading = 0,
		BS_PlayerTurnChoice,
		BS_PlayerTurnAttack,
		BS_PlayerTurnRun,
		BS_EnemyTurn,
		BS_Results
	}BS_States;
	enum BS_States InternalState;
	BaseStateManager InternalStateManager;

	/* Public Function Pointers*/
	/* Initiallize, requires an instance of itself */
	void(*Initiallize)();
	/* Updates, requires an instance of itself as well as delta time */
	void(*Update)();
	/* Renders, requires an instance of itself and a passed engine*, writes the data of the scene to the screen's buffer */
	void(*Render)();
	/* Exits, requires an instance, calls free if memory was allocated */
	void(*Exit)();

}BattleScene;

///****************************************************************************
// Public Function Prototypes
///****************************************************************************
/* Setup function that initiallizes the BattleScene */
void BattleScene_Setup(BattleScene* Self);

#endif //_BATTLE_SCENE_H