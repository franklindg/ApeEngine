#ifndef _TIMER_H
#define _TIMER_H

class Timer
{
public:
	Timer();
	~Timer();

	void Frame();

	float GetTime();

	void StartTimer();
	void StopTimer();
	int GetTiming();

private:
	float m_frequency;
	INT64 m_startTime;
	float m_frameTime;
	INT64 m_beginTime, m_endTime;
};

#endif