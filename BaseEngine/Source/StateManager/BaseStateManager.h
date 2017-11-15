/******************************************************************************
filename    BaseStateManager.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 7 November 2017

Brief Description:
This file contains a BaseStateManager, a generic struct that is for other state 
managers to "inherit" by linking their own custom functions to the function 
pointers within this struct.

******************************************************************************/

/* Header Guards */
#ifndef _BASE_STATE_MANAGER_H
#define _BASE_STATE_MANAGER_H

/* Public Struct & Functions */
typedef struct BaseStateManager
{
	/* Public Parameters */
	/* No Public Parameters For This Class Is Supposed To Be Generic */

	/* Public Function Pointers*/
	/* These function pointers will be generic such that we can make custom scene based state managers with different linked functions */
	/* Initiallizes, requires an instance of itself */
	void(*Initiallize)();
	/* Updates, requires an instance of itself as well as delta time */
	void(*Update)();
	/* Renders, requires an instance of itself */
	void(*Render)();
	/* Exits, requires an instance, calls free if memory was allocated */
	void(*Exit)();

}BaseStateManager;

#endif //_BASE_STATE_MANAGER_H