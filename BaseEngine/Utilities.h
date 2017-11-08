/******************************************************************************
filename    Utilities.h
author      Qingping Zheng
DP email    qingping.zheng@digipen.edu
course      GAM100

Brief Description:
This header file has helpful tools that are not associated with anything.

******************************************************************************/

//Header Guards
#ifndef _U_H
#define _U_H

//Required Includes
#include <Windows.h>
#include <stdio.h>

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
Function Name: d_toString
Brief Description: converts a double to a C-styled string
Parameters:
value : the value to convert
length : the amount of spaces in memory to assign
	Note that the decimal point counts as a significant figure in this case
	Thus, 123.56 takes up 7 spaces due to null terminating character
*/
char* d_toString(double value, int length)
{
	//Assign a temporary memory
	// Take note that we provide 1 more space in memory for the null character
	char* str = malloc(length + 1);
	//converts the value and stores it in str
	snprintf(str, length + 1, "%f", value);
	//return the C-string
	return str;
}

#endif