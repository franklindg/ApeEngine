// 
cbuffer MatrixBuffer
{
	float4x4 WorldMatrix;
	float4x4 WorldViewMatrix;
	float4x4 WorldViewProjectionMatrix;
};

cbuffer MaterialBuffer
{
	float3 SpecularAlbedo;
	float SpecularPower;
};

// Textures/Samplers
Texture2D DiffuseMap	  : register(t0);
Texture2D NormalMap		  : register(t1);
SamplerState AnisoSampler : register(s0);

struct VSInput
{
	float4 Position  : POSITION;
	float2 TexCoord  : TEXCOORD;
	float3 Normal	 : NORMAL;
	float3 Tangent	 : TANGENT;
	float3 Bitangent : BINORMAL;
};

struct VSOutput
{
	float4 PositionCS  : SV_Position;
	float2 TexCoord	   : TEXCOORD;
	float3 NormalWS    : NORMALWS;
	float3 PositionWS  : POSITIONWS;
	float3 TangentWS   : TANGENTWS;
	float3 BitangentWS : BITANGEDWS;
};

struct PSInput
{
	float4 PositionSS  : SV_Position;
	float2 TexCoord    : TEXCOORD0;
	float3 NormalWS    : NORMALWS;
	float3 PositionWS  : POSITIONWS;
	float3 TangentWS   : TANGENTWS;
	float3 BitangentWS : BITANGENTWS;
};

struct PSOutput
{
	float4 Normal		  : SV_Target0;
	float4 DiffuseAlbedo  : SV_Target1;
	float4 SpecularAlbedo : SV_Target2;
	float4 Position		  : SV_Target3;
};

VSOutput VSMain(in VSInput input)
{
	VSOutput output;

	// Convert the position and normals to world space.
	output.PositionWS = mul(input.Position, WorldMatrix).xyz;
	float3 normalWS = normalize(mul(input.Normal, (float3x3) WorldMatrix));
	output.NormalWS = normalWS;

	// Reconstruct the rest of the tangent frame.
	// float3 tangentWS = normalize(mul(input.Tangent.xyz, (float3x3) WorldMatrix));
	// float3 bitangentWS = normalize(cross(normalWS, tangentWS)) * input.Tangent.w;

	output.TangentWS = mul(input.Tangent.xyz, (float3x3)WorldMatrix);
	output.TangentWS = normalize(output.TangentWS);
	output.BitangentWS = mul(input.Bitangent.xyz, (float3x3)WorldMatrix);
	output.BitangentWS = normalize(output.BitangentWS);

	// Calculate the clip-space position.
	output.PositionCS = mul(input.Position, WorldViewProjectionMatrix);

	output.TexCoord = input.TexCoord;

	return output;
}

PSOutput PSMain(in PSInput input)
{
	PSOutput output;

	// Sample the diffuse map.
	float3 diffuseAlbedo = DiffuseMap.Sample(AnisoSampler, input.TexCoord).rgb;

	// Normalize the tangent frame after interpolation.
	float3x3 tangentFrameWS = float3x3(normalize(input.TangentWS),
									   normalize(input.BitangentWS),
									   normalize(input.NormalWS));

	// Sample the tangent-space normal map and decompress.
	float3 normalTS = NormalMap.Sample(AnisoSampler, input.TexCoord).rgb;
	normalTS = normalize(normalTS * 2.0f - 1.0f);

	// Convert to world space.
	float3 normalWS = mul(normalTS, tangentFrameWS);

	// Output the G-Buffer values.
	output.Normal = float4(normalWS, 1.0f);
	output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	output.SpecularAlbedo = float4(SpecularAlbedo, SpecularPower);
	output.Position = float4(input.PositionWS, 1.0f);

	return output;
}