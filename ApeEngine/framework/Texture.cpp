#include <ApePCH.h>
#include "Texture.h"

Texture::Texture(ID3D11Device* device)
	: m_pDevice(device)
{
	m_pDevice->GetImmediateContext(&m_pDeviceContext);
}

Texture::~Texture()
{
	m_pDevice.Reset();
	m_pDeviceContext.Reset();

	for (UINT i = 0; i < m_Textures.size(); i++)
	{
		m_Textures.at(i).pTextures.Reset();
		m_Textures.at(i).pResourceViews.Reset();
		m_Textures.at(i).textureResourceFormat = DXGI_FORMAT_UNKNOWN;
		m_Textures.at(i).uiTextureHeight = 0;
		m_Textures.at(i).uiTextureWidth = 0;
		m_Textures.at(i).uiBPP = 0;
		m_Textures.at(i).uiPitch = 0;
	}

	m_Textures.clear();
}

bool Texture::LoadTexture(const char* filename)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	Textures textures;
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

	textures.uiBPP = FreeImage_GetBPP(dib);
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(dib);

	switch (textures.uiBPP)
	{
	case 8:
	{
		switch (imageType)
		{
		case FIT_BITMAP:
		{
			textures.textureResourceFormat = DXGI_FORMAT_R8_UNORM;
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
			textures.textureResourceFormat = DXGI_FORMAT_R8G8_UNORM;
		}
		break;
		case FIT_UINT16:
		{
			textures.textureResourceFormat = DXGI_FORMAT_R16_UINT;
		}
		break;
		case FIT_INT16:
		{
			textures.textureResourceFormat = DXGI_FORMAT_R16_SINT;
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
			textures.textureResourceFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
#else
			textures.textureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
		}
		break;
		case FIT_FLOAT:
		{
			textures.textureResourceFormat = DXGI_FORMAT_R32_FLOAT;
		}
		break;
		case FIT_INT32:
		{
			textures.textureResourceFormat = DXGI_FORMAT_R32_SINT;
		}
		break;
		case FIT_UINT32:
		{
			textures.textureResourceFormat = DXGI_FORMAT_R32_UINT;
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
		textures.uiBPP = FreeImage_GetBPP(dib);

#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
		textures.textureResourceFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
#else
		textures.textureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
	}
	break;
	}

	// Gets the height, width, and pitch from the flipped
	// texture data.
	textures.uiTextureHeight = FreeImage_GetHeight(dib);
	textures.uiTextureWidth = FreeImage_GetWidth(dib);
	textures.uiPitch = FreeImage_GetPitch(dib);

	// Sets the 2D texture description with the bitmap data,
	// and creates and empty 2D texture.
	/* TODO: Add multiple file type functionality. */
	textureDesc.Height = textures.uiTextureHeight;
	textureDesc.Width = textures.uiTextureWidth;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = textures.textureResourceFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(m_pDevice->CreateTexture2D(&textureDesc, NULL, &textures.pTextures)))
	{
		return false;
	}

	BYTE* textureData = FreeImage_GetBits(dib);
	m_pDeviceContext->UpdateSubresource(textures.pTextures.Get(), 0, NULL, textureData, textures.uiPitch, 0);

	// Sets the Shader Resource View description,
	// and creates the Shader Resource View.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = -1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(m_pDevice->CreateShaderResourceView(textures.pTextures.Get(), &srvDesc, &textures.pResourceViews)))
		return false;

	m_pDeviceContext->GenerateMips(textures.pResourceViews.Get());

	m_Textures.push_back(textures);

	FreeImage_Unload(dib);

	return true;
}

ID3D11ShaderResourceView* Texture::GetTexture(int id)
{
	return m_Textures.at(id).pResourceViews.Get();
}