/******************************************************************************
filename    DungeonScene.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 16 November 2017

Brief Description:
The dungeon scene of the game. Contains game state logic involving the battle
mechanics.

******************************************************************************/

/* Header Guards */
#ifndef _DUNGEON_SCENE_H
#define _DUNGEON_SCENE_H

#include "../StateManager/BaseStateManager.h"

/* Public Struct & Functions */
typedef struct DungeonScene
{
	/* Public Parameters */
	enum {
		DS_Loading = 0,
		DS_Exploration,
		DS_Results
	}DS_States;
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

}DungeonScene;

///****************************************************************************
// Public Function Prototypes
///****************************************************************************
/* Setup function that initiallizes the DungeonScene */
void DungeonScene_Setup(DungeonScene* Self);

#endif //_DUNGEON_SCENE_H