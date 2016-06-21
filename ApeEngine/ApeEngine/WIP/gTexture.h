#ifndef _GTEXTURE_H
#define _GTEXTURE_H

class Texture
{
public:
	enum class Dimension
	{
		Texture1D,
		Texture1DArray,
		Texture2D,
		Texture2DArray,
		Texture3D,
		TextureCube,
	};

	enum class Components
	{
		R,
		RG,
		RGB,
		RGBA,
		Depth,
		DepthStencil
	};

	enum class Type
	{
		Typeless,
		UnsignedNormalized,
		SignedNormalized,
		Float,
		UnsignedInteger,
		SignedInteger,
	};

	struct TextureFormat
	{
		Texture::Components Components;
		Texture::Type Type;

		uint8_t NumSamples;

		uint8_t RedBits;
		uint8_t GreenBits;
		uint8_t BlueBits;
		uint8_t AlphaBits;
		uint8_t DepthBits;
		uint8_t StencilBits;

		TextureFormat(Texture::Components components = Components::RGBA,
					  Texture::Type type = Type::UnsignedNormalized,
					  uint8_t numSamples = 1,
					  uint8_t redBits = 8,
					  uint8_t greenBits = 8,
					  uint8_t blueBits = 8,
					  uint8_t alphaBits = 8,
					  uint8_t depthBits = 0,
					  uint8_t stencilBits = 0)
			: Components(components)
			, Type(type)
			, NumSamples(numSamples)
			, RedBits(redBits)
			, GreenBits(greenBits)
			, BlueBits(blueBits)
			, AlphaBits(alphaBits)
			, DepthBits(depthBits)
			, StencilBits(stencilBits)
		{}
	};
#endif
