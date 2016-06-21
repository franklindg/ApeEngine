/////////////////////////////////////////////
// Filename: MiniMap.cpp
/////////////////////////////////////////////
#include "MiniMap.h"

MiniMap::MiniMap()
{
	m_MiniMapBitmap = 0;
	m_PointBitmap = 0;
}

MiniMap::MiniMap(const MiniMap& other)
{

}

MiniMap::~MiniMap()
{

}

bool MiniMap::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight,
					float terrainWidth, float terrainHeight)
{
	bool result;

	m_mapSizeX = 150.0f;
	m_mapSizeY = 150.0f;

	m_mapLocationX = screenWidth - (int)m_mapSizeX - 10;
	m_mapLocationY = 10;

	m_terrainWidth = terrainWidth;
	m_terrainHeight = terrainHeight;

	m_MiniMapBitmap = new Bitmap;
	if (!m_MiniMapBitmap)
	{
		return false;
	}

	result = m_MiniMapBitmap->Initialize(device, deviceContext, screenWidth, screenHeight, 154, 154, "minimap.tga");
	if (!result)
	{
		return false;
	}

	m_PointBitmap = new Bitmap;
	if (!m_PointBitmap)
	{
		return false;
	}

	result = m_PointBitmap->Initialize(device, deviceContext, screenWidth, screenHeight, 3, 3, "point.tga");
	if (!result)
	{
		return false;
	}

	return true;
}

void MiniMap::Shutdown()
{
	if (m_PointBitmap)
	{
		m_PointBitmap->Shutdown();
		delete m_PointBitmap;
		m_PointBitmap = 0;
	}

	if (m_MiniMapBitmap)
	{
		m_MiniMapBitmap->Shutdown();
		delete m_MiniMapBitmap;
		m_MiniMapBitmap = 0;
	}

	return;
}

bool MiniMap::Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, CXMMATRIX worldMatrix,
					 CXMMATRIX viewMatrix, CXMMATRIX orthoMatrix)
{
	bool result;

	result = m_MiniMapBitmap->Render(deviceContext, m_mapLocationX, m_mapLocationY);
	if (!result)
	{
		return false;
	}

	result = shaderManager->RenderTextureShader(deviceContext, m_MiniMapBitmap->GetIndexCount(), worldMatrix, viewMatrix,
		orthoMatrix, m_MiniMapBitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	result = m_PointBitmap->Render(deviceContext, m_pointLocationX, m_pointLocationY);
	if (!result)
	{
		return false;
	}

	result = shaderManager->RenderTextureShader(deviceContext, m_PointBitmap->GetIndexCount(), worldMatrix, viewMatrix,
		orthoMatrix, m_PointBitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	return true;
}

void MiniMap::PositionUpdate(float positionX, float positionZ)
{
	float percentX, percentY;

	if (positionX < 0)
	{
		positionX = 0;
	}

	if (positionZ < 0)
	{
		positionZ = 0;
	}

	if (positionX > m_terrainWidth)
	{
		positionX = m_terrainWidth;
	}

	if (positionZ > m_terrainHeight)
	{
		positionZ = m_terrainHeight;
	}

	percentX = positionX / m_terrainWidth;
	percentY = 1.0f - positionZ / m_terrainHeight;

	m_pointLocationX = (m_mapLocationX + 2) + (int)(percentX * m_mapSizeX);
	m_pointLocationY = (m_mapLocationY + 2) + (int)(percentY * m_mapSizeY);

	m_pointLocationX = m_pointLocationX - 1;
	m_pointLocationY = m_pointLocationY - 1;

	return;
}