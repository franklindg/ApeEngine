/////////////////////////////////////////////
// Filename: DeferredShader.cpp
/////////////////////////////////////////////
#include "DeferredShader.h"

DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc);

DeferredShader::DeferredShader(ID3D11Device* pDevice)
	: m_pDevice(pDevice)
	, m_pVertexShader(nullptr)
	, m_pHullShader(nullptr)
	, m_pDomainShader(nullptr)
	, m_pGeometryShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_pComputeShader(nullptr)
	, m_pLayout(nullptr)
	, m_pSamplerState(nullptr)
	, m_pReflection(nullptr)
	, m_pMatrixBuffer(nullptr)
	, m_pMaterialBuffer(nullptr)
	, m_pBlob(nullptr)
{
	m_pDevice->GetImmediateContext(&m_pDeviceContext);
}

DeferredShader::DeferredShader(const DeferredShader& other)
{
}


DeferredShader::~DeferredShader()
{
}

bool DeferredShader::Initialize(HWND hwnd)
{
	bool result;

	result = InitializeShader(hwnd, L"data/shaders/deferred/deferredrendering.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void DeferredShader::Shutdown()
{
	ShutdownShader();

	return;
}

bool DeferredShader::Render(UINT indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX projectionMatrix,
							ID3D11ShaderResourceView* textureMap, ID3D11ShaderResourceView* normalMap)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, textureMap, normalMap);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(indexCount);

	return true;
}

bool DeferredShader::InitializeShader(HWND hwnd, WCHAR* shaderFilename)
{
	HRESULT hr;
	D3D11_SAMPLER_DESC samplerDesc;
	
	CreateShader(VertexShader, shaderFilename, "VSMain", "vs_5_0");
	GetShaderInformation();
	CreateShader(PixelShader, shaderFilename, "PSMain", "ps_5_0");

	m_pBlob.Reset();

	// Create a wrap texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; 
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	if (FAILED(hr))
	{
		return false;
	}
	
	// Sets the description, then creates the matrix constant buffer.
	size_t bufferSize = sizeof(MatrixBufferType);
	D3D11_BUFFER_DESC matrixBuffer;
	ZeroMemory(&matrixBuffer, sizeof(D3D11_BUFFER_DESC));

	matrixBuffer.Usage = D3D11_USAGE_DYNAMIC;
	matrixBuffer.ByteWidth = (UINT)bufferSize;
	matrixBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBuffer.MiscFlags = 0;
	matrixBuffer.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&matrixBuffer, nullptr, &m_pMatrixBuffer)))
	{
		return false;
	}

	// Sets the description, then creates the material constant buffer.
	bufferSize = sizeof(MaterialBufferType);
	D3D11_BUFFER_DESC materialBuffer;
	ZeroMemory(&materialBuffer, sizeof(D3D11_BUFFER_DESC));

	materialBuffer.Usage = D3D11_USAGE_DYNAMIC;
	materialBuffer.ByteWidth = (UINT)bufferSize;
	materialBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBuffer.MiscFlags = 0;
	materialBuffer.StructureByteStride = 0;

	if(FAILED(m_pDevice->CreateBuffer(&materialBuffer, NULL, &m_pMaterialBuffer)))
	{
		return false;
	}

	return true;
}

void DeferredShader::ShutdownShader()
{
	m_pDevice.Reset();
	m_pDeviceContext.Reset();

	m_pMaterialBuffer.Reset();
	m_pMatrixBuffer.Reset();
	m_pSamplerState.Reset();

	m_pLayout.Reset();
	m_pVertexShader.Reset();
	m_pPixelShader.Reset();
	m_pReflection.Reset();
	m_pBlob.Reset();

	return;
}

void DeferredShader::OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("data/shaders/shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool DeferredShader::CreateShader(DeferredShader::ShaderType shaderType, WCHAR* fileName, char* entryPoint, char* targetFeature)
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorMessage;

	hr = D3DCompileFromFile(fileName, NULL, NULL, entryPoint, targetFeature, D3D10_SHADER_ENABLE_STRICTNESS, 0, &m_pBlob, &pErrorMessage);
	if (FAILED(hr))
	{
		if (pErrorMessage)
		{
			return false;
		}
		else
		{
			MessageBox(NULL, fileName, L"Missing shader file.", MB_OK);
		}

		return false;
	}

	switch (shaderType)
	{
	case DeferredShader::VertexShader:
	{
		hr = m_pDevice->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), NULL, &m_pVertexShader);
		D3DReflect(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &m_pReflection);
	}
		break;
	case DeferredShader::PixelShader:
		hr = m_pDevice->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), NULL, &m_pPixelShader);
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

	return true;
}

bool DeferredShader::SetShaderParameters(CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	MaterialBufferType* dataPtr2;
	XMFLOAT3 specAlbedo(1.0f, 1.0f, 1.0f);
	float specPower(32.0f);

	XMMATRIX WorldMatrix = XMMatrixTranspose(worldMatrix);
	XMMATRIX WorldViewMatrix = XMMatrixTranspose(XMMatrixMultiply(worldMatrix, viewMatrix));
	XMMATRIX WorldViewProjectionMatrix = XMMatrixTranspose(XMMatrixMultiply(XMMatrixMultiply(worldMatrix, viewMatrix), projectionMatrix));

	// Lock the constant buffer so it can be written to.
	result = m_pDeviceContext->Map(m_pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->World = WorldMatrix;
	dataPtr->WorldView = WorldViewMatrix;
	dataPtr->WorldViewProjection = WorldViewProjectionMatrix;

	m_pDeviceContext->Unmap(m_pMatrixBuffer.Get(), 0);

	// Set the position of the constant buffer in the vertex shader.
	ID3D11Buffer* pBuffers[] = { m_pMatrixBuffer.Get() };
	bufferNumber = 0;
	// Now set the constant buffer in the vertex shader with the updated values.
	m_pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, pBuffers);

	

	// Set shader texture resource in the pixel shader.
	m_pDeviceContext->PSSetShaderResources(0, 1, &texture);
	m_pDeviceContext->PSSetShaderResources(1, 1, &normalMap);

	result = m_pDeviceContext->Map(m_pMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (MaterialBufferType*)mappedResource.pData;
	
	dataPtr2->specularAlbedo = specAlbedo;
	dataPtr2->specularPower = specPower;

	m_pDeviceContext->Unmap(m_pMaterialBuffer.Get(), 0);

	ID3D11Buffer* pBuffers2[] = { m_pMaterialBuffer.Get() };
	bufferNumber = 0;
	m_pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, pBuffers2);
	
	return true;
}

void DeferredShader::RenderShader(UINT indexCount)
{
	// Set the vertex input layout.
	m_pDeviceContext->IASetInputLayout(m_pLayout.Get());

	// Set the vertex and pixel shaders that will be used to render.
	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	// Set the sampler states in the pixel shader.
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

	// Render the geometry.
	m_pDeviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

bool DeferredShader::GetShaderInformation()
{
	HRESULT hr;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;

	// Gets the shader reflection description.
	D3D11_SHADER_DESC reflectDesc;
	if (FAILED(m_pReflection->GetDesc(&reflectDesc)))
	{
		MessageBox(NULL, L"Failed to get the shader reflection description.", L"Error", MB_OK);
		return false;
	}
	
	// Gets the input elements from the reflection description,
	// and stores them in the input element description.
	for (UINT i = 0; i < reflectDesc.InputParameters; i++)
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
		inputElement.Format = GetDXGIFormat(paramSignature);

		inputElements.push_back(inputElement);
	}

	// Creates the vertex input layout.
	hr = m_pDevice->CreateInputLayout(&inputElements[0], (UINT)inputElements.size(), m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(),	&m_pLayout);
	if (FAILED(hr))
	{
		return false;
	}

	// De-allocate the buffer blob since the layout information has been distributed.
	m_pBlob.Reset();

	return true;
}

DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc)
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
