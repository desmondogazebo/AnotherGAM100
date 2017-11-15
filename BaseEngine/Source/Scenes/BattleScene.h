/******************************************************************************
filename    BattleScene.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 10 November 2017

Brief Description:
The battle scene of the game. Utilizes the base state manager to allow logic to
be tied in together with the scene.

******************************************************************************/

/* Header Guards */
#ifndef _CUSTOM_STATE_MANAGER_H
#define _CUSTOM_STATE_MANAGER_H
	
#include "../StateManager/BaseStateManager.h"

/* Public Struct & Functions */
typedef struct BattleScene
{
	/* Public Parameters */
	enum {
		CSM_Loading = 0,
		CSM_PlayerTurn,
		CSM_EnemyTurn,
		CSM_BattleSequence,
		CSM_Results
	}CSM_States;
	enum CSM_States InternalState;
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

#endif //_CUSTOM_STATE_MANAGER_H