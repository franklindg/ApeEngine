#include <ApePCH.h>
#include "FramesPerSec.h"

FramesPerSec::FramesPerSec()
	: m_fps(0)
	, m_count(0)
{
	m_startTime = timeGetTime();
}

FramesPerSec::~FramesPerSec()
{
}

void FramesPerSec::Frame()
{
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;

		m_startTime = timeGetTime();
	}
}

int FramesPerSec::GetFps()
{
	return m_fps;
}