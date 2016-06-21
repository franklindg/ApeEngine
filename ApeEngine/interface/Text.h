/////////////////////////////////////////////
// Filename: Text.h
/////////////////////////////////////////////
#ifndef _TEXT_H
#define _TEXT_H

////////
// CLASS INCLUDES //
////////
#include "apeFont.h"
#include "../framework/ShaderManager.h"


///////////////////
// Class name: Text //
///////////////////
class Text
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, int, bool, apeFont*, char*, int, int, float, float, float);
	void Shutdown();
	void Render(ID3D11DeviceContext*, ShaderManager*, CXMMATRIX, CXMMATRIX, CXMMATRIX, ID3D11ShaderResourceView*);

	bool UpdateSentence(ID3D11DeviceContext*, apeFont*, char*, int, int, float, float, float);
private:
	bool InitializeSentence(ID3D11Device*, ID3D11DeviceContext*, apeFont*, char*, int, int, float, float, float);
	void RenderSentence(ID3D11DeviceContext*, ShaderManager*, CXMMATRIX, CXMMATRIX, CXMMATRIX, ID3D11ShaderResourceView*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, *m_vertexBuffer2, *m_indexBuffer2;
	int m_screenWidth, m_screenHeight, m_maxLength, m_vertexCount, m_indexCount;
	bool m_shadow;
	XMFLOAT4 m_pixelColor;
};

#endif