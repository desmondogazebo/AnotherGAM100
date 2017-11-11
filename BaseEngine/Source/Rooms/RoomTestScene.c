/******************************************************************************
filename    RoomTestScene.c
author      Keith Cheng
DP email    keith.cheng@digipen.edu

Brief Description:
Scene for testing rooms

******************************************************************************/

#include "RoomTestScene.h"
#include "RoomFactory.h"
#include "../Utilities/Utilities.h"

#include <stdlib.h>

RoomTestScene Create_RoomTestScene()
{
	RoomTestScene roomTestScene;
	roomTestScene.currentRoom = NULL;
	roomTestScene.Init = Initialize_RoomTestScene;
	roomTestScene.Update = Update_RoomTestScene;
	roomTestScene.Exit = Exit_RoomTestScene;
	roomTestScene.AddRoom = Add_Room;
	roomTestScene.wKeyPressed = 0;
	roomTestScene.sKeyPressed = 0;
	roomTestScene.aKeyPressed = 0;
	roomTestScene.dKeyPressed = 0;
	return roomTestScene;
}

void Initialize_RoomTestScene(RoomTestScene* self)
{
	InitRoomArray(&(self->roomList), 5);
	self->player.position.x = 5;
	self->player.position.y = 5;
}

void Update_RoomTestScene(RoomTestScene* self, double dt)
{
	if (isKeyPressed('W'))
	{
		// Key press down
		if (self->wKeyPressed == 0)
		{
			self->wKeyPressed = 1;
			self->player.position.y--;
		}	
	}
	else
	{
		// Key press up
		if (self->wKeyPressed == 1)
		{
			self->wKeyPressed = 0;
		}
	}

	if (isKeyPressed('S'))
	{
		// Key press down
		if (self->sKeyPressed == 0)
		{
			self->sKeyPressed = 1;
			self->player.position.y++;
		}
	}
	else
	{
		// Key press up
		if (self->sKeyPressed == 1)
		{
			self->sKeyPressed = 0;
		}
	}

	if (isKeyPressed('A'))
	{
		// Key press down
		if (self->aKeyPressed == 0)
		{
			self->aKeyPressed = 1;
			self->player.position.x--;
		}
	}
	else
	{
		// Key press up
		if (self->aKeyPressed == 1)
		{
			self->aKeyPressed = 0;
		}
	}

	if (isKeyPressed('D'))
	{
		// Key press down
		if (self->dKeyPressed == 0)
		{
			self->dKeyPressed = 1;
			self->player.position.x++;
		}
	}
	else
	{
		// Key press up
		if (self->dKeyPressed == 1)
		{
			self->dKeyPressed = 0;
		}
	}
}

void Exit_RoomTestScene(RoomTestScene* self)
{
	FreeRoomArray(&(self->roomList));
}

void Add_Room(RoomTestScene* self, char *mapString)
{
	Room* roomToAdd = RoomFactory_CreateRoom();
	roomToAdd->LoadMap(roomToAdd, mapString);
	AddToRoomArray(&(self->roomList), roomToAdd);
	if (self->currentRoom == NULL)
	{
		self->currentRoom = roomToAdd;
	}
}