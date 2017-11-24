/******************************************************************************
filename    DungeonCamera.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 23 November 2017

Brief Description:
This file contains a camera object that manipulates what is being rendered in 
the dungeon scene

******************************************************************************/

/* Header Guards */
#ifndef _DUNGEON_CAMERA_H
#define _DUNGEON_CAMERA_H

#include "../Utilities/Vector2.h"

typedef struct DungeonCamera 
{
	/* Public Parameters */
	Vector2 CalculatedMapOffset;

	/* Public Function Pointers*/
	/* Uses passed TextDataLoader and Vec2 Player coords to determine the offset of the map */
	void(*UpdateCameraLogic)();

}DungeonCamera;

void DungeonCamera_Setup(DungeonCamera* Self);

#endif //_DUNGEON_CAMERA_H