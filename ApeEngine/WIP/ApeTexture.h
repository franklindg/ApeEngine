#pragma once
#ifndef APETEXTURE_H
#define APETEXTURE_H

#include "../framework/ApePCH.h"

class ApeTexture
{
public:
	struct TextureData
	{
		TextureData()
			: mID(0)
			, mBPP(0)
			, mHeight(0)
			, mWidth(0)
			, mPitch(0)
			, mFormat(DXGI_FORMAT_UNKNOWN)
			, mTexture(nullptr)
			, mTextureView(nullptr)
		{}

		UINT mID;

		uint8_t mBPP;
		uint16_t mHeight;
		uint16_t mWidth;
		uint16_t mPitch;

		DXGI_FORMAT mFormat;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureView;
	};

	ApeTexture(ID3D11Device* pDevice);
	~ApeTexture();

	bool LoadTextureFromFile(UINT id, CHAR* filename);
	ID3D11ShaderResourceView* GetTexture(UINT id);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	std::vector<TextureData*> m_Textures;
};

#endif