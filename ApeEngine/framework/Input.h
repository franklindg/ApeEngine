/////////////////////////////////////////////
// Filename: Input.h
/////////////////////////////////////////////
#ifndef _INPUT_H
#define _INPUT_H
#define DIRECTINPUT_VERSION 0x0800

//////////
// LINK DEPENDENCIES //
//////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//////////
// INCLUDES //
//////////
#include <dinput.h>
#include "ApePCH.h"

///////////////////
// Class name: Input //
///////////////////
class Input
{
public:
	Input(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	Input(const Input&);
	~Input();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsWPressed();
	bool IsAPressed();
	bool IsSPressed();
	bool IsDPressed();
	bool IsQPressed();
	bool IsEPressed();
	bool IsZPressed();
	bool IsPgUpPressed();
	bool IsPgDownPressed();
	bool IsF1Toggled();
	bool IsF2Toggled();
	bool IsF3Toggled();
	bool IsF4Toggled();

	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyBoard();
	bool ReadMouse();
	void ProcessInput();

private:
	Microsoft::WRL::ComPtr<IDirectInput8> m_pDirectInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_pKeyboardDevice;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_pMouseDevice;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_iScreenWidth, m_iScreenHeight;
	int m_iMousePosX, m_iMousePosY;
	bool m_bIsF1Released;
	bool m_bIsF2Released;
	bool m_bIsF3Released;
	bool m_bIsF4Released;
};

#endif
