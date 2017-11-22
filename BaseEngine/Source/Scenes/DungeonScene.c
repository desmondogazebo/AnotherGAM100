/******************************************************************************
filename    DungeonScene.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 16 November 2017

Brief Description:
The dungeon scene of the game. Contains game state logic involving the battle
mechanics.

******************************************************************************/
#include "DungeonScene.h"

#include <stdio.h>

// Inclusion of utility functions
#include "../Utilities/TextDataLoader.h"
#include "../Utilities/Utilities.h"

// Included for Rendering
#include "../Engine/BaseEngine.h"

///****************************************************************************
// Private Variables
///****************************************************************************
TextDataLoader DungeonScene_Loader;
double LoadTimer = 0;

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void DungeonScene_LinkedInitiallize(DungeonScene* Self);
// Linked Update function that will be set to the struct's Update
void DungeonScene_LinkedUpdate(DungeonScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the struct's Render
void DungeonScene_LinkedRender(DungeonScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the struct's Exit
void DungeonScene_LinkedExit(DungeonScene* Self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void DungeonScene_LinkedInternalInitiallize(DungeonScene* Self);
// Linked Update function that will be set to the InternalStateManager.Update
void DungeonScene_LinkedInternalUpdate(DungeonScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void DungeonScene_LinkedInternalRender(DungeonScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the InternalStateManager.Exit
void DungeonScene_LinkedInternalExit(DungeonScene* Self);

///****************************************************************************
// Function Definitions
///****************************************************************************
void DungeonScene_Setup(DungeonScene* Self)
{
	// Set up the InternalStateManager
	Self->InternalStateManager.Initiallize = DungeonScene_LinkedInternalInitiallize;
	Self->InternalStateManager.Update = DungeonScene_LinkedInternalUpdate;
	Self->InternalStateManager.Render = DungeonScene_LinkedInternalRender;
	Self->InternalStateManager.Exit = DungeonScene_LinkedInternalExit;

	// Set the current state
	Self->InternalState = DS_Loading;

	// Set up the functions of this object
	Self->Initiallize = DungeonScene_LinkedInitiallize;
	Self->Update = DungeonScene_LinkedUpdate;
	Self->Render = DungeonScene_LinkedRender;
	Self->Exit = DungeonScene_LinkedExit;
}

// Linked Initiallize function that will be set to the struct's Initiallize
void DungeonScene_LinkedInitiallize(DungeonScene* Self)
{
	Self->InternalStateManager.Initiallize(Self);
}

// Linked Update function that will be set to the struct's Update
void DungeonScene_LinkedUpdate(DungeonScene* Self, Engine* BaseEngine, double Delta)
{
	Self->InternalStateManager.Update(Self, BaseEngine, Delta);
}

// Linked Render function that will be set to the struct's Render
void DungeonScene_LinkedRender(DungeonScene* Self, Engine* BaseEngine)
{
	Self->InternalStateManager.Render(Self, BaseEngine);
}

// Linked Exit function that will be set to the struct's Exit
void DungeonScene_LinkedExit(DungeonScene* Self)
{
	Self->InternalStateManager.Exit(Self);
}

// Linked Initiallize function that will be set to the InternalStateManager
void DungeonScene_LinkedInternalInitiallize(DungeonScene* Self)
{
	// Here I will initiallize the internal state manager
	// Setup the loader that I am about to use.
	TextDataLoader_Setup(&DungeonScene_Loader);
	// Load the sprites that will be used in the battle scene
	DungeonScene_Loader.LoadResource(&DungeonScene_Loader, "Resources/DigiPenLogo(Unofficial).txt");
}

// Linked Update function that will be set to the InternalStateManager
void DungeonScene_LinkedInternalUpdate(DungeonScene* Self, Engine* BaseEngine, double Delta)
{
	// Do some state logic for the internal state manager
	switch (Self->InternalState)
	{
	case DS_Loading:
		if (LoadTimer >= 1)
		{
			BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_WorldView);
			LoadTimer = 0;
		}
		else LoadTimer += Delta;
		break;
	case DS_Exploration:
		break;
	case DS_Results:
		break;
	default: 
		break;
	}
}

// Linked Render function that will be set to the InternalStateManager
void DungeonScene_LinkedInternalRender(DungeonScene* Self, Engine* BaseEngine)
{
	// Renders the appropriate scene
	switch (Self->InternalState)
	{
	case DS_Loading:
		BaseEngine->g_console->map_WriteToBuffer(BaseEngine->g_console, DungeonScene_Loader.TextData, DungeonScene_Loader.NumberOfRows, DungeonScene_Loader.NumberOfColumns, getColor(c_black, c_white));
		break;
	case DS_Exploration:
		break;
	case DS_Results:
		break;
	default:
		break;
	}
}

// Linked Exit function that will be set to the InternalStateManager
void DungeonScene_LinkedInternalExit(DungeonScene* Self)
{
	// Free the stuff initiallized in the Internal State Manager
	DungeonScene_Loader.Exit(&DungeonScene_Loader);
}