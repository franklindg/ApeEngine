#ifndef _SYSTEM_H
#define _SYSTEM_H

// INCLUDES 
#include "ApePCH.h"
#include "Application.h"

///////////////////
// Class name: System //
///////////////////
class System
{
public:
	System();
	~System();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	WCHAR* m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	std::shared_ptr<Application> m_pApplication;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GLOBAL VARIABLES
static System* ApplicationHandle = 0;

#endif
