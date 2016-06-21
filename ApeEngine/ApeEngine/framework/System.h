/////////////////////////////////////////////
// Filename: System.h
/////////////////////////////////////////////

#ifndef _SYSTEM_H
#define _SYSTEM_H

////////////
// PRE-PROCESSING DIRECTIVES //
////////////
#define WIN32_LEAN_AND_MEAN

/////////
// WINDOWS INCLUDES //
/////////
#include <Windows.h>

////////
// CLASS INCLUDES //
////////
#include "Application.h"

///////////////////
// Class name: System //
///////////////////
class System
{
public:
	System();
	System(const System&);
	~System();

	bool Initialize();
	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	WCHAR* m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	Application* m_Application;
};

////////
// FUNCTION PROTOTYPES //
////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

////////
// GLOBALS //
////////
static System* ApplicationHandle = 0;

#endif
