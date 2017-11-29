/******************************************************************************
filename    WorldViewScene.h
author      Keith Cheng
DP email    keith.cheng@digipen.edu

Created on 14 November 2017

Brief Description:
The main world view scene of the game. This is where the player roams around before entering dungeons.

******************************************************************************/

/* Header Guards */
#ifndef WORLDVIEW_SCENE_H
#define WORLDVIEW_SCENE_H

#include "../Rooms/Room.h"
#include "../Rooms/RoomArray.h"
#include "../Player/Player.h"

#include "../StateManager/BaseStateManager.h"

/* Public Struct & Functions */
typedef struct WorldViewScene
{
	/* Public Parameters */
	enum {
		WVS_ROAMING = 0,
		WVS_TRANSITION,
		WVS_DUNGEONTRANSITION,
		WVS_TOTAL
	}WVS_States;
	enum WVS_States InternalState;
	BaseStateManager InternalStateManager;

	RoomArray roomList;
	Room* currentRoom;

	Player player;

	short wKeyPressed;
	short sKeyPressed;
	short aKeyPressed;
	short dKeyPressed;

	/* Public Function Pointers*/
	/* Initiallize, requires an instance of itself */
	void(*Initiallize)();
	/* Updates, requires an instance of itself as well as delta time */
	void(*Update)();
	/* Renders, requires an instance of itself and a passed engine*, writes the data of the scene to the screen's buffer */
	void(*Render)();
	/* Exits, requires an instance, calls free if memory was allocated */
	void(*Exit)();

}WorldViewScene;

///****************************************************************************
// Public Function Prototypes
///****************************************************************************
/* Setup function that initiallizes the WorldViewScene */
void WorldViewScene_Setup(WorldViewScene* self);

#endif // WORLDVIEW_SCENE_H