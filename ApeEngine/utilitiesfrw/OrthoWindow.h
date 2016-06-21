///////////////////////////////////////
// Filename: OrthoWindow.h
///////////////////////////////////////
#ifndef _ORTHO_WINDOW_H_
#define _ORTHO_WINDOW_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11_2.h>
#include <DirectXMath.h>
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
	OrthoWindow();
	OrthoWindow(const OrthoWindow&);
	~OrthoWindow();

	bool Initialize(ID3D11Device*, int, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*, int, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif
