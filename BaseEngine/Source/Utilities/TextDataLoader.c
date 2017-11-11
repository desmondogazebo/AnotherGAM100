/******************************************************************************
filename    TextDataLoader.c
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Brief Description:
This file contains utility functions to load and access the data within a text
file.

******************************************************************************/
#include "TextDataLoader.h"
#include <stdlib.h>
#include <string.h>

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
/* Loads the specified Resource into the TextData within the struct */
void TextDataLoader_LoadResource(TextDataLoader* Self, const char* Resource);
/* Destructs the data within the struct */
void TextDataLoader_Exit(TextDataLoader* Self);
/* Test Cases */
//void TextDataLoader_TestWorkspace();
/* Utility Functions */
//void Print2DArray(char** Array, unsigned short NumberOfRows);

///****************************************************************************
// Function Definitions
///****************************************************************************
/* Setup function that initiallizes the TextDataLoader */
void TextDataLoader_Setup(TextDataLoader* Self)
{
	// Set up base parameters and handle function pointer linking
	Self->TextData = NULL;
	Self->NumberOfRows = 0;
	Self->NumberOfColumns = 0;
	Self->LoadResource = TextDataLoader_LoadResource;
	Self->Exit = TextDataLoader_Exit;
}

/* Loads the specified Resource into the TextData within the struct */
void TextDataLoader_LoadResource(TextDataLoader* Self, const char* Resource)
{
	FILE* FilePtr = NULL;
	/* Opens the file as a read only resource */
	fopen_s(&FilePtr, Resource, "r");
	if (FilePtr && Self)
	{
		/* Iterate the file and find the number of rows and columns */
		char TempArray[256];
		Self->NumberOfRows = 0;
		/* Getting the number of columns and rows from a single line of text */
		while (fgets(TempArray, 256, FilePtr)) /* Only checking up to 256 characters */
		{
			/* Adds the current line of the file into the temp array for checking */
			unsigned short MaxCharacters = 0;
			while (MaxCharacters < 256)
			{
				if (TempArray[MaxCharacters] == '\0')
				{
					if (Self->NumberOfColumns < MaxCharacters)
						Self->NumberOfColumns = MaxCharacters;
					break;
				}
				else ++MaxCharacters;
			}
			++Self->NumberOfRows;
		}

		/* Memory allocation for the number of rows */
		Self->TextData = (char **)malloc(Self->NumberOfRows * sizeof(char *));
		for (int i = 0; i < Self->NumberOfRows; i++)
			/* Memory allocation for the number of columns in a single row */
			Self->TextData[i] = (char *)malloc((Self->NumberOfColumns  + 1) * sizeof(char));

		// Value Setting
		unsigned short RowIndex = 0;
		rewind(FilePtr);
		while (fgets(TempArray, Self->NumberOfColumns + 1, FilePtr))
		{
			strcpy_s(Self->TextData[RowIndex], Self->NumberOfColumns + 1, TempArray); // Adding 1 more to the length for the null terminator
			//printf("%s", Self->TextData[RowIndex]); // Debug purposes
			++RowIndex;
		}
		fclose(FilePtr);
	}
	else printf("Cannot open file of name: %s \n", Resource);
}

/* Destructs the data within the struct */
void TextDataLoader_Exit(TextDataLoader* Self)
{
	// Check if I have loaded something before
	if (Self->TextData)
	{
		// Free each row of the double pointer and every cell within the row
		for (int i = 0; i < Self->NumberOfRows; ++i) 
			free(Self->TextData[i]);
		// Free the top level pointer
		free(Self->TextData);
	}
}

/* Test Cases */
void TextDataLoader_TestWorkspace()
{
	/* How to use the TextDataLoader */
	/* Linking Example - Variable Order */
	TextDataLoader Loader;
	TextDataLoader_Setup(&Loader);
	/* Call Example */
	/* Load a file into a reference of the loader */
	Loader.LoadResource(&Loader, "Resources/DigiPenLogo(Unofficial).txt");
	/* Do something with the file */
	//Print2DArray(Loader.TextData, Loader.NumberOfRows);
}

/* Utility Functions */
void Print2DArray(char** Array, unsigned short NumberOfRows)
{
	// Value Reading
	for (int i = 0; i < NumberOfRows; i++)
		printf("%s", Array[i]);
}