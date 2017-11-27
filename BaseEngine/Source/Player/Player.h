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
#include "../Utilities/TextDataLoader.h"

typedef struct Player
{
	Vector2 position;
	DIRECTION dir;
}Player;

void Initialize_Player(Player* player, Vector2 position);

short MovePlayer(Player *player, Vector2 move, TextDataLoader Loader);

#endif