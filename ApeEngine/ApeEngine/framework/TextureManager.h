/////////////////////////////////////////////
// Filename: TextureManager.h
/////////////////////////////////////////////
/////////////////////////////////////////////
#pragma once
#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

//////////
// CLASS INCLUDES //
//////////
#include "../utilitiesfrw/Texture.h"
#include "ApePCH.h"

///////////////////
// Class name: TextureManager //
///////////////////
class TextureManager
{
public:
	TextureManager(ID3D11Device* pDevice);
	TextureManager(const TextureManager&);	
	~TextureManager();

	bool Initialize(int);
	void Shutdown();

	bool LoadTexture(const char*, int);
	bool LoadTextureA(UINT ID, char* filename);
	ID3D11ShaderResourceView* GetTexture(int);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Texture* m_TextureArray;
	
	int m_textureCount;
};

#endif