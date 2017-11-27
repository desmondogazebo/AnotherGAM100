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
#include "../LogicalObjects/DungeonCamera.h"
#include "../Player/Player.h"

///****************************************************************************
// Private Variables
///****************************************************************************
TextDataLoader DungeonScene_Loader;
DungeonCamera DungeonScene_Camera;
Vector2 moveDirection; // Direction of player movement

// Timers for letting the player run
double dungeon_wvs_initialRunDelay = 0.3; // How long it takes before the player starts running
double dungeon_wvs_runDelayX = 0.1; // How fast the player runs on X axis, speed = 1/runDelayX
double dungeon_wvs_runDelayY = 0.15; // How fast the player runs on Y axis
double dungeon_wvs_runTimerX = 0;
double dungeon_wvs_runTimerY = 0;

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

void DungeonScene_PlayerControls(DungeonScene* self, Engine* BaseEngine, double Delta);

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
	Self->InternalState = DS_Exploration;

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
	DungeonScene_Loader.LoadResource(&DungeonScene_Loader, "Resources/Maps/dungeonTemplate.txt");
	DungeonCamera_Setup(&DungeonScene_Camera);
	Initialize_Player(&Self->player, Vec2(50, 5));
}

// Linked Update function that will be set to the InternalStateManager
void DungeonScene_LinkedInternalUpdate(DungeonScene* Self, Engine* BaseEngine, double Delta)
{
	// Do some state logic for the internal state manager
	switch (Self->InternalState)
	{
	case DS_Loading:
		break;
	case DS_Exploration:
		DungeonScene_PlayerControls(Self, BaseEngine, Delta);
		DungeonScene_Camera.UpdateCameraLogic(&DungeonScene_Camera, BaseEngine->g_console, &DungeonScene_Loader, &Self->player.position);
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
		BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(-10, 0), DungeonScene_Loader.TextData, DungeonScene_Loader.NumberOfRows, DungeonScene_Loader.NumberOfColumns, getColor(c_black, c_white));
		break;
	case DS_Exploration:
		BaseEngine->g_console->dungeon_WriteToBuffer(BaseEngine->g_console, DungeonScene_Loader.TextData, DungeonScene_Camera.CalculatedMapOffset.x, DungeonScene_Camera.CalculatedMapOffset.y, getColor(c_black, c_white));
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(Self->player.position.x - DungeonScene_Camera.CalculatedMapOffset.x, Self->player.position.y - DungeonScene_Camera.CalculatedMapOffset.y), "O", getColor(c_black, c_aqua));
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

void DungeonScene_PlayerControls(DungeonScene* self, Engine* BaseEngine, double Delta)
{
	if (isKeyPressed('W'))
	{
		// Key press down
		if (self->wKeyPressed == 0)
		{
			self->wKeyPressed = 1;
			moveDirection.y = -1;
			short plrMoveCode = MovePlayer(&self->player, moveDirection, DungeonScene_Loader);
		}
	}
	else
	{
		// Key press up
		if (self->wKeyPressed == 1)
		{
			self->wKeyPressed = 0;
			moveDirection.y = 0;
		}
	}

	if (isKeyPressed('S'))
	{
		// Key press down
		if (self->sKeyPressed == 0)
		{
			self->sKeyPressed = 1;
			moveDirection.y = 1;
			short plrMoveCode = MovePlayer(&self->player, moveDirection, DungeonScene_Loader);
		}
	}
	else
	{
		// Key press up
		if (self->sKeyPressed == 1)
		{
			self->sKeyPressed = 0;
			moveDirection.y = 0;
		}
	}

	if (isKeyPressed('A'))
	{
		// Key press down
		if (self->aKeyPressed == 0)
		{
			self->aKeyPressed = 1;
			moveDirection.x = -1;
			short plrMoveCode = MovePlayer(&self->player, moveDirection, DungeonScene_Loader);
		}
	}
	else
	{
		// Key press up
		if (self->aKeyPressed == 1)
		{
			self->aKeyPressed = 0;
			moveDirection.x = 0;
		}
	}

	if (isKeyPressed('D'))
	{
		// Key press down
		if (self->dKeyPressed == 0)
		{
			self->dKeyPressed = 1;
			moveDirection.x = 1;
			short plrMoveCode = MovePlayer(&self->player, moveDirection, DungeonScene_Loader);
		}
	}
	else
	{
		// Key press up
		if (self->dKeyPressed == 1)
		{
			self->dKeyPressed = 0;
			moveDirection.x = 0;
		}
	}

	if (self->aKeyPressed == 1 || self->dKeyPressed == 1)
	{
		if ((dungeon_wvs_runTimerX += Delta) > dungeon_wvs_initialRunDelay) // Initial delay before running
		{
			if (dungeon_wvs_runTimerX > dungeon_wvs_initialRunDelay + dungeon_wvs_runDelayX) // 0.4 - 0.3 = 0.1, delay in between each "run step"
			{
				dungeon_wvs_runTimerX = dungeon_wvs_initialRunDelay;
				Vector2 tempDirection = moveDirection;
				tempDirection.y = 0;
				short plrMoveCode = MovePlayer(&self->player, moveDirection, DungeonScene_Loader);
			}
		}
	}
	else
	{
		dungeon_wvs_runTimerX = 0;
		moveDirection.x = 0;
	}


	if (self->wKeyPressed == 1 || self->sKeyPressed == 1)
	{
		if ((dungeon_wvs_runTimerY += Delta) > dungeon_wvs_initialRunDelay) // Initial delay before running
		{
			if (dungeon_wvs_runTimerY > dungeon_wvs_initialRunDelay + dungeon_wvs_runDelayY) // 0.4 - 0.3 = 0.1, delay in between each "run step"
			{
				dungeon_wvs_runTimerY = dungeon_wvs_initialRunDelay;
				Vector2 tempDirection = moveDirection;
				tempDirection.x = 0;
				short plrMoveCode = MovePlayer(&self->player, moveDirection, DungeonScene_Loader);
			}
		}
	}
	else
	{
		dungeon_wvs_runTimerY = 0;
		moveDirection.y = 0;
	}
}