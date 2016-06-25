#ifndef _TEXTURE_H
#define _TEXTURE_H

// INCLUDES
#include "ApePCH.h"

///////////////////
// Class name: Texture //
///////////////////
class Texture
{
public:
	struct Textures
	{
		Textures()
			: uiBPP(0)
			, uiTextureHeight(0)
			, uiTextureWidth(0)
			, uiPitch(0)
			, pTextures(nullptr)
			, pResourceViews(nullptr)
		{}

		uint8_t uiBPP;
		uint16_t uiTextureHeight;
		uint16_t uiTextureWidth;
		uint16_t uiPitch;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTextures;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pResourceViews;
		DXGI_FORMAT textureResourceFormat;
	};

	Texture(ID3D11Device* device);
	~Texture();

	bool LoadTexture(const char* filename);

	ID3D11ShaderResourceView* GetTexture(int id);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

	std::vector<Textures> m_Textures;
};

#endif