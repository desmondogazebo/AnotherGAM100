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

#include "../Enemy/Enemy.h"
#include "../Enemy/EnemyDataLoader.h"

// Included for Rendering
#include "../Engine/BaseEngine.h"

///****************************************************************************
// Private Variables
///****************************************************************************
// Variables required for transitioning between screens
char** wvs_transitionMap;
int wvs_transitionMapRows;
int wvs_transitionMapColumns;
double wvs_roomTransitionTimer = 0;
int wvs_transitionCounter = 0;
Vector2 wvs_fakePlayerPosition; // A fake player used for cosmetic effect when transitioning

double wvs_roomTransitionDelayY = 0.06; // Change this to change how fast rooms transition on the Y axis
double wvs_roomTransitionDelayX = 0.02; // Same but X axis


// Timers for letting the player run
double wvs_initialRunDelay = 0.3; // How long it takes before the player starts running
double wvs_runDelayX = 0.1; // How fast the player runs on X axis, speed = 1/runDelayX
double wvs_runDelayY = 0.15; // How fast the player runs on Y axis
double wvs_runTimerX = 0;
double wvs_runTimerY = 0;

// Dungeon transition variables
short wvs_dungeonTransitionCount = 0;
double wvs_dungeonTransitionTimer = 0;
double wvs_dungeonTransitionTimerDelay = 0.05;
short wvs_dungeonWaitToggle = 0;
double wvs_dungeonTransitionTimerWaitDelay = 0.3;
TextDataLoader wvs_dungeonTransitionSprite;

// Battle transition variables
short wvs_battleTransitionCount;
double wvs_battleTransitionTimer;
double wvs_battleTransitionTimerDelay;
short wvs_battleWaitToggle;
double wvs_battleTransitionWaitDelay;
TextDataLoader wvs_battleTransitionSprite;

Vector2 wvs_moveDirection = { 0,0 }; // Direction of player movement

///****************************************************************************
// Private Function Prototypes
///****************************************************************************

// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void WorldViewScene_LinkedInitiallize(WorldViewScene* self);
// Linked Update function that will be set to the struct's Update
void WorldViewScene_LinkedUpdate(WorldViewScene* self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the struct's Render
void WorldViewScene_LinkedRender(WorldViewScene* self, Engine* BaseEngine);
// Linked Exit function that will be set to the struct's Exit
void WorldViewScene_LinkedExit(WorldViewScene* self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void WorldViewScene_LinkedInternalInitiallize(WorldViewScene* self);
// Linked Update function that will be set to the InternalStateManager.Update
void WorldViewScene_LinkedInternalUpdate(WorldViewScene* self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void WorldViewScene_LinkedInternalRender(WorldViewScene* self, Engine* BaseEngine);
// Linked Exit function that will be set to the InternalStateManager.Exit
void WorldViewScene_LinkedInternalExit(WorldViewScene* self);

// Split functions to make file neater
void PlayerControls(WorldViewScene* self, Engine* BaseEngine, double Delta);
void HandlePlayerMovement(int plrMoveCode, WorldViewScene* self, Engine* BaseEngine, short *MovementCheck);

void WrapPlayer(WorldViewScene* self);
Room* Add_Room(WorldViewScene* self, char *mapString);
void CopyMapData(char*** targetArray, char** copyArray, int numRow, int numCol);
void FreeMapData(char*** mapToFree, int numRow, int numCol);

void ShiftMap(char*** mapToShift, char** shiftIntoMap, int numRow, int numCol, int totalNumOfShifts, DIRECTION dir);

// Initializing the maps of the world
void InitializeWorldMaps(WorldViewScene* self);

// Room transition
void RoomTransition(WorldViewScene* self, double Delta);

// Dungeon transitions
void DungeonTransition(WorldViewScene* self, Engine* BaseEngine, double Delta);
void BattleTransition(WorldViewScene* self, Engine* BaseEngine, double Delta);

// Render the dungeon indicators in room 5 (center)
void RenderDungeonIndicators(WorldViewScene* self, Engine* BaseEngine, Vector2 offset);
void ReplaceDungeonEntrance(WorldViewScene* self, Engine* BaseEngine);
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
void WorldViewScene_LinkedUpdate(WorldViewScene* self, Engine* BaseEngine, double Delta)
{
	self->InternalStateManager.Update(self, BaseEngine, Delta);
}

// Linked Render function that will be set to the struct's Render
void WorldViewScene_LinkedRender(WorldViewScene* self, Engine* BaseEngine)
{
	self->InternalStateManager.Render(self, BaseEngine);
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

	self->InternalState = WVS_ROAMING;

	// Setup the loader that I am about to use.

	// Initializing variables
	self->currentRoom = NULL;
	self->wKeyPressed = 0;
	self->sKeyPressed = 0;
	self->aKeyPressed = 0;
	self->dKeyPressed = 0;

	wvs_moveDirection.x = wvs_moveDirection.y = 0;

	TextDataLoader_Setup(&wvs_dungeonTransitionSprite);
	wvs_dungeonTransitionSprite.LoadResource(&wvs_dungeonTransitionSprite, "Resources/DungeonTransition.txt");

	// Initializing room list and player

	InitRoomArray(&(self->roomList), 5);
	self->currentRoomIndex = 0;
	self->previousRoomIndex = 0;
	Initialize_Player(&self->player, Vec2(5, 5));

	InitializeWorldMaps(self);

	TextDataLoader_Setup(&wvs_battleTransitionSprite);
	wvs_battleTransitionSprite.LoadResource(&wvs_battleTransitionSprite, "Resources/BattleTransition.txt");

	wvs_battleTransitionCount = 0;
	wvs_battleTransitionTimer = 0;
	wvs_battleTransitionTimerDelay = 0.01;
	wvs_battleWaitToggle = 0;
	wvs_battleTransitionWaitDelay = 0.5;
}

// Linked Update function that will be set to the InternalStateManager
void WorldViewScene_LinkedInternalUpdate(WorldViewScene* self, Engine* BaseEngine, double Delta)
{
	// Debug code
	if (isKeyPressed('Q'))
	{
		Delta *= 4;
	}

	if (isKeyPressed('1'))
	{
		BaseEngine->playerData.bossFlag = 1;
	}
	if (isKeyPressed('2'))
	{
		BaseEngine->playerData.bossFlag = 2;
	}
	if (isKeyPressed('3'))
	{
		BaseEngine->playerData.bossFlag = 3;
	}
	if (isKeyPressed('4'))
	{
		BaseEngine->playerData.bossFlag = 4;
	}

	// Do some state logic for the internal state manager
	// Testing code state cycling
	switch (self->InternalState)
	{
	case WVS_ROAMING:
		PlayerControls(self, BaseEngine, Delta);
		WrapPlayer(self);
		break;
	case WVS_TRANSITION:
		RoomTransition(self, Delta);
		break;
	case WVS_DUNGEONTRANSITION:
		DungeonTransition(self, BaseEngine, Delta);
		break;
	case WVS_BATTLETRANSITION:
		BattleTransition(self, BaseEngine, Delta);
		break;
	}
}

// Linked Render function that will be set to the InternalStateManager
void WorldViewScene_LinkedInternalRender(WorldViewScene* self, Engine* BaseEngine)
{
	// Renders the appropriate scene
	switch (self->InternalState)
	{
	case WVS_ROAMING:	
		BaseEngine->g_console->map_WriteToBuffer(BaseEngine->g_console, self->currentRoom->mapToRender, self->currentRoom->Loader.NumberOfRows, self->currentRoom->Loader.NumberOfColumns, getColor(c_black, c_white));
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, self->player.position, "O", getColor(c_black, c_aqua));
		if (self->currentRoomIndex == 4)
		{
			RenderDungeonIndicators(self, BaseEngine, Vec2(0,0));
		}
		ReplaceDungeonEntrance(self, BaseEngine);
		BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '&', 5, getColor(c_black, c_green));
		break;
	case WVS_TRANSITION:
		BaseEngine->g_console->map_WriteToBuffer(BaseEngine->g_console, wvs_transitionMap, wvs_transitionMapRows, wvs_transitionMapColumns, getColor(c_black, c_white));
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, wvs_fakePlayerPosition, "O", getColor(c_black, c_aqua));

		if (self->currentRoomIndex == 4)
		{
			Vector2 offset = { 0,0 };
			if (self->player.dir == NORTH)
			{				
				offset.y = -BaseEngine->g_console->consoleSize.Y + wvs_transitionCounter;
			}
			else if (self->player.dir == SOUTH)
			{
				offset.y = BaseEngine->g_console->consoleSize.Y - wvs_transitionCounter;
			}
			else if (self->player.dir == EAST)
			{
				offset.x = BaseEngine->g_console->consoleSize.X - wvs_transitionCounter;
			}
			else if (self->player.dir == WEST)
			{
				offset.x = -BaseEngine->g_console->consoleSize.X + wvs_transitionCounter;
			}
			RenderDungeonIndicators(self, BaseEngine, offset);		
		}
		else if (self->previousRoomIndex == 4)
		{
			Vector2 offset = { 0,0 };
			if (self->player.dir == NORTH)
			{
				offset.y = wvs_transitionCounter;
			}
			else if (self->player.dir == SOUTH)
			{
				offset.y = -wvs_transitionCounter;
			}
			else if (self->player.dir == EAST)
			{
				offset.x = -wvs_transitionCounter;
			}
			else if (self->player.dir == WEST)
			{
				offset.x = wvs_transitionCounter;
			}
			RenderDungeonIndicators(self, BaseEngine, offset);
		}

		ReplaceDungeonEntrance(self, BaseEngine);
		BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '&', 5, getColor(c_black, c_green));
		break;
	case WVS_DUNGEONTRANSITION:
		{
			Vector2 location = { 0, wvs_dungeonTransitionSprite.NumberOfRows - wvs_dungeonTransitionCount };
			BaseEngine->g_console->map_WriteToBuffer(BaseEngine->g_console, self->currentRoom->mapToRender, self->currentRoom->Loader.NumberOfRows, self->currentRoom->Loader.NumberOfColumns, getColor(c_black, c_white));
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, self->player.position, "O", getColor(c_black, c_aqua));
			BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, location, wvs_dungeonTransitionSprite.TextData, wvs_dungeonTransitionSprite.NumberOfRows, wvs_dungeonTransitionSprite.NumberOfColumns, getColor(c_black, c_white));
		}
		ReplaceDungeonEntrance(self, BaseEngine);
		BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '&', 5, getColor(c_black, c_green));
		break;
	case WVS_BATTLETRANSITION:
		{
			Vector2 location = { -BaseEngine->g_console->consoleSize.X + wvs_battleTransitionCount, 0 };
			BaseEngine->g_console->map_WriteToBuffer(BaseEngine->g_console, self->currentRoom->mapToRender, self->currentRoom->Loader.NumberOfRows, self->currentRoom->Loader.NumberOfColumns, getColor(c_black, c_white));
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, self->player.position, "O", getColor(c_black, c_aqua));
			BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, location, wvs_battleTransitionSprite.TextData, wvs_battleTransitionSprite.NumberOfRows, wvs_battleTransitionSprite.NumberOfColumns, getColor(c_black, c_white));
		}
		ReplaceDungeonEntrance(self, BaseEngine);
		BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '&', 5, getColor(c_black, c_green));
		break;
		}
		
}

// Linked Exit function that will be set to the InternalStateManager
void WorldViewScene_LinkedInternalExit(WorldViewScene* self)
{
	// Free the stuff initiallized in the Internal State Manager
	wvs_dungeonTransitionSprite.Exit(&wvs_dungeonTransitionSprite);
	wvs_battleTransitionSprite.Exit(&wvs_battleTransitionSprite);
	FreeRoomArray(&(self->roomList));
}


void PlayerControls(WorldViewScene* self, Engine* BaseEngine, double Delta)
{
	short MovementCheck = 0;

	if (isKeyPressed('W'))
	{
		// Key press down
		if (self->wKeyPressed == 0)
		{
			self->wKeyPressed = 1;
			wvs_moveDirection.y--;
			short plrMoveCode = MovePlayer(&self->player, Vec2(0, wvs_moveDirection.y), self->currentRoom->Loader);
			HandlePlayerMovement(plrMoveCode, self, BaseEngine, &MovementCheck);
		}
	}
	else
	{
		// Key press up
		if (self->wKeyPressed == 1)
		{
			self->wKeyPressed = 0;
			wvs_moveDirection.y++;
		}
	}

	if (isKeyPressed('S'))
	{
		// Key press down
		if (self->sKeyPressed == 0)
		{
			self->sKeyPressed = 1;
			wvs_moveDirection.y++;
			short plrMoveCode = MovePlayer(&self->player, Vec2(0, wvs_moveDirection.y), self->currentRoom->Loader);
			HandlePlayerMovement(plrMoveCode, self, BaseEngine, &MovementCheck);
		}
	}
	else
	{
		// Key press up
		if (self->sKeyPressed == 1)
		{
			self->sKeyPressed = 0;
			wvs_moveDirection.y--;
		}
	}

	if (isKeyPressed('A'))
	{
		// Key press down
		if (self->aKeyPressed == 0)
		{
			self->aKeyPressed = 1;
			wvs_moveDirection.x--;
			short plrMoveCode = MovePlayer(&self->player, Vec2(wvs_moveDirection.x, 0), self->currentRoom->Loader);
			HandlePlayerMovement(plrMoveCode, self, BaseEngine, &MovementCheck);
		}
	}
	else
	{
		// Key press up
		if (self->aKeyPressed == 1)
		{
			self->aKeyPressed = 0;
			wvs_moveDirection.x++;
		}
	}

	if (isKeyPressed('D'))
	{
		// Key press down
		if (self->dKeyPressed == 0)
		{
			self->dKeyPressed = 1;
			wvs_moveDirection.x++;
			short plrMoveCode = MovePlayer(&self->player, Vec2(wvs_moveDirection.x, 0), self->currentRoom->Loader);
			HandlePlayerMovement(plrMoveCode, self, BaseEngine, &MovementCheck);
		}
	}
	else
	{
		// Key press up
		if (self->dKeyPressed == 1)
		{
			self->dKeyPressed = 0;
			wvs_moveDirection.x--;
		}
	}

	if (self->aKeyPressed == 1 || self->dKeyPressed == 1 || self->wKeyPressed == 1 || self->sKeyPressed == 1)
	{
		if ((wvs_runTimerX += Delta) > wvs_initialRunDelay) // Initial delay before running
		{
			if (wvs_runTimerX > wvs_initialRunDelay + wvs_runDelayX) // 0.4 - 0.3 = 0.1, delay in between each "run step"
			{
				wvs_runTimerX = wvs_initialRunDelay;

				short plrMoveCode = MovePlayer(&self->player, Vec2(wvs_moveDirection.x, 0), self->currentRoom->Loader);
				short plrMoveCode2 = MovePlayer(&self->player, Vec2(0, wvs_moveDirection.y), self->currentRoom->Loader);
				HandlePlayerMovement(plrMoveCode, self, BaseEngine, &MovementCheck);
				HandlePlayerMovement(plrMoveCode2, self, BaseEngine, &MovementCheck);
			}
		}
	}
	else
	{
		wvs_runTimerX = 0;
		wvs_moveDirection.x = 0;
	}

	if (MovementCheck == 1)
	{
		// Check if a monster has been encountered
		if (EnemyEncounterHandler_RandomizeEncounter(&BaseEngine->InternalSceneSystem.InternalEncounterHandler, 2, Enemy_Bird, Enemy_Rat) == 1)
		{
			// Do something
			self->InternalState = WVS_BATTLETRANSITION;
			BaseEngine->InternalSceneSystem.InternalEncounterHandler.PreviousSceneWasDungeon = 0;
			BaseEngine->InternalSceneSystem.InternalBattleScene.Exit(&BaseEngine->InternalSceneSystem.InternalBattleScene);
			BaseEngine->InternalSceneSystem.InternalBattleScene.Initiallize(&BaseEngine->InternalSceneSystem.InternalBattleScene);
		}
	}
}

void WrapPlayer(WorldViewScene* self)
{
	if (self->player.dir != D_TOTAL)
	{
		if (self->currentRoom->exits[self->player.dir] != NULL)
		{
			CopyMapData(&wvs_transitionMap, self->currentRoom->mapToRender, self->currentRoom->Loader.NumberOfRows, self->currentRoom->Loader.NumberOfColumns);
			self->currentRoom = self->currentRoom->exits[self->player.dir];
			self->InternalState = WVS_TRANSITION;
			wvs_fakePlayerPosition = self->player.position;
			switch (self->player.dir)
			{
			case NORTH:
				self->player.position.y = self->currentRoom->Loader.NumberOfRows - 1;
				self->currentRoomIndex += 3;
				break;
			case SOUTH:
				self->player.position.y = 0;
				self->currentRoomIndex -= 3;
				break;
			case EAST:
				self->player.position.x = 0;
				self->currentRoomIndex += 1;
				break;
			case WEST:
				self->player.position.x = self->currentRoom->Loader.NumberOfColumns - 2;
				self->currentRoomIndex -= 1;
				break;
			}
		}
	}
}

Room* Add_Room(WorldViewScene* self, char *mapString)
{
	Room* roomToAdd = RoomFactory_CreateRoom();
	roomToAdd->LoadMap(roomToAdd, mapString);
	AddToRoomArray(&(self->roomList), roomToAdd);
	if (self->currentRoom == NULL)
	{
		self->currentRoom = roomToAdd;
	}
	return roomToAdd;
}

void CopyMapData(char*** targetArray, char** copyArray, int numRow, int numCol)
{
	wvs_transitionMapRows = numRow;
	wvs_transitionMapColumns = numCol;

	if (wvs_transitionMapColumns > 80)
	{
		wvs_transitionMapColumns = 80;
	}
	
	char **tempArray;
	tempArray = (char**)malloc(numRow * sizeof(char*));
	for (int i = 0; i < numRow; ++i)
	{
		tempArray[i] = (char*)malloc(wvs_transitionMapColumns * sizeof(char));
	}


	for (int x = 0; x < wvs_transitionMapColumns; ++x)
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
	wvs_roomTransitionTimer += Delta;
	if ((wvs_roomTransitionTimer > wvs_roomTransitionDelayY && (self->player.dir == NORTH || self->player.dir == SOUTH)) ||
		(wvs_roomTransitionTimer > wvs_roomTransitionDelayX && (self->player.dir == EAST || self->player.dir == WEST)))
	{
		wvs_roomTransitionTimer = 0;
		wvs_transitionCounter += 1;

		ShiftMap(&wvs_transitionMap, self->currentRoom->mapToRender, wvs_transitionMapRows, wvs_transitionMapColumns, wvs_transitionCounter, self->player.dir);

		if ((wvs_transitionCounter == wvs_transitionMapRows && (self->player.dir == NORTH || self->player.dir == SOUTH)) ||
			(wvs_transitionCounter == wvs_transitionMapColumns && (self->player.dir == EAST || self->player.dir == WEST)))
		{
			wvs_transitionCounter = 0;
			FreeMapData(&wvs_transitionMap, wvs_transitionMapRows, wvs_transitionMapColumns);
			self->player.dir = D_TOTAL;
			self->InternalState = WVS_ROAMING;
			self->previousRoomIndex = self->currentRoomIndex;
		}
	}
}

void ShiftMap(char*** mapToShift, char** shiftIntoMap, int numRow, int numCol, int totalNumOfShifts, DIRECTION dir)
{
	switch (dir)
	{
	case NORTH: // SHIFT DOWN
		wvs_fakePlayerPosition.y++;
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
		wvs_fakePlayerPosition.y--;
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
		wvs_fakePlayerPosition.x--;
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
		wvs_fakePlayerPosition.x++;
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

void InitializeWorldMaps(WorldViewScene* self)
{
	// Initialization of maps
	Room* map1 = Add_Room(self, "Resources/Maps/WorldMap1.txt"); // Map1
	Room* map2 = Add_Room(self, "Resources/Maps/WorldMap2.txt"); // Map2
	Room* map3 = Add_Room(self, "Resources/Maps/WorldMap3.txt"); // Map3
	Room* map4 = Add_Room(self, "Resources/Maps/WorldMap4.txt"); // Map4
	Room* map5 = Add_Room(self, "Resources/Maps/WorldMap5.txt"); // Map5
	Room* map6 = Add_Room(self, "Resources/Maps/WorldMap6.txt"); // Map6
	Room* map7 = Add_Room(self, "Resources/Maps/WorldMap7.txt"); // Map7
	Room* map8 = Add_Room(self, "Resources/Maps/WorldMap8.txt"); // Map8
	Room* map9 = Add_Room(self, "Resources/Maps/WorldMap9.txt"); // Map9

	// MAP LAYOUT IS AS SUCH
	// 7 // 8 // 9
	// 4 // 5 // 6
	// 1 // 2 // 3

	// Adding exits to rooms
	map1->AddExit(map1, map2, EAST);
	map1->AddExit(map1, map4, NORTH);

	map2->AddExit(map2, map1, WEST);
	map2->AddExit(map2, map5, NORTH);
	map2->AddExit(map2, map3, EAST);

	map3->AddExit(map3, map2, WEST);
	map3->AddExit(map3, map6, NORTH);

	map4->AddExit(map4, map7, NORTH);
	map4->AddExit(map4, map5, EAST);
	map4->AddExit(map4, map1, SOUTH);

	map5->AddExit(map5, map8, NORTH);
	map5->AddExit(map5, map6, EAST);
	map5->AddExit(map5, map2, SOUTH);
	map5->AddExit(map5, map4, WEST);

	map6->AddExit(map6, map9, NORTH);
	map6->AddExit(map6, map5, WEST);
	map6->AddExit(map6, map3, SOUTH);

	map7->AddExit(map7, map8, EAST);
	map7->AddExit(map7, map4, SOUTH);

	map8->AddExit(map8, map7, WEST);
	map8->AddExit(map8, map5, SOUTH);
	map8->AddExit(map8, map9, EAST);

	map9->AddExit(map9, map8, WEST);
	map9->AddExit(map9, map6, SOUTH);
}

void DungeonTransition(WorldViewScene* self, Engine* BaseEngine, double Delta)
{
	wvs_dungeonTransitionTimer += Delta;

	if (wvs_dungeonWaitToggle == 0)
	{
		if (wvs_dungeonTransitionTimer > wvs_dungeonTransitionTimerDelay)
		{
			wvs_dungeonTransitionCount++;
			wvs_dungeonTransitionTimer = 0;
			if (wvs_dungeonTransitionCount == self->currentRoom->Loader.NumberOfRows)
			{
				wvs_dungeonWaitToggle = 1;
			}
		}
	}
	else if (wvs_dungeonWaitToggle == 1)
	{
		if (wvs_dungeonTransitionTimer > wvs_dungeonTransitionTimerWaitDelay)
		{
			wvs_dungeonTransitionCount = 0;
			wvs_dungeonTransitionTimer = 0;
			wvs_dungeonWaitToggle = 0;
			BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_Dungeon);
			self->InternalState = WVS_ROAMING;
			BaseEngine->InternalSceneSystem.InternalDungeonScene.Exit(&BaseEngine->InternalSceneSystem.InternalDungeonScene);
			BaseEngine->InternalSceneSystem.InternalDungeonScene.Initiallize(&BaseEngine->InternalSceneSystem.InternalDungeonScene);
		}
	}


}

void RenderDungeonIndicators(WorldViewScene* self, Engine* BaseEngine, Vector2 offset)
{
	if (BaseEngine->playerData.bossFlag == 0)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(35 + offset.x, 15 + offset.y), "^", getColor(c_black, c_aqua));
	else if (BaseEngine->playerData.bossFlag > 0)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(35 + offset.x, 15 + offset.y), "^", getColor(c_black, c_lime));

	if (BaseEngine->playerData.bossFlag < 1)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(35 + offset.x, 10 + offset.y), "^", getColor(c_black, c_red));
	else if (BaseEngine->playerData.bossFlag == 1)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(35 + offset.x, 10 + offset.y), "^", getColor(c_black, c_aqua));
	else if (BaseEngine->playerData.bossFlag > 1)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(35 + offset.x, 10 + offset.y), "^", getColor(c_black, c_lime));

	if (BaseEngine->playerData.bossFlag < 2)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(45 + offset.x, 10 + offset.y), "^", getColor(c_black, c_red));
	else if (BaseEngine->playerData.bossFlag == 2)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(45 + offset.x, 10 + offset.y), "^", getColor(c_black, c_aqua));
	else if (BaseEngine->playerData.bossFlag > 2)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(45 + offset.x, 10 + offset.y), "^", getColor(c_black, c_lime));

	if (BaseEngine->playerData.bossFlag < 3)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(45 + offset.x, 15 + offset.y), "^", getColor(c_black, c_red));
	else if (BaseEngine->playerData.bossFlag == 3)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(45 + offset.x, 15 + offset.y), "^", getColor(c_black, c_aqua));
	else if (BaseEngine->playerData.bossFlag > 3)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(45 + offset.x, 15 + offset.y), "^", getColor(c_black, c_lime));
}

void ReplaceDungeonEntrance(WorldViewScene* self, Engine* BaseEngine)
{
	if ((self->currentRoomIndex == 6 || self->previousRoomIndex == 6) && BaseEngine->playerData.bossFlag < 1)
	{
		BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '@', '@', getColor(c_black, c_red));
	}
	if ((self->currentRoomIndex == 8 || self->previousRoomIndex == 8) && BaseEngine->playerData.bossFlag < 2)
	{
		BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '@', '@', getColor(c_black, c_red));
	}
	if ((self->currentRoomIndex == 2 || self->previousRoomIndex == 2) && BaseEngine->playerData.bossFlag < 3)
	{
		BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '@', '@', getColor(c_black, c_red));
	}
	if ((self->currentRoomIndex == 4 || self->previousRoomIndex == 4) && BaseEngine->playerData.bossFlag < 4)
	{
		BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '@', ' ', getColor(c_black, c_black));
	}
}

void BattleTransition(WorldViewScene* self, Engine* BaseEngine, double Delta)
{
	wvs_battleTransitionTimer += Delta;

	if (wvs_battleWaitToggle == 0)
	{
		if (wvs_battleTransitionTimer > wvs_battleTransitionTimerDelay)
		{
			wvs_battleTransitionCount++;
			wvs_battleTransitionTimer = 0;
			if (wvs_battleTransitionCount == 80)
			{
				wvs_battleWaitToggle = 1;
			}
		}
	}
	else if (wvs_battleWaitToggle == 1)
	{
		if (wvs_battleTransitionTimer > wvs_battleTransitionWaitDelay)
		{
			wvs_battleTransitionCount = 0;
			wvs_battleTransitionTimer = 0;
			wvs_battleWaitToggle = 0;
			self->InternalState = WVS_ROAMING;
			BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_Battle);
		}
	}
}

void HandlePlayerMovement(int plrMoveCode, WorldViewScene* self, Engine* BaseEngine , short *MovementCheck)
{
	if (plrMoveCode == 1)
	{
		if (self->currentRoomIndex == 6 && BaseEngine->playerData.bossFlag >= 1)
			self->InternalState = WVS_DUNGEONTRANSITION;
		else if (self->currentRoomIndex == 8 && BaseEngine->playerData.bossFlag >= 2)
			self->InternalState = WVS_DUNGEONTRANSITION;
		else if (self->currentRoomIndex == 2 && BaseEngine->playerData.bossFlag >= 3)
			self->InternalState = WVS_DUNGEONTRANSITION;
		else if (self->currentRoomIndex == 4 && BaseEngine->playerData.bossFlag >= 4)
			self->InternalState = WVS_DUNGEONTRANSITION;
		else if (self->currentRoomIndex == 0)
			self->InternalState = WVS_DUNGEONTRANSITION;
	}
	else if (plrMoveCode == 3)
		(*MovementCheck) = 1;
}