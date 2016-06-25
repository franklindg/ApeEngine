#include <ApePCH.h>
#include "DeferredBuffers.h"

DeferredBuffers::DeferredBuffers(ID3D11Device* pDevice, int iWidth, int iHeight, float fDepth, float fNear)
	: m_pDevice(pDevice)
	, m_pDepthStencilBuffer(nullptr)
	, m_pDepthStencilView(0)
{
	m_pDevice->GetImmediateContext(&m_pDeviceContext);

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		m_paTexture[i] = 0;
		m_paRenderTarget[i] = 0;
		m_paShaderResource[i] = nullptr;
	}

	Initialize(iWidth, iHeight, fDepth, fNear);
}

DeferredBuffers::~DeferredBuffers()
{
	m_pDevice.Reset();
	m_pDeviceContext.Reset();

	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = 0;
	}

	m_pDepthStencilBuffer.Reset();

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		if (m_paShaderResource[i])
		{
			m_paShaderResource[i].Reset();
		}

		if (m_paRenderTarget[i])
		{
			m_paRenderTarget[i]->Release();
			m_paRenderTarget[i] = 0;
		}

		if (m_paTexture[i])
		{
			m_paTexture[i]->Release();
			m_paTexture[i] = 0;
		}
	}
}

bool DeferredBuffers::Initialize(int iWidth, int iHeight, float fDepth, float fNear)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	int i;

	// Store the width and height of the render texture.
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = iWidth;
	textureDesc.Height = iHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target textures.
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		result = m_pDevice->CreateTexture2D(&textureDesc, NULL, &m_paTexture[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target views.
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		result = m_pDevice->CreateRenderTargetView(m_paTexture[i], &renderTargetViewDesc, &m_paRenderTarget[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource views.
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		result = m_pDevice->CreateShaderResourceView(m_paTexture[i], &shaderResourceViewDesc, &m_paShaderResource[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = iWidth;
	depthBufferDesc.Height = iHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the viewport for rendering.
	m_Viewport.Width = (float)iWidth;
	m_Viewport.Height = (float)iHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;

	return true;
}

void DeferredBuffers::SetRenderTargets()
{
	m_pDeviceContext->OMSetRenderTargets(BUFFER_COUNT, m_paRenderTarget, m_pDepthStencilView);
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);
	return;
}

void DeferredBuffers::ClearRenderTargets(float red, float green, float blue, float alpha)
{
	float color[4];
	int i;

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		m_pDeviceContext->ClearRenderTargetView(m_paRenderTarget[i], color);
	}

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

ID3D11ShaderResourceView* DeferredBuffers::GetShaderResourceView(int view)
{
	return m_paShaderResource[view].Get();
}