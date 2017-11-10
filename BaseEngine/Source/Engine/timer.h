#ifndef _TIMER_H
#define _TIMER_H

#include <windows.h>
#pragma comment(lib, "winmm.lib") // tell compiler to use winmm.lib

typedef struct Timer{
	LARGE_INTEGER frequency;
	LARGE_INTEGER prevTime, currTime;
	double(*LIToSecs)();
	UINT wTimerRes;

	int(*Init)(); //constructor
	void(*Shutdown)(); //destructor

	void(*StartTimer)(); //starts the timer
	double(*GetElapsedTime)(); //similar to DT
	void(*WaitUntil)();

}Timer;

int Timer_initTimer(Timer* t);
void Timer_terminateTimer(Timer* t);
void Timer_startTimer(Timer* t);
double Timer_liToSecs(Timer* t, LARGE_INTEGER L);
double Timer_getElapsedTime(Timer* t);
void Timer_waitUntil(Timer* t, long long time);

Timer* Timer_Create();

#endif // _TIMER_H