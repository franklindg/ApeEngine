/////////////////////////////////////////////
// Filename: ShaderManager.h
/////////////////////////////////////////////
#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
	m_ColorShader = 0;
	m_TextureShader = 0;
	m_TerrainShader = 0;
	m_SkydomeShader = 0;
	//m_LightShader = 0;
	m_FontShader = 0;
}

ShaderManager::ShaderManager(const ShaderManager& other)
{

}

ShaderManager::~ShaderManager()
{

}

bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	/****** Color Shader ******/
	m_ColorShader = new ColorShader;
	if (!m_ColorShader)
	{
		return false;
	}

	result = m_ColorShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	/****** Texture Shader ******/
	m_TextureShader = new TextureShader;
	if (!m_TextureShader)
	{
		return false;
	}

	result = m_TextureShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	///****** Terrain Shader ******/
	//m_TerrainShader = new TerrainShader;
	//if (!m_TerrainShader)
	//{
	//	return false;
	//}

	//result = m_TerrainShader->Initialize(device, hwnd);
	//if (!result)
	//{
	//	return false;
	//}

	/****** Skydome Shader ******/
	m_SkydomeShader = new SkydomeShader;
	if (!m_SkydomeShader)
	{
		return false;
	}

	result = m_SkydomeShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	///****** Light Shader ******/
	//m_LightShader = new LightShader;
	//if (!m_LightShader)
	//{
	//	return false;
	//}

	//result = m_LightShader->Initialize(device, hwnd);
	//if (!result)
	//{
	//	return false;
	//}


	/****** Font Shader ******/
	m_FontShader = new FontShader;
	if (!m_FontShader)
	{
		return false;
	}

	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void ShaderManager::Shutdown()
{
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	if (m_SkydomeShader)
	{
		m_SkydomeShader->Shutdown();
		delete m_SkydomeShader;
		m_SkydomeShader = 0;
	}

	/*if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}*/

	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	return;
}

bool ShaderManager::RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
									  XMFLOAT4X4 projectionMatrix)
{
	return m_ColorShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix);
}

bool ShaderManager::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
										CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	return m_TextureShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

bool ShaderManager::RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
										XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap,
										ID3D11ShaderResourceView* normalMap2, ID3D11ShaderResourceView* normalMap3, XMFLOAT3 lightDirection,
										XMFLOAT4 diffuseColor)
{
	return m_TerrainShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, normalMap, normalMap2,
								   normalMap3, lightDirection, diffuseColor);
}

bool ShaderManager::RenderSkyDomeShader(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
										XMFLOAT4X4 projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
	return m_SkydomeShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
}

//bool ShaderManager::RenderLightShader(ID3D11DeviceContext2* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
//									  XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, XMFLOAT3 lightDirection)
//{
//	return m_LightShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, colorTexture, normalTexture, lightDirection);
//}

bool ShaderManager::RenderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
									 CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 color)
{
	return m_FontShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, color);
}