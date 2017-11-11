/******************************************************************************
filename    TextDataLoader.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 31 October 2017

Brief Description:
This file contains utility functions to load and access the data within a text
file.

******************************************************************************/

/* Header Guards */
#ifndef _TEXT_DATA_LOADER_H
#define _TEXT_DATA_LOADER_H

#include <stdio.h>

/* Public Struct & Functions */
typedef struct TextDataLoader
{	
	/* Public Parameters */
	char** TextData;
	unsigned short NumberOfRows;
	unsigned short NumberOfColumns;
	/* Public Function Pointers*/
	/* Load specified resource into a TextData */
	void(*LoadResource)();
	/* Loader Exit, calls free if memory was allocated to the TextData */
	void(*Exit)();

}TextDataLoader;

///****************************************************************************
// Public Function Prototypes
///****************************************************************************
/* Setup function that initiallizes the TextDataLoader */
void TextDataLoader_Setup(TextDataLoader* Self);

#endif //_TEXT_DATA_LOADER_H