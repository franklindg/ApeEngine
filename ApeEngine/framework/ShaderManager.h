/////////////////////////////////////////////
// Filename: ShaderManager.h
/////////////////////////////////////////////
#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H

//////////
// CLASS INCLUDES //
//////////
#include "D3DManager.h"
#include "../shader/ColorShader.h"
#include "../shader/TextureShader.h"
#include "../shader/LightShader.h"
#include "../shader/TerrainShader.h"
#include "../shader/SkydomeShader.h"
#include "../shader/FontShader.h"


///////////////////
// Class name: ShaderManager //
///////////////////
class ShaderManager
{
public:
	ShaderManager();
	ShaderManager(const ShaderManager&);
	~ShaderManager();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	
	bool RenderColorShader(ID3D11DeviceContext*, int, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4X4);

	bool RenderTextureShader(ID3D11DeviceContext*, int, CXMMATRIX, CXMMATRIX, CXMMATRIX, ID3D11ShaderResourceView*);

	bool RenderTerrainShader(ID3D11DeviceContext*, int, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4X4, ID3D11ShaderResourceView*,
							 ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);

	bool RenderSkyDomeShader(ID3D11DeviceContext*, int, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4, XMFLOAT4);

	//bool RenderLightShader(ID3D11DeviceContext2*, int, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4X4, ID3D11ShaderResourceView*, XMFLOAT3,
	//					   XMFLOAT4, XMFLOAT4, XMFLOAT3, XMFLOAT4, float);

	bool RenderFontShader(ID3D11DeviceContext*, int, CXMMATRIX, CXMMATRIX, CXMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);

private:
	ColorShader*	  m_ColorShader;
	TextureShader*	  m_TextureShader;
	TerrainShader*    m_TerrainShader;
	SkydomeShader*    m_SkydomeShader;
	LightShader*	  m_LightShader;
	FontShader*		  m_FontShader;
};

#endif
