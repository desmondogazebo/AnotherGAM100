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
#include "..\Utilities\Utilities.h"

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
	void(*text_WriteToBuffer)();
	void(*sprite_WriteToBuffer)();
	void(*replace_withColor)();
	void(*map_WriteToBuffer)();
	void(*dungeon_WriteToBuffer)();
	void(*WriteToConsole)();
	//Cleanup function
	void(*ShutdownConsole)();
} Console;

//public functions

void Console_constructor(Console* ptr, Vector2 size, char* title);
void Console_setConsoleFont(Console* ptr, Vector2 size, LPCWSTR fontName);
void Console_shutdownConsole(Console* ptr);
void Console_flushBufferToConsole(Console* ptr);
void Console_setConsoleSize(Console* ptr, Vector2 size);

void Console_clearBuffer(Console* ptr, WORD c);
void Console_text_writeToBuffer(Console* ptr, Vector2 loc, char* data, WORD c);
void Console_sprite_writeToBuffer(Console* ptr, Vector2 loc, char** data, unsigned short row, unsigned short col, WORD c);
void Console_replace_withColor(Console* ptr, char target, char replacement, WORD c);
void Console_map_writeToBuffer(Console* ptr, char** data, unsigned short row, unsigned short col, WORD c);
void Console_dungeon_writeToBuffer(Console* ptr, char** data, int offsetX, int offsetY, WORD c);

void Console_writeToConsole(Console* ptr, const CHAR_INFO* theBuffer);

Console* MakeConsole();

#endif // ! 