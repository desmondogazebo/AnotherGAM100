/******************************************************************************
filename	Player.h
author      Keith Cheng
DP email	keith.cheng@digipen.edu

Brief Description:
Player class to store player variables

******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "../Utilities/Vector2.h"
#include "../Rooms/Room.h"

typedef struct Player
{
	Vector2 position;
	Room** currentRoom;
	DIRECTION dir;
}Player;

void Initialize_Player(Player* player, Vector2 position, Room** currentRoom);

short MovePlayer(Player *player, Vector2 move);

#endif