/******************************************************************************
filename    DungeonCamera.h
author      Rui An Ryan Lim & Qingping Zheng
DP email    l.ruianryan@digipen.edu

Created on 23 November 2017

Brief Description:
This file contains a camera object that manipulates what is being rendered in
the dungeon scene

******************************************************************************/
#include "DungeonCamera.h"
#include "../Utilities/TextDataLoader.h"
#include "../Engine/Console.h"

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
/* Uses passed TextDataLoader and Vec2 Player coords to determine the offset of the map */
void DungeonCamera_UpdateCameraLogic(DungeonCamera* Self, Console* console, TextDataLoader* MapData, Vector2* PlayerLocation);

///****************************************************************************
// Function Definitions
///****************************************************************************
/* Setup function that initiallizes the DungeonCamera */
void DungeonCamera_Setup(DungeonCamera* Self)
{
	// Set up base parameters and handle function pointer linking
	Self->UpdateCameraLogic = DungeonCamera_UpdateCameraLogic;
}

/* Uses passed TextDataLoader and Vec2 Player coords to determine the offset of the map */
void DungeonCamera_UpdateCameraLogic(DungeonCamera* Self, Console* console, TextDataLoader* MapData, Vector2* PlayerLocation)
{
	// Default the Offset Variable
	Self->CalculatedMapOffset = Vec2(0, 0);

	// Do some logic to calculate how much the view must be offsetted depending on how much the player has traversed the scene
	
	// Make changes to the offset variable if required


	if (PlayerLocation->x > (console->consoleSize.X >> 1)) //started to offset
	{
		Self->CalculatedMapOffset.x = PlayerLocation->x - (console->consoleSize.X >> 1);
	}
	if (PlayerLocation->y > (console->consoleSize.Y >> 1)) //started to offset
	{
		Self->CalculatedMapOffset.y = PlayerLocation->y - (console->consoleSize.Y >> 1);
	}

	if (Self->CalculatedMapOffset.x > MapData->NumberOfColumns - console->consoleSize.X -1)
	{
		Self->CalculatedMapOffset.x = MapData->NumberOfColumns - console->consoleSize.X - 1;
	}
	if (Self->CalculatedMapOffset.y > MapData->NumberOfRows - console->consoleSize.Y)
	{
		Self->CalculatedMapOffset.y = MapData->NumberOfRows - console->consoleSize.Y;
	}
}