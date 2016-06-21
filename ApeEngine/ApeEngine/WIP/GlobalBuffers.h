#ifndef _GLOBAL_BUFFERS_H
#define _GLOBAL_BUFFERS_H

#include <DirectXMath.h>
using namespace DirectX;

class ConstantBuffers
{
	struct MatrixBufferType
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};

	struct LightBufferType
	{
		XMFLOAT4 DiffuseColor;
		XMFLOAT4 AmbientColor;
		XMFLOAT4 EmissiveColor;
		XMFLOAT4 SpecularColor;
		XMFLOAT3 LightDirection;
		float SpecularPower;
	};
};


#endif