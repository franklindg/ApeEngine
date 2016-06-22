/////////////////////////////////////////////
// Filename: LightShader.cpp
/////////////////////////////////////////////
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

	if (!InitializeShader(hwnd, L"data/shaders/deferred/DeferredLightV.hlsl", L"data/shaders/deferred/DeferredLightP.hlsl"))
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

bool LightShader::Render(int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
	CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* diffuseAlbedo,
	ID3D11ShaderResourceView* specularAlbedo, ID3D11ShaderResourceView* position, XMFLOAT3 cameraPosition, XMFLOAT3 lightDirection)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, normal, diffuseAlbedo, specularAlbedo,
								 position, cameraPosition, lightDirection);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
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
	// This setup needs to match the VertexType structure in the ModelClass and in the shader.
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
	samplerDesc.Filter		   = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU	   = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV	   = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW	   = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias	   = 0.0f;
	samplerDesc.MaxAnisotropy  = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD		   = 0;
	samplerDesc.MaxLOD		   = D3D11_FLOAT32_MAX;
	
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
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

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

bool LightShader::SetShaderParameters(CXMMATRIX worldMatrix, CXMMATRIX viewMatrix,
	CXMMATRIX projectionMatrix, ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* diffuseAlbedo,
	ID3D11ShaderResourceView* specularAlbedo, ID3D11ShaderResourceView* position, XMFLOAT3 cameraPos, XMFLOAT3 lightDirection)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	CameraBufferType* dataPtr3;
	
	XMMATRIX WorldMatrix = XMMatrixTranspose(worldMatrix);
	XMMATRIX WorldViewMatrix = XMMatrixTranspose(XMMatrixMultiply(worldMatrix, viewMatrix));
	XMMATRIX WorldViewProjectionMatrix = XMMatrixTranspose(XMMatrixMultiply(XMMatrixMultiply(worldMatrix, viewMatrix), projectionMatrix));

	// Lock the constant buffer so it can be written to.
	if (FAILED(m_pDeviceContext->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->World = WorldMatrix;
	dataPtr->WorldView = WorldViewMatrix;
	dataPtr->WorldViewProjection = WorldViewProjectionMatrix;

	m_pDeviceContext->Unmap(m_matrixBuffer.Get(), 0);

	ID3D11Buffer* pBuffer1[] = { m_matrixBuffer.Get() };

	bufferNumber = 0;

	m_pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, pBuffer1);

	// Set shader texture resource in the pixel shader.
	m_pDeviceContext->PSSetShaderResources(0, 1, &normal);
	m_pDeviceContext->PSSetShaderResources(1, 1, &diffuseAlbedo);
	m_pDeviceContext->PSSetShaderResources(2, 1, &specularAlbedo);
	m_pDeviceContext->PSSetShaderResources(3, 1, &position);
	
	// Lock the camera constant buffer so it can be written to.
	result = m_pDeviceContext->Map(m_cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr3 = (CameraBufferType*)mappedResource.pData;

	dataPtr3->cameraPosition = cameraPos;
	dataPtr3->padding1 = 0.0f;

	m_pDeviceContext->Unmap(m_cameraBuffer.Get(), 0);

	ID3D11Buffer* pBuffer2[] = { m_cameraBuffer.Get() };

	bufferNumber = 1;
	// Now set the camera constant buffer in the vertex shader with the updated values.
	m_pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, pBuffer2);

	// Lock the light constant buffer so it can be written to.
	if (FAILED(m_pDeviceContext->Map(m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	// Gets a pointer to the data in the constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->lightDirection = lightDirection;
	dataPtr2->padding = 0.0f;

	m_pDeviceContext->Unmap(m_lightBuffer.Get(), 0);

	ID3D11Buffer* pBuffer3[] = { m_lightBuffer.Get() };

	bufferNumber = 0;

	m_pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, pBuffer3);

	return true;
}

void LightShader::RenderShader(int indexCount)
{
	// Set the vertex input layout.
	m_pDeviceContext->IASetInputLayout(m_layout.Get());

	// Set the vertex and pixel shader that will be used to render this triangle.
	m_pDeviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_pDeviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);

	// Set the sampler state in the pixel shader.
	m_pDeviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	m_pDeviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}