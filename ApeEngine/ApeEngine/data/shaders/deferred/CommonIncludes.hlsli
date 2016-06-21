/////////////////////////////////////////////////////////////////////
/////	HLSL Includes
/////////////////////////////////////////////////////////////////////
#ifndef NUM_LIGHTS
#pragma message("NUM_LIGHTS undefined. Default to 8.")
#define NUM_LIGHTS 8
#endif

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2


struct Material
{
	float4 GlobalAmbient;
	//------------------------- (16 bytes)
	float4 AmbientColor;
	//------------------------- (16 bytes)
	float4 EmissiveColor;
	//------------------------- (16 bytes)
	float4 DiffuseColor;
	//------------------------- (16 bytes)
	float4 SpecularColor;
	//------------------------- (16 bytes)
	float4 Reflectance;
	//------------------------- (16 bytes)
	float Opacity;
	float SpecularPower;
	float IndexOfRefraction;
	bool HasAmbientTexture;
	//------------------------- (16 bytes)
	bool HasEmissiveTexture;
	bool HasDiffuseTexture;
	bool HasSpecularTexture;
	bool HasSpecularPowerTexture;
	//------------------------- (16 bytes)
	bool HasNormalTexture;
	bool HasBumpTexture;
	bool HasOpacityTexture;
	float BumpIntensity;
	//------------------------- (16 bytes)
	float SpecularScale;
	float AlphaThreshold;
	float2 Padding;
	//------------------------- (16 bytes)
	//------------------------- (160 total bytes)
};


struct Light
{
	float4 PositionWS;
	float4 DirectionWS;
	float4 PositionVS;
	float4 DirectionVS;
	float4 Color;
	float SpotlightAngle;
	float Range;
	float Intensity;
	bool Enabled;
	bool Selected;

	uint Type;
	float2 Padding;
};

struct AppData
{
	float3 position : POSITION;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

cbuffer PerObject : register(b0)
{
	matrix ModelViewProjection;
	matrix ModelView;
};

cbuffer Material : register(b2)
{
	Material Mat;
};

cbuffer ScreenToViewParams : register(b3)
{
	float4x4 InverseProjection;
	float2 ScreenDimensions;
}

Texture2D AmbientTexture		: register(t0);
Texture2D EmissiveTexture		: register(t1);
Texture2D DiffuseTexture		: register(t2);
Texture2D SpecularTexture		: register(t3);
Texture2D SpecularPowerTexture	: register(t4);
Texture2D NormalTexture			: register(t5);
Texture2D BumpTexture			: register(t6);
Texture2D OpacityTexture		: register(t7);

StructuredBuffer<Light> Lights  : register(t8);

sampler LinearRepeatSampler		: register(s0);
sampler LinearClampSampler		: register(s1);


struct VertexShaderOutput
{
	float3 positionVS : TEXCOORD0;
	float2 texCoord	  : TEXCOORD1;
	float3 tangentVS  : TANGENT;
	float3 binormalVS : BINORMAL;
	float3 normalVS   : NORMAL;
	float4 position   : SV_POSITION;
};


float4 ClipToView(float4 clip)
{
	float4 view = mul(InverseProjection, clip);
	view = view / view.w;

	return view;
}

float4 ScreenToView(float4 screen)
{
	float2 texCoord = screen.xy / ScreenDimensions;
	float4 clip = float4(float2(texCoord.x, 1.0f - texCoord.y) * 2.0f - 1.0f, screen.z, screen.w);

	return ClipToView(clip);
}

float3 ExpandNormal(float3 n)
{
	return n * 2.0f - 1.0f;
}

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

float4 DoNormalMapping(float3x3 TBN, Texture2D tex, sampler s, float2 uv)
{
	float3 normal = tex.Sample(s, uv).xyz;
	normal = ExpandNormal(normal);

	// Transform normal from tangent space to view space.
	normal = mul(normal, TBN);
	return normalize(float4(normal, 0));
}

float4 DoBumpMapping(float3x3 TBN, Texture2D tex, sampler s, float2 uv, float bumpScale)
{
	float height = tex.Sample(s, uv).r * bumpScale;
	float heightU = tex.Sample(s, uv, int2(1, 0)).r * bumpScale;
	float heightV = tex.Sample(s, uv, int2(0, 1)).r * bumpScale;

	float3 p = { 0, 0, height };
	float3 pU = { 1, 0, heightU };
	float3 pV = { 0, 1, heightV };

	// normal = tangent * bitangent
	float3 normal = cross(normalize(pU - p), normalize(pV - p));
	// Transform normal from tangent space to view space.
	normal = mul(normal, TBN);

	return float4(normal, 0);
}

float4 DoDiffuse(Light light, float4 L, float4 N)
{
	float NdotL = max(dot(N, L), 0);
	return light.Color * NdotL;
}

float4 DoSpecular(Light light, Material material, float4 V, float4 L, float4 N)
{
	float4 R = normalize(reflect(-L, N));
	float RdotV = max(dot(R, V), 0);

	return light.Color * pow(RdotV, material.SpecularPower);
}

float DoAttenuation(Light light, float d)
{
	return 1.0f - smoothstep(light.Range * 0.75f, light.Range, d);
}

float DoSpotCone(Light light, float4 L)
{
	float minCos = cos(radians(light.SpotlightAngle));
	float maxCos = lerp(minCos, 1, 0.5f);
	float cosAngle = dot(light.DirectionVS, -L);

	return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoPointLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
	LightingResult result;

	float4 L = light.PositionVS - P;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);

	result.Diffuse = DoDiffuse(light, L, N) * attenuation * light.Intensity;

	return result;
}

LightingResult DoDirectionalLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
	LightingResult result;

	float4 L = normalize(-light.DirectionVS);

	result.Diffuse = DoDiffuse(light, L, N) * light.Intensity;
	result.Specular = DoSpecular(light, mat, V, L, N) * light.Intensity;

	return result;
}

LightingResult DoSpotLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
	LightingResult result;

	float4 L = light.PositionVS - P;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);
	float spotIntensity = DoSpotCone(light, L);

	result.Diffuse = DoDiffuse(light, L, N) * attenuation * spotIntensity * light.Intensity;
	result.Specular = DoSpecular(light, mat, V, L, N) * attenuation * spotIntensity * light.Intensity;

	return result;
}

LightingResult DoLighting(StructuredBuffer<Light> lights, Material mat, float4 eyePos, float4 P, float4 N)
{
	float4 V = normalize(eyePos - P);

	LightingResult totalResult = (LightingResult) 0;

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		LightingResult result = (LightingResult) 0;

		if (!lights[i].Enabled)
			continue;
		if (lights[i].Type != DIRECTIONAL_LIGHT && length(lights[i].PositionVS - P) > lights[i].Range)
			continue;

		switch(lights[i].Type)
		{
		case DIRECTIONAL_LIGHT:
		{
			result = DoDirectionalLight(lights[i], mat, V, P, N);
		}
		break;
		case POINT_LIGHT:
		{
			result = DoPointLight(lights[i], mat, V, P, N);
		}
		break;
		case SPOT_LIGHT:
		{
			result = DoSpotLight(lights[i], mat, V, P, N);
		}
		break;
		}
		totalResult.Diffuse += result.Diffuse;
		totalResult.Specular += result.Specular;
	}

	return totalResult;
}