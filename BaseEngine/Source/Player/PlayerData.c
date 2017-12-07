/******************************************************************************
filename	PlayerData.c
author      Keith Cheng & Rui An Ryan Lim
DP email	keith.cheng@digipen.edu & l.ruianryan@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Brief Description:
PlayerData class used to store playerdata variables as well as various functions
to get player stats

******************************************************************************/

#include "PlayerData.h"

void Setup_PlayerData(PlayerData* player)
{
	player->lvl = 1;
	player->exp = 0;
	player->bossFlag = 0;
}

short Gain_PlayerExp(PlayerData* player, int exp)
{
	player->exp += exp;

	short levelUp = 0;

	while (player->exp >= Get_PlayerRequiredEXPForLevel(player))
	{
		player->exp -= Get_PlayerRequiredEXPForLevel(player);
		player->lvl++;
		levelUp = 1;
	}

	return levelUp;
}

// Amount of total exp to next level
short Get_PlayerRequiredEXPForLevel(PlayerData* player)
{
	return player->lvl * 25;
}

// Amount of remaining exp to next level
short Get_PlayerRemainingEXPForLevel(PlayerData* player)
{
	return Get_PlayerRequiredEXPForLevel(player) - player->exp;
}

int Get_PlayerATK(PlayerData* player)
{
	int atk = player->lvl * 2;
	return atk;
}
int Get_PlayerHP(PlayerData* player)
{
	int hp = player->lvl * 5;
	return hp;
}

int Get_PlayerSPD(PlayerData* player)
{
	int spd = player->lvl * 2;
	return spd;
}