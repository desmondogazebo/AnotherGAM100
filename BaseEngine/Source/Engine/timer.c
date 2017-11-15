/******************************************************************************
filename    timer.c
author      Qingping Zheng
DP email    qingping.zheng@digipen.edu
course      GAM100

Brief Description:
This file contains definitions for the timer object.

******************************************************************************/

//Includes
#include "timer.h"

//Function Prototypes (PUBLIC)
void Timer_initTimer(Timer* theTimer);
void Timer_terminateTimer(Timer* theTimer);
void Timer_startTimer(Timer* theTimer);
double Timer_liToSecs(Timer* theTimer, LARGE_INTEGER L);
double Timer_getElapsedTime(Timer* theTimer);
void Timer_waitUntil(Timer* theTimer, long long time);

/*
Function Name: Timer_initTimer
Brief Description: This instantiates the timer object
Parameters:
theTimer: The pointer to the timer object to manipulate
*/

void Timer_initTimer(Timer* theTimer)
{
	QueryPerformanceFrequency(&theTimer->frequency);

#define TARGET_RESOLUTION 1         // 1-millisecond target resolution
	TIMECAPS tc;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
	{
		// Error; application can't continue.
		return;
	}

	theTimer->wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(theTimer->wTimerRes);
}

/*
Function Name: Timer_terminateTimer
Brief Description: This stops the timer object
Parameters:
theTimer: The pointer to the timer object to manipulate
*/

void Timer_terminateTimer(Timer* theTimer)
{
	timeEndPeriod(theTimer->wTimerRes);
}

/*
Function Name: Timer_startTimer
Brief Description: This starts the timer object
Parameters:
theTimer: The pointer to the timer object to manipulate
*/

void Timer_startTimer(Timer* theTimer)
{
	QueryPerformanceCounter(&theTimer->prevTime);
}

/*
Function Name: Timer_liToSecs
Brief Description: This is a helper function to convert a LARGE_INTEGER
	to a double, by means of division.
Parameters:
theTimer: The pointer to the timer object to manipulate
L: The value to convert
*/

double Timer_liToSecs(Timer* theTimer, LARGE_INTEGER L)
{
	double a = (double)L.QuadPart / (double)(theTimer->frequency.QuadPart);
	return a;
}

/*
Function Name: Timer_getElapsedTime
Brief Description: This reports the time (in milliseconds)
	passed since last call of QueryPerformanceCounter.
Parameters:
theTimer: The pointer to the timer object to manipulate
*/

double Timer_getElapsedTime(Timer* theTimer)
{
	return theTimer->dt;
}

/*
Function Name: Timer_waitUntil
Brief Description: This causes the timer to wait,
	essentially slowing the program to a halt.
Parameters:
theTimer: The pointer to the timer object to manipulate
time: the duration to wait for
*/
void Timer_waitUntil(Timer* theTimer, long long time)
{
	LARGE_INTEGER nowTime;
	LONGLONG timeElapsed;
	while (1)
	{
		QueryPerformanceCounter(&nowTime);
		timeElapsed = ((nowTime.QuadPart - theTimer->prevTime.QuadPart) * 1000 / (theTimer->frequency.QuadPart));
		if (timeElapsed > time)
			return;
		else if (time - timeElapsed > 1)
			Sleep(1);
	}
}

void Timer_update(Timer* theTimer)
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&theTimer->currTime);
	time.QuadPart = theTimer->currTime.QuadPart - theTimer->prevTime.QuadPart;
	theTimer->prevTime = theTimer->currTime;
	theTimer->dt = theTimer->LIToSecs(theTimer, time);
}

/*
Function Name: Timer_Create
Brief Description: This creates the timer object
	and binds its relevant functions
*/

Timer* Timer_Create()
{
	Timer* theTimer = (Timer*)malloc(sizeof(Timer));

	theTimer->Init = &Timer_initTimer; //constructor
	theTimer->Shutdown = &Timer_terminateTimer; //destructor
	theTimer->StartTimer = &Timer_startTimer;
	theTimer->GetElapsedTime = &Timer_getElapsedTime;
	theTimer->LIToSecs = &Timer_liToSecs;
	theTimer->WaitUntil = &Timer_waitUntil;
	theTimer->Update = &Timer_update;

	return theTimer;
}