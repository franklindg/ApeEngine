#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H

// CLASS INCLUDES
#include "ApePCH.h"
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

	bool RenderFontShader(ID3D11DeviceContext*, int, CXMMATRIX, CXMMATRIX, CXMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);

private:
	std::shared_ptr<ColorShader>	m_pColorShader;
	std::shared_ptr<TextureShader>	m_pTextureShader;
	std::shared_ptr<TerrainShader>	m_pTerrainShader;
	std::shared_ptr<SkydomeShader>	m_pSkydomeShader;
	std::shared_ptr<FontShader>		m_pFontShader;
};

#endif
