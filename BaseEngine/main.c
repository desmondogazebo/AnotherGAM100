/******************************************************************************
filename    main.c
author      Qingping Zheng
DP email    qingping.zheng@digipen.edu
course      GAM100

Brief Description:
This file implements the main function and loop.

******************************************************************************/

//memory leak checker and required includes
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>

//Header Include
#include "BaseEngine.h"

/* The main program loop */
int main()
{
	//Create an Instance of the Engine
	Engine* theEngine = MakeEngine();

	//Vec2 or Vector2 is okay

	//Define desired 'windowSize', that is,
	//number of characters that can fit in the screen
	//max for a console window is 80 by 25 for now.
	Vec2 windowSize = { 80, 25 };
	
	//Define desired 'fontSize', where both
	//width and height are scalable
	Vec2 fontSize = { 10, 18 };

	//Initialize the Engine
	theEngine->Init(theEngine, 60, windowSize, fontSize);

	//Start the internal clock of the program
	theEngine->g_timer->StartTimer(theEngine->g_timer);

	//Main Loop
	while (!theEngine->g_quitGame)
	{
		//Update Cycle
		theEngine->Update(theEngine, theEngine->g_timer);
		//Render Cycle
		theEngine->Render(theEngine);
		//Frame cap, preventing program from running faster
		theEngine->g_timer->WaitUntil(theEngine->g_timer, theEngine->frameTime);
	}
	//shutdown
	theEngine->Shutdown(theEngine);

	//DEBUG: display memory leaks in Output Window (if any)
	_CrtDumpMemoryLeaks();
	/* Cleanly exit */
	return 0;
}