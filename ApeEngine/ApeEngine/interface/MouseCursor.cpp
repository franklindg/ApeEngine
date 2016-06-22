/////////////////////////////////////////////
// Filename: MouseCursor.cpp
/////////////////////////////////////////////
#include "MouseCursor.h"

MouseCursor::MouseCursor()
{
	m_mousePosX = 0;
	m_mousePosY = 0;
	m_CursorBitmap = 0;
}

MouseCursor::MouseCursor(const MouseCursor& other)
{

}

MouseCursor::~MouseCursor()
{

}

bool MouseCursor::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight)
{
	bool result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_cursorSizeX = 100.0f;
	m_cursorSizeY = 100.0f;

	m_cursorLocationX = m_mousePosX;
	m_cursorLocationY = m_mousePosY;

	m_CursorBitmap = new Bitmap;
	if (!m_CursorBitmap)
	{
		return false;
	}

	result = m_CursorBitmap->Initialize(device, deviceContext, screenWidth, screenHeight, 101, 101);
	if (!result)
	{
		return false;
	}

	return true;
}

void MouseCursor::Shutdown()
{
	if (m_CursorBitmap)
	{
		m_CursorBitmap->Shutdown();
		delete m_CursorBitmap;
		m_CursorBitmap = 0;
	}

	return;
}

bool MouseCursor::Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, CXMMATRIX worldMatrix,
	CXMMATRIX viewMatrix, CXMMATRIX projMatrix, ID3D11ShaderResourceView* texture)
{
	bool result;

	result = m_CursorBitmap->Render(deviceContext, m_cursorLocationX, m_cursorLocationY);
	if (!result)
	{
		return false;
	}

	result = shaderManager->RenderTextureShader(m_CursorBitmap->GetIndexCount(), worldMatrix, viewMatrix, projMatrix,
												texture);
	if (!result)
	{
		return false;
	}

	return true;
}

void MouseCursor::PositionUpdate(Input* input)
{
	
	input->GetMouseLocation(m_mousePosX, m_mousePosY);

	if (m_mousePosX < 0)
	{
		m_mousePosX = 0;
	}

	if (m_mousePosY < 0)
	{
		m_mousePosY = 0;
	}

	if (m_mousePosX > m_screenWidth)
	{
		m_mousePosX = m_screenWidth;
	}

	if (m_mousePosY > m_screenHeight)
	{
		m_mousePosY = m_screenHeight;
	}

	m_cursorLocationX = m_mousePosX;
	m_cursorLocationY = m_mousePosY;

	return;
}

