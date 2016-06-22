/////////////////////////////////////////////
// Filename: ShaderManager.h
/////////////////////////////////////////////
#include "ShaderManager.h"


ShaderManager::ShaderManager()
	: m_pColorShader(nullptr)
	, m_pTextureShader(nullptr)
	, m_pTerrainShader(nullptr)
	, m_pSkydomeShader(nullptr)
	, m_pFontShader(nullptr)
{}

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
	m_pColorShader = std::make_shared<ColorShader>();
	if (!m_pColorShader)
	{
		return false;
	}

	result = m_pColorShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	/****** Texture Shader ******/
	m_pTextureShader = std::make_shared<TextureShader>();
	if (!m_pTextureShader)
	{
		return false;
	}

	result = m_pTextureShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	/****** Skydome Shader ******/
	m_pSkydomeShader = std::make_shared<SkydomeShader>();
	if (!m_pSkydomeShader)
	{
		return false;
	}

	result = m_pSkydomeShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	/****** Font Shader ******/
	m_pFontShader = std::make_shared<FontShader>();
	if (!m_pFontShader)
	{
		return false;
	}

	result = m_pFontShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void ShaderManager::Shutdown()
{
	if (m_pColorShader)
	{
		m_pColorShader->Shutdown();
		m_pColorShader.reset();
	}

	if (m_pTextureShader)
	{
		m_pTextureShader->Shutdown();
		m_pTextureShader.reset();
	}

	if (m_pTerrainShader)
	{
		m_pTerrainShader->Shutdown();
		m_pTerrainShader.reset();
	}

	if (m_pSkydomeShader)
	{
		m_pSkydomeShader->Shutdown();
		m_pSkydomeShader.reset();
	}

	if (m_pFontShader)
	{
		m_pFontShader->Shutdown();
		m_pFontShader.reset();
	}

	return;
}

bool ShaderManager::RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
									  XMFLOAT4X4 projectionMatrix)
{
	return m_pColorShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix);
}

bool ShaderManager::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
										CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	return m_pTextureShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

bool ShaderManager::RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
										XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap,
										ID3D11ShaderResourceView* normalMap2, ID3D11ShaderResourceView* normalMap3, XMFLOAT3 lightDirection,
										XMFLOAT4 diffuseColor)
{
	return m_pTerrainShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, normalMap, normalMap2,
								   normalMap3, lightDirection, diffuseColor);
}

bool ShaderManager::RenderSkyDomeShader(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
										XMFLOAT4X4 projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
	return m_pSkydomeShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
}

bool ShaderManager::RenderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
									 CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 color)
{
	return m_pFontShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, color);
}