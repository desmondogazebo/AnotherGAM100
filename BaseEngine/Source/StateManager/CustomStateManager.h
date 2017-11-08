/******************************************************************************
filename    CustomStateManager.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 7 November 2017

Brief Description:
A template on creating a customized state manager

******************************************************************************/

/* Header Guards */
#ifndef _CUSTOM_STATE_MANAGER_H
#define _CUSTOM_STATE_MANAGER_H

#include "BaseStateManager.h"

/* Public Struct & Functions */

typedef struct CustomStateManager
{
	/* Public Parameters */
	enum{
		CSM_Idle = 0,
		CSM_Active,
		CSM_Inactive
	}CSM_States;
	enum CSM_States InternalState;
	BaseStateManager InternalStateManager;

	/* Public Function Pointers*/
	/* Updates, requires an instance of itself as well as delta time */
	void(*Update)();
	/* Exits, requires an instance, calls free if memory was allocated */
	void(*Exit)();

}CustomStateManager;

///****************************************************************************
// Public Function Prototypes
///****************************************************************************
void CustomStateManager_Setup(CustomStateManager* Self);

#endif //_CUSTOM_STATE_MANAGER_H