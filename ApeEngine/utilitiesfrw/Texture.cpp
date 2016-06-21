/////////////////////////////////////////////
// Filename: Texture.cpp
/////////////////////////////////////////////
#include "Texture.h"

Texture::Texture()
	: m_Texture(0)
	, m_TextureView(0)
	, m_TextureResourceFormat(DXGI_FORMAT_UNKNOWN)
	, m_TextureHeight(0)
	, m_TextureWidth(0)
	, m_Pitch(0)
{}

Texture::Texture(const Texture& other)
{

}

Texture::~Texture()
{

}

bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	std::string basePath = "data/textures/";
	std::string fullPath = basePath + filename;

	// Gets the FreeImage format from the filename.
	// Returns false if the format is unrecognized.
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fullPath.c_str());
	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(fullPath.c_str());
	}

	if (fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif))
	{
		return false;
	}

	// Loads texture data into a FreeImage bitmap.
	// Returns false if no pixel data is received.
	/* TODO: Debugging functionality. */
	FIBITMAP* dib = FreeImage_Load(fif, fullPath.c_str());
	if (dib == nullptr || FreeImage_HasPixels(dib) == FALSE)
	{
		return false;
	}

	// The bitmap data is flipped vertically since FreeImage
	// reads the data from bottom to top.
	FreeImage_FlipVertical(dib);

	m_BPP = FreeImage_GetBPP(dib);
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(dib);

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
		FIBITMAP* dib32 = FreeImage_ConvertTo32Bits(dib);
		FreeImage_Unload(dib);
		dib = dib32;
		m_BPP = FreeImage_GetBPP(dib);

#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
		m_TextureResourceFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
#else
		m_TextureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
	}
	break;
	}

	// Gets the height, width, and pitch from the flipped
	// texture data.
	m_TextureHeight = FreeImage_GetHeight(dib);
	m_TextureWidth = FreeImage_GetWidth(dib);
	m_Pitch = FreeImage_GetPitch(dib);
	
	// Sets the 2D texture description with the bitmap data,
	// and creates and empty 2D texture.
	/* TODO: Add multiple file type functionality. */
	textureDesc.Height = m_TextureHeight;
	textureDesc.Width = m_TextureWidth;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = m_TextureResourceFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(device->CreateTexture2D(&textureDesc, NULL, &m_Texture)))
	{
		return false;
	}

	BYTE* textureData = FreeImage_GetBits(dib);
	deviceContext->UpdateSubresource(m_Texture, 0, NULL, textureData, m_Pitch, 0);

	// Sets the Shader Resource View description,
	// and creates the Shader Resource View.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = -1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(device->CreateShaderResourceView(m_Texture, &srvDesc, &m_TextureView)))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_TextureView);


	FreeImage_Unload(dib);

	return true;
}

void Texture::Shutdown()
{
	if (m_TextureView)
	{
		m_TextureView->Release();
		m_TextureView = 0;
	}

	if (m_Texture)
	{
		m_Texture->Release();
		m_Texture = 0;
	}

	if (m_TextureResourceFormat)
	{
		m_TextureResourceFormat = DXGI_FORMAT_UNKNOWN;
	}

	if (m_TextureHeight)
	{
		m_TextureHeight = 0;
	}

	if (m_TextureWidth)
	{
		m_TextureWidth = 0;
	}

	if (m_Pitch)
	{
		m_Pitch = 0;
	}

	return;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_TextureView;
}