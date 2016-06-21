/////////////////////////////////////////////
// Filename: Texture.h
/////////////////////////////////////////////
#ifndef _TEXTURE_H
#define _TEXTURE_H

//////////
// INCLUDES //
//////////

#include "../framework/ApePCH.h"

///////////////////
// Class name: Texture //
///////////////////
class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	uint8_t m_BPP;
	uint16_t m_TextureHeight;
	uint16_t m_TextureWidth;
	uint16_t m_Pitch;

	DXGI_FORMAT m_TextureResourceFormat;

	ID3D11Texture2D* m_Texture;
	ID3D11ShaderResourceView* m_TextureView;
};

#endif