#include "StructuredBuffer.h"



StructuredBuffer::StructuredBuffer(ID3D11Device* pDevice, UINT bindFlags, const void* data, size_t count, UINT stride,
								   CPUAccess cpuAccess, bool bUAV)
	: m_pDevice(pDevice)
	, m_uiStride(stride)
	, m_uiCount((UINT)count)
	, m_BindFlags(bindFlags)
	, m_bIsDirty(false)
	, m_CPUAccess(cpuAccess)
{
	m_bDynamic = (int)m_CPUAccess != 0;
	m_bUAV = bUAV && !m_bDynamic;

	size_t numBytes = m_uiCount * m_uiStride;

	if (data)
	{
		m_Data.assign((uint8_t*)data, (uint8_t*)data + numBytes);
	}
	else
	{
		m_Data.reserve(numBytes);
	}

	// Creates the GPU buffer to store the data.
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = (UINT)numBytes;

	if (((int)m_CPUAccess & (int)CPUAccess::Read) != 0)
	{
		bufferDesc.Usage = D3D11_USAGE_STAGING;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	}
	else if (((int)m_CPUAccess & (int)CPUAccess::Write) != 0)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else
	{
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (m_bUAV)
		{
			bufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}
	}

	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = m_uiStride;

	D3D11_SUBRESOURCE_DATA subrData;
	subrData.pSysMem = (void*)m_Data.data();
	subrData.SysMemPitch = 0;
	subrData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&bufferDesc, &subrData, &m_pBuffer)))
	{
		ReportError("Failed to create read/write buffer.");
		return;
	}

	if ((bufferDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = m_uiCount;

		if (FAILED(m_pDevice->CreateShaderResourceView(m_pBuffer.Get(), &srvDesc, &m_pSRV)))
		{
			ReportError("Failed to create the shader resource view.");
			return;
		}
	}

	if ((bufferDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) != 0)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = m_uiCount;
		uavDesc.Buffer.Flags = 0;

		if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pBuffer.Get(), &uavDesc, &m_pUAV)))
		{
			ReportError("Failed to create the unordered access view.");
			return;
		}
	}

	m_pDevice->GetImmediateContext(&m_pDeviceContext);
}

StructuredBuffer::~StructuredBuffer()
{
}
