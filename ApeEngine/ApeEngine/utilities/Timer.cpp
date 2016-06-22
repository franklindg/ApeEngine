/////////////////////////////////////////////
// Filename: Timer.cpp
/////////////////////////////////////////////
#include "Timer.h"
#include "../framework/ApePCH.h"

Timer::Timer()
{
	INT64 frequency;

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0)
	{
		ReportError("Failed to initialize the Timer object.");
	}

	m_frequency = (float)frequency;

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
}

Timer::~Timer()
{
}

void Timer::Frame()
{
	INT64 currentTime;
	INT64 elapsedTicks;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	elapsedTicks = currentTime - m_startTime;

	m_frameTime = (float)elapsedTicks / m_frequency;

	m_startTime = currentTime;

	return;
}

float Timer::GetTime()
{
	return m_frameTime;
}

void Timer::StartTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_beginTime);
	return;
}

void Timer::StopTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_endTime);
	return;
}

int Timer::GetTiming()
{
	float elapsedTicks;
	INT64 frequency;
	float milliseconds;

	elapsedTicks = (float)(m_endTime - m_beginTime);

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	milliseconds = (elapsedTicks / (float)frequency) * 1000.0f;

	return (int)milliseconds;
}