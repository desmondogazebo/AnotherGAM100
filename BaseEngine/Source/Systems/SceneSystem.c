/*****************************************************************************
filename    SceneSystem.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 10 November 2017

Brief Description:
A template on creating a customized state manager

******************************************************************************/
#include "SceneSystem.h"

#include <stdio.h>

// Included for Rendering
#include "../Engine/BaseEngine.h"

///****************************************************************************
// Private Variables
///****************************************************************************
// Scene Info
BaseStateManager* SceneDataStorage[SS_Total];

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
// Scene System Functions
// Initiallize and store the required scenes
void InitiallizeScenes(SceneSystem* Self);

// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void SceneSystem_LinkedInitiallize(SceneSystem* Self);
// Linked Update function that will be set to the struct's Update
void SceneSystem_LinkedUpdate(SceneSystem* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the struct's Render
void SceneSystem_LinkedRender(SceneSystem* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the struct's Exit
void SceneSystem_LinkedExit(SceneSystem* Self);
// Linked SetCurrentScene function that will be set to the struct's SetCurrentScene
void SceneSystem_LinkedSetCurrentScene(SceneSystem* Self, enum SS_Scenes DesiredScene);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void SceneSystem_LinkedInternalInitiallize(SceneSystem* Self);
// Linked Update function that will be set to the InternalStateManager.Update
void SceneSystem_LinkedInternalUpdate(SceneSystem* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void SceneSystem_LinkedInternalRender(SceneSystem* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the InternalStateManager.Exit
void SceneSystem_LinkedInternalExit(SceneSystem* Self);

///****************************************************************************
// Function Definitions
///****************************************************************************
// Initiallize and store the required scenes
void InitiallizeScenes(SceneSystem* Self)
{
	// Setup the scenes
	// Battle Scene Init
	BattleScene_Setup(&Self->InternalBattleScene);
	Self->InternalBattleScene.Initiallize(&Self->InternalBattleScene);
	// World View Scene Init
	WorldViewScene_Setup(&Self->InternalWorldViewScene);
	Self->InternalWorldViewScene.Initiallize(&Self->InternalWorldViewScene);
	// Dungeon Scene Init
	DungeonScene_Setup(&Self->InternalDungeonScene);
	Self->InternalDungeonScene.Initiallize(&Self->InternalDungeonScene);
	// Splash Scene Init
	SplashScene_Setup(&Self->InternalSplashScene);
	Self->InternalSplashScene.Initiallize(&Self->InternalSplashScene);
	// Menu Scene Init
	MenuScene_Setup(&Self->InternalMenuScene);
	Self->InternalMenuScene.Initiallize(&Self->InternalMenuScene);
	// Win Scene Init
	WinScene_Setup(&Self->InternalWinScene);
	Self->InternalWinScene.Initiallize(&Self->InternalWinScene);
	// Set their state managers
	SceneDataStorage[SS_Battle] = &Self->InternalBattleScene.InternalStateManager;
	SceneDataStorage[SS_WorldView] = &Self->InternalWorldViewScene.InternalStateManager;
	SceneDataStorage[SS_Dungeon] = &Self->InternalDungeonScene.InternalStateManager;
	SceneDataStorage[SS_Splash] = &Self->InternalSplashScene.InternalStateManager;
	SceneDataStorage[SS_MainMenu] = &Self->InternalMenuScene.InternalStateManager;
	SceneDataStorage[SS_Win] = &Self->InternalWinScene.InternalStateManager;
}

void SceneSystem_Setup(SceneSystem* Self)
{
	// Set up the InternalStateManager
	Self->InternalStateManager.Initiallize = SceneSystem_LinkedInternalInitiallize;
	Self->InternalStateManager.Update = SceneSystem_LinkedInternalUpdate;
	Self->InternalStateManager.Render = SceneSystem_LinkedInternalRender;
	Self->InternalStateManager.Exit = SceneSystem_LinkedInternalExit;
	Self->SetCurrentScene = SceneSystem_LinkedSetCurrentScene;

	// Set the current state
	Self->InternalState = SS_Splash;

	// Set up the functions of this object
	Self->Initiallize = SceneSystem_LinkedInitiallize;
	Self->Update = SceneSystem_LinkedUpdate;
	Self->Render = SceneSystem_LinkedRender;
	Self->Exit = SceneSystem_LinkedExit;
}

// Linked Initiallize function that will be set to the struct's Initiallize
void SceneSystem_LinkedInitiallize(SceneSystem* Self)
{
	Self->InternalStateManager.Initiallize(Self);
}

// Linked Update function that will be set to the struct's Update
void SceneSystem_LinkedUpdate(SceneSystem* Self, Engine* BaseEngine, double Delta)
{
	Self->InternalStateManager.Update(Self, BaseEngine, Delta);
}

// Linked Render function that will be set to the struct's Render
void SceneSystem_LinkedRender(SceneSystem* Self, Engine* BaseEngine)
{
	Self->InternalStateManager.Render(Self, BaseEngine);
}

// Linked Exit function that will be set to the struct's Exit
void SceneSystem_LinkedExit(SceneSystem* Self)
{
	Self->InternalStateManager.Exit(Self);
}

// Linked SetCurrentScene function that will be set to the struct's SetCurrentScene
void SceneSystem_LinkedSetCurrentScene(SceneSystem* Self, enum SS_Scenes DesiredScene)
{
	Self->InternalState = DesiredScene;
}

// Linked Initiallize function that will be set to the InternalStateManager
void SceneSystem_LinkedInternalInitiallize(SceneSystem* Self)
{
	// Here I will initiallize the internal state manager
	// Initiallizing Scenes
	InitiallizeScenes(Self);
	EnemyEncounterHandler_Setup(&Self->InternalEncounterHandler);
	Self->InternalEncounterHandler.Initiallize(&Self->InternalEncounterHandler);
}

// Linked Update function that will be set to the InternalStateManager
void SceneSystem_LinkedInternalUpdate(SceneSystem* Self, Engine* BaseEngine, double Delta)
{
	// Do some state logic for the internal state manager
	// Determine if the scene is valid for updates
	if (SceneDataStorage[Self->InternalState])
		switch (Self->InternalState)
		{
			case SS_WorldView:
				SceneDataStorage[Self->InternalState]->Update(&Self->InternalWorldViewScene, BaseEngine, Delta);
				break;
			case SS_Dungeon:
				SceneDataStorage[Self->InternalState]->Update(&Self->InternalDungeonScene, BaseEngine, Delta);
				break;
			case SS_Battle:
				SceneDataStorage[Self->InternalState]->Update(&Self->InternalBattleScene, BaseEngine, Delta);
				break;
			case SS_Splash:
				SceneDataStorage[Self->InternalState]->Update(&Self->InternalSplashScene, BaseEngine, Delta);
				break;
			case SS_MainMenu:
				SceneDataStorage[Self->InternalState]->Update(&Self->InternalMenuScene, BaseEngine, Delta);
				break;
			case SS_Win:
				SceneDataStorage[Self->InternalState]->Update(&Self->InternalWinScene, BaseEngine, Delta);
				break;
			default:
				break;
		}
}

// Linked Render function that will be set to the InternalStateManager
void SceneSystem_LinkedInternalRender(SceneSystem* Self, Engine* BaseEngine)
{
	// Render the appropriate scene
	if (SceneDataStorage[Self->InternalState] != NULL)
		switch (Self->InternalState)
		{
			case SS_WorldView:
				SceneDataStorage[Self->InternalState]->Render(&Self->InternalWorldViewScene, BaseEngine);
				break;
			case SS_Dungeon:
				SceneDataStorage[Self->InternalState]->Render(&Self->InternalDungeonScene, BaseEngine);
				break;
			case SS_Battle:
				SceneDataStorage[Self->InternalState]->Render(&Self->InternalBattleScene, BaseEngine);
				break;
			case SS_Splash:
				SceneDataStorage[Self->InternalState]->Render(&Self->InternalSplashScene, BaseEngine);
				break;
			case SS_MainMenu:
				SceneDataStorage[Self->InternalState]->Render(&Self->InternalMenuScene, BaseEngine);
				break;
			case SS_Win:
				SceneDataStorage[Self->InternalState]->Render(&Self->InternalWinScene, BaseEngine);
				break;
			default:
				break;
		}
}

// Linked Exit function that will be set to the InternalStateManager
void SceneSystem_LinkedInternalExit(SceneSystem* Self)
{
	// Free up other miscellaneous objects
	Self->InternalEncounterHandler.Exit(&Self->InternalEncounterHandler);
	// Free the stuff initiallized in the Internal State Manager
	Self->InternalBattleScene.Exit(&Self->InternalBattleScene);
	Self->InternalDungeonScene.Exit(&Self->InternalDungeonScene);
	Self->InternalWorldViewScene.Exit(&Self->InternalWorldViewScene);
	Self->InternalMenuScene.Exit(&Self->InternalMenuScene);
	Self->InternalSplashScene.Exit(&Self->InternalSplashScene);
	Self->InternalWinScene.Exit(&Self->InternalWinScene);
}