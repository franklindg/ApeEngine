#ifndef _DEFERRED_SHADER_H
#define _DEFERRED_SHADER_H

using namespace DirectX;

///////////////////
// Class name: DeferredShader //
///////////////////
class DeferredShader
{
private:
	enum ShaderType
	{
		VertexShader,
		PixelShader,
	};

	struct MatrixBufferType
	{
		XMMATRIX World;
		XMMATRIX WorldView;
		XMMATRIX WorldViewProjection;
	};

	struct MaterialBufferType
	{
		XMFLOAT3 specularAlbedo;
		float specularPower;
	};

public:
	DeferredShader(ID3D11Device* pDevice);
	~DeferredShader();

	bool Render(UINT, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX projectionMatrix, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*);

private:
	void OutputShaderErrorMessage(ID3DBlob*, HWND, WCHAR*);

	bool CreateShader(DeferredShader::ShaderType shaderType, WCHAR* fileName, char* entryPoint, char* targetFeature);
	bool SetShaderParameters(CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* diffuseMap, ID3D11ShaderResourceView* normalMap);
	void RenderShader(UINT);

	bool GetShaderInformation();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
		
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11HullShader> m_pHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> m_pDomainShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_pComputeShader;

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> m_pReflection;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;

	Microsoft::WRL::ComPtr<ID3DBlob> m_pBlob;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pMatrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pMaterialBuffer;
};

#endif