/******************************************************************************
filename    BattleScene.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 10 November 2017

Brief Description:
A template on creating a customized state manager

******************************************************************************/
#include "BattleScene.h"

#include <stdio.h>

// Inclusion of utility functions
#include "../Utilities/TextDataLoader.h"
#include "../Utilities/Utilities.h"

// Included for Rendering
#include "../Engine/BaseEngine.h"

///****************************************************************************
// Private Variables
///****************************************************************************
TextDataLoader BattleScene_Loader_Base;
TextDataLoader BattleScene_Loader_Layout;
TextDataLoader BattleScene_Loader_Enemy;

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void BattleScene_LinkedInitiallize(BattleScene* Self);
// Linked Update function that will be set to the struct's Update
void BattleScene_LinkedUpdate(BattleScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the struct's Render
void BattleScene_LinkedRender(BattleScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the struct's Exit
void BattleScene_LinkedExit(BattleScene* Self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void BattleScene_LinkedInternalInitiallize(BattleScene* Self);
// Linked Update function that will be set to the InternalStateManager.Update
void BattleScene_LinkedInternalUpdate(BattleScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void BattleScene_LinkedInternalRender(BattleScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the InternalStateManager.Exit
void BattleScene_LinkedInternalExit(BattleScene* Self);

///****************************************************************************
// Function Definitions
///****************************************************************************
void BattleScene_Setup(BattleScene* Self)
{
	// Set up the InternalStateManager
	Self->InternalStateManager.Initiallize = BattleScene_LinkedInternalInitiallize;
	Self->InternalStateManager.Update = BattleScene_LinkedInternalUpdate;
	Self->InternalStateManager.Render = BattleScene_LinkedInternalRender;
	Self->InternalStateManager.Exit = BattleScene_LinkedInternalExit;

	// Set the current state
	Self->InternalState = CSM_Loading;

	// Set up the functions of this object
	Self->Initiallize = BattleScene_LinkedInitiallize;
	Self->Update = BattleScene_LinkedUpdate;
	Self->Render = BattleScene_LinkedRender;
	Self->Exit = BattleScene_LinkedExit;
}

// Linked Initiallize function that will be set to the struct's Initiallize
void BattleScene_LinkedInitiallize(BattleScene* Self)
{
	Self->InternalStateManager.Initiallize(Self);
}

// Linked Update function that will be set to the struct's Update
void BattleScene_LinkedUpdate(BattleScene* Self, Engine* BaseEngine, double Delta)
{
	Self->InternalStateManager.Update(Self, BaseEngine, Delta);
}

// Linked Render function that will be set to the struct's Render
void BattleScene_LinkedRender(BattleScene* Self, Engine* BaseEngine)
{
	Self->InternalStateManager.Render(Self, BaseEngine);
}

// Linked Exit function that will be set to the struct's Exit
void BattleScene_LinkedExit(BattleScene* Self)
{
	Self->InternalStateManager.Exit(Self);
}

// Linked Initiallize function that will be set to the InternalStateManager
void BattleScene_LinkedInternalInitiallize(BattleScene* Self)
{
	// Here I will initiallize the internal state manager
	// Setup the loader that I am about to use.
	TextDataLoader_Setup(&BattleScene_Loader_Base);
	TextDataLoader_Setup(&BattleScene_Loader_Layout);
	TextDataLoader_Setup(&BattleScene_Loader_Enemy);
	// Load the sprites that will be used in the battle scene
	BattleScene_Loader_Base.LoadResource(&BattleScene_Loader_Base, "Resources/DigiPenLogo(Unofficial).txt");
	BattleScene_Loader_Layout.LoadResource(&BattleScene_Loader_Layout, "Resources/Maps/dungeonTemplate.txt");

}

// Linked Update function that will be set to the InternalStateManager
void BattleScene_LinkedInternalUpdate(BattleScene* Self, Engine* BaseEngine, double Delta)
{
	// Do some state logic for the internal state manager
	// Testing code state cycling
	switch (Self->InternalState)
	{
	case CSM_Loading:
		//Self->InternalState = ;
		break;
	case CSM_PlayerTurn:
		break;
	case CSM_EnemyTurn:
		break;
	case CSM_BattleSequence:
		break;
	case CSM_Results:
		break;
	}
}

// Linked Render function that will be set to the InternalStateManager
void BattleScene_LinkedInternalRender(BattleScene* Self, Engine* BaseEngine)
{
	// Renders the appropriate scene
	switch (Self->InternalState)
	{
	case CSM_Loading:
		BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(0, 0), BattleScene_Loader_Layout.TextData, BattleScene_Loader_Layout.NumberOfRows, BattleScene_Loader_Layout.NumberOfColumns, getColor(c_black, c_white));
		BaseEngine->g_console->map_WriteToBuffer(BaseEngine->g_console, BattleScene_Loader_Base.TextData, BattleScene_Loader_Base.NumberOfRows, BattleScene_Loader_Base.NumberOfColumns, getColor(c_black, c_white));
		break;
	case CSM_PlayerTurn:
		break;
	case CSM_EnemyTurn:
		break;
	case CSM_BattleSequence:
		break;
	case CSM_Results:
		break;
	}
}

// Linked Exit function that will be set to the InternalStateManager
void BattleScene_LinkedInternalExit(BattleScene* Self)
{
	// Free the stuff initiallized in the Internal State Manager
	BattleScene_Loader_Base.Exit(&BattleScene_Loader_Base);
	BattleScene_Loader_Layout.Exit(&BattleScene_Loader_Layout);
	BattleScene_Loader_Enemy.Exit(&BattleScene_Loader_Enemy);
}