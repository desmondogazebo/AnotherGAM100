/******************************************************************************
filename    SplashScene.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Created on 23 November 2017

Brief Description:
The splash screen, displays splash logos/icons for a few seconds before 
transiting scenes

******************************************************************************/

/* Header Guards */
#ifndef _SPLASH_SCENE_H
#define _SPLASH_SCENE_H

#include "../StateManager/BaseStateManager.h"

/* Public Struct & Functions */
typedef struct SplashScene
{
	/* Public Parameters */
	enum {
		SS_Loading = 0,
	}SS_States;
	enum CSM_States InternalState;
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

}SplashScene;

///****************************************************************************
// Public Function Prototypes
///****************************************************************************
/* Setup function that initiallizes the SplashScene */
void SplashScene_Setup(SplashScene* Self);

#endif //_SPLASH_SCENE_H