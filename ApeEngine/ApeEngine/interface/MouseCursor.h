/////////////////////////////////////////////
// Filename: MouseCursor.h
/////////////////////////////////////////////
#ifndef _MOUSE_CURSOR_H
#define _MOUSE_CURSOR_H


//////////
// INCLUDES //
//////////
#include <DirectXMath.h>
using namespace DirectX;

////////
// CLASS INCLUDES //
////////
#include "../render2d/Bitmap.h"
#include "../framework/ShaderManager.h"
#include "../framework/Input.h"
#include "../utilitiesfrw/Texture.h"

///////////////////
// Class name: MouseCursor //
///////////////////
class MouseCursor
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	MouseCursor();
	MouseCursor(const MouseCursor&);
	~MouseCursor();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX projMatrix, ID3D11ShaderResourceView* texture);

	void PositionUpdate(Input*);

private:
	int m_cursorLocationX, m_cursorLocationY;
	int m_screenWidth, m_screenHeight;
	float m_cursorSizeX, m_cursorSizeY;
	int m_mousePosX, m_mousePosY;
	Bitmap* m_CursorBitmap;
};

#endif