/******************************************************************************
filename	Room.h
author      Keith Cheng
DP email	keith.cheng@digipen.edu

Brief Description:
A base class for rooms

******************************************************************************/

#ifndef ROOM_H
#define ROOM_H

typedef struct Room Room;

struct Room
{
	char **mapToRender; // a 2D char array
	Room *exits[4]; // Pointers to other rooms (Edges of map)


	void(*Free)();
	void(*AddExit)();
};

void Room_Free(Room* self);

void Room_AddExit(Room* self, Room* exitRoom, char direction);

#endif