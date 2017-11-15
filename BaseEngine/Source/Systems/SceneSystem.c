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

// Inclusion of Scenes
#include "../Scenes/BattleScene.h"

///****************************************************************************
// Private Variables
///****************************************************************************
// Scene Info
BaseStateManager* SceneDataStorage[SS_Total];

// Scene Variables
BattleScene InternalBattleScene;

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
// Scene System Functions
// Initiallize and store the required scenes
void InitiallizeScenes();

// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void SceneSystem_LinkedInitiallize(SceneSystem* Self);
// Linked Update function that will be set to the struct's Update
void SceneSystem_LinkedUpdate(SceneSystem* Self, float Delta);
// Linked Render function that will be set to the struct's Render
void SceneSystem_LinkedRender(SceneSystem* Self, Engine* Renderer);
// Linked Exit function that will be set to the struct's Exit
void SceneSystem_LinkedExit(SceneSystem* Self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void SceneSystem_LinkedInternalInitiallize(SceneSystem* Self);
// Linked Update function that will be set to the InternalStateManager.Update
void SceneSystem_LinkedInternalUpdate(SceneSystem* Self, float Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void SceneSystem_LinkedInternalRender(SceneSystem* Self, Engine* Renderer);
// Linked Exit function that will be set to the InternalStateManager.Exit
void SceneSystem_LinkedInternalExit(SceneSystem* Self);

///****************************************************************************
// Function Definitions
///****************************************************************************
// Initiallize and store the required scenes
void InitiallizeScenes()
{
	// Setup the scenes
	BattleScene_Setup(&InternalBattleScene);
	InternalBattleScene.Initiallize(&InternalBattleScene);
	// Set their state managers
	SceneDataStorage[SS_Battle] = &InternalBattleScene.InternalStateManager;
}

void SceneSystem_Setup(SceneSystem* Self)
{
	// Set up the InternalStateManager
	Self->InternalStateManager.Initiallize = SceneSystem_LinkedInternalInitiallize;
	Self->InternalStateManager.Update = SceneSystem_LinkedInternalUpdate;
	Self->InternalStateManager.Render = SceneSystem_LinkedInternalRender;
	Self->InternalStateManager.Exit = SceneSystem_LinkedInternalExit;

	// Set the current state
	Self->InternalState = SS_Battle;

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
void SceneSystem_LinkedUpdate(SceneSystem* Self, float Delta)
{
	Self->InternalStateManager.Update(Self, Delta);
}

// Linked Render function that will be set to the struct's Render
void SceneSystem_LinkedRender(SceneSystem* Self, Engine* Renderer)
{
	Self->InternalStateManager.Render(Self);
}

// Linked Exit function that will be set to the struct's Exit
void SceneSystem_LinkedExit(SceneSystem* Self)
{
	Self->InternalStateManager.Exit(Self);
}

// Linked Initiallize function that will be set to the InternalStateManager
void SceneSystem_LinkedInternalInitiallize(SceneSystem* Self)
{
	// Here I will initiallize the internal state manager
	// Initiallizing Scenes
	InitiallizeScenes();
}

// Linked Update function that will be set to the InternalStateManager
void SceneSystem_LinkedInternalUpdate(SceneSystem* Self, float Delta)
{
	// Do some state logic for the internal state manager
	// Testing code state cycling
	switch (Self->InternalState)
	{
	case SS_WorldView:
		Self->InternalState = SS_Battle;
		break;
	case SS_Battle:
		Self->InternalState = SS_WorldView;
		break;
	default:
		break;
	}
}

// Linked Render function that will be set to the InternalStateManager
void SceneSystem_LinkedInternalRender(SceneSystem* Self, Engine* Renderer)
{
	// Render the appropriate scene
	if (SceneDataStorage[Self->InternalState] != NULL)
		switch (Self->InternalState)
		{
			case SS_Battle:
				SceneDataStorage[Self->InternalState]->Render(&InternalBattleScene, Renderer);
				break;
			default:
				break;
		}
}

// Linked Exit function that will be set to the InternalStateManager
void SceneSystem_LinkedInternalExit(SceneSystem* Self)
{
	// Free the stuff initiallized in the Internal State Manager
	InternalBattleScene.Exit(&InternalBattleScene);
}