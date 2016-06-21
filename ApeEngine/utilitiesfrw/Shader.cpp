#include "Shader.h"

//DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc);

Shader::Shader(ID3D11Device* pDevice)
	: m_pDevice(pDevice)
	, m_pInputLayout(nullptr)
	, m_pVertexShader(nullptr)
	, m_pPixelShader(nullptr)
{
	m_pDevice->GetImmediateContext(&m_pDeviceContext);
}

bool Shader::CreateShader(Shader::ShaderType shaderType, WCHAR* fileName, char* entryPoint, char* targetFeature)
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3DBlob> pBufferBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorMessage;

	hr = D3DCompileFromFile(fileName, NULL, NULL, entryPoint, targetFeature, D3D10_SHADER_ENABLE_STRICTNESS, 0, &pBufferBlob, &pErrorMessage);
	if (FAILED(hr))
	{
		if (pErrorMessage)
		{
			ReportShaderError(pErrorMessage.Get(), fileName);
		}
		else
		{
			MessageBox(NULL, fileName, L"Missing shader file.", MB_OK);
		}

		return false;
	}

	switch (shaderType)
	{
	case Shader::VertexShader:
		hr = m_pDevice->CreateVertexShader(pBufferBlob->GetBufferPointer(), pBufferBlob->GetBufferSize(), NULL, &m_pVertexShader);
		break;
	case Shader::PixelShader:
		hr = m_pDevice->CreatePixelShader(pBufferBlob->GetBufferPointer(), pBufferBlob->GetBufferSize(), NULL, &m_pPixelShader);
		break;
	default:
		return false;
		break;
	}

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create shader.", L"Error.", MB_OK);
		return false;
	}

	if (FAILED(D3DReflect(pBufferBlob->GetBufferPointer(), pBufferBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &m_pReflection)))
	{
		MessageBox(NULL, L"Failed to create shader reflection", L"Error.", MB_OK);
		return false;
	}

	return true;
}

bool Shader::GetShaderInformation()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;

	D3D11_SHADER_DESC shaderDesc;
	if (FAILED(m_pReflection->GetDesc(&shaderDesc)))
	{
		MessageBox(NULL, L"Failed to get shader description.", L"Error.", MB_OK);
		return false;
	}

	for (UINT i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_INPUT_ELEMENT_DESC inputElement;
		D3D11_SIGNATURE_PARAMETER_DESC paramSignature;

		m_pReflection->GetInputParameterDesc(i, &paramSignature);

		inputElement.SemanticName = paramSignature.SemanticName;
		inputElement.SemanticIndex = paramSignature.SemanticIndex;
		inputElement.InputSlot = 0;
		inputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElement.InstanceDataStepRate = 0;
		//inputElement.Format = GetDXGIFormat(paramSignature);

		inputElements.push_back(inputElement);
	}

	return true;
}

void Shader::ReportShaderError(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("data/shaders/shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(NULL, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

//DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc)
//{
//	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
//	if (paramDesc.Mask == 1) // 1 component
//	{
//		switch (paramDesc.ComponentType)
//		{
//		case D3D_REGISTER_COMPONENT_UINT32:
//		{
//			format = DXGI_FORMAT_R32_UINT;
//		}
//		break;
//		case D3D_REGISTER_COMPONENT_SINT32:
//		{
//			format = DXGI_FORMAT_R32_SINT;
//		}
//		break;
//		case D3D_REGISTER_COMPONENT_FLOAT32:
//		{
//			format = DXGI_FORMAT_R32_FLOAT;
//		}
//		break;
//		}
//	}
//	else if (paramDesc.Mask <= 3) // 2 components
//	{
//		switch (paramDesc.ComponentType)
//		{
//		case D3D_REGISTER_COMPONENT_UINT32:
//		{
//			format = DXGI_FORMAT_R32G32_UINT;
//		}
//		break;
//		case D3D_REGISTER_COMPONENT_SINT32:
//		{
//			format = DXGI_FORMAT_R32G32_SINT;
//		}
//		break;
//		case D3D_REGISTER_COMPONENT_FLOAT32:
//		{
//			format = DXGI_FORMAT_R32G32_FLOAT;
//		}
//		break;
//		}
//	}
//	else if (paramDesc.Mask <= 7) // 3 components
//	{
//		switch (paramDesc.ComponentType)
//		{
//		case D3D_REGISTER_COMPONENT_UINT32:
//		{
//			format = DXGI_FORMAT_R32G32B32_UINT;
//		}
//		break;
//		case D3D_REGISTER_COMPONENT_SINT32:
//		{
//			format = DXGI_FORMAT_R32G32B32_SINT;
//		}
//		break;
//		case D3D_REGISTER_COMPONENT_FLOAT32:
//		{
//			format = DXGI_FORMAT_R32G32B32_FLOAT;
//		}
//		break;
//		}
//	}
//	else if (paramDesc.Mask <= 15) // 4 components
//	{
//		switch (paramDesc.ComponentType)
//		{
//		case D3D_REGISTER_COMPONENT_UINT32:
//		{
//			format = DXGI_FORMAT_R32G32B32A32_UINT;
//		}
//		break;
//		case D3D_REGISTER_COMPONENT_SINT32:
//		{
//			format = DXGI_FORMAT_R32G32B32A32_SINT;
//		}
//		break;
//		case D3D_REGISTER_COMPONENT_FLOAT32:
//		{
//			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//		}
//		break;
//		}
//	}
//
//	return format;
//}