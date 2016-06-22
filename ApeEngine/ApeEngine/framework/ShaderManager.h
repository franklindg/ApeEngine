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
	ShaderManager(ID3D11Device* device, HWND hwnd);
	~ShaderManager();

	bool Initialize(ID3D11Device*, HWND);
	
	bool RenderColorShader(int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
						   XMFLOAT4X4 projectionMatrix);

	bool RenderTextureShader(int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
							 CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

	bool RenderTerrainShader(int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
							 XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap,
							 ID3D11ShaderResourceView* normalMap2, ID3D11ShaderResourceView* normalMap3, XMFLOAT3 lightDirection,
							 XMFLOAT4 diffuseColor);

	bool RenderSkyDomeShader(int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
							 XMFLOAT4X4 projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor);

	bool RenderFontShader(int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
						  CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 color);

private:
	std::shared_ptr<ColorShader>	m_pColorShader;
	std::shared_ptr<TextureShader>	m_pTextureShader;
	std::shared_ptr<TerrainShader>	m_pTerrainShader;
	std::shared_ptr<SkydomeShader>	m_pSkydomeShader;
	std::shared_ptr<FontShader>		m_pFontShader;

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
};

#endif
