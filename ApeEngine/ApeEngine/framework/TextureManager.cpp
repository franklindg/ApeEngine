/////////////////////////////////////////////
// Filename: TextureManager.cpp
/////////////////////////////////////////////
#include "TextureManager.h"

TextureManager::TextureManager(ID3D11Device* pDevice, int count)
	: m_TextureArray(0)
	, m_pDevice(pDevice)
{
	m_pDevice->GetImmediateContext(&m_pDeviceContext);
	m_textureCount = count;

	m_TextureArray[] = new Texture(m_pDevice.Get())[count];
	if (!m_TextureArray)
	{
		ReportError("Failed to initialize texture array.");
	}
}

TextureManager::TextureManager(const TextureManager& other)
{

}

TextureManager::~TextureManager()
{
	if (m_TextureArray)
	{
		for (int i = 0; i < m_textureCount; i++)
		{
			m_TextureArray[i].~Texture();
		}
		delete[] m_TextureArray;
		m_TextureArray = 0;
	}
}

bool TextureManager::LoadTexture(const char* fileName, int location)
{
	
	if (!m_TextureArray[location].Initialize(fileName))
	{
		ReportError("Failed to load texture.");
	}

	return true;
}

ID3D11ShaderResourceView* TextureManager::GetTexture(int id)
{
	return m_TextureArray[id].GetTexture();
}