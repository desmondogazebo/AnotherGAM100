/******************************************************************************
filename    Room.c
author      Keith Cheng
DP email    keith.cheng@digipen.edu

Brief Description:
Room class functons

******************************************************************************/

#include <stdlib.h>
#include "Room.h"

void Room_Init(Room* self)
{
	TextDataLoader_Setup(&self->Loader);
	self->exits[NORTH] = NULL;
	self->exits[SOUTH] = NULL;
	self->exits[EAST] = NULL;
	self->exits[WEST] = NULL;
}

void Room_Free(Room *self)
{
	self->Loader.Exit(&(self->Loader));
	free(self);
}

void Room_AddExit(Room* self, Room* exitRoom, DIRECTION dir)
{
	self->exits[dir] = exitRoom;
}

void Room_LoadMap(Room* self, char* mapString)
{
	self->Loader.LoadResource(&(self->Loader), mapString);
	self->mapToRender = self->Loader.TextData;
}