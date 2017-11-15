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
	Vector2 startingPosition;
	startingPosition.x = startingPosition.y = 5;
	Initialize_Player(&self->player, startingPosition, &self->currentRoom);

	self->AddRoom(self, "Resources/Maps/TestMap.txt"); // Room0
	self->AddRoom(self, "Resources/Maps/TestMap2.txt"); // Room1

	Room* room0 = self->roomList.array[0];
	Room* room1 = self->roomList.array[1];
	room0->AddExit(room0, room1, NORTH);
	room1->AddExit(room1, room0, SOUTH);
}

void Update_RoomTestScene(RoomTestScene* self, double dt)
{
	PlayerControls(self);
	if (self->player.dir != D_TOTAL)
	{
		if (self->currentRoom->exits[self->player.dir] != NULL)
		{
			self->currentRoom = self->currentRoom->exits[self->player.dir];

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
				self->player.position.x = self->currentRoom->Loader.NumberOfColumns - 1;
				break;
			}

		}
		self->player.dir = D_TOTAL;
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

void PlayerControls(RoomTestScene* self)
{
	if (isKeyPressed('W'))
	{
		// Key press down
		if (self->wKeyPressed == 0)
		{
			self->wKeyPressed = 1;
			Vector2 moveDirection;
			moveDirection.x = 0;
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
		}
	}

	if (isKeyPressed('S'))
	{
		// Key press down
		if (self->sKeyPressed == 0)
		{
			self->sKeyPressed = 1;
			Vector2 moveDirection;
			moveDirection.x = 0;
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
		}
	}

	if (isKeyPressed('A'))
	{
		// Key press down
		if (self->aKeyPressed == 0)
		{
			self->aKeyPressed = 1;
			Vector2 moveDirection;
			moveDirection.x = -1;
			moveDirection.y = 0;
			MovePlayer(&self->player, moveDirection);
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
			Vector2 moveDirection;
			moveDirection.x = 1;
			moveDirection.y = 0;
			MovePlayer(&self->player, moveDirection);
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