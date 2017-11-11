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
}

void Room_Free(Room *self)
{
	self->Loader.Exit(&(self->Loader));
	free(self);
}

void Room_AddExit(Room* self, Room* exitRoom, char direction)
{
	switch (direction)
	{
	case 'n':
		self->exits[0] = exitRoom;
		break;
	case 'e':
		self->exits[1] = exitRoom;
		break;
	case 's':
		self->exits[2] = exitRoom;
		break;
	case 'w':
		self->exits[3] = exitRoom;
		break;
	}
}

void Room_LoadMap(Room* self, char* mapString)
{
	self->Loader.LoadResource(&(self->Loader), mapString);
	self->mapToRender = self->Loader.TextData;
}