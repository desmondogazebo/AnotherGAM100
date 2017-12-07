/******************************************************************************
filename    Vector2.h
author      Qingping Zheng
DP email    qingping.zheng@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Brief Description:
Vector2 is just a more intuitive version of the COORD system

******************************************************************************/

//Header Guards
#ifndef VEC2
#define VEC2

//Class Structure of Vector2
typedef struct {
	short x;
	short y;
}Vector2;

/*
Function Name: Vec2
Brief Description: A lazy init for a Vector2 variable.
Parameters:
x : x of the vector
y : y of the vector
Returns : A vector2 variable of values x and y.
*/
Vector2 Vec2(short x, short y);

#endif
