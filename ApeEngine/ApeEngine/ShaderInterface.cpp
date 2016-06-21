#include "ShaderInterface.h"



ShaderInterface::ShaderInterface()
{
}


ShaderInterface::~ShaderInterface()
{
}

bool ShaderInterface::LoadShaderFromFile(LPCWSTR& filename, ShaderType shaderType, LPCSTR& entryPoint, LPCSTR& profile)
{
	HRESULT hr;
	ID3DBlob* errorMessage = NULL;

	// Loads a shader from a shader filepath.
	hr = D3DCompileFromFile(filename, NULL, NULL, entryPoint, profile, D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&m_pShaderBlob, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			ShaderError(errorMessage, filename);
		}
		else
		{
			MessageBox(NULL, L"Could not find specified shader file.", L"Error", MB_OK);
		}

		return false;
	}

	// Translates the ShaderType enumerators to create a shader
	// of the corresponding type.
	switch (shaderType)
	{
	case ShaderInterface::VertexShader:
		hr = m_pDevice->CreateVertexShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), NULL, &m_pVertexShader);
		break;
	case ShaderInterface::HullShader:
		hr = m_pDevice->CreateHullShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), NULL, &m_pHullShader);
		break;
	case ShaderInterface::DomainShader:
		hr = m_pDevice->CreateDomainShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), NULL, &m_pDomainShader);
		break;
	case ShaderInterface::GeometryShader:
		hr = m_pDevice->CreateGeometryShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), NULL, &m_pGeometryShader);
		break;
	case ShaderInterface::PixelShader:
		hr = m_pDevice->CreatePixelShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), NULL, &m_pPixelShader);
		break;
	case ShaderInterface::ComputeShader:
		hr = m_pDevice->CreateComputeShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), NULL, &m_pComputeShader);
		break;
	}

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create the shader.", L"Error", MB_OK);
		return false;
	}

	// Loads the reflection of the shader data.
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> pShaderReflection = NULL;
	hr = D3DReflect(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &pShaderReflection);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to reflect the shader data.", L"Error", MB_OK);
		return false;
	}

	// Creates a description of the shader reflection.
	D3D11_SHADER_DESC reflectDesc;
	hr = pShaderReflection->GetDesc(&reflectDesc);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to get description from shader reflection.", L"Error", MB_OK);
		return false;
	}

	UINT numInputParameters = reflectDesc.InputParameters;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
	for (UINT i = 0; i < numInputParameters; i++)
	{
		D3D11_INPUT_ELEMENT_DESC inputElement;
		D3D11_SIGNATURE_PARAMETER_DESC paramSignature;

		pShaderReflection->GetInputParameterDesc(i, &paramSignature);

		inputElement.SemanticName = paramSignature.SemanticName;
		inputElement.SemanticIndex = paramSignature.SemanticIndex;
		inputElement.InputSlot = 0;
		inputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElement.InstanceDataStepRate = 0;
		inputElement.Format = GetDXGIFormat(paramSignature);

		inputElements.push_back(inputElement);
	}

	return true;
}

void ShaderInterface::SetElementLayout(int elementCount)
{	
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
	D3D11_INPUT_ELEMENT_DESC inputElement;

	inputElement.SemanticName = "POSITION";
	inputElement.SemanticIndex = 0;
	inputElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElement.InputSlot = 0;
	inputElement.AlignedByteOffset = 0;
	inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputElement.InstanceDataStepRate = 0;
}

char* ShaderInterface::TranslateElement(ElementType element)
{
	D3D11_INPUT_ELEMENT_DESC input;

	switch (element)
	{
	case ShaderInterface::Position:
	{
		m_SemanticName = "POSITION";
	}
	break;
	case ShaderInterface::Texture:
	{
		m_SemanticName = "TEXCOORD";
	}
	break;
	case ShaderInterface::Normal:
	{
		m_SemanticName = "NORMAL";
	}
	break;
	case ShaderInterface::Tangent:
	{
		m_SemanticName = "TANGENT";
	}
	break;
	case ShaderInterface::Bitangent:
	{
		m_SemanticName = "BINORMAL";
	}
	break;
	default:
	break;
	}

	return m_SemanticName;
}

void ShaderInterface::ShaderError(ID3DBlob* errorMessage, LPCWSTR& filename)
{
	WCHAR* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	compileErrors = (WCHAR*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();
	fout.open("data/shaders/shader-error.txt");

	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = 0;

	MessageBox(NULL, L"Error compiling shader. Check shader-error.txt for error list.", filename, MB_OK);

	return;
}


DXGI_FORMAT ShaderInterface::GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc)
{
	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
	if (paramDesc.Mask == 1) // 1 component
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32_FLOAT;
		}
		break;
		}
	}
	else if (paramDesc.Mask <= 3) // 2 components
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32G32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32G32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32G32_FLOAT;
		}
		break;
		}
	}
	else if (paramDesc.Mask <= 7) // 3 components
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32G32B32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32G32B32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		break;
		}
	}
	else if (paramDesc.Mask <= 15) // 4 components
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32G32B32A32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32G32B32A32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		break;
		}
	}

	return format;
}