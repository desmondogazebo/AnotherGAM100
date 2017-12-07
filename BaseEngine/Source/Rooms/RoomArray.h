/******************************************************************************
filename	RoomArray.h
author      	Keith Cheng
DP email	keith.cheng@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Brief Description:
Room array for dynamic allocation of room

******************************************************************************/

#ifndef ROOMARRAY_H
#define ROOMARRAY_H

#include "Room.h"
#include <stdlib.h>

typedef struct {
	Room **array;
	size_t currentSize;
	size_t maxSize;
} RoomArray;

void InitRoomArray(RoomArray *roomArray, size_t initialSize);

void AddToRoomArray(RoomArray *roomArray, Room* roomToAdd);

void FreeRoomArray(RoomArray *roomArray);

#endif
