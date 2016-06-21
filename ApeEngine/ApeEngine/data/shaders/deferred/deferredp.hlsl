////////////////////////////////////////////////////////////////////////////////
// Filename: deferred.ps
////////////////////////////////////////////////////////////////////////////////


//////////////
// TEXTURES //
//////////////
Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
SamplerState AnisoSampler : register(s0);

cbuffer MatProperties
{
	float3 SpecularAlbedo;
	float SpecularPower;
}

struct PixelInputType
{
	float4 PositionSS : SV_Position;
	float2 TexCoord : TEXCOORD0;
	float3 NormalWS : NORMALWS;
	float3 PositionWS : POSITIONWS;
	float3 TangentWS : TANGENTWS;
	float3 BitangentWS : BITANGENTWS;
};

struct PixelOutputType
{
	float4 Normal : SV_Target0;
	float4 DiffuseAlbedo : SV_Target1;
	float4 SpecularAlbedo : SV_Target2;
	float4 Position : SV_Target3;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
[earlydepthstencil]
PixelOutputType DeferredPixelShader(PixelInputType input) : SV_TARGET
{
	PixelOutputType output;
	
	float3 diffuseAlbedo = DiffuseMap.Sample(AnisoSampler, input.TexCoord).rgb;

	float3x3 tangentFrameWS = float3x3(normalize(input.TangentWS),
										normalize(input.BitangentWS),
										normalize(input.NormalWS));

	float3 normalTS = NormalMap.Sample(AnisoSampler, input.TexCoord).rgb;
	normalTS = normalize(normalTS * 2.0f - 1.0f);

	float3 normalWS = mul(normalTS, tangentFrameWS);

	output.Normal = float4(normalWS, 1.0f);
	output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	output.SpecularAlbedo = float4(SpecularAlbedo, SpecularPower);
	output.Position = float4(input.PositionWS, 1.0f);

	return output;
}