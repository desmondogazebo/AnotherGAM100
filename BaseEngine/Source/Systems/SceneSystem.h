/******************************************************************************
filename    SceneSystem.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 12 November 2017

Brief Description:
A system that utillizes states to manage the activity of each scene.

******************************************************************************/

/* Header Guards */
#ifndef _SCENE_SYSTEM_H
#define _SCENE_SYSTEM_H

#include "../StateManager/BaseStateManager.h"

// Inclusion of Scenes
#include "../Scenes/BattleScene.h"
#include "../Scenes/WorldViewScene.h"
#include "../Scenes/DungeonScene.h"
#include "../Scenes/SplashScene.h"
#include "../Scenes/MenuScene.h"

// Inclusion of Other Objects
#include "../LogicalObjects/EnemyEncounterHandler.h"

/* Public Struct & Functions */
typedef struct SceneSystem
{
	/* Public Parameters */
	enum {
		SS_Splash = 0,
		SS_MainMenu,
		SS_WorldView,
		SS_PlayerMenu,
		SS_Dungeon,
		SS_Battle,
		SS_Story,
		SS_Total,
	}SS_Scenes;
	enum CSM_States InternalState;
	BaseStateManager InternalStateManager;

	/* Public Function Pointers*/
	/* Initiallize, requires an instance of itself */
	void(*Initiallize)();
	/* Updates, requires an instance of itself as well as delta time */
	void(*Update)();
	/* Renders, requires an instance of itself and a passed engine*, writes the data of the currently active scene to the screen's buffer */
	void(*Render)();
	/* Exits, requires an instance, calls free if memory was allocated */
	void(*Exit)();

	/* Set Current Scene */
	void(*SetCurrentScene)();

	// Scene Variables
	BattleScene InternalBattleScene;
	WorldViewScene InternalWorldViewScene;
	DungeonScene InternalDungeonScene;
	SplashScene InternalSplashScene;
	MenuScene InternalMenuScene;

	// Objects
	EnemyEncounterHandler InternalEncounterHandler;

}SceneSystem;

///****************************************************************************
// Public Function Prototypes
///****************************************************************************
/* Setup function that initiallizes the SceneSystem */
void SceneSystem_Setup(SceneSystem* Self);

#endif //_CUSTOM_STATE_MANAGER_H