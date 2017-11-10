#include "timer.h"

int Timer_initTimer(Timer* t)
{
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
void Timer_terminateTimer(Timer* t)
{
	timeEndPeriod(t->wTimerRes);
}
void Timer_startTimer(Timer* t)
{
	QueryPerformanceCounter(&t->prevTime);
}
double Timer_liToSecs(Timer* t, LARGE_INTEGER L)
{
	return ((double)L.QuadPart / (double)(t->frequency.QuadPart));
}
double Timer_getElapsedTime(Timer* t)
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&t->currTime);
	time.QuadPart = t->currTime.QuadPart - t->prevTime.QuadPart;
	t->prevTime = t->currTime;
	return t->LIToSecs(t, time);
}
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

Timer* Timer_Create()
{
	Timer* t = (Timer*)malloc(sizeof(Timer));

	t->Init = &Timer_initTimer; //constructor
	t->Shutdown = &Timer_terminateTimer; //destructor
	t->StartTimer = &Timer_startTimer;
	t->GetElapsedTime = &Timer_getElapsedTime;
	t->LIToSecs = &Timer_liToSecs;
	t->WaitUntil = &Timer_waitUntil;

	return t;
}