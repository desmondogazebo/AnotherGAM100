/******************************************************************************
filename	EnemyDataLoader.h
author      	Keith Cheng
DP email	keith.cheng@digipen.edu
Course: 	GAM100F17

Copyright © 2017 DigiPen (USA) Corporation

Brief Description:
A set of functions used to help populate the variables of an enemy

******************************************************************************/

#ifndef ENEMYDATALOADER_H
#define ENEMYDATALOADER_H

#include "Enemy.h"

void PopulateEnemy(Enemy* target, char* filePath);
void FreeEnemy(Enemy* target);

#endif