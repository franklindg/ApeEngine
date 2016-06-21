#ifndef _SHADER_INTERFACE_H
#define _SHADER_INTERFACE_H

#include "framework/ApePCH.h"

class ShaderInterface
{
public:
	enum ShaderType
	{
		VertexShader,
		HullShader,
		DomainShader,
		GeometryShader,
		PixelShader,
		ComputeShader,
	};

	enum ElementType
	{
		Position,
		Texture,
		Normal,
		Tangent,
		Bitangent,
		None,
	};

public:
	ShaderInterface();
	~ShaderInterface();

	bool LoadShaderFromFile(LPCWSTR& filename, ShaderType shaderType, LPCSTR& entryPoint, LPCSTR& profile);

private:
	void SetElementLayout(int elementCount);

	char* TranslateElement(ElementType element);
	void ShaderError(ID3DBlob* errorMessage, LPCWSTR& filename);

	DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc);
	
private:
	uint8_t m_ElementCount;

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;

	Microsoft::WRL::ComPtr<ID3DBlob> m_pShaderBlob;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>	 m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11HullShader>	 m_pHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>   m_pDomainShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	 m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader>  m_pComputeShader;

	D3D11_INPUT_ELEMENT_DESC m_elementDesc;
	
	D3D_SHADER_MACRO m_ShaderMacroArray;

	DXGI_FORMAT m_ElementFormat;
	char* m_SemanticName;
};

#endif