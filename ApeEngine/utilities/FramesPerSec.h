#ifndef _FRAMES_PER_SEC_H
#define _FRAMES_PER_SEC_H

class FramesPerSec
{
public:
	FramesPerSec();
	~FramesPerSec();

	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

#endif