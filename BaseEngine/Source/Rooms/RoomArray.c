/******************************************************************************
filename	RoomArray.c
author      Keith Cheng
DP email	keith.cheng@digipen.edu

Brief Description:
Room array for dynamic allocation of room

******************************************************************************/

#include "RoomArray.h"

void InitRoomArray(RoomArray *roomArray, size_t initialSize)
{
	roomArray->array = (Room**)malloc(initialSize * sizeof(Room*));
	roomArray->currentSize = 0;
	roomArray->maxSize = initialSize;
}

void AddToRoomArray(RoomArray *roomArray, Room* roomToAdd)
{
	if (roomArray->currentSize == roomArray->maxSize)
	{
		roomArray->maxSize *= 2;
		roomArray->array = (Room**)realloc(roomArray->array, roomArray->maxSize * sizeof(Room*));
	}
	roomArray->array[roomArray->currentSize] = roomToAdd;
	roomArray->currentSize++;
}

void FreeRoomArray(RoomArray *roomArray)
{
	free(roomArray->array);
	roomArray = NULL;
	roomArray->currentSize = 0;
	roomArray->maxSize = 0;
}
