#include "Materials.h"



Materials::Materials()
{
}


Materials::~Materials()
{
}

void Materials::SetTexture(TextureValue ID, std::shared_ptr<Texture> texture)
{
	m_Textures[ID] = texture;

	switch (ID)
	{
	case TextureValue::Ambient:

	default:
		break;
	}
}

void Materials::SetAmbient(XMFLOAT4 color)
{
	m_pProperties->m_AmbientColor = color;
}

void Materials::SetEmissive(XMFLOAT4 color)
{
	m_pProperties->m_EmissiveColor = color;
}

void Materials::SetDiffuse(XMFLOAT4 color)
{
	m_pProperties->m_DiffuseColor = color;
}

void Materials::SetSpecular(XMFLOAT4 color)
{
	m_pProperties->m_SpecularColor = color;
}

void Materials::SetSpecularPower(float specularPower)
{
	m_pProperties->m_SpecularPower = specularPower;
}

void Materials::SetOpacity(float opacity)
{
	m_pProperties->m_Opacity = opacity;
}

void Materials::SetIndexOfRefraction(float IoF)
{
	m_pProperties->m_IndexOfRefraction = IoF;
}

void Materials::SetBumpIntensity(float bumpIntensity)
{
	m_pProperties->m_BumpIntensity = bumpIntensity;
}

void Materials::SetReflectance(XMFLOAT4 reflectance)
{
	m_pProperties->m_Reflectance = reflectance;
}
