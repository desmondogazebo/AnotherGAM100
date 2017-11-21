/******************************************************************************
filename	Player.c
author      Keith Cheng
DP email	keith.cheng@digipen.edu

Brief Description:
Player class to store player variables

******************************************************************************/

#include "Player.h"

void Initialize_Player(Player* player, Vector2 position, Room** currentRoom)
{
	player->position = position;
	player->currentRoom = currentRoom;
	player->dir = D_TOTAL;
}

short MovePlayer(Player *player, Vector2 move)
{
	Vector2 finalPosition;
	finalPosition.x = player->position.x + move.x;
	finalPosition.y = player->position.y + move.y;

	if (player->currentRoom != NULL)
	{
		if (finalPosition.x < (*player->currentRoom)->Loader.NumberOfColumns - 1 && finalPosition.x >= 0 &&
			finalPosition.y < (*player->currentRoom)->Loader.NumberOfRows && finalPosition.y >= 0)
		{
			if ((*player->currentRoom)->mapToRender[finalPosition.y][finalPosition.x] != '#')
			{
				player->position = finalPosition;
				return 1;
			}
		}
		else
		{
			if (move.x > 0)
			{
				player->dir = EAST;
			}
			else if (move.x < 0)
			{
				player->dir = WEST;
			}
			else if (move.y < 0)
			{
				player->dir = NORTH;
			}
			else if (move.y > 0)
			{
				player->dir = SOUTH;
			}
		}
	}
	return 0;
}