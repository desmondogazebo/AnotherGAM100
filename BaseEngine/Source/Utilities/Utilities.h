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
#include "Vector2.h"

/*
Function Name: isKeyPressed
Brief Description: Checks whether a certain key is pressed
Parameters:
key : the keycode to check for
	read more: https://msdn.microsoft.com/en-us/library/ms927178.aspx
*/

int isKeyPressed(unsigned short key);

/*
Function Name: double_toString
Brief Description: converts a double to a C-styled string
Parameters:
value : the value to convert
length : the amount of spaces in memory to assign
	Note that the decimal point counts as a significant figure in this case
	Thus, 123.56 takes up 7 spaces due to null terminating character
*/
char* double_toString(double value, int length);

/*
Function Name: int_toString
Brief Description: converts an integer to a C-styled string
Parameters:
value : the value to convert
length : the amount of spaces in memory to assign
Note that the null terminating character counts as a significant figure in this case
*/
char* int_toString(int value, int length);

/*
enum Name: Color
Brief Description: an accessible enumerator of colors
	normally used for rendering.
*/

enum {
	c_black,
	c_dblue,
	c_dgreen,
	c_daqua,
	c_dred,
	c_purple,
	c_lime,
	c_lgrey,
	c_dgrey,
	c_blue,
	c_green,
	c_aqua,
	c_red,
	c_pink,
	c_yellow,
	c_white,
	TOTAL_COLOR
} Color;

/*
Function Name: getColor
Brief Description: gives a unsigned short Color value
Parameters:
foreground : the enum color value of the foreground
background : the enum color value of the background
*/

WORD getColor(enum Color foreground, enum Color background);

#endif