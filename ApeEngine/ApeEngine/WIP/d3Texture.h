#ifndef _D3TEXTURE_H
#define _D3TEXTURE_H

#include <d3d11_2.h>
#include <FreeImage.h>
#include <Utilities.h>
#include <wrl.h>

class D3Texture
{
	enum class TextureFormat
	{
		R,
		RG,
		RGB,
		RGBA,
	};

public:
	D3Texture(ID3D11Device* device);
	~D3Texture();

	bool LoadTexture(TextureFormat textureFormat, char* fileName);

	DXGI_SAMPLE_DESC GetSupportedSampleCount(DXGI_FORMAT format, uint8_t numSamples);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UnorderedAccessView;

	char* m_FileName;
	uint8_t m_BPP;
	uint16_t m_Pitch;
	uint16_t m_TextureWidth;
	uint16_t m_TextureHeight;
	uint16_t m_NumSlices;

	UINT m_TextureResourceFormatSupport;
	UINT m_RenderTargetViewFormatSupport;
	UINT m_ShaderResourceViewFormatSupport;

	DXGI_FORMAT m_TextureResourceFormat;
	DXGI_FORMAT m_RenderTargetViewFormat;
	DXGI_FORMAT m_ShaderResourceViewFormat;

	DXGI_SAMPLE_DESC m_SampleDesc;

	ID3D11ShaderResourceView* m_TextureView;

	bool m_GenerateMipMaps;
	bool m_IsTransparent;
};

#endif