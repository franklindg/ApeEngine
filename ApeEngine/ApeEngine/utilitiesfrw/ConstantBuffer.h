#ifndef _CONSTANT_BUFFER_H
#define _CONSTANT_BUFFER_H

#include "../framework/ApePCH.h"

class ConstantBuffer 
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	bool InitializeCBuffer(ID3D11Device* pDevice, size_t bufferSize);
	bool SetCBuffer();
protected:

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

	size_t m_BufferSize;
};

#endif