/******************************************************************************
filename    DungeonScene.h
author      Rui An Ryan Lim & Qingping Zheng
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
Vector2 dungeon_moveDirection = { 0,0 }; // Direction of player movement

// Timers for letting the player run
double dungeon_initialRunDelay; // How long it takes before the player starts running
double dungeon_runDelayX; // How fast the player runs on X axis, speed = 1/runDelayX
double dungeon_runDelayY; // How fast the player runs on Y axis
double dungeon_runTimerX;
double dungeon_runTimerY;

//dungeon transition vairiables
short dungeon_transitionCount;
double dungeon_transitionTimer;
double dungeon_transitionTimerDelay;
short dungeon_waitToggle;
double dungeon_transitionWaitDelay;
TextDataLoader DungeonTransition_Loader;

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

//Local functions that relate to the scene.
void DungeonScene_PlayerControls(DungeonScene* self, Engine* BaseEngine, double Delta);
Vector2 parseRandomSpawnPoint(TextDataLoader map, char charToLookOutFor);
void DungeonScene_Transition(DungeonScene* Self, Engine* BaseEngine, double Delta);
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

//local function, parses the map for an E, and 
Vector2 parseRandomSpawnPoint(TextDataLoader map, char charToLookOutFor)
{
	int spawnPointOccurenceCount = 0;
	//we take note of how many potential spawnpoints there are
	for (int y = 0; y < map.NumberOfRows; ++y)
	{
		for (int x = 0; x < map.NumberOfColumns; ++x)
		{
			if (map.TextData[y][x] == charToLookOutFor) //potential spawnpoint
			{
				spawnPointOccurenceCount++;
			}
		}
	}
	
	//we now know the number of spawnpoints in the provided map
	Vector2* possiblelocations = (Vector2*)malloc(spawnPointOccurenceCount * sizeof(Vector2));

	//re-iterate and add to list of spawnpoints
	int counter = 0;
	for (int y = 0; y < map.NumberOfRows; ++y)
	{
		for (int x = 0; x < map.NumberOfColumns; ++x)
		{
			if (map.TextData[y][x] == charToLookOutFor) //potential spawnpoint
			{
				possiblelocations[counter] = Vec2(x, y);
				counter++;
			}
		}
	}
	//gimme a random spawnpoint
	int randomIndex = rand() % spawnPointOccurenceCount;
	//we got the spawnpoint, free memory
	Vector2 copyVector = Vec2(possiblelocations[randomIndex].x, possiblelocations[randomIndex].y);
	free(possiblelocations);
	return copyVector;
}

// Linked Initiallize function that will be set to the InternalStateManager
void DungeonScene_LinkedInternalInitiallize(DungeonScene* Self)
{
	// Here I will initiallize the internal state manager
	// Setup the loader that I am about to use.
	// Load the sprites that will be used in the battle scene
	TextDataLoader_Setup(&DungeonTransition_Loader);
	DungeonTransition_Loader.LoadResource(&DungeonTransition_Loader, "Resources/BattleTransition.txt");

	TextDataLoader_Setup(&DungeonScene_Loader);
	// Set the current state
	Self->InternalState = DS_Exploration;

	Self->wKeyPressed = Self->sKeyPressed = Self->aKeyPressed = Self->dKeyPressed = 0;

	//ADD MORE IF NEEDED.
	char *maps[] = {
		"Resources/Dungeons/dungeon0.txt", //completed
		"Resources/Dungeons/dungeon1.txt", //todo
		"Resources/Dungeons/dungeon2.txt", //todo
		"Resources/Dungeons/dungeon3.txt"  //todo
	};

	int randomMap = rand() % (sizeof(maps) / sizeof(char*));

	DungeonScene_Loader.LoadResource(&DungeonScene_Loader, maps[randomMap]);
	Initialize_Player(&Self->player, parseRandomSpawnPoint(DungeonScene_Loader, 'E'));
	DungeonCamera_Setup(&DungeonScene_Camera);

	//LOCAL VARIABLES
	dungeon_initialRunDelay = 0.3;
	dungeon_runDelayX = 0.1;
	dungeon_runDelayY = 0.15;
	dungeon_runTimerX = 0;
	dungeon_runTimerY = 0;
	dungeon_transitionCount = 0;
	dungeon_transitionTimer = 0;
	dungeon_transitionTimerDelay = 0.01;
	dungeon_waitToggle = 0;
	dungeon_transitionWaitDelay = 0.5;
}

// Linked Update function that will be set to the InternalStateManager
void DungeonScene_LinkedInternalUpdate(DungeonScene* Self, Engine* BaseEngine, double Delta)
{
	// Debug code
	if (isKeyPressed('Q'))
	{
		Delta *= 4;
	}

	// Do some state logic for the internal state manager
	switch (Self->InternalState)
	{
	case DS_TransitionToWorld:
		BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_WorldView);
		break;
	case DS_Exploration:
		DungeonScene_PlayerControls(Self, BaseEngine, Delta);
		DungeonScene_Camera.UpdateCameraLogic(&DungeonScene_Camera, BaseEngine->g_console, &DungeonScene_Loader, &Self->player.position);
		break;
	case DS_TransitionToBoss:
		DungeonScene_Transition(Self, BaseEngine, Delta);
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
	case DS_TransitionToWorld:
		break;
	case DS_Exploration:
		BaseEngine->g_console->dungeon_WriteToBuffer(BaseEngine->g_console, DungeonScene_Loader.TextData, DungeonScene_Camera.CalculatedMapOffset.x, DungeonScene_Camera.CalculatedMapOffset.y, getColor(c_black, c_white));
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(Self->player.position.x - DungeonScene_Camera.CalculatedMapOffset.x, Self->player.position.y - DungeonScene_Camera.CalculatedMapOffset.y), "O", getColor(c_black, c_aqua));
		break;
	case DS_TransitionToBoss:
	{
		Vector2 location = { -BaseEngine->g_console->consoleSize.X + dungeon_transitionCount, 0 };
		BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, location, DungeonTransition_Loader.TextData, DungeonTransition_Loader.NumberOfRows, DungeonTransition_Loader.NumberOfColumns, getColor(c_black, c_white));
	}
	default:
		break;
	}
}

// Linked Exit function that will be set to the InternalStateManager
void DungeonScene_LinkedInternalExit(DungeonScene* Self)
{
	// Free the stuff initiallized in the Internal State Manager
	DungeonTransition_Loader.Exit(&DungeonTransition_Loader);
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
			dungeon_moveDirection.y--;
			short plrMoveCode = MovePlayer(&self->player, Vec2(0, dungeon_moveDirection.y), DungeonScene_Loader);
			if (plrMoveCode == 1)
			{
				self->InternalState = DS_TransitionToWorld;
			}
			else if (plrMoveCode == 2)
			{
				self->InternalState = DS_TransitionToBoss;
			}
		}
	}
	else
	{
		// Key press up
		if (self->wKeyPressed == 1)
		{
			self->wKeyPressed = 0;
			dungeon_moveDirection.y++;
		}
	}

	if (isKeyPressed('S'))
	{
		// Key press down
		if (self->sKeyPressed == 0)
		{
			self->sKeyPressed = 1;
			dungeon_moveDirection.y++;
			short plrMoveCode = MovePlayer(&self->player, Vec2(0, dungeon_moveDirection.y), DungeonScene_Loader);
			if (plrMoveCode == 1)
			{
				self->InternalState = DS_TransitionToWorld;
			}
			else if (plrMoveCode == 2)
			{
				self->InternalState = DS_TransitionToBoss;
			}
		}
	}
	else
	{
		// Key press up
		if (self->sKeyPressed == 1)
		{
			self->sKeyPressed = 0;
			dungeon_moveDirection.y--;
		}
	}

	if (isKeyPressed('A'))
	{
		// Key press down
		if (self->aKeyPressed == 0)
		{
			self->aKeyPressed = 1;
			dungeon_moveDirection.x--;
			short plrMoveCode = MovePlayer(&self->player, Vec2(dungeon_moveDirection.x, 0), DungeonScene_Loader);
			if (plrMoveCode == 1)
			{
				self->InternalState = DS_TransitionToWorld;
			}
			else if (plrMoveCode == 2)
			{
				self->InternalState = DS_TransitionToBoss;
			}
		}
	}
	else
	{
		// Key press up
		if (self->aKeyPressed == 1)
		{
			self->aKeyPressed = 0;
			dungeon_moveDirection.x++;
		}
	}

	if (isKeyPressed('D'))
	{
		// Key press down
		if (self->dKeyPressed == 0)
		{
			self->dKeyPressed = 1;
			dungeon_moveDirection.x++;
			short plrMoveCode = MovePlayer(&self->player, Vec2(dungeon_moveDirection.x, 0), DungeonScene_Loader);
			if (plrMoveCode == 1)
			{
				self->InternalState = DS_TransitionToWorld;
			}
			else if (plrMoveCode == 2)
			{
				self->InternalState = DS_TransitionToBoss;
			}
		}
	}
	else
	{
		// Key press up
		if (self->dKeyPressed == 1)
		{
			self->dKeyPressed = 0;
			dungeon_moveDirection.x--;
		}
	}

	if (self->aKeyPressed == 1 || self->dKeyPressed == 1 || self->wKeyPressed == 1 || self->sKeyPressed == 1)
	{
		if ((dungeon_runTimerX += Delta) > dungeon_initialRunDelay) // Initial delay before running
		{
			if (dungeon_runTimerX > dungeon_initialRunDelay + dungeon_runDelayX) // 0.4 - 0.3 = 0.1, delay in between each "run step"
			{
				dungeon_runTimerX = dungeon_initialRunDelay;

				short plrMoveCode = MovePlayer(&self->player, Vec2(dungeon_moveDirection.x, 0), DungeonScene_Loader);
				if (plrMoveCode == 1)
				{
					self->InternalState = DS_TransitionToWorld;
				}
				else if (plrMoveCode == 2)
				{
					self->InternalState = DS_TransitionToBoss;
				}
				plrMoveCode = MovePlayer(&self->player, Vec2(0, dungeon_moveDirection.y), DungeonScene_Loader);
				if (plrMoveCode == 1)
				{
					self->InternalState = DS_TransitionToWorld;
				}
				else if (plrMoveCode == 2)
				{
					self->InternalState = DS_TransitionToBoss;
				}
			}
		}
	}
	else
	{
		dungeon_runTimerX = 0;
		dungeon_moveDirection.x = 0;
	}
}

void DungeonScene_Transition(DungeonScene* self, Engine* BaseEngine, double Delta)
{
	dungeon_transitionTimer += Delta;

	if (dungeon_waitToggle == 0)
	{
		if (dungeon_transitionTimer > dungeon_transitionTimerDelay)
		{
			dungeon_transitionCount++;
			dungeon_transitionTimer = 0;
			if (dungeon_transitionCount == 80)
			{
				dungeon_waitToggle = 1;
			}
		}
	}
	else if (dungeon_waitToggle == 1)
	{
		if (dungeon_transitionTimer > dungeon_transitionWaitDelay)
		{
			dungeon_transitionCount = 0;
			dungeon_transitionTimer = 0;
			dungeon_waitToggle = 0;
			BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_Battle);
		}
	}
}