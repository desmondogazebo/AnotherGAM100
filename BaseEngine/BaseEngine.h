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
#include "Utilities.h"
#include "Vector2.h"
#include "timer.h"
#include "Console.h"
#include "TextDataLoader.h"

//Class Structure of the Engine
typedef struct
{
	//Base Functions
	void(*Init)(void* self, unsigned short fps, Vec2 s_size, Vec2 f_size);
	void(*Update)(void* self, Timer* t);
	void(*Render)(void* self);
	void(*Shutdown)(void* self);

	//Time and console entities
	Timer* g_timer;
	Console* g_console;
	TextDataLoader_ ldr;

	//global variables
	int g_quitGame;
	unsigned short FPS;
	unsigned short frameTime;
} Engine;

/*
Function Name: init
Brief Description: initializer of the Engine
Parameters:
ptr : the engine pointer itself, to allow for internal referencing
fps : the desired fps to run the game
s_size : window size
f_size : font size
*/

void init(void* ptr, unsigned short fps, Vec2 s_size, Vec2 f_size)
{
	//Typecast of the void pointer
	Engine* E = (Engine*)ptr;
	
	//Private function call
	E->g_console = MakeConsole();
	//Constructor call
	E->g_console->c_Console(E->g_console, s_size, "GAM100 PROJECT");
	//Set font of the console
	E->g_console->setConsoleFont(E->g_console, f_size, L"Raster Fonts");

	//Initializing variables
	E->g_quitGame = 0;
	E->FPS = fps;
	E->frameTime = 1000 / E->FPS;

	//Creating the internal clock object
	E->g_timer = Timer_Create();
	//Initializing the clock object
	E->g_timer->InitTimer(E->g_timer);

	/* How to use the TextDataLoader */
	/* Linking Example - Variable Order */
	TextDataLoader_ Loader = { NULL, 0, 0, TextDataLoader_Initiallize, TextDataLoader_LoadResource , TextDataLoader_Exit };
	E->ldr = Loader;
	/* Call Example */
	/* Call Initiallize */
	E->ldr.Initiallize(&E->ldr);
	/* Load a file into a reference of the loader */
	E->ldr.LoadResource(&E->ldr, "Resources/DigiPenLogo(Unofficial).txt");
}

/*
Function Name: update
Brief Description: The main loop of the engine
Parameters:
ptr : the engine pointer itself, to allow for internal referencing
t : the pointer to the timer, should there be a need to use GetElapsedTime();
*/

void update(void* ptr, Timer* t)
{
	//Typecast of the void pointer
	Engine* E = (Engine*)ptr;
	//TEST CODE
	if (isKeyPressed(VK_ESCAPE))
	{
		E->g_quitGame = 1;
	}
}

/*
Function Name: render
Brief Description: The render code
Parameters:
ptr : the engine pointer itself, to allow for internal referencing
*/
void render(void* ptr)
{
	//Typecast of the void pointer
	Engine* E = (Engine*)ptr;

	//Clear the Screen every frame
	E->g_console->clearBuffer(E->g_console, 0x0F);

	E->g_console->ptr_writeToBuffer(E->g_console, E->ldr.TextData, E->ldr.NumberOfRows, E->ldr.NumberOfColumns, 0x0A);

	//TEST CODE
	Vec2 test = { 0, 0 };
	double i = 1 / E->g_timer->GetElapsedTime(E->g_timer);
	//NOTE THIS SECTION, THIS IS HOW YOU PRINT A DOUBLE IN CHAR*
	char* input = d_toString(i, 6); //PLEASE UNDERSTAND THAT THE DECIMAL IS 1
	E->g_console->writeToBuffer(E->g_console, test, input, 0x0B);
	free(input); //VERY IMPORTANT

	//Send the new data to the Console
	E->g_console->flushBufferToConsole(E->g_console);
}

/*
Function Name: m_shutdown
Brief Description: prefixed with an m, this is a supposed private function
to shutdown the Engine and free associated memory
Parameters:
ptr : the Engine pointer itself, to allow for internal referencing
*/
void m_shutdown(void* ptr)
{
	//Typecast of the void pointer
	Engine* E = (Engine*)ptr;
	E->ldr.Exit(&E->ldr);
	//stop the internal clock
	E->g_timer->TerminateTimer(E->g_timer);
	//release the memory
	free(E->g_timer);
	//ask the console to shutdown
	E->g_console->shutdownConsole(E->g_console);
	//release the memory
	free(E->g_console);
	//finally, release the engine memory
	free(E);
}

//forward declaration
Engine* MakeEngine(); 

/*
Function Name: MakeConsole
Brief Description: Allocates memory needed for the Engine entity and
binds the relevant functions to said entity.
*/
Engine* MakeEngine()
{
	//Allocation of memory
	Engine* e = (Engine*)malloc(sizeof(Engine));
	//Binding functions
	e->Init = &init;
	e->Render = &render;
	e->Update = &update;
	e->Shutdown = &m_shutdown;

	//Returns the modified Engine entity
	return e;
}

#endif // _BE_H