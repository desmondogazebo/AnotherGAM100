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

int isKeyPressed(unsigned short key);

/*
Function Name: d_toString
Brief Description: converts a double to a C-styled string
Parameters:
value : the value to convert
length : the amount of spaces in memory to assign
	Note that the decimal point counts as a significant figure in this case
	Thus, 123.56 takes up 7 spaces due to null terminating character
*/
char* d_toString(double value, int length);

//GLOBAL ENUM OF COLORS

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

typedef struct IntRange {
	int min;
	int max;
}IntRange;

int randomFromRange(IntRange i);

int math_clamp(int variable, int min, int max);

#endif