/////////////////////////////////////////////
// Filename: ShaderManager.h
/////////////////////////////////////////////
#include "ShaderManager.h"


ShaderManager::ShaderManager(ID3D11Device* device, HWND hwnd)
	: m_pDevice(device)
	, m_pColorShader(nullptr)
	, m_pTextureShader(nullptr)
	, m_pTerrainShader(nullptr)
	, m_pSkydomeShader(nullptr)
	, m_pFontShader(nullptr)
{
	m_pDevice->GetImmediateContext(&m_pDeviceContext);

	/****** Color Shader ******/
	m_pColorShader = std::make_shared<ColorShader>();
	
	if (!m_pColorShader->Initialize(device, hwnd))
	{
		ReportError("Failed to initialize the color shader.");
	}

	/****** Texture Shader ******/
	m_pTextureShader = std::make_shared<TextureShader>();

	if (!m_pTextureShader->Initialize(device, hwnd))
	{
		ReportError("Failed to initialize the texture shader.");
	}

	/****** Skydome Shader ******/
	m_pSkydomeShader = std::make_shared<SkydomeShader>();

	if (!m_pSkydomeShader->Initialize(device, hwnd))
	{
		ReportError("Failed to initialize the skydome shader.");
	}

	/****** Font Shader ******/
	m_pFontShader = std::make_shared<FontShader>();

	if (!m_pFontShader->Initialize(device, hwnd))
	{
		ReportError("Failed to initialize the font shader.");
	}
}

ShaderManager::~ShaderManager()
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
}

bool ShaderManager::RenderColorShader(int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
									  XMFLOAT4X4 projectionMatrix)
{
	return m_pColorShader->Render(m_pDeviceContext.Get(), indexCount, worldMatrix, viewMatrix, projectionMatrix);
}

bool ShaderManager::RenderTextureShader(int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
										CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	return m_pTextureShader->Render(m_pDeviceContext.Get(), indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

bool ShaderManager::RenderTerrainShader(int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
										XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap,
										ID3D11ShaderResourceView* normalMap2, ID3D11ShaderResourceView* normalMap3, XMFLOAT3 lightDirection,
										XMFLOAT4 diffuseColor)
{
	return m_pTerrainShader->Render(m_pDeviceContext.Get(), indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, normalMap, normalMap2,
								   normalMap3, lightDirection, diffuseColor);
}

bool ShaderManager::RenderSkyDomeShader(int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
										XMFLOAT4X4 projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
	return m_pSkydomeShader->Render(m_pDeviceContext.Get(), indexCount, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
}

bool ShaderManager::RenderFontShader(int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
									 CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 color)
{
	return m_pFontShader->Render(m_pDeviceContext.Get(), indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, color);
}