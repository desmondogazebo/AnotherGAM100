/******************************************************************************
filename	PlayerData.h
author      Keith Cheng
DP email	keith.cheng@digipen.edu

Brief Description:
PlayerData class used to store playerdata variables as well as various functions
to get player stats

******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

typedef struct PlayerData
{
	int lvl;
	int exp;
}PlayerData;

void Setup_PlayerData(PlayerData* player);

short Gain_PlayerExp(PlayerData* player, int exp);

int Get_PlayerATK(PlayerData* player);
int Get_PlayerHP(PlayerData* player);
int Get_PlayerSPD(PlayerData* player);


#endif