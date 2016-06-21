/////////////////////////////////////////////
// Filename: TextureManager.cpp
/////////////////////////////////////////////
#include "TextureManager.h"

TextureManager::TextureManager(ID3D11Device* pDevice)
	: m_TextureArray(0)
	, m_pDevice(pDevice)
{
	m_pDevice->GetImmediateContext(&m_pDeviceContext);
}

TextureManager::TextureManager(const TextureManager& other)
{

}

TextureManager::~TextureManager()
{

}

bool TextureManager::Initialize(int count)
{
	m_textureCount = count;

	m_TextureArray = new Texture[m_textureCount];
	if (!m_TextureArray)
	{
		ReportError("Failed to initialize texture array.");
	}

	return true;
}

void TextureManager::Shutdown()
{
	int i;

	if (m_TextureArray)
	{
		for (i = 0; i < m_textureCount; i++)
		{
			m_TextureArray[i].Shutdown();
		}
		delete[] m_TextureArray;
		m_TextureArray = 0;
	}

	return;
}

bool TextureManager::LoadTexture(const char* fileName, int location)
{
	
	if (!m_TextureArray[location].Initialize(m_pDevice.Get(), m_pDeviceContext.Get(), fileName))
	{
		ReportError("Failed to load texture.");
	}

	return true;
}

ID3D11ShaderResourceView* TextureManager::GetTexture(int id)
{
	return m_TextureArray[id].GetTexture();
}