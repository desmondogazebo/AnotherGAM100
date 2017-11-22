/******************************************************************************
filename    BaseEngine.h
author      Qingping Zheng
DP email    qingping.zheng@digipen.edu
course      GAM100

Brief Description:
This file shows an outline of the Engine

******************************************************************************/

//Header Guards
#ifndef _BE_H
#define _BE_H

//Required Includes
#include "timer.h"
#include "Console.h"
#include "..\Systems\SceneSystem.h"

//Class Structure of the Engine
typedef struct Engine
{
	//Base Functions
	void(*Init)();
	void(*Update)();
	void(*Render)();
	void(*Shutdown)();

	//Time and console entities
	Timer* g_timer;
	Console* g_console;

	//global variables
	int g_quitGame;
	double FPS;
	double frameTime;
	// The Engine shall hold an internal scene system to encapsulate and handle all forms of scene logic and rendering
	SceneSystem InternalSceneSystem;

} Engine;

Engine* MakeEngine();

#endif // _BE_H