/////////////////////////////////////////////
// Filename: SkydomeShader.h
/////////////////////////////////////////////
#ifndef _SKYDOME_SHADER_H
#define _SKYDOME_SHADER_H


//////////
// INCLUDES //
//////////
#include <d3d11_2.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
using namespace std;


///////////////////
// Class name: SkydomeShader //
///////////////////
class SkydomeShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct ColorBufferType
	{
		XMFLOAT4 apexColor;
		XMFLOAT4 centerColor;
	};

public:
	SkydomeShader();
	SkydomeShader(const SkydomeShader&);
	~SkydomeShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4, XMFLOAT4);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3DBlob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_colorBuffer;
};

#endif