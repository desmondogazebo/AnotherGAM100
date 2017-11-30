/******************************************************************************
filename    WinScene.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 23 November 2017

Brief Description:
The splash screen, displays splash logos/icons for a few seconds before 
transiting scenes

******************************************************************************/

/* Header Guards */
#ifndef _WIN_SCENE_H
#define _WIN_SCENE_H

#include "../StateManager/BaseStateManager.h"

/* Public Struct & Functions */
typedef struct WinScene
{
	/* Public Parameters */
	enum {
		WS_Loading = 0,
	}WS_States;
	enum WS_States InternalState;
	BaseStateManager InternalStateManager;

	/* Public Function Pointers*/
	/* Initiallize, requires an instance of itself */
	void(*Initiallize)();
	/* Updates, requires an instance of itself as well as delta time */
	void(*Update)();
	/* Renders, requires an instance of itself and a passed engine*, writes the data of the scene to the screen's buffer */
	void(*Render)();
	/* Exits, requires an instance, calls free if memory was allocated */
	void(*Exit)();

}WinScene;

///****************************************************************************
// Public Function Prototypes
///****************************************************************************
/* Setup function that initiallizes the WinScene */
void WinScene_Setup(WinScene* Self);

#endif //_SPLASH_SCENE_H