/////////////////////////////////////////////
// Filename: UserInterface.h
/////////////////////////////////////////////
#ifndef _USER_INTERFACE_H
#define _USER_INTERFACE_H


//////////
// CLASS INCLUDES //
//////////
#include "Input.h"
#include "../utilitiesfrw/Texture.h"
#include "../interface/Text.h"
#include "../interface/MiniMap.h"
#include "../interface/MouseCursor.h"

///////////////////
// Class name: UserInterface //
///////////////////
class UserInterface
{
public:
	UserInterface();
	UserInterface(const UserInterface&);
	~UserInterface();

	bool Initialize(D3DManager*, int, int);
	void Shutdown();

	bool Frame(ID3D11DeviceContext*, Input*, int, float, float, float, float, float, float);
	bool Render(D3DManager* direct3D, ShaderManager* shaderManager, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX projMatrix, 
				ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture2);

	bool UpdateRenderCounts(ID3D11DeviceContext*, int, int, int);

private:
	bool UpdateFpsString(ID3D11DeviceContext*, int);
	bool UpdatePositionStrings(ID3D11DeviceContext*, float, float, float, float, float, float);

private:
	apeFont* m_pFont;
	Text* m_pFpsString;
	Text* m_pVideoStrings;
	Text* m_pPositionStrings;
	Text* m_pRenderCountStrings;
	MouseCursor* m_pMouseCursor;
	int m_previousFps;
	int m_previousPosition[6];
};

#endif