/******************************************************************************
filename    Utilities.c
author      Qingping Zheng & Rui An Ryan Lim
DP email    qingping.zheng@digipen.edu & l.ruianryan@digipen.edu
course      GAM100

Brief Description:
This file contains definitions for functions in Utilities.h

******************************************************************************/
#include "Utilities.h"

/*
Function Name: isKeyPressed
Brief Description: Checks whether a certain key is pressed
Parameters:
key : the keycode to check for
read more: https://msdn.microsoft.com/en-us/library/ms927178.aspx
*/

int isKeyPressed(unsigned short key)
{
	//Returns whether the key is pressed.
	return ((GetAsyncKeyState(key) & 0x8001));
	//Note: this function is polled every frame.
}

/*
Function Name: double_toString
Brief Description: converts a double to a C-styled string
Parameters:
value : the value to convert
length : the amount of spaces in memory to assign

Note that the decimal point counts as a significant figure in this case
Thus, 123.56 takes up 7 spaces. 5 numbers + 1 decimal point + 1 null terminating character.
*/
char* double_toString(double value, int length)
{
	//Assign a temporary memory
	// Take note that we provide 1 more space in memory for the null character
	char* str = malloc(length + 1);
	//converts the value and stores it in str
	snprintf(str, length + 1, "%f", value);
	//return the C-string
	return str;
}

/*
Function Name: int_toString
Brief Description: converts an integer to a C-styled string
Parameters:
value : the value to convert
length : the amount of spaces in memory to assign
Note that the null terminating character counts as a significant figure in this case
*/
char* int_toString(int value, int length)
{
	//Assign a temporary memory
	// Take note that we provide 1 more space in memory for the null character
	char* str = malloc(length + 1);
	//converts the value and stores it in str
	snprintf(str, length + 1, "%d", value);
	//return the C-string
	return str;
}

/*
Function Name: getColor
Brief Description: provides a unsigned short for a representation of color
	based on understanding of hex values and windows values.
Parameters:
background : the enum color value of the background
foreground : the enum color value of the foreground
*/

WORD getColor(enum Color background, enum Color foreground)
{
	return ((int)background * 16) + foreground; //hex base 16
}

// Utility Functions
/* A function that takes in a Char* Array and resets it's contents to '\0' */
void ResetCharArray(char* Array)
{
	memset(Array, 0, sizeof(char) * strlen(Array));
}

/* A function that in two variables to signify a range to randomize between and returns a random value within the provided range */
int IntRandomizeRange(int Lower, int Upper) 
{
	return((rand() % (Upper - Lower + 1)) + Lower);
}