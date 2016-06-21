#ifndef _STRUCTURED_BUFFER_H
#define _STRUCTURED_BUFFER_H

#include "../framework/ApePCH.h"
#include "CPUAccess.h"
#include "Buffer.h"

class StructuredBuffer : public Buffer
{
public:
	StructuredBuffer(ID3D11Device* pDevice, UINT bindFlags, const void* data, size_t count, UINT stride,
					 CPUAccess cpuAccess = CPUAccess::None, bool bUAV = false);
	~StructuredBuffer();

	virtual void Bind(uint32_t ID, Shader::ShaderType shaderType, ShaderParameter::Type parameterType);
	virtual void Unbind(uint32_t ID, Shader::ShaderType shaderType, ShaderParameter::Type parameterType);

protected:

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_pUAV;

	typedef std::vector<uint8_t> BufferType;
	BufferType m_Data;

	UINT m_uiStride;
	UINT m_BindFlags;
	UINT m_uiCount;
	UINT m_uiSlot;

	bool m_bIsDirty;
	bool m_bUAV;
	bool m_bDynamic;

	CPUAccess m_CPUAccess;
};

#endif 