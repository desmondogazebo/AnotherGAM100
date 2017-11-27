#include "Console.h"

/*
Function Name: con_Console
Brief Description: A constructor for the console entity.
Parameters:
ptr : the console pointer itself, to allow for internal referencing
size : defines the size of the console window in terms of characters
title : sets the title of the command window.
*/

void Console_constructor(Console* theConsole, Vector2 size, char* title)
{
	//Sets console title
	SetConsoleTitleA(title);
	//Sets the default CodePage of the console.
	//Read more here
	//https://en.wikipedia.org/wiki/CodEngine_pagEngine_437
	SetConsoleOutputCP(437);
	//Sets the size of the Data buffer, we should not need more than this.
	theConsole->screenDataBufferSize = size.x * size.y;
	//Assigns just enough memory to the data buffer.
	//Take note, the data buffer is of type CHAR_INFO[]
	theConsole->screenDataBuffer = (CHAR_INFO*)malloc(theConsole->screenDataBufferSize * sizeof(CHAR_INFO));

	//Creates the screen buffer, the actual content of the screen.
	theConsole->hScreenBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ |          // read permissions
		GENERIC_WRITE,			// write permissions
		FILE_SHARE_READ |		// shared read
		FILE_SHARE_WRITE,       // shared write
		NULL,                   // default security attributes 
		CONSOLE_TEXTMODE_BUFFER,// must be TEXTMODE 
		NULL);					// NULL since we are already working on the screen buffer

								//sets the screen buffer to be active for our use.
	SetConsoleActiveScreenBuffer(theConsole->hScreenBuffer);
	//sets the size of the console
	theConsole->SetConsoleSize(theConsole, size);

	//Hides the annoying blinking cursor from the screen.
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(theConsole->hScreenBuffer, &cursorInfo);
	cursorInfo.bVisible = 0; // set the cursor visibility
	SetConsoleCursorInfo(theConsole->hScreenBuffer, &cursorInfo);
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

void Console_setConsoleFont(Console* theConsole, Vector2 size, LPCWSTR fontName)
{
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
	SetCurrentConsoleFontEx(theConsole->hScreenBuffer, FALSE, &cfi);
}

/*
Function Name: m_shutdownConsole
Brief Description: prefixed with an m, this is a supposed private function
to shutdown the console and free associated memory
Parameters:
ptr : the console pointer itself, to allow for internal referencing
*/

void Console_shutdownConsole(Console* theConsole)
{
	//clear the screen  and fill it with black
	theConsole->ClearBuffer(theConsole, 0x00);
	//free the data buffer memory
	free(theConsole->screenDataBuffer);
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

void Console_flushBufferToConsole(Console* theConsole)
{
	//Write from data buffer to console
	theConsole->WriteToConsole(theConsole, theConsole->screenDataBuffer);
}

/*
Function Name: m_setConsoleSize
Brief Description: prefixed with an m, this is a supposed private function
to set window and buffer size of the console
Parameters:
ptr : the console pointer itself, to allow for internal referencing
size : defines the size of the console window in terms of characters
*/

void Console_setConsoleSize(Console* theConsole, Vector2 size)
{
	//grabs the buffer info
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(theConsole->hScreenBuffer, &info);

	//Clamping of maximum limits
	if (size.x > info.dwMaximumWindowSize.X) size.x = info.dwMaximumWindowSize.X;
	if (size.y > info.dwMaximumWindowSize.Y) size.y = info.dwMaximumWindowSize.Y;

	//window coordinates start from 0, thus -1.
	SMALL_RECT windowSize = { 0, 0, size.x - 1, size.y - 1 };
	//set the size of the buffer
	COORD buffSize = { size.x, size.y };
	//Copy variables that we should use elsewhere
	theConsole->consoleSize.X = size.x;
	theConsole->consoleSize.Y = size.y;

	//set the window size
	SetConsoleWindowInfo(theConsole->hScreenBuffer, TRUE, &windowSize);
	//set the buffer sie
	SetConsoleScreenBufferSize(theConsole->hScreenBuffer, buffSize);
}

/*
Function Name: m_clearBuffer
Brief Description: prefixed with an m, this is a supposed private function
to fill the screen with a certain color
Parameters:
ptr : the console pointer itself, to allow for internal referencing
c : the desired color to write with
*/

void Console_clearBuffer(Console* theConsole, WORD colour)
{
	//fill up every slot with a ' ' character of a certain color
	for (int i = 0; i < theConsole->screenDataBufferSize; ++i)
	{
		theConsole->screenDataBuffer[i].Char.AsciiChar = ' ';
		theConsole->screenDataBuffer[i].Attributes = colour;
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

void Console_text_writeToBuffer(Console* theConsole, Vector2 loc, char* data, WORD colour)
{
	//Set the write position
	size_t index = max(loc.x + theConsole->consoleSize.X * loc.y, 0);
	//find the length of the data
	size_t length = strlen(data);
	//if the length of the data exceeds the buffer size, we chop it off at the end
	length = min(theConsole->screenDataBufferSize - index - 1, length);
	//write the data to buffer manually
	for (size_t i = 0; i < length; ++i)
	{
		theConsole->screenDataBuffer[index + i].Char.AsciiChar = data[i];
		theConsole->screenDataBuffer[index + i].Attributes = colour;
	}
}

void Console_sprite_writeToBuffer(Console* theConsole, Vector2 location, char** data, unsigned short row, unsigned short col, WORD colour)
{
	if (col > theConsole->consoleSize.X) col = theConsole->consoleSize.X; 
	if (row > theConsole->consoleSize.Y) row = theConsole->consoleSize.Y;

	//Set the write position
	int writeIndex = location.x + theConsole->consoleSize.X * location.y; //this is the starting print location.

	int offset = location.x; //offset from left of screen. this is a variant that counts absolute lefts.

	for (int y = 0; y < row; ++y)
	{
		int length = col + 1; //we have the length of one life of data.

		int trim = 0; //the amount we truncate by from left.
		int addedSpace = 0; //number of spaces to append from left

		if (length + offset > theConsole->consoleSize.X)
		{		
			//by calculation, if the length + startIndex is greater than the size of one line,
			trim = (length + offset) - theConsole->consoleSize.X - 1; //we need to trim excess!
		}
		else if(offset < 0) //if i'm printing out of screen (left edge)
		{
			addedSpace = -offset;
		}

		for (int x = addedSpace; x < col - trim; ++x) //we write every single character that isn't trimmed yet.
		{
			if (data[y][x] == '\n') //this is assuming the sprite has constant col and row throughout
			{
				writeIndex += theConsole->consoleSize.X - strlen(data[y]); //move initial write location if \n
				break;
			}
			if (data[y][x] == '\0')
			{
				break; //ignore if null character
			}
			int actualValue = writeIndex + (x + y * col);
			if (actualValue < 0)
			{
				continue; // skip if attempting to write to illegal location
			}
			if (actualValue < theConsole->screenDataBufferSize) //while i still have space to write...
			{
				if (data[y][x] == '~')
				{
					if (!data[y][x])
					{
						theConsole->screenDataBuffer[actualValue].Char.AsciiChar = ' ';
					}
					theConsole->screenDataBuffer[actualValue].Attributes = colour;
				}
				else
				{
					theConsole->screenDataBuffer[actualValue].Char.AsciiChar = data[y][x];
					theConsole->screenDataBuffer[actualValue].Attributes = colour;
				}
			}
			//everything else is chopped off
		}
	}
}

void Console_map_writeToBuffer(Console* theConsole, char** data, unsigned short row, unsigned short col, WORD colour)
{
	if (col > theConsole->consoleSize.X) col = theConsole->consoleSize.X;
	if (row > theConsole->consoleSize.Y) row = theConsole->consoleSize.Y;

	for (size_t y = 0; y < row; ++y)
	{
		for (size_t x = 0; x < col; ++x) //we write every single character that isn't trimmed yet.
		{
			if (data[y][x] == '~')
			{
				if (!data[y][x])
				{
					theConsole->screenDataBuffer[(x + y * col)].Char.AsciiChar = ' ';
				}
				theConsole->screenDataBuffer[x + y * col].Attributes = colour;
			}
			else
			{
				theConsole->screenDataBuffer[(x + y * col)].Char.AsciiChar = data[y][x];
				theConsole->screenDataBuffer[(x + y * col)].Attributes = colour;
			}
		}
	}
}

void Console_dungeon_writeToBuffer(Console* theConsole, char** data, int offsetX, int offsetY, WORD colour)
{
	int col = theConsole->consoleSize.X;
	int row = theConsole->consoleSize.Y;

	for (int y = 0; y < row; ++y)
	{
		for (int x = 0; x < col; ++x) //we write every single character that isn't trimmed yet.
		{
			if (data[y + offsetY][x + offsetX] == '~') //TO CHANGE
			{
				if (!data[y + offsetY][x + offsetX]) //TO CHANGE
				{
					theConsole->screenDataBuffer[(x + y * col)].Char.AsciiChar = ' ';
				}
				theConsole->screenDataBuffer[x + y * col].Attributes = colour;
			}
			else
			{
				theConsole->screenDataBuffer[(x + y * col)].Char.AsciiChar = data[y + offsetY][x + offsetX]; //TO CHANGE
				theConsole->screenDataBuffer[(x + y * col)].Attributes = colour;
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

void Console_writeToConsole(Console* theConsole, const CHAR_INFO* theBuffer)
{
	//A filler variable
	COORD characterPos = { 0, 0 };
	//the area to write, in this case, the entire console.
	SMALL_RECT WriteRegion = { 0, 0, theConsole->consoleSize.X - 1, theConsole->consoleSize.Y - 1 };
	// WriteConsoleOutputA for ASCII text
	WriteConsoleOutputA(theConsole->hScreenBuffer, theBuffer, theConsole->consoleSize, characterPos, &WriteRegion);
}

/*
Function Name: MakeConsole
Brief Description: Allocates memory needed for the console entity and
binds the relevant functions to said entity.
*/

Console* MakeConsole()
{
	//Allocation of memory
	Console* theConsole = (Console*)malloc(sizeof(Console));
	//Binding functions
	theConsole->Init = &Console_constructor;
	theConsole->SetConsoleFont = &Console_setConsoleFont;
	theConsole->FlushBufferToConsole = &Console_flushBufferToConsole;
	theConsole->ClearBuffer = &Console_clearBuffer;

	theConsole->text_WriteToBuffer = &Console_text_writeToBuffer;
	theConsole->map_WriteToBuffer = &Console_map_writeToBuffer;
	theConsole->sprite_WriteToBuffer= &Console_sprite_writeToBuffer;
	theConsole->dungeon_WriteToBuffer = &Console_dungeon_writeToBuffer;

	theConsole->SetConsoleSize = &Console_setConsoleSize;
	theConsole->WriteToConsole = &Console_writeToConsole;
	theConsole->ShutdownConsole = &Console_shutdownConsole;

	//Returns the modified console entity
	return theConsole;
}