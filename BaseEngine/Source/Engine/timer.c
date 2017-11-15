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
void Timer_initTimer(Timer* t);
void Timer_terminateTimer(Timer* t);
void Timer_startTimer(Timer* t);
double Timer_liToSecs(Timer* t, LARGE_INTEGER L);
double Timer_getElapsedTime(Timer* t);
void Timer_waitUntil(Timer* t, long long time);

/*
Function Name: Timer_initTimer
Brief Description: This instantiates the timer object
Parameters:
t: The pointer to the timer object to manipulate
*/

void Timer_initTimer(Timer* t)
{
	QueryPerformanceFrequency(&t->frequency);

#define TARGET_RESOLUTION 1         // 1-millisecond target resolution
	TIMECAPS tc;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
	{
		// Error; application can't continue.
		return;
	}

	t->wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(t->wTimerRes);
}

/*
Function Name: Timer_terminateTimer
Brief Description: This stops the timer object
Parameters:
t: The pointer to the timer object to manipulate
*/

void Timer_terminateTimer(Timer* t)
{
	timeEndPeriod(t->wTimerRes);
}

/*
Function Name: Timer_startTimer
Brief Description: This starts the timer object
Parameters:
t: The pointer to the timer object to manipulate
*/

void Timer_startTimer(Timer* t)
{
	QueryPerformanceCounter(&t->prevTime);
}

/*
Function Name: Timer_liToSecs
Brief Description: This is a helper function to convert a LARGE_INTEGER
	to a double, by means of division.
Parameters:
t: The pointer to the timer object to manipulate
L: The value to convert
*/

double Timer_liToSecs(Timer* t, LARGE_INTEGER L)
{
	double a = (double)L.QuadPart / (double)(t->frequency.QuadPart);
	return a;
}

/*
Function Name: Timer_getElapsedTime
Brief Description: This reports the time (in milliseconds)
	passed since last call of QueryPerformanceCounter.
Parameters:
t: The pointer to the timer object to manipulate
*/

double Timer_getElapsedTime(Timer* t)
{
	return t->dt;
}

/*
Function Name: Timer_waitUntil
Brief Description: This causes the timer to wait,
	essentially slowing the program to a halt.
Parameters:
t: The pointer to the timer object to manipulate
time: the duration to wait for
*/
void Timer_waitUntil(Timer* t, long long time)
{
	LARGE_INTEGER nowTime;
	LONGLONG timeElapsed;
	while (1)
	{
		QueryPerformanceCounter(&nowTime);
		timeElapsed = ((nowTime.QuadPart - t->prevTime.QuadPart) * 1000 / (t->frequency.QuadPart));
		if (timeElapsed > time)
			return;
		else if (time - timeElapsed > 1)
			Sleep(1);
	}
}

void Timer_update(Timer* t)
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&t->currTime);
	time.QuadPart = t->currTime.QuadPart - t->prevTime.QuadPart;
	t->prevTime = t->currTime;
	t->dt = t->LIToSecs(t, time);
}

/*
Function Name: Timer_Create
Brief Description: This creates the timer object
	and binds its relevant functions
*/

Timer* Timer_Create()
{
	Timer* t = (Timer*)malloc(sizeof(Timer));

	t->Init = &Timer_initTimer; //constructor
	t->Shutdown = &Timer_terminateTimer; //destructor
	t->StartTimer = &Timer_startTimer;
	t->GetElapsedTime = &Timer_getElapsedTime;
	t->LIToSecs = &Timer_liToSecs;
	t->WaitUntil = &Timer_waitUntil;
	t->Update = &Timer_update;

	return t;
}