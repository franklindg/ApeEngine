//// testshader.fx ////

cbuffer MatrixBufferType
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}



float4 ambientColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
float ambientIntensity = 0.1f;



struct VertexInputType
{
	float4 position : POSITION;
};

struct VertexOutputType
{
	float4 position : POSITION;
};


//// Vertex Shader ////
VertexOutputType VertexShaderFunction(VertexInputType input)
{
	VertexOutputType output;

	float4 worldPosition = mul(input.position, worldMatrix);
	float4 viewPosition = mul(worldPosition, viewMatrix);
	output.position = mul(viewPosition, projectionMatrix);

	return output;
}


//// Pixel Shader ////
float4 PixelShaderFunction(VertexOutputType input) : COLOR0
{
	return ambientColor * ambientIntensity;
}


//// technique pass ////
technique Ambient
{
	pass Pass1
	{
		VertexShader = compile vs_5_0 VertexShaderFunction();
		PixelShader = compile ps_5_0 PixelShaderFunction();
	}
}