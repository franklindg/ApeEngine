#include "ApeTexture.h"



ApeTexture::ApeTexture(ID3D11Device* pDevice)
	: m_pDevice(pDevice)
{
	m_pDevice->GetImmediateContext(&m_pDeviceContext);
}


ApeTexture::~ApeTexture()
{
	m_Textures.clear();
	m_pDevice.Reset();
	m_pDeviceContext.Reset();
}

bool ApeTexture::LoadTextureFromFile(UINT ID, CHAR* filename)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// Initialize the texture object.
	TextureData* texture = new TextureData;
	if (!texture)
	{
		return false;
	}

	// Gets the FreeImage format from the filename.
	// Returns false if the format is unrecognized.
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename);
	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(filename);
	}

	if (fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif))
	{
		return false;
	}

	// Loads texture data into a FreeImage bitmap.
	// Returns false if no pixel data is received.
	/* TODO: Debugging functionality. */
	FIBITMAP* dib = FreeImage_Load(fif, filename);
	if (dib == nullptr || FreeImage_HasPixels(dib) == FALSE)
	{
		return false;
	}

	// The bitmap data is flipped vertically since FreeImage
	// reads the data from bottom to top.
	FreeImage_FlipVertical(dib);

	texture->mBPP = FreeImage_GetBPP(dib);
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(dib);

	switch (texture->mBPP)
	{
	case 8:
	{
		switch (imageType)
		{
		case FIT_BITMAP:
		{
			texture->mFormat = DXGI_FORMAT_R8_UNORM;
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
			texture->mFormat = DXGI_FORMAT_R8G8_UNORM;
		}
		break;
		case FIT_UINT16:
		{
			texture->mFormat = DXGI_FORMAT_R16_UINT;
		}
		break;
		case FIT_INT16:
		{
			texture->mFormat = DXGI_FORMAT_R16_SINT;
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
			texture->mFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
#else
			texture->mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
		}
		break;
		case FIT_FLOAT:
		{
			texture->mFormat = DXGI_FORMAT_R32_FLOAT;
		}
		break;
		case FIT_INT32:
		{
			texture->mFormat = DXGI_FORMAT_R32_SINT;
		}
		break;
		case FIT_UINT32:
		{
			texture->mFormat = DXGI_FORMAT_R32_UINT;
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
		texture->mBPP = FreeImage_GetBPP(dib);

#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
		texture->mFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
#else
		texture->mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
	}
	break;
	}

	// Gets the height, width, and pitch from the flipped
	// texture data.
	texture->mHeight = FreeImage_GetHeight(dib);
	texture->mWidth = FreeImage_GetWidth(dib);
	texture->mPitch = FreeImage_GetPitch(dib);

	// Sets the 2D texture description with the bitmap data,
	// and creates and empty 2D texture.
	/* TODO: Add multiple file type functionality. */
	textureDesc.Height = texture->mHeight;
	textureDesc.Width = texture->mWidth;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = texture->mFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(m_pDevice->CreateTexture2D(&textureDesc, NULL, &texture->mTexture)))
	{
		return false;
	}

	BYTE* textureData = FreeImage_GetBits(dib);
	m_pDeviceContext->UpdateSubresource(texture->mTexture.Get(), 0, NULL, textureData, texture->mPitch, 0);

	// Sets the Shader Resource View description,
	// and creates the Shader Resource View.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = -1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(m_pDevice->CreateShaderResourceView(texture->mTexture.Get(), &srvDesc, &texture->mTextureView)))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	m_pDeviceContext->GenerateMips(texture->mTextureView.Get());

	// Adds data to the texture vector.
	m_Textures.push_back(texture);

	// Free reserved memory.
	delete texture;
	texture = 0;
	FreeImage_Unload(dib);

	return true;
}

ID3D11ShaderResourceView* ApeTexture::GetTexture(UINT id)
{
	return m_Textures[id]->mTextureView.Get();
}
