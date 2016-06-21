/////////////////////////////////////////////
// Filename: Bitmap.h
/////////////////////////////////////////////
#ifndef _BITMAP_H
#define _BITMAP_H


//////////
// INCLUDES //
//////////
#include <d3d11_2.h>
#include <DirectXMath.h>
using namespace DirectX;

////////
// CLASS INCLUDES //
////////
#include "../utilitiesfrw/Texture.h"

///////////////////
// Class name: Bitmap //
///////////////////
class Bitmap
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	Bitmap();
	Bitmap(const Bitmap&);
	~Bitmap();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, int, int, char*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
	
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_prevPosX, m_prevPosY;
};

#endif
