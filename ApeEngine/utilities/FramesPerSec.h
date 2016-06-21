/////////////////////////////////////////////
// Filename: FramesPerSec.h
/////////////////////////////////////////////
#ifndef _FRAMES_PER_SEC_H
#define _FRAMES_PER_SEC_H

//////////
// LINKER //
//////////
#pragma comment(lib, "winmm.lib")


//////////
// INCLUDES //
//////////
#include <Windows.h>
//#include <mmsystem.h>


///////////////////
// Class name: FramesPerSec //
///////////////////
class FramesPerSec
{
public:
	FramesPerSec();
	FramesPerSec(const FramesPerSec&);
	~FramesPerSec();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

#endif