#ifndef _SHADER_H
#define _SHADER_H

#include "../framework/ApePCH.h"

class Shader
{
public:
	enum ShaderType
	{
		UnknownShaderType = 0,
		VertexShader,
		PixelShader,
	};

	Shader(ID3D11Device* pDevice);
	~Shader();

	bool CreateShader(Shader::ShaderType shaderType, WCHAR* fileName, char* entryPoint, char* targetFeature);

private:
	bool GetShaderInformation();
	void ReportShaderError(ID3D10Blob* errorMessage, WCHAR* shaderFilename);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> m_pReflection;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
};
#endif