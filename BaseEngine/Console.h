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
#include "Vector2.h"

//Class Structure of the Console
typedef struct {

	//C-Styled constructor
	void(*c_Console)(void* ptr, Vector2 size, char* title); 

	//Fills screen with color Hex values.
	void(*clearBuffer)(void* ptr, WORD c); 

	//Supposed private variables
	HANDLE hScreenBuffer;
	CHAR_INFO* screenDataBuffer;
	COORD consoleSize;
	int screenDataBufferSize;

	//Setters for Size and Font
	void(*setConsoleSize)(void* ptr, Vector2 dimensions);
	void(*setConsoleFont)(void* ptr, Vec2 size, LPCWSTR fontName);

	//Render-Type functions
	void(*flushBufferToConsole)(void* ptr);
	void(*writeToBuffer)(void* ptr, Vector2 loc, char* data, WORD c);
	void(*ptr_writeToBuffer)(void* ptr, char** data, unsigned short row, unsigned short col, WORD c);
	void(*writeToConsole)(void* ptr, const CHAR_INFO* theBuffer);

	//Cleanup function
	void(*shutdownConsole)(void* ptr);
} Console;

/*
Function Name: con_Console
Brief Description: A constructor for the console entity.
Parameters:
	ptr : the console pointer itself, to allow for internal referencing
	size : defines the size of the console window in terms of characters
	title : sets the title of the command window.
*/

void con_Console(void* ptr, Vector2 size, char* title)
{
	//Typecast of the void pointer
	Console* c = (Console*)ptr;

	//Sets console title
	SetConsoleTitleA(title);
	//Sets the default CodePage of the console.
	//Read more here
	//https://en.wikipedia.org/wiki/Code_page_437
	SetConsoleOutputCP(437);
	//Sets the size of the Data buffer, we should not need more than this.
	c->screenDataBufferSize = size.x * size.y;
	//Assigns just enough memory to the data buffer.
	//Take note, the data buffer is of type CHAR_INFO[]
	c->screenDataBuffer = (CHAR_INFO*)malloc(c->screenDataBufferSize * sizeof(CHAR_INFO));

	//Creates the screen buffer, the actual content of the screen.
	c->hScreenBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ |          // read permissions
		GENERIC_WRITE,			// write permissions
		FILE_SHARE_READ |		// shared read
		FILE_SHARE_WRITE,       // shared write
		NULL,                   // default security attributes 
		CONSOLE_TEXTMODE_BUFFER,// must be TEXTMODE 
		NULL);					// NULL since we are already working on the screen buffer

	//sets the screen buffer to be active for our use.
	SetConsoleActiveScreenBuffer(c->hScreenBuffer);
	//sets the size of the console
	c->setConsoleSize(c, size);

	//Hides the annoying blinking cursor from the screen.
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(c->hScreenBuffer, &cursorInfo);
	cursorInfo.bVisible = 0; // set the cursor visibility
	SetConsoleCursorInfo(c->hScreenBuffer, &cursorInfo);
}

/*
Function Name: m_setConsoleFont
Brief Description: prefixed with an m, this is a supposed private function
	to allow setting of the console font
Parameters:
ptr : the console pointer itself, to allow for internal referencing
size : defines the size of the font by pixels
fontName : name of the font to use
*/

void m_setConsoleFont(void* ptr, Vec2 size, LPCWSTR fontName)
{
	//Typecast of the void pointer
	Console* c = (Console*)ptr;
	//read more here : https://docs.microsoft.com/en-us/windows/console/console-font-infoex
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = size.x;
	cfi.dwFontSize.Y = size.y;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	//since we are using pure C, wsccpy falls into the C99 standard.
	//please add _CRT_SECURE_NO_WARNINGS to disable issues with deprecation
	//Visual Studio does not support C99 and has deprecated many functions
	wcscpy(cfi.FaceName, fontName);
	SetCurrentConsoleFontEx(c->hScreenBuffer, FALSE, &cfi);
}

/*
Function Name: m_shutdownConsole
Brief Description: prefixed with an m, this is a supposed private function
to shutdown the console and free associated memory
Parameters:
ptr : the console pointer itself, to allow for internal referencing
*/

void m_shutdownConsole(void* ptr)
{
	//Typecast of the void pointer
	Console* c = (Console*)ptr;
	//clear the screen  and fill it with black
	c->clearBuffer(c, 0x00);
	//free the data buffer memory
	free(c->screenDataBuffer);
	//remove the currently active screen buffer
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
}

/*
Function Name: m_shutdownConsole
Brief Description: prefixed with an m, this is a supposed private function
to help send information from the data buffer to the screen
Parameters:
ptr : the console pointer itself, to allow for internal referencing
*/

void m_flushBufferToConsole(void* ptr)
{
	//Typecast of the void pointer
	Console* c = (Console*)ptr;
	//Write from data buffer to console
	c->writeToConsole(c, c->screenDataBuffer);
}

/*
Function Name: m_setConsoleSize
Brief Description: prefixed with an m, this is a supposed private function
to set window and buffer size of the console
Parameters:
ptr : the console pointer itself, to allow for internal referencing
size : defines the size of the console window in terms of characters
*/

void m_setConsoleSize(void* ptr, Vec2 size)
{
	//Typecast of the void pointer
	Console* c = (Console*)ptr;
	
	//grabs the buffer info
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(c->hScreenBuffer, &info);

	//Clamping of maximum limits
	if (size.x > info.dwMaximumWindowSize.X) size.x = info.dwMaximumWindowSize.X;
	if (size.y > info.dwMaximumWindowSize.Y) size.y = info.dwMaximumWindowSize.Y;

	//window coordinates start from 0, thus -1.
	SMALL_RECT windowSize = { 0, 0, size.x - 1, size.y - 1 };
	//set the size of the buffer
	COORD buffSize = { size.x, size.y };
	//Copy variables that we should use elsewhere
	c->consoleSize.X = size.x;
	c->consoleSize.Y = size.y;

	//set the window size
	SetConsoleWindowInfo(c->hScreenBuffer, TRUE, &windowSize);
	//set the buffer sie
	SetConsoleScreenBufferSize(c->hScreenBuffer, buffSize);
}

/*
Function Name: m_clearBuffer
Brief Description: prefixed with an m, this is a supposed private function
to fill the screen with a certain color
Parameters:
ptr : the console pointer itself, to allow for internal referencing
c : the desired color to write with
*/

void m_clearBuffer(void* ptr, WORD c)
{
	//Typecast of the void pointer
	Console* e = (Console*)ptr;
	//fill up every slot with a ' ' character of a certain color
	for (int i = 0; i < e->screenDataBufferSize; ++i)
	{
		e->screenDataBuffer[i].Char.AsciiChar = ' ';
		e->screenDataBuffer[i].Attributes = c;
	}
}

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

void m_writeToBuffer(void* ptr, Vector2 loc, char* data, WORD c)
{
	//Typecast of the void pointer
	Console* e = (Console*)ptr;
	//Set the write position
	size_t index = max(loc.x + e->consoleSize.X * loc.y, 0);
	//find the length of the data
	size_t length = strlen(data);
	//if the length of the data exceeds the buffer size, we chop it off at the end
	length = min(e->screenDataBufferSize - index - 1, length);
	//write the data to buffer manually
	for (size_t i = 0; i < length; ++i)
	{
		e->screenDataBuffer[index + i].Char.AsciiChar = data[i];
		e->screenDataBuffer[index + i].Attributes = c;
	}
}

void m_ptr_writeToBuffer(void* ptr, char** data, unsigned short row, unsigned short col, WORD c)
{
	//Typecast of the void pointer
	Console* e = (Console*)ptr;

	//write the data to buffer manually
	for (size_t y = 0; y < row; ++y)
	{
		for (size_t x = 0; x < col; ++x)
		{
			if (data[y][x] == '\n')
			{
				e->screenDataBuffer[x + y * col].Char.AsciiChar = ' ';
			}
			else
			{
				e->screenDataBuffer[x + y * col].Char.AsciiChar = data[y][x];
			}
		}
	}
}

/*
Function Name: m_writeToConsole
Brief Description: prefixed with an m, this is a supposed private function
to write info to the console using a buffer
Parameters:
ptr : the console pointer itself, to allow for internal referencing
theBuffer : the desired buffer to write to. Note that this allows for
	multi-buffering.
*/

void m_writeToConsole(void* ptr, const CHAR_INFO* theBuffer)
{
	//Typecast of the void pointer
	Console* e = (Console*)ptr;
	//A filler variable
	COORD characterPos = { 0, 0 };
	//the area to write, in this case, the entire console.
	SMALL_RECT WriteRegion = { 0, 0, e->consoleSize.X - 1, e->consoleSize.Y - 1 };
	// WriteConsoleOutputA for ASCII text
	WriteConsoleOutputA(e->hScreenBuffer, theBuffer, e->consoleSize, characterPos, &WriteRegion);
}

//forward declaration
Console* MakeConsole(); 

/*
Function Name: MakeConsole
Brief Description: Allocates memory needed for the console entity and
	binds the relevant functions to said entity.
*/

Console* MakeConsole()
{
	//Allocation of memory
	Console* e = (Console*)malloc(sizeof(Console));
	//Binding functions
	e->c_Console = &con_Console;
	e->setConsoleFont = &m_setConsoleFont;
	e->flushBufferToConsole = &m_flushBufferToConsole;
	e->clearBuffer = &m_clearBuffer;
	e->writeToBuffer = &m_writeToBuffer;
	e->setConsoleSize = &m_setConsoleSize;
	e->writeToConsole = &m_writeToConsole;
	e->shutdownConsole = &m_shutdownConsole;
	e->ptr_writeToBuffer = &m_ptr_writeToBuffer;

	//Returns the modified console entity
	return e;
}

#endif // ! 