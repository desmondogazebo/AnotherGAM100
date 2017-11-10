/******************************************************************************
filename    Console.h
author      Qingping Zheng
DP email    qingping.zheng@digipen.edu
course      GAM100

Brief Description:
This header file shows an outline of the Console generation.

******************************************************************************/

//Header Guards
#ifndef _CONSOLE_H
#define _CONSOLE_H

//Required Includes
#include <windows.h>
#include "..\Utilities\Vector2.h"

//Class Structure of the Console
typedef struct Console{

	//C-Styled constructor
	void(*Init)(); 
	//Fills screen with color Hex values.
	void(*ClearBuffer)(); 
	//Supposed private variables
	HANDLE hScreenBuffer;
	CHAR_INFO* screenDataBuffer;
	COORD consoleSize;
	int screenDataBufferSize;
	//Setters for Size and Font
	void(*SetConsoleSize)();
	void(*SetConsoleFont)();
	//Render-Type functions
	void(*FlushBufferToConsole)();
	void(*WriteToBuffer)();
	void(*Ptr_writeToBuffer)();
	void(*WriteToConsole)();
	//Cleanup function
	void(*ShutdownConsole)();
} Console;

/*
Function Name: con_Console
Brief Description: A constructor for the console entity.
Parameters:
	ptr : the console pointer itself, to allow for internal referencing
	size : defines the size of the console window in terms of characters
	title : sets the title of the command window.
*/

void Console_constructor(Console* ptr, Vector2 size, char* title);

/*
Function Name: m_setConsoleFont
Brief Description: prefixed with an m, this is a supposed private function
	to allow setting of the console font
Parameters:
ptr : the console pointer itself, to allow for internal referencing
size : defines the size of the font by pixels
fontName : name of the font to use
*/

void Console_setConsoleFont(Console* ptr, Vec2 size, LPCWSTR fontName);

/*
Function Name: m_shutdownConsole
Brief Description: prefixed with an m, this is a supposed private function
to shutdown the console and free associated memory
Parameters:
ptr : the console pointer itself, to allow for internal referencing
*/

void Console_shutdownConsole(Console* ptr);

/*
Function Name: m_shutdownConsole
Brief Description: prefixed with an m, this is a supposed private function
to help send information from the data buffer to the screen
Parameters:
ptr : the console pointer itself, to allow for internal referencing
*/

void Console_flushBufferToConsole(Console* ptr);

/*
Function Name: m_setConsoleSize
Brief Description: prefixed with an m, this is a supposed private function
to set window and buffer size of the console
Parameters:
ptr : the console pointer itself, to allow for internal referencing
size : defines the size of the console window in terms of characters
*/

void Console_setConsoleSize(Console* ptr, Vec2 size);

/*
Function Name: m_clearBuffer
Brief Description: prefixed with an m, this is a supposed private function
to fill the screen with a certain color
Parameters:
ptr : the console pointer itself, to allow for internal referencing
c : the desired color to write with
*/

void Console_clearBuffer(Console* ptr, WORD c);

/*
Function Name: m_writeToBuffer
Brief Description: prefixed with an m, this is a supposed private function
to write information to the buffer
Parameters:
ptr : the console pointer itself, to allow for internal referencing
loc : the console coordinates to write to: NOTE, TOP LEFT is (0,0)
data : the data to write
c : the desired color to write with
*/

void Console_writeToBuffer(Console* ptr, Vector2 loc, char* data, WORD c);

void Console_ptr_writeToBuffer(Console* ptr, char** data, unsigned short row, unsigned short col, WORD c);

/*
Function Name: m_writeToConsole
Brief Description: prefixed with an m, this is a supposed private function
to write info to the console using a buffer
Parameters:
ptr : the console pointer itself, to allow for internal referencing
theBuffer : the desired buffer to write to. Note that this allows for
	multi-buffering.
*/

void Console_writeToConsole(Console* ptr, const CHAR_INFO* theBuffer);

/*
Function Name: MakeConsole
Brief Description: Allocates memory needed for the console entity and
	binds the relevant functions to said entity.
*/

Console* MakeConsole();

#endif // ! 