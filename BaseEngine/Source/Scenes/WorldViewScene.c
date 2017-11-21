/******************************************************************************
filename    WorldViewScene.c
author      Keith Cheng
DP email    keith.cheng@digipen.edu

Created on 14 November 2017

Brief Description:
The main world view scene of the game. This is where the player roams around before entering dungeons.

******************************************************************************/
#include "WorldViewScene.h"

#include <stdio.h>

// Inclusion of utility functions
#include "../Utilities/TextDataLoader.h"
#include "../Utilities/Utilities.h"
#include "../Rooms/RoomFactory.h"

// Included for Rendering
#include "../Engine/BaseEngine.h"

///****************************************************************************
// Private Variables
///****************************************************************************
// Variables required for transitioning between screens
char** transitionMap;
int transitionMapRows;
int transitionMapColumns;
double roomTransitionTimer = 0;
int transitionCounter = 0;
Vector2 fakePlayerPosition; // A fake player used for cosmetic effect when transitioning

double roomTransitionDelayY = 0.06; // Change this to change how fast rooms transition on the Y axis
double roomTransitionDelayX = 0.02; // Same but X axis


// Timers for letting the player run
double initialRunDelay = 0.3; // How long it takes before the player starts running
double runDelayX = 0.1; // How fast the player runs on X axis, speed = 1/runDelayX
double runDelayY = 0.15; // How fast the player runs on Y axis
double runTimerX = 0;
double runTimerY = 0;

Vector2 moveDirection; // Direction of player movement

///****************************************************************************
// Private Function Prototypes
///****************************************************************************

// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void WorldViewScene_LinkedInitiallize(WorldViewScene* self);
// Linked Update function that will be set to the struct's Update
void WorldViewScene_LinkedUpdate(WorldViewScene* self, double Delta);
// Linked Render function that will be set to the struct's Render
void WorldViewScene_LinkedRender(WorldViewScene* self, Engine* Renderer);
// Linked Exit function that will be set to the struct's Exit
void WorldViewScene_LinkedExit(WorldViewScene* self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void WorldViewScene_LinkedInternalInitiallize(WorldViewScene* self);
// Linked Update function that will be set to the InternalStateManager.Update
void WorldViewScene_LinkedInternalUpdate(WorldViewScene* self, double Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void WorldViewScene_LinkedInternalRender(WorldViewScene* self, Engine* Renderer);
// Linked Exit function that will be set to the InternalStateManager.Exit
void WorldViewScene_LinkedInternalExit(WorldViewScene* self);

// Split functions to make file neater
void PlayerControls(WorldViewScene* self, double Delta);
void WrapPlayer(WorldViewScene* self);
void Add_Room(WorldViewScene* self, char *mapString);
void CopyMapData(char*** targetArray, char** copyArray, int numRow, int numCol);
void FreeMapData(char*** mapToFree, int numRow, int numCol);

void ShiftMap(char*** mapToShift, char** shiftIntoMap, int numRow, int numCol, int totalNumOfShifts, DIRECTION dir);

// Room transition
void RoomTransition(WorldViewScene* self, double Delta);

///****************************************************************************
// Function Definitions
///****************************************************************************
void WorldViewScene_Setup(WorldViewScene* self)
{
	// Set up the InternalStateManager
	self->InternalStateManager.Initiallize = WorldViewScene_LinkedInternalInitiallize;
	self->InternalStateManager.Update = WorldViewScene_LinkedInternalUpdate;
	self->InternalStateManager.Render = WorldViewScene_LinkedInternalRender;
	self->InternalStateManager.Exit = WorldViewScene_LinkedInternalExit;

	// Set the current state
	self->InternalState = WVS_ROAMING;

	// Set up the functions of this object
	self->Initiallize = WorldViewScene_LinkedInitiallize;
	self->Update = WorldViewScene_LinkedUpdate;
	self->Render = WorldViewScene_LinkedRender;
	self->Exit = WorldViewScene_LinkedExit;
}

// Linked Initiallize function that will be set to the struct's Initiallize
void WorldViewScene_LinkedInitiallize(WorldViewScene* self)
{
	self->InternalStateManager.Initiallize(self);
}

// Linked Update function that will be set to the struct's Update
void WorldViewScene_LinkedUpdate(WorldViewScene* self, double Delta)
{
	self->InternalStateManager.Update(self, Delta);
}

// Linked Render function that will be set to the struct's Render
void WorldViewScene_LinkedRender(WorldViewScene* self, Engine* Renderer)
{
	self->InternalStateManager.Render(self, Renderer);
}

// Linked Exit function that will be set to the struct's Exit
void WorldViewScene_LinkedExit(WorldViewScene* self)
{
	self->InternalStateManager.Exit(self);
}

// Linked Initiallize function that will be set to the InternalStateManager
void WorldViewScene_LinkedInternalInitiallize(WorldViewScene* self)
{
	// Here I will initiallize the internal state manager
	// Setup the loader that I am about to use.

	// Initializing variables
	self->currentRoom = NULL;
	self->wKeyPressed = 0;
	self->sKeyPressed = 0;
	self->aKeyPressed = 0;
	self->dKeyPressed = 0;

	moveDirection.x = moveDirection.y = 0;

	// Initializing room list and player

	InitRoomArray(&(self->roomList), 5);
	Vector2 startingPosition = { 5,5 };
	Initialize_Player(&self->player, startingPosition, &self->currentRoom);

	Add_Room(self, "Resources/Maps/TestMap.txt"); // Room0
	Add_Room(self, "Resources/Maps/TestMap2.txt"); // Room1
	Add_Room(self, "Resources/Maps/TestMap2.txt"); // Room2

	Room* room0 = self->roomList.array[0];
	Room* room1 = self->roomList.array[1];
	Room* room2 = self->roomList.array[2];
	room0->AddExit(room0, room1, NORTH);
	room0->AddExit(room0, room2, EAST);
	room0->AddExit(room0, room2, WEST);
	room1->AddExit(room1, room0, SOUTH);

	room2->AddExit(room2, room0, EAST);
}

// Linked Update function that will be set to the InternalStateManager
void WorldViewScene_LinkedInternalUpdate(WorldViewScene* self, double Delta)
{
	// Do some state logic for the internal state manager
	// Testing code state cycling
	switch (self->InternalState)
	{
	case WVS_ROAMING:
		PlayerControls(self, Delta);
		WrapPlayer(self);
		break;
	case WVS_TRANSITION:
		RoomTransition(self, Delta);
		break;
	}
}

// Linked Render function that will be set to the InternalStateManager
void WorldViewScene_LinkedInternalRender(WorldViewScene* self, Engine* Renderer)
{
	// Renders the appropriate scene
	switch (self->InternalState)
	{
	case WVS_ROAMING:	
		Renderer->g_console->map_WriteToBuffer(Renderer->g_console, self->currentRoom->mapToRender, self->currentRoom->Loader.NumberOfRows, self->currentRoom->Loader.NumberOfColumns, getColor(c_black, c_white));
		Renderer->g_console->text_WriteToBuffer(Renderer->g_console, self->player.position, "O", getColor(c_black, c_aqua));
		break;
	case WVS_TRANSITION:
		Renderer->g_console->map_WriteToBuffer(Renderer->g_console, transitionMap, transitionMapRows, transitionMapColumns, getColor(c_black, c_white));
		Renderer->g_console->text_WriteToBuffer(Renderer->g_console, fakePlayerPosition, "O", getColor(c_black, c_aqua));
		break;
	}
}

// Linked Exit function that will be set to the InternalStateManager
void WorldViewScene_LinkedInternalExit(WorldViewScene* self)
{
	// Free the stuff initiallized in the Internal State Manager
	FreeRoomArray(&(self->roomList));
}


void PlayerControls(WorldViewScene* self, double Delta)
{
	if (isKeyPressed('W'))
	{
		// Key press down
		if (self->wKeyPressed == 0)
		{
			self->wKeyPressed = 1;
			moveDirection.y = -1;
			MovePlayer(&self->player, moveDirection);
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
			MovePlayer(&self->player, moveDirection);
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
			MovePlayer(&self->player, moveDirection);
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
			MovePlayer(&self->player, moveDirection);
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
		if ((runTimerX += Delta) > initialRunDelay) // Initial delay before running
		{
			if (runTimerX > initialRunDelay + runDelayX) // 0.4 - 0.3 = 0.1, delay in between each "run step"
			{
				runTimerX = initialRunDelay;
				Vector2 tempDirection = moveDirection;
				tempDirection.y = 0;
				MovePlayer(&self->player, tempDirection);
			}
		}
	}
	else
	{
		runTimerX = 0;
		moveDirection.x = 0;
	}


	if (self->wKeyPressed == 1 || self->sKeyPressed == 1)
	{
		if ((runTimerY += Delta) > initialRunDelay) // Initial delay before running
		{
			if (runTimerY > initialRunDelay + runDelayY) // 0.4 - 0.3 = 0.1, delay in between each "run step"
			{
				runTimerY = initialRunDelay;
				Vector2 tempDirection = moveDirection;
				tempDirection.x = 0;
				MovePlayer(&self->player, tempDirection);
			}
		}
	}
	else
	{
		runTimerY = 0;
		moveDirection.y = 0;
	}

}

void WrapPlayer(WorldViewScene* self)
{
	if (self->player.dir != D_TOTAL)
	{
		if (self->currentRoom->exits[self->player.dir] != NULL)
		{
			CopyMapData(&transitionMap, self->currentRoom->mapToRender, self->currentRoom->Loader.NumberOfRows, self->currentRoom->Loader.NumberOfColumns);
			self->currentRoom = self->currentRoom->exits[self->player.dir];
			self->InternalState = WVS_TRANSITION;
			fakePlayerPosition = self->player.position;
			switch (self->player.dir)
			{
			case NORTH:
				self->player.position.y = self->currentRoom->Loader.NumberOfRows - 1;
				break;
			case SOUTH:
				self->player.position.y = 0;
				break;
			case EAST:
				self->player.position.x = 0;
				break;
			case WEST:
				self->player.position.x = self->currentRoom->Loader.NumberOfColumns - 2;
				break;
			}
		}
	}
}

void Add_Room(WorldViewScene* self, char *mapString)
{
	Room* roomToAdd = RoomFactory_CreateRoom();
	roomToAdd->LoadMap(roomToAdd, mapString);
	AddToRoomArray(&(self->roomList), roomToAdd);
	if (self->currentRoom == NULL)
	{
		self->currentRoom = roomToAdd;
	}
}

void CopyMapData(char*** targetArray, char** copyArray, int numRow, int numCol)
{
	transitionMapRows = numRow;
	transitionMapColumns = numCol;

	if (transitionMapColumns > 80)
	{
		transitionMapColumns = 80;
	}
	
	char **tempArray;
	tempArray = (char**)malloc(numRow * sizeof(char*));
	for (int i = 0; i < numRow; ++i)
	{
		tempArray[i] = (char*)malloc(transitionMapColumns * sizeof(char));
	}


	for (int x = 0; x < transitionMapColumns; ++x)
	{
		for (int y = 0; y < numRow; ++y)
		{
			tempArray[y][x] = copyArray[y][x];
		}
	}

	size_t size = strlen(*tempArray);
	*targetArray = tempArray;
}

void FreeMapData(char*** mapToFree, int numRow, int numCol)
{
	for (int i = 0; i < numRow; ++i)
	{
		free((*mapToFree)[i]);
	}
	free(*mapToFree);
	*mapToFree = NULL;
}

void RoomTransition(WorldViewScene* self, double Delta)
{
	roomTransitionTimer += Delta;
	if ((roomTransitionTimer > roomTransitionDelayY && (self->player.dir == NORTH || self->player.dir == SOUTH)) ||
		(roomTransitionTimer > roomTransitionDelayX && (self->player.dir == EAST || self->player.dir == WEST)))
	{
		roomTransitionTimer = 0;
		transitionCounter += 1;

		ShiftMap(&transitionMap, self->currentRoom->mapToRender, transitionMapRows, transitionMapColumns, transitionCounter, self->player.dir);

		if ((transitionCounter == transitionMapRows && (self->player.dir == NORTH || self->player.dir == SOUTH)) ||
			(transitionCounter == transitionMapColumns && (self->player.dir == EAST || self->player.dir == WEST)))
		{
			transitionCounter = 0;
			FreeMapData(&transitionMap, transitionMapRows, transitionMapColumns);
			self->player.dir = D_TOTAL;
			self->InternalState = WVS_ROAMING;
		}
	}
}

void ShiftMap(char*** mapToShift, char** shiftIntoMap, int numRow, int numCol, int totalNumOfShifts, DIRECTION dir)
{
	switch (dir)
	{
	case NORTH: // SHIFT DOWN
		fakePlayerPosition.y++;
		for (int y = numRow - 1; y > 0; --y)
		{
			for (int x = 0; x < numCol; ++x)
			{
				(*mapToShift)[y][x] = (*mapToShift)[y - 1][x];
			}
		}
		for (int i = 0; i < totalNumOfShifts; ++i)
		{
			for (int x = 0; x < numCol; ++x)
			{
				//(*mapToShift)[i][x] = ' ';
				(*mapToShift)[i][x] = shiftIntoMap[numRow - totalNumOfShifts + i][x];
			}
		}
		break;
	case SOUTH: // SHIFT UP
		fakePlayerPosition.y--;
		for (int y = 0; y < numRow - 1; ++y)
		{
			for (int x = 0; x < numCol; ++x)
			{
				(*mapToShift)[y][x] = (*mapToShift)[y + 1][x];
			}
		}
		for (int i = 0; i < totalNumOfShifts; ++i)
		{
			for (int x = 0; x < numCol; ++x)
			{
				//(*mapToShift)[i][x] = ' ';
				(*mapToShift)[numRow - totalNumOfShifts + i][x] = shiftIntoMap[i][x];
			}
		}
		break;
	case EAST: // SHIFT LEFT
		fakePlayerPosition.x--;
		for (int y = 0;y < numRow;++y)
		{
			for (int x = 0; x < numCol - 1; ++x)
			{
				(*mapToShift)[y][x] = (*mapToShift)[y][x + 1];
			}
		}
		for (int i = 0; i < totalNumOfShifts; ++i)
		{
			for (int y = 0; y < numRow; ++y)
			{
				//(*mapToShift)[y][numCol - totalNumOfShifts + i] = ' ';
				(*mapToShift)[y][numCol - totalNumOfShifts + i] = shiftIntoMap[y][i];
			}
		}
		break;
	case WEST: // SHIFT RIGHT
		fakePlayerPosition.x++;
		for (int y = 0; y < numRow; ++y)
		{
			for (int x = numCol - 1; x > 0; --x)
			{
				(*mapToShift)[y][x] = (*mapToShift)[y][x -1];
			}
		}
		for (int i = 0; i < totalNumOfShifts; ++i)
		{
			for (int y = 0; y < numRow; ++y)
			{
				//(*mapToShift)[y][numCol - totalNumOfShifts + i] = ' ';
				(*mapToShift)[y][totalNumOfShifts - 1 - i] = shiftIntoMap[y][numCol - 1 - i];
			}
		}
		break;
	}
}