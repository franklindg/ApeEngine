#pragma once
#ifndef MATERIALS_H
#define MATERIALS_H

#include "Texture.h"
#include "../framework/ApePCH.h"
using namespace DirectX;

class Materials
{
public:
	enum class TextureValue
	{
		Ambient		  = 0,
		Emissive	  = 1,
		Diffuse		  = 2,
		Specular	  = 3,
		SpecularPower = 4,
		Normal		  = 5,
		Bump		  = 6,
		Opacity		  = 7,
	};

	Materials();
	~Materials();

	void SetTexture(TextureValue ID, std::shared_ptr<Texture> texture);
	void SetAmbient(XMFLOAT4 color);
	void SetEmissive(XMFLOAT4 color);
	void SetDiffuse(XMFLOAT4 color);
	void SetSpecular(XMFLOAT4 color);
	void SetSpecularPower(float specularPower);
	void SetOpacity(float opacity);
	void SetIndexOfRefraction(float IoF);
	void SetBumpIntensity(float bumpIntensity);
	void SetReflectance(XMFLOAT4 reflectance);

private:
	__declspec(align(16)) struct MatProperties
	{
		MatProperties()
			: m_GlobalAmbient(0.1f, 0.1f, 0.15f, 1.0f)
			, m_AmbientColor(0.0f, 0.0f, 0.0f, 1.0f)
			, m_EmissiveColor(0.0f, 0.0f, 0.0f, 1.0f)
			, m_DiffuseColor(1.0f, 1.0f, 1.0f, 1.0f)
			, m_SpecularColor(0.0f, 0.0f, 0.0f, 1.0f)
			, m_Reflectance(0.0f, 0.0f, 0.0f, 0.0f)
			, m_Opacity(1.0f)
			, m_SpecularPower(-1.0f)
			, m_IndexOfRefraction(-1.0f)
			, m_HasAmbientTexture(false)
			, m_HasEmissiveTexture(false)
			, m_HasDiffuseTexture(false)
			, m_HasSpecularTexture(false)
			, m_HasSpecularPowerTexture(false)
			, m_HasNormalTexture(false)
			, m_HasBumpTexture(false)
			, m_HasOpacityTexture(false)
			, m_BumpIntensity(5.0f)
			, m_SpecularScale(128.0f)
			, m_AlphaThreshold(0.1f)
		{}

		XMFLOAT4 m_GlobalAmbient;
		XMFLOAT4 m_AmbientColor;
		XMFLOAT4 m_EmissiveColor;
		XMFLOAT4 m_DiffuseColor;
		XMFLOAT4 m_SpecularColor;
		XMFLOAT4 m_Reflectance;

		float m_Opacity;					//	*
		float m_SpecularPower;				//	*
		float m_IndexOfRefraction;			//	*
		uint32_t m_HasAmbientTexture;		//	- 16 bytes

		uint32_t m_HasEmissiveTexture;		//	*
		uint32_t m_HasDiffuseTexture;		//	*
		uint32_t m_HasSpecularTexture;		//	*
		uint32_t m_HasSpecularPowerTexture;	//	- 16 bytes

		uint32_t m_HasNormalTexture;		//	*
		uint32_t m_HasBumpTexture;			//	*
		uint32_t m_HasOpacityTexture;		//	*
		float m_BumpIntensity;				//	- 16 bytes

		float m_SpecularScale;				//	*
		float m_AlphaThreshold;				//	*
		XMFLOAT2 m_Padding;					//	- 16 bytes
	};

	typedef std::map<TextureValue, std::shared_ptr<Texture>> TextureMap;
	TextureMap m_Textures;

	MatProperties* m_pProperties;
};

#endif