/******************************************************************************
filename    RoomFactory.c
author      Keith Cheng
DP email    keith.cheng@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Brief Description:


******************************************************************************/

#include <stdlib.h> 

#include "RoomFactory.h"

Room* RoomFactory_CreateRoom()
{
	Room* room = malloc(sizeof(Room));
	room->Init = Room_Init;
	room->Free = Room_Free;
	room->AddExit = Room_AddExit;
	room->LoadMap = Room_LoadMap;
	room->mapToRender = NULL;

	room->Init(room);

	return room;
}

void RoomFactory_DeleteRoom(Room* room)
{
	room->Free(room);
}