#ifndef _ORTHO_WINDOW_H_
#define _ORTHO_WINDOW_H_

// INCLUDES 
#include "../framework/ApePCH.h"

using namespace DirectX;


///////////////////////////////////////
// Class name: OrthoWindow
///////////////////////////////////////
class OrthoWindow
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	OrthoWindow(ID3D11Device* device, int windowWidth, int windowHeight);
	~OrthoWindow();

	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*, int, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	int m_iVertexCount, m_iIndexCount;
};

#endif
