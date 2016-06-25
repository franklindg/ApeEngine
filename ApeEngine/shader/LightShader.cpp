#include <ApePCH.h>
#include "LightShader.h"

LightShader::LightShader(ID3D11Device* device, HWND hwnd)
	: m_pDevice(device)
	, m_vertexShader(nullptr)
	, m_pixelShader(nullptr)
	, m_layout(nullptr)
	, m_sampleState(nullptr)
	, m_matrixBuffer(nullptr)
	, m_cameraBuffer(nullptr)
	, m_lightBuffer(nullptr)
{
	m_pDevice->GetImmediateContext(&m_pDeviceContext);

	if (!InitializeShader(hwnd, L"data/shaders/DeferredLightV.hlsl", L"data/shaders/DeferredLightP.hlsl"))
	{
		ReportError("Failed to initialize the light shader.");
	}
}

LightShader::~LightShader()
{
	m_pDevice.Reset();
	m_pDeviceContext.Reset();

	m_lightBuffer.Reset();
	m_cameraBuffer.Reset();
	m_matrixBuffer.Reset();

	m_sampleState.Reset();
	m_layout.Reset();

	m_pixelShader.Reset();
	m_vertexShader.Reset();
}

bool LightShader::Render(int indexCount, DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix,
	DirectX::CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* diffuseAlbedo,
	ID3D11ShaderResourceView* specularAlbedo, ID3D11ShaderResourceView* position, DirectX::XMFLOAT3 cameraPosition, DirectX::XMFLOAT3 lightDirection)
{
	// Sets the shader parameters that it will use for rendering.
	if (!SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, normal, diffuseAlbedo, specularAlbedo,
		position, cameraPosition, lightDirection))
	{
		return false;
	}

	// Renders the prepared buffers with the shader.
	RenderShader(indexCount);

	return true;
}

bool LightShader::InitializeShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3DBlob* errorMessage(0);
	ID3DBlob* vertexShaderBuffer(0);
	ID3DBlob* pixelShaderBuffer(0);

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// TODO: Make dynamic.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = m_pDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
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
	result = m_pDevice->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_pDevice->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_pDevice->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void LightShader::OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
}

bool LightShader::SetShaderParameters(DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix,
	DirectX::CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* diffuseAlbedo,
	ID3D11ShaderResourceView* specularAlbedo, ID3D11ShaderResourceView* position, DirectX::XMFLOAT3 cameraPos, DirectX::XMFLOAT3 alightDirection)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	CameraBufferType* dataPtr3;

	DirectX::XMMATRIX WorldMatrix = XMMatrixTranspose(worldMatrix);
	DirectX::XMMATRIX WorldViewMatrix = XMMatrixTranspose(XMMatrixMultiply(worldMatrix, viewMatrix));
	DirectX::XMMATRIX WorldViewProjectionMatrix = XMMatrixTranspose(XMMatrixMultiply(XMMatrixMultiply(worldMatrix, viewMatrix), projectionMatrix));

	// Lock the constant buffer so it can be written to.
	if (FAILED(m_pDeviceContext->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData; // Gets a pointer to the data from the constant buffer.
	dataPtr->World = WorldMatrix;
	dataPtr->WorldView = WorldViewMatrix;
	dataPtr->WorldViewProjection = WorldViewProjectionMatrix;

	m_pDeviceContext->Unmap(m_matrixBuffer.Get(), 0);

	ID3D11Buffer* pBuffer1[] = { m_matrixBuffer.Get() };
	m_pDeviceContext->VSSetConstantBuffers(0, 1, pBuffer1);

	// Set shader texture resource in the pixel shader.
	m_pDeviceContext->PSSetShaderResources(0, 1, &normal);
	m_pDeviceContext->PSSetShaderResources(1, 1, &diffuseAlbedo);
	m_pDeviceContext->PSSetShaderResources(2, 1, &specularAlbedo);
	m_pDeviceContext->PSSetShaderResources(3, 1, &position);

	// Lock the camera constant buffer so it can be written to.
	if (FAILED(m_pDeviceContext->Map(m_cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	dataPtr3 = (CameraBufferType*)mappedResource.pData; // Gets a pointer to the data from the constant buffer.
	dataPtr3->cameraPosition = cameraPos;
	dataPtr3->padding1 = 0.0f;

	m_pDeviceContext->Unmap(m_cameraBuffer.Get(), 0);

	ID3D11Buffer* pBuffer2[] = { m_cameraBuffer.Get() };
	m_pDeviceContext->VSSetConstantBuffers(1, 1, pBuffer2);

	// Lock the light constant buffer so it can be written to.
	if (FAILED(m_pDeviceContext->Map(m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	dataPtr2 = (LightBufferType*)mappedResource.pData; // Gets a pointer to the data from the constant buffer.
	dataPtr2->lightDirection = alightDirection;
	dataPtr2->padding = 0.0f;

	m_pDeviceContext->Unmap(m_lightBuffer.Get(), 0);

	ID3D11Buffer* pBuffer3[] = { m_lightBuffer.Get() };
	m_pDeviceContext->PSSetConstantBuffers(0, 1, pBuffer3);

	return true;
}

void LightShader::RenderShader(int indexCount)
{
	m_pDeviceContext->IASetInputLayout(m_layout.Get()); // Sets the shader input layout.

	m_pDeviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0); // Sets the vertex shader.
	m_pDeviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);  // Sets the pixel shader.

	m_pDeviceContext->PSSetSamplers(0, 1, &m_sampleState); // Sets the shader sampler state.

	m_pDeviceContext->DrawIndexed(indexCount, 0, 0); // Draws the shader data.
}