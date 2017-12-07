/******************************************************************************
filename	PlayerData.h
author      	Keith Cheng
DP email	keith.cheng@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Brief Description:
PlayerData class used to store playerdata variables as well as various functions
to get player stats

******************************************************************************/

#ifndef PLAYERDATA_H
#define PLAYERDATA_H

typedef struct PlayerData
{
	int lvl;
	int exp;
	int bossFlag;
}PlayerData;

void Setup_PlayerData(PlayerData* player);

short Gain_PlayerExp(PlayerData* player, int exp);

// Amount of total exp to next level
short Get_PlayerRequiredEXPForLevel(PlayerData* player);
// Amount of remaining exp to next level
short Get_PlayerRemainingEXPForLevel(PlayerData* player);

int Get_PlayerATK(PlayerData* player);
int Get_PlayerHP(PlayerData* player);
int Get_PlayerSPD(PlayerData* player);


#endif