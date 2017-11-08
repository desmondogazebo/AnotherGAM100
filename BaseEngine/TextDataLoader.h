/******************************************************************************
filename    TextDataLoader.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Brief Description:
This file contains utility functions to load and access the data within a text
file.

******************************************************************************/

/* Header Guards */
#ifndef _TEXT_DATA_LOADER_H
#define _TEXT_DATA_LOADER_H

#include <stdio.h>

/* Public Struct & Functions */

/* TODO Personal : Add more functionality to the TDL for features such as CSV loading or file writing that will be used in the team assignment. */

typedef struct TextDataLoader
{
	/* Public Parameters */
	char** TextData;
	unsigned short NumberOfRows;
	unsigned short NumberOfColumns;
	/* Public Function Pointers*/
	/* Initiallize the loader, requires an instance of a TDL */
	void (*Initiallize)();
	/* Load specified resource into a TextData */
	void(*LoadResource)();
	/* Loader Exit, calls free if memory was allocated to the TextData */
	void(*Exit)();

} TextDataLoader_;

/* Text Data Loader Functions */
/* Initiallizes the data within the struct */
void TextDataLoader_Initiallize(TextDataLoader_* Self);

/* Loads the specified Resource into the TextData within the struct */
void TextDataLoader_LoadResource(TextDataLoader_* Self, const char* Resource);

/* Destructs the data within the struct */
void TextDataLoader_Exit(TextDataLoader_* Self);

/* Test Cases */
void TextDataLoader_TestWorkspace();

/* Utility Functions */
void Print2DArray(char** Array, unsigned short NumberOfRows);

#endif //_TEXT_DATA_LOADER_H