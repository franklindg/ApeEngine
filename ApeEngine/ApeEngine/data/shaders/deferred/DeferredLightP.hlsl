Texture2D NormalTexture : register(t0);
Texture2D DiffuseAlbedoTexture : register(t1);
Texture2D SpecularAlbedoTexture : register(t2);
Texture2D PositionTexture : register(t3);

//SamplerState AnisoSampler : register(s0);

cbuffer LightBuffer
{
	float3 lightDirection;
	float padding;
};


void GetGBufferAttributes(in float2 screenPos, out float3 normal,
						  out float3 position,
						  out float3 diffuseAlbedo, out float3 specularAlbedo,
						  out float specularPower)
{
	int3 sampleIndices = int3(screenPos.xy, 0);

	normal = NormalTexture.Load(sampleIndices).xyz;
	position = PositionTexture.Load(sampleIndices).xyz;
	diffuseAlbedo = DiffuseAlbedoTexture.Load(sampleIndices).xyz;
	float4 spec = SpecularAlbedoTexture.Load(sampleIndices);

	specularAlbedo = spec.xyz;
	specularPower = spec.w;
}

float3 CalcLighting(in float3 normal,
					in float3 position,
					in float3 viewDirection,
					in float3 diffuseAlbedo,
					in float3 specularAlbedo,
					in float specularPower)
{
	
	float attenuation = 1.0f;

	float3 L = -lightDirection;
	float distance = length(L);
	L = L / distance;
	distance = distance * distance;

	float3 LightColor = { 1.0f, 1.0f, 1.0f };
	float nDotL = dot(normal, L);
	float intensity = saturate(nDotL);

	float3 diffuse = intensity * LightColor * diffuseAlbedo;

	float3 H = normalize(L + viewDirection);
	float3 specular = pow(saturate(dot(normal.xyz, H)), specularPower) * LightColor * specularAlbedo.xyz * nDotL;

	return (diffuse + specular) * attenuation;
}

struct PixelInputType
{
	float4 position		 : SV_POSITION;
	float3 viewDirection : TEXCOORD0;
};

[earlydepthstencil]
float4 LightPixelShader(PixelInputType input) : SV_Target
{
	float3 normal;
	float3 position;

	float3 diffuseAlbedo;
	float3 specularAlbedo;
	float specularPower;

	GetGBufferAttributes(input.position.xy, normal, position, diffuseAlbedo, specularAlbedo, specularPower);
	float3 lighting = CalcLighting(normal, position, input.viewDirection, diffuseAlbedo, specularAlbedo, specularPower);

	return float4(lighting, 1.0f);
}