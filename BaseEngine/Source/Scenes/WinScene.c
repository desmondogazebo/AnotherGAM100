/******************************************************************************
filename    WinScene.c
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Created on 23 November 2017

Brief Description:
The splash screen, displays splash logos/icons for a few seconds before
transiting scenes

******************************************************************************/
#include "WinScene.h"

#include <stdio.h>

// Inclusion of utility functions
#include "../Utilities/TextDataLoader.h"
#include "../Utilities/Utilities.h"

// Included for Rendering
#include "../Engine/BaseEngine.h"

///****************************************************************************
// Private Variables
///****************************************************************************
TextDataLoader WinScene_Loader;

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void WinScene_LinkedInitiallize(WinScene* Self);
// Linked Update function that will be set to the struct's Update
void WinScene_LinkedUpdate(WinScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the struct's Render
void WinScene_LinkedRender(WinScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the struct's Exit
void WinScene_LinkedExit(WinScene* Self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void WinScene_LinkedInternalInitiallize(WinScene* Self);
// Linked Update function that will be set to the InternalStateManager.Update
void WinScene_LinkedInternalUpdate(WinScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void WinScene_LinkedInternalRender(WinScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the InternalStateManager.Exit
void WinScene_LinkedInternalExit(WinScene* Self);


///****************************************************************************
// Function Definitions
///****************************************************************************
void WinScene_Setup(WinScene* Self)
{
	// Set up the InternalStateManager
	Self->InternalStateManager.Initiallize = WinScene_LinkedInternalInitiallize;
	Self->InternalStateManager.Update = WinScene_LinkedInternalUpdate;
	Self->InternalStateManager.Render = WinScene_LinkedInternalRender;
	Self->InternalStateManager.Exit = WinScene_LinkedInternalExit;

	// Set the current state
	Self->InternalState = WS_Loading;

	// Set up the functions of this object
	Self->Initiallize = WinScene_LinkedInitiallize;
	Self->Update = WinScene_LinkedUpdate;
	Self->Render = WinScene_LinkedRender;
	Self->Exit = WinScene_LinkedExit;
}

// Linked Initiallize function that will be set to the struct's Initiallize
void WinScene_LinkedInitiallize(WinScene* Self)
{
	Self->InternalStateManager.Initiallize(Self);
}

// Linked Update function that will be set to the struct's Update
void WinScene_LinkedUpdate(WinScene* Self, Engine* BaseEngine, double Delta)
{
	Self->InternalStateManager.Update(Self, BaseEngine, Delta);
}

// Linked Render function that will be set to the struct's Render
void WinScene_LinkedRender(WinScene* Self, Engine* BaseEngine)
{
	Self->InternalStateManager.Render(Self, BaseEngine);
}

// Linked Exit function that will be set to the struct's Exit
void WinScene_LinkedExit(WinScene* Self)
{
	Self->InternalStateManager.Exit(Self);
}

// Linked Initiallize function that will be set to the InternalStateManager
void WinScene_LinkedInternalInitiallize(WinScene* Self)
{
	// Here I will initiallize the internal state manager
	// Setup the loader that I am about to use.
	TextDataLoader_Setup(&WinScene_Loader);
	// Load the sprites that will be used in the battle scene
	WinScene_Loader.LoadResource(&WinScene_Loader, "Resources/WinScreen.txt");
}

// Linked Update function that will be set to the InternalStateManager
void WinScene_LinkedInternalUpdate(WinScene* Self, Engine* BaseEngine, double Delta)
{
	// Do some state logic for the internal state manager
	switch (Self->InternalState)
	{
	case WS_Loading:
		if (isKeyPressed(VK_SPACE))
		{
			BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_MainMenu);
			BaseEngine->InternalSceneSystem.InternalWorldViewScene.Exit(&BaseEngine->InternalSceneSystem.InternalWorldViewScene);
			BaseEngine->InternalSceneSystem.InternalWorldViewScene.Initiallize(&BaseEngine->InternalSceneSystem.InternalWorldViewScene);
		}
		break;
	default: 
		break;
	}
}

// Linked Render function that will be set to the InternalStateManager
void WinScene_LinkedInternalRender(WinScene* Self, Engine* BaseEngine)
{
	// Renders the appropriate scene
	switch (Self->InternalState)
	{
	case WS_Loading:
		BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(0, 0), WinScene_Loader.TextData, WinScene_Loader.NumberOfRows, WinScene_Loader.NumberOfColumns, getColor(c_black, c_white));
		break;
	default:
		break;
	}
}

// Linked Exit function that will be set to the InternalStateManager
void WinScene_LinkedInternalExit(WinScene* Self)
{
	// Free the stuff initiallized in the Internal State Manager
	WinScene_Loader.Exit(&WinScene_Loader);
}