/******************************************************************************
filename    	timer.h
author      	Qingping Zheng
DP email    	qingping.zheng@digipen.edu
Course: 	GAM100F17
Copyright © 2017 DigiPen (USA) Corporation

Brief Description:
This header file contains information for the console timer, which enables
the engines real-time system.

******************************************************************************/

#ifndef _TIMER_H
#define _TIMER_H

#include <windows.h>
#pragma comment(lib, "winmm.lib") // tell compiler to use winmm.lib

typedef struct Timer{
	LARGE_INTEGER frequency;
	LARGE_INTEGER prevTime, currTime;
	UINT wTimerRes;
	double dt;

	void(*Init)(); //constructor
	void(*Update)();
	void(*Shutdown)(); //destructor
	void(*StartTimer)(); //starts the timer
	double(*GetElapsedTime)(); //similar to DT
	double(*LIToSecs)();
	void(*WaitUntil)();

}Timer;

//public functions
Timer* Timer_Create();

#endif // _TIMER_H