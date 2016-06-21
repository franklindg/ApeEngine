// Textures
Texture2D NormalTexture : register(t0);
Texture2D DiffuseAlbedoTexture : register(t1);
Texture2D SpecularAlbedoTexture : register(t2);
Texture2D PositionTexture : register(t3);

// Constants
cbuffer LightBuffer
{
	float3 LightPos;
	float3 LightColor;
	float3 LightDirection;
	float2 SpotlightAngles;
	float4 LightRange;
	float padding0;
};

cbuffer CameraBuffer
{
	float3 CameraPos;
	float padding1;
}

// Helper function for getting G-Buffer attributes.
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
					in float3 diffuseAlbedo,
					in float3 specularAlbedo,
					in float specularPower)
{
	float3 L = 0;
	float attenuation = 1.0f;
	//#if POINTLIGHT || SPOTLIGHT
	//		L = LightPos - position;
	//		// Calculate attenuation based on distance from the light source.
	//		float dist = length(L);
	//		attenuation = max(0, 1.0f - (dist / LightRange.x));
	//
	//		L /= dist;
	//#elif DIRECTIONALLIGHT
			L = -LightDirection;
	//#endif

	//#if SPOTLIGHT
	//float3 L2 = LightDirection;
	//float rho = dot(-L, L2);
	//attenuation *= saturate((rho - SpotlightAngles.y) / (SpotlightAngles.x - SpotlightAngles.y));
	//#endif

	float nDotL = saturate(dot(normal, L));
	float3 diffuse = nDotL * LightColor * diffuseAlbedo;

	// Calculate the specular term.
	float3 V = CameraPos - position;
	float3 H = normalize(L + V);
	float3 specular = pow(saturate(dot(normal, H)), specularPower) * LightColor * specularAlbedo.xyz * nDotL;

	return (diffuse + specular) * attenuation;
}

float4 PSMain(in float4 screenPos : SV_Position) : SV_Target0
{
	float3 normal;
	float3 position;
	float3 diffuseAlbedo;
	float3 specularAlbedo;
	float specularPower;

	GetGBufferAttributes(screenPos.xy, normal, position, diffuseAlbedo, specularAlbedo, specularPower);

	float3 lighting = CalcLighting(normal, position, diffuseAlbedo, specularAlbedo, specularPower);

	return float4(lighting, 1.0f);
}