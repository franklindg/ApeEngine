cbuffer MatrixBuffer
{
	float4x4 WorldMatrix;
	float4x4 WorldViewMatrix;
	float4x4 WorldViewProjectionMatrix;
};

cbuffer CameraBuffer
{
	float3 cameraPos;
	float padding;
}

///////////
//TypeDef//
///////////
struct VertexInputType
{
	float4 position : POSITION;
	//float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	//float2 tex : TEXCOORD0;
	float3 viewDirection : TEXCOORD0;
};

/////////
// VertexShader
/////////
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

	////output.position = mul(input.position, WorldViewProjectionMatrix);
	output.position = input.position;
	////output.tex = input.tex;
	
	worldPosition = mul(input.position, WorldMatrix);

	output.viewDirection = cameraPos.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);
	return output;
}