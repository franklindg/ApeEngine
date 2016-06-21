////////////////////////////////////////////////////////////////////////////////
// Filename: deferred.vs
////////////////////////////////////////////////////////////////////////////////


//////////////////////
// CONSTANT BUFFERS //
//////////////////////
cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix WorldViewMatrix;
	matrix WorldViewProjectionMatrix;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 tangent : TANGENT;
	//float3 binormal : BINORMAL;
};

struct VSOutput
{
	float4 PositionCS  : SV_Position;
	float2 TexCoord    : TEXCOORD0;
	float3 NormalWS    : NORMAL;
	float3 PositionWS  : POSITIONWS;
	float3 TangentWS   : TANGENT;
	float3 BitangentWS : BITANGENTWS;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
[earlydepthstencil]
VSOutput DeferredVertexShader(VertexInputType input)
{
	VSOutput output;


	output.PositionWS = mul(input.position, WorldMatrix).xyz;
	float3 normalWS = normalize(mul(input.normal, (float3x3) WorldMatrix));
	output.NormalWS = normalWS;

	float3 tangentWS = normalize(mul(input.tangent.xyz, (float3x3) WorldMatrix));
	float3 bitangentWS = normalize(cross(normalWS, tangentWS)) * input.tangent.w;

	output.TangentWS = tangentWS;
	output.BitangentWS = bitangentWS;

	output.PositionCS = mul(input.position, WorldViewProjectionMatrix);


	output.TexCoord = input.tex;

	return output;
}