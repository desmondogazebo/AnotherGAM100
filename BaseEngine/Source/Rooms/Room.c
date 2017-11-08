/******************************************************************************
filename    Room.c
author      Keith Cheng
DP email    keith.cheng@digipen.edu

Brief Description:
Room class functons

******************************************************************************/

#include <stdlib.h>
#include "Room.h"

void Room_Free(Room *self)
{
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