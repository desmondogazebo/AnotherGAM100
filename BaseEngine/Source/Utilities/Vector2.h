/******************************************************************************
filename    Vector2.h
author      Qingping Zheng
DP email    qingping.zheng@digipen.edu
course      GAM100

Brief Description:
Vector2 is just a more intuitive version of the COORD system

******************************************************************************/

//Header Guards
#ifndef VEC2
#define VEC2

//Class Structure of Vector2
typedef struct Vector2{
	short x;
	short y;
}Vector2;

Vector2 Vec2(short x, short y);

#endif
