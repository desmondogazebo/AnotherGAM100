/******************************************************************************
filename    RoomFactory.c
author      Keith Cheng
DP email    keith.cheng@digipen.edu

Brief Description:


******************************************************************************/

#include <stdlib.h> 

#include "RoomFactory.h"

Room* RoomFactory_CreateRoom()
{
	Room* room = malloc(sizeof(Room*));
	room->Free = Room_Free;
	room->AddExit = Room_AddExit;
	room->mapToRender = NULL;
	
	// Debug code
	//printf("Room created");

	return room;
}

void RoomFactory_DeleteRoom(Room* room)
{
	room->Free(room);
	
	//Debug code
	printf("Room deleted");
}