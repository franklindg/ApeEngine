#include "Input.h"

Input::Input(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
	: m_pDirectInput(nullptr)
	, m_pKeyboardDevice(nullptr)
	, m_pMouseDevice(nullptr)
	, m_iScreenWidth(screenWidth)
	, m_iScreenHeight(screenHeight)
	, m_iMousePosX(0)
	, m_iMousePosY(0)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL)))
	{
		ReportError("Failed to create input instance.");
	}

	if(FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL)))
	{
		ReportError("Failed to create input device");
	}

	if(FAILED(m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		ReportError("Failed to set keyboard data format.");
	}

	if(FAILED(m_pKeyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
	{
		ReportError("Failed to set keyboard cooperative level.");
	}

	if (FAILED(m_pKeyboardDevice->Acquire()))
	{
		ReportError("Failed to acquire keyboard.");
	}

	if(FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL)))
	{
		ReportError("Failed to create mouse device");
	}

	if(FAILED(m_pMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		ReportError("Failed to set mouse data format.");
	}


	if(FAILED(m_pMouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		ReportError("Failed to set mouse cooperative level.");
	}


	if(FAILED(m_pMouseDevice->Acquire()))
	{
		ReportError("Failed to acquire mouse.");
	}
}

Input::~Input()
{
	if (m_pMouseDevice)
	{
		m_pMouseDevice->Unacquire();
		m_pMouseDevice.Reset();
	}

	if (m_pKeyboardDevice)
	{
		m_pKeyboardDevice->Unacquire();
		m_pKeyboardDevice.Reset();
	}

	if (m_pDirectInput)
	{
		m_pDirectInput.Reset();
	}
}

bool Input::Frame()
{
	if (!ReadKeyBoard())
		return false;

	if (!ReadMouse())
		return false;

	ProcessInput();

	return true;
}

bool Input::ReadKeyBoard()
{
	HRESULT result;

	result = m_pKeyboardDevice->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_pKeyboardDevice->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT result;

	result = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_pMouseDevice->Acquire();
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

void Input::ProcessInput()
{
	m_iMousePosX += m_mouseState.lX;
	m_iMousePosY += m_mouseState.lY;

	if (m_iMousePosX < 0) { m_iMousePosX = 0; }
	if (m_iMousePosY < 0) { m_iMousePosY = 0; }
	if (m_iMousePosX > m_iScreenWidth) { m_iMousePosX = m_iScreenWidth; }
	if (m_iMousePosY > m_iScreenHeight) { m_iMousePosY = m_iScreenHeight; }

	return;
}

bool Input::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsLeftPressed()
{
	if (m_keyboardState[DIK_LEFTARROW] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsRightPressed()
{
	if (m_keyboardState[DIK_RIGHTARROW] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsUpPressed()
{
	if (m_keyboardState[DIK_UPARROW] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsDownPressed()
{
	if (m_keyboardState[DIK_DOWNARROW] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsWPressed()
{
	if (m_keyboardState[DIK_W] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsAPressed()
{
	if (m_keyboardState[DIK_A] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsSPressed()
{
	if (m_keyboardState[DIK_S] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsDPressed()
{
	if (m_keyboardState[DIK_D] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsQPressed()
{
	if (m_keyboardState[DIK_Q] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsEPressed()
{
	if (m_keyboardState[DIK_E] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsZPressed()
{
	if (m_keyboardState[DIK_Z] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsPgUpPressed()
{
	if (m_keyboardState[DIK_PGUP] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsPgDownPressed()
{
	if (m_keyboardState[DIK_PGDN] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsF1Toggled()
{
	if (m_keyboardState[DIK_F1] & 0x80)
	{
		if (m_bIsF1Released)
		{
			m_bIsF1Released = false;
			return true;
		}
	}
	else
	{
		m_bIsF1Released = true;
	}

	return false;
}

bool Input::IsF2Toggled()
{
	if (m_keyboardState[DIK_F2] & 0x80)
	{
		if (m_bIsF2Released)
		{
			m_bIsF2Released = false;
			return true;
		}
	}
	else
	{
		m_bIsF2Released = true;
	}

	return false;
}

bool Input::IsF3Toggled()
{
	if (m_keyboardState[DIK_F3] & 0x80)
	{
		if (m_bIsF3Released)
		{
			m_bIsF3Released = false;
			return true;
		}
	}
	else
	{
		m_bIsF3Released = true;
	}

	return false;
}

bool Input::IsF4Toggled()
{
	if (m_keyboardState[DIK_F4] & 0x80)
	{
		if (m_bIsF4Released)
		{
			m_bIsF4Released = false;
			return true;
		}
	}
	else
	{
		m_bIsF4Released = true;
	}

	return false;
}

void Input::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_iMousePosX;
	mouseY = m_iMousePosY;

	return;
}