/******************************************************************************
filename    BaseEngine.c
author      Qingping Zheng
DP email    qingping.zheng@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Brief Description:
This file contains definitions for Engine code

******************************************************************************/

#include "BaseEngine.h"
#include "..\Utilities\Utilities.h"
#include "..\Utilities\Vector2.h"

//PROTOTYPES
void Engine_init(Engine* theEngine, unsigned short fps, Vector2 screenSize, Vector2 fontSize);
void Engine_update(Engine* theEngine, Timer* theTimer);
void Engine_render(Engine* theEngine);
void Engine_exit(Engine* theEngine);
void FMODErrorCheck(FMOD_RESULT result);

/*
Function Name: FMODErrorCheck
Brief Description: FMOD error check utility
Parameters:
result: output from FMOD operations
*/
void FMODErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		//some error thing
		exit(-1);
	}
}

/*
Function Name: Engine_init
Brief Description: initializer of the Engine
Parameters:
theEngine : the engine pointer itself, to allow for internal referencing
fps : the desired fps to run the game
screenSize : window size
fontSize : font size
*/

void Engine_init(Engine* theEngine, unsigned short fps, Vector2 screenSize, Vector2 fontSize)
{
	//Private function call
	theEngine->g_console = MakeConsole();
	//Constructor call
	theEngine->g_console->Init(theEngine->g_console, screenSize, "GAM100 PROJECT");
	//Set font of the console
	theEngine->g_console->SetConsoleFont(theEngine->g_console, fontSize, L"Raster Fonts");

	//Initializing variables
	theEngine->g_quitGame = 0;
	theEngine->FPS = fps;
	theEngine->frameTime = 1000 / theEngine->FPS;
	FMOD_RESULT result = FMOD_System_Create(&theEngine->SoundEngine);	
	FMODErrorCheck(result);
	result = FMOD_System_Init(theEngine->SoundEngine, 32, FMOD_INIT_NORMAL, 0);

	//Creating the internal clock object
	theEngine->g_timer = Timer_Create();
	//Initializing the clock object
	theEngine->g_timer->Init(theEngine->g_timer);

	// Added to showcase the custom state manager
	SceneSystem_Setup(&theEngine->InternalSceneSystem);
	theEngine->InternalSceneSystem.Initiallize(&theEngine->InternalSceneSystem);
}

/*
Function Name: Engine_update
Brief Description: The main loop of the engine
Parameters:
theEngine : the engine pointer itself, to allow for internal referencing
theTimer : the pointer to the timer, should there be a need to use GetElapsedTime();
*/

void Engine_update(Engine* theEngine, Timer* theTimer)
{
	//TEST CODE
	if (isKeyPressed(VK_ESCAPE))
	{
		theEngine->g_quitGame = 1;
	}
	
	theTimer->Update(theTimer);
	// Debug code
	if (isKeyPressed(VK_SHIFT))
	{
		theTimer->dt *= 4;
	}
	// Update the Scene System's current Update target
	theEngine->InternalSceneSystem.Update(&theEngine->InternalSceneSystem, theEngine, theTimer->dt);
}

/*
Function Name: Engine_render
Brief Description: The render code
Parameters:
theEngine : the engine pointer itself, to allow for internal referencing
*/
void Engine_render(Engine* theEngine)
{
	//Clear the Screen every frame
	theEngine->g_console->ClearBuffer(theEngine->g_console, 0x0F);

	// Render the Scene System's current render target
	theEngine->InternalSceneSystem.Render(&theEngine->InternalSceneSystem, theEngine);
	
	////TEST CODE
	//double i = 1 / theEngine->g_timer->dt;
	////NOTE THIS SECTION, THIS IS HOW YOU PRINT A DOUBLE IN CHAR*
	//char* input = double_toString(i, 6); //Decimals are worth 1 SF, the final escape character is worth 1 SF.
	//theEngine->g_console->text_WriteToBuffer(theEngine->g_console, Vec2(0, 0), input, getColor(c_black, c_white));
	//free(input); //VERY IMPORTANT

	//Send the new data to the Console
	theEngine->g_console->FlushBufferToConsole(theEngine->g_console);
}

/*
Function Name: Engine_exit
Brief Description: Shutdown the Engine and free associated memory
Parameters:
ptr : the Engine pointer itself, to allow for internal referencing
*/
void Engine_exit(Engine* theEngine)
{
	// exit the scene
	// Clear up the memory used within all scenes linked to the SceneSystem
	theEngine->InternalSceneSystem.Exit(&theEngine->InternalSceneSystem);
	//stop the internal clock
	theEngine->g_timer->Shutdown(theEngine->g_timer);
	//release the memory
	free(theEngine->g_timer);
	//ask the console to shutdown
	theEngine->g_console->ShutdownConsole(theEngine->g_console);
	//release the memory
	free(theEngine->g_console);
	//release the soundEngine
	FMOD_System_Release(theEngine->SoundEngine);
	//finally, release the engine memory
	free(theEngine);
}

void Engine_LoadSound(Engine* theEngine, char* filename, FMOD_SOUND** soundToLoadTo) 
{
	FMOD_RESULT result = FMOD_System_CreateSound(theEngine->SoundEngine, filename, FMOD_DEFAULT, 0, soundToLoadTo);
	FMODErrorCheck(result);
}

void Engine_PlaySound(Engine* theEngine, FMOD_SOUND* soundToPlay)
{
	FMOD_RESULT result = FMOD_System_PlaySound(theEngine->SoundEngine, soundToPlay, 0, 0, 0);
	FMODErrorCheck(result);
}

void ReleaseSound(FMOD_SOUND* SoundToDelete)
{
	if (SoundToDelete == NULL)
		return;
	FMOD_RESULT result = FMOD_Sound_Release(SoundToDelete);
	FMODErrorCheck(result);
}


/*
Function Name: MakeEngine
ACCESS LEVEL: GLOBAL
Brief Description: Allocates memory needed for the Engine entity and
binds the relevant functions to said entity.
*/
Engine* MakeEngine()
{
	//Allocation of memory
	Engine* theEngine = (Engine*)malloc(sizeof(Engine));
	//Binding functions
	theEngine->Init = &Engine_init;
	theEngine->Render = &Engine_render;
	theEngine->Update = &Engine_update;
	theEngine->Shutdown = &Engine_exit;
	theEngine->Load_Sound = &Engine_LoadSound;
	theEngine->Play_Sound = &Engine_PlaySound;
	//Returns the modified Engine entity
	return theEngine;
}