#ifndef _TIMER_H
#define _TIMER_H

#include <windows.h>
#pragma comment(lib, "winmm.lib") // tell compiler to use winmm.lib

typedef struct{
	LARGE_INTEGER frequency;
	LARGE_INTEGER prevTime, currTime;
	double(*LIToSecs)(void* self, LARGE_INTEGER L);
	UINT wTimerRes;

	int(*InitTimer)(void* self); //constructor
	void(*TerminateTimer)(void* self); //destructor

	void(*StartTimer)(void* self); //starts the timer
	double(*GetElapsedTime)(void* self); //similar to DT
	void(*WaitUntil)(void* self, long long time);

}Timer;

int initTimer(void* ptr)
{
	Timer* t = (Timer*)ptr;

	QueryPerformanceFrequency(&t->frequency);

#define TARGET_RESOLUTION 1         // 1-millisecond target resolution
	TIMECAPS tc;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
	{
		// Error; application can't continue.
	}

	t->wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(t->wTimerRes);

	return 0;
}
void terminateTimer(void* ptr)
{
	Timer* t = (Timer*)ptr;
	timeEndPeriod(t->wTimerRes);
}
void startTimer(void* ptr)
{
	Timer* t = (Timer*)ptr;
	QueryPerformanceCounter(&t->prevTime);
}
double liToSecs(void* ptr, LARGE_INTEGER L)
{
	Timer* t = (Timer*)ptr;
	return ((double)L.QuadPart / (double)(t->frequency.QuadPart));
}
double getElapsedTime(void* ptr)
{
	Timer* t = (Timer*)ptr;
	LARGE_INTEGER time;
	QueryPerformanceCounter(&t->currTime);
	time.QuadPart = t->currTime.QuadPart - t->prevTime.QuadPart;
	t->prevTime = t->currTime;
	return t->LIToSecs(t, time);
}
void waitUntil(void* ptr, long long time)
{
	Timer* t = (Timer*)ptr;
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

Timer* Timer_Create(); // forward declared so that main can use
Timer* Timer_Create()
{
	Timer* t = (Timer*)malloc(sizeof(Timer));

	t->InitTimer = &initTimer; //constructor
	t->TerminateTimer = &terminateTimer; //destructor
	t->StartTimer = &startTimer;
	t->GetElapsedTime = &getElapsedTime;
	t->LIToSecs = &liToSecs;
	t->WaitUntil = &waitUntil;

	return t;
}

#endif // _TIMER_H