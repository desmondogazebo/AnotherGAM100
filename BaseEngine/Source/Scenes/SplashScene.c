/******************************************************************************
filename    SplashScene.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 23 November 2017

Brief Description:
The splash screen, displays splash logos/icons for a few seconds before
transiting scenes

******************************************************************************/
#include "SplashScene.h"

#include <stdio.h>

// Inclusion of utility functions
#include "../Utilities/TextDataLoader.h"
#include "../Utilities/Utilities.h"

// Included for Rendering
#include "../Engine/BaseEngine.h"

///****************************************************************************
// Private Variables
///****************************************************************************
TextDataLoader SplashScene_Loader;
double SplashScene_LoadTimer = 0;

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void SplashScene_LinkedInitiallize(SplashScene* Self);
// Linked Update function that will be set to the struct's Update
void SplashScene_LinkedUpdate(SplashScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the struct's Render
void SplashScene_LinkedRender(SplashScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the struct's Exit
void SplashScene_LinkedExit(SplashScene* Self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void SplashScene_LinkedInternalInitiallize(SplashScene* Self);
// Linked Update function that will be set to the InternalStateManager.Update
void SplashScene_LinkedInternalUpdate(SplashScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void SplashScene_LinkedInternalRender(SplashScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the InternalStateManager.Exit
void SplashScene_LinkedInternalExit(SplashScene* Self);

///****************************************************************************
// Function Definitions
///****************************************************************************
void SplashScene_Setup(SplashScene* Self)
{
	// Set up the InternalStateManager
	Self->InternalStateManager.Initiallize = SplashScene_LinkedInternalInitiallize;
	Self->InternalStateManager.Update = SplashScene_LinkedInternalUpdate;
	Self->InternalStateManager.Render = SplashScene_LinkedInternalRender;
	Self->InternalStateManager.Exit = SplashScene_LinkedInternalExit;

	// Set the current state
	Self->InternalState = SS_Loading;

	// Set up the functions of this object
	Self->Initiallize = SplashScene_LinkedInitiallize;
	Self->Update = SplashScene_LinkedUpdate;
	Self->Render = SplashScene_LinkedRender;
	Self->Exit = SplashScene_LinkedExit;
}

// Linked Initiallize function that will be set to the struct's Initiallize
void SplashScene_LinkedInitiallize(SplashScene* Self)
{
	Self->InternalStateManager.Initiallize(Self);
}

// Linked Update function that will be set to the struct's Update
void SplashScene_LinkedUpdate(SplashScene* Self, Engine* BaseEngine, double Delta)
{
	Self->InternalStateManager.Update(Self, BaseEngine, Delta);
}

// Linked Render function that will be set to the struct's Render
void SplashScene_LinkedRender(SplashScene* Self, Engine* BaseEngine)
{
	Self->InternalStateManager.Render(Self, BaseEngine);
}

// Linked Exit function that will be set to the struct's Exit
void SplashScene_LinkedExit(SplashScene* Self)
{
	Self->InternalStateManager.Exit(Self);
}

// Linked Initiallize function that will be set to the InternalStateManager
void SplashScene_LinkedInternalInitiallize(SplashScene* Self)
{
	// Here I will initiallize the internal state manager
	// Setup the loader that I am about to use.
	TextDataLoader_Setup(&SplashScene_Loader);
	// Load the sprites that will be used in the battle scene
	SplashScene_Loader.LoadResource(&SplashScene_Loader, "Resources/DigiPenLogo(Unofficial).txt");
}

// Linked Update function that will be set to the InternalStateManager
void SplashScene_LinkedInternalUpdate(SplashScene* Self, Engine* BaseEngine, double Delta)
{
	// Do some state logic for the internal state manager
	switch (Self->InternalState)
	{
	case SS_Loading:
		if (SplashScene_LoadTimer >= 1)
		{
			//BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_Dungeon);
			BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_WorldView);
			SplashScene_LoadTimer = 0;
		}
		else SplashScene_LoadTimer += Delta;
		break;
	default: 
		break;
	}
}

// Linked Render function that will be set to the InternalStateManager
void SplashScene_LinkedInternalRender(SplashScene* Self, Engine* BaseEngine)
{
	// Renders the appropriate scene
	switch (Self->InternalState)
	{
	case SS_Loading:
		BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(1, 0), SplashScene_Loader.TextData, SplashScene_Loader.NumberOfRows, SplashScene_Loader.NumberOfColumns, getColor(c_black, c_white));
		break;
	default:
		break;
	}
}

// Linked Exit function that will be set to the InternalStateManager
void SplashScene_LinkedInternalExit(SplashScene* Self)
{
	// Free the stuff initiallized in the Internal State Manager
	SplashScene_Loader.Exit(&SplashScene_Loader);
}