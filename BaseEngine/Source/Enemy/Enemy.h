/******************************************************************************
filename	Enemy.h
author      Keith Cheng
DP email	keith.cheng@digipen.edu

Brief Description:
An enemy struct to store enemy stats
******************************************************************************/

#ifndef ENEMY_H
#define ENEMY_H

typedef struct Enemy
{
	char* name;
	int hp;
	int atk;
	int lvl;
	int spd;

	char** sprite;
	int spriteRows;
	int spriteColumns;
}Enemy;


#endif