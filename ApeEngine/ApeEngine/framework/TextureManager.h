#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H


// INCLUDES 
#include "../utilitiesfrw/Texture.h"
#include "ApePCH.h"

///////////////////
// Class name: TextureManager //
///////////////////
class TextureManager
{
public:
	TextureManager(ID3D11Device* pDevice, int count);
	TextureManager(const TextureManager&);	
	~TextureManager();

	bool LoadTexture(const char*, int);
	ID3D11ShaderResourceView* GetTexture(int);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Texture* m_TextureArray;
	
	int m_textureCount;
};

#endif