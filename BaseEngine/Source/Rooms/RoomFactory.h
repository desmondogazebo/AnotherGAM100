/******************************************************************************
filename	RoomFactory.h
author      	Keith Cheng
DP email	keith.cheng@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Brief Description:
A factory to create rooms from passed in data

******************************************************************************/

#ifndef ROOMFACTORY_H
#define ROOMFACTORY_H

#include "Room.h"

Room* RoomFactory_CreateRoom();

void RoomFactory_DeleteRoom(Room* room);

#endif