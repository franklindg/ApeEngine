#ifndef _LIGHT_SHADER_H
#define _LIGHT_SHADER_H

using namespace DirectX;

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
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	bool Render(int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
		CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* diffuseAlbedo,
		ID3D11ShaderResourceView* specularAlbedo, ID3D11ShaderResourceView* position, XMFLOAT3 cameraPosition, XMFLOAT3 lightDirection);

private:
	bool InitializeShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
		CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* diffuseAlbedo,
		ID3D11ShaderResourceView* specularAlbedo, ID3D11ShaderResourceView* position, XMFLOAT3 cameraPos, XMFLOAT3 lightDirection);
	void RenderShader(int indexCount);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampleState;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cameraBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_lightBuffer;
};
#endif