#include "d3Texture.h"
#include <string>

D3Texture::D3Texture(ID3D11Device* device)
	: m_Device(device)
	, m_TextureWidth(0)
	, m_TextureHeight(0)
	, m_NumSlices(0)
	, m_TextureResourceFormatSupport(0)
	, m_ShaderResourceViewFormatSupport(0)
	, m_RenderTargetViewFormatSupport(0)
	, m_TextureResourceFormat(DXGI_FORMAT_UNKNOWN)
	, m_ShaderResourceViewFormat(DXGI_FORMAT_UNKNOWN)
	, m_RenderTargetViewFormat(DXGI_FORMAT_UNKNOWN)
	, m_BPP(0)
	, m_Pitch(0)
	, m_IsTransparent(false)
{
	m_Device->GetImmediateContext(&m_DeviceContext);
}

D3Texture::~D3Texture()
{}

bool D3Texture::LoadTexture(TextureFormat textureFormat, char* fileName)
{
	bool result;

	m_FileName = fileName;

	// Get FreeImage Format and test for reading support.
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fileName);
	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(fileName);
	}

	if (fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif))
	{
		return false;
	}

	// Load the texture into a FreeImage bitmap.
	FIBITMAP* bmp = FreeImage_Load(fif, fileName);
	if (bmp == nullptr || FreeImage_HasPixels(bmp) == FALSE)
	{
		return false;
	}

	m_BPP = FreeImage_GetBPP(bmp);
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bmp);

	// Check for an alpha-channel in the texture.
	m_IsTransparent = (FreeImage_IsTransparent(bmp) == TRUE);

	// Select a DXGI texture format based on the texture BPP,
	// and the FreeImage image type.
	switch (m_BPP)
	{
	case 8:
	{
		switch (imageType)
		{
		case FIT_BITMAP:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R8_UNORM;
		}
		break;
		default:
		{
			return false;
		}
		break;
		}
	}
	break;
	case 16:
	{
		switch (imageType)
		{
		case FIT_BITMAP:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R8G8_UNORM;
		}
		break;
		case FIT_UINT16:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R16_UINT;
		}
		break;
		case FIT_INT16:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R16_SINT;
		}
		break;
		default:
		{
			return false;
		}
		break;
		}
	}
	break;
	case 32:
	{
		switch (imageType)
		{
		case FIT_BITMAP:
		{
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
			m_TextureResourceFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
#else
			m_TextureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
		}
		break;
		case FIT_FLOAT:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R32_FLOAT;
		}
		break;
		case FIT_INT32:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R32_SINT;
		}
		break;
		case FIT_UINT32:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R32_UINT;
		}
		break;
		default:
		{
			return false;
		}
		break;
		}
	}
	break;
	default:
	{
		FIBITMAP* bmp32 = FreeImage_ConvertTo32Bits(bmp);
		FreeImage_Unload(bmp);
		bmp = bmp32;
		m_BPP = FreeImage_GetBPP(bmp);

#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
		m_TextureResourceFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
#else
		m_TextureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
	}
	break;
	}

	m_TextureHeight = FreeImage_GetHeight(bmp);
	m_TextureWidth = FreeImage_GetWidth(bmp);
	m_NumSlices = 1;
	m_Pitch = FreeImage_GetPitch(bmp);
	
	m_ShaderResourceViewFormat = m_RenderTargetViewFormat = m_TextureResourceFormat;
	m_SampleDesc = GetSupportedSampleCount(m_TextureResourceFormat, 1);

	if (FAILED(m_Device->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport)))
	{
		return false;
	}
	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURE2D) == 0)
	{
		return false;
	}

	m_ShaderResourceViewFormatSupport = m_RenderTargetViewFormatSupport = m_TextureResourceFormatSupport;
 

	D3D11_TEXTURE2D_DESC textureDesc = { 0 };

	textureDesc.Width = m_TextureWidth;
	textureDesc.Height = m_TextureHeight;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = m_TextureResourceFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	if ((m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE) != 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}
	if ((m_RenderTargetViewFormatSupport & D3D11_FORMAT_SUPPORT_RENDER_TARGET) != 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	BYTE* textureData = FreeImage_GetBits(bmp);

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = textureData;
	subresourceData.SysMemPitch = m_Pitch;
	subresourceData.SysMemSlicePitch = 0;

	if (FAILED(m_Device->CreateTexture2D(&textureDesc, NULL, &m_Texture2D)))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	resourceViewDesc.Format = m_ShaderResourceViewFormat;
	resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = -1;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(m_Device->CreateShaderResourceView(m_Texture2D.Get(), &resourceViewDesc, &m_ShaderResourceView)))
	{
		return false;
	}

	m_DeviceContext->UpdateSubresource(m_Texture2D.Get(), 0, nullptr, textureData, m_Pitch, 0);
	m_DeviceContext->GenerateMips(m_ShaderResourceView.Get());

	FreeImage_Unload(bmp);

	return true;
}

DXGI_SAMPLE_DESC D3Texture::GetSupportedSampleCount(DXGI_FORMAT format, uint8_t numSamples)
{
	DXGI_SAMPLE_DESC sampleDesc;

	UINT sampleCount = 1;
	UINT qualityLevels = 0;

	while (sampleCount <= numSamples && SUCCEEDED(m_Device->CheckMultisampleQualityLevels(format, sampleCount, &qualityLevels)) && qualityLevels > 0)
	{
		sampleDesc.Count = sampleCount;
		sampleDesc.Quality = qualityLevels - 1;

		sampleCount = sampleCount * 2;
	}

	return sampleDesc;
}