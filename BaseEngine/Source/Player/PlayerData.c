/******************************************************************************
filename	PlayerData.h
author      Keith Cheng
DP email	keith.cheng@digipen.edu

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

	while (player->exp >= (player->lvl * 50))
	{
		player->exp -= (player->lvl * 50);
		player->lvl++;
		levelUp = 1;
	}

	return levelUp;
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
	int spd = player->lvl * 1;
	return spd;
}