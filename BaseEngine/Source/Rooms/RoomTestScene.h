/******************************************************************************
filename	RoomTestScene.h
author      Keith Cheng
DP email	keith.cheng@digipen.edu

Brief Description:
A test scene made for the purpose of testing rooms

******************************************************************************/

#ifndef ROOMTESTSCENE_H
#define ROOMTESTSCENE_H

typedef struct RoomTestScene RoomTestScene;

#include "Room.h"
#include "RoomArray.h"
#include "../Player/Player.h"

struct RoomTestScene
{
	RoomArray roomList;
	Room* currentRoom;

	Player player;

	short wKeyPressed;
	short sKeyPressed;
	short aKeyPressed;
	short dKeyPressed;

	void(*Init)();
	void(*Update)();
	void(*Exit)();
	void(*AddRoom)();
};

// This is a function to create a RoomTestScene object
RoomTestScene Create_RoomTestScene();

// These are functions to assign to the RoomTestScene function pointers
void Initialize_RoomTestScene(RoomTestScene* self);

void Update_RoomTestScene(RoomTestScene* self, double dt);

void Exit_RoomTestScene(RoomTestScene* self);

void Add_Room(RoomTestScene* self, char* mapString);

// Splitting up code into various functions
void PlayerControls(RoomTestScene* self);

#endif