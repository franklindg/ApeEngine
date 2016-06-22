/////////////////////////////////////////////
// Filename: Font.h
/////////////////////////////////////////////
/////////////////////////////////////////////
#pragma once
#ifndef _APEFONT_H
#define _APEFONT_H

//////////
// INCLUDES //
//////////
#include <DirectXMath.h>
#include <fstream>	
using namespace DirectX;
using namespace std;

////////
// CLASS INCLUDES //
////////
#include "../utilitiesfrw/Texture.h"


///////////////////
// Class name: Font //
///////////////////

class apeFont
{
private:
	struct FontType3
	{
		float left, right;
		int size;
	};

	struct VertexType2
	{
		XMFLOAT3 mPos;
		XMFLOAT2 mTexture;
	};

public:
	apeFont();
	apeFont(const apeFont&);
	~apeFont();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, float, int);
	void Shutdown();

	
	void BuildVertexArray(void*, char*, float, float);
	int GetSentencePixelLength(char*);
	int GetFontHeight();

private:
	bool LoadFontData(char*);
	void ReleaseFontData();

private:
	FontType3* m_Font;
	Texture* m_FontTexture;
	float m_fontHeight;
	int m_spaceSize;
};

#endif
