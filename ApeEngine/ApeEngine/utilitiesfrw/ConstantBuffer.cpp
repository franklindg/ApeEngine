#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pBuffer(nullptr)
	, m_BufferSize(0)
{}


ConstantBuffer::~ConstantBuffer()
{
}

bool ConstantBuffer::InitializeCBuffer(ID3D11Device* pDevice, size_t bufferSize)
{
	m_pDevice = pDevice;
	m_pDevice->GetImmediateContext(&m_pDeviceContext);
	m_BufferSize = bufferSize;

	D3D11_BUFFER_DESC desc;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = (UINT)m_BufferSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	if (!m_pDevice->CreateBuffer(&desc, NULL, &m_pBuffer))
	{
		ReportError("Failed to initialize constant buffer.");
	}

	return true;
}

bool ConstantBuffer::SetCBuffer(void* data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	m_pDeviceContext->Map(m_pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(m_pBuffer.Get(), data, (UINT)m_BufferSize);

	m_pDeviceContext->Unmap(m_pBuffer.Get(), 0);

	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pBuffer);
}
