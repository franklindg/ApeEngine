/////////////////////////////////////////////
// Filename: LightShader.h
/////////////////////////////////////////////
#ifndef _LIGHT_SHADER_H
#define _LIGHT_SHADER_H

//////////
// INCLUDES //
//////////
#include "../framework/ApePCH.h"
#include <fstream>

using namespace DirectX;
using namespace std;



///////////////////
// Class name: LightShader //
///////////////////
class LightShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX World;
		XMMATRIX WorldView;
		XMMATRIX WorldViewProjection;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding1;
	};

	struct LightBufferType
	{
		//XMFLOAT4 ambientColor;
		//XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		//float specularPower;
		float padding;
		//XMFLOAT4 specularColor;
	};

public:
	LightShader();
	LightShader(const LightShader&);
	~LightShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, CXMMATRIX, CXMMATRIX, CXMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT3);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, CXMMATRIX, CXMMATRIX, CXMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT3);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampleState;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cameraBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_lightBuffer;
};
#endif