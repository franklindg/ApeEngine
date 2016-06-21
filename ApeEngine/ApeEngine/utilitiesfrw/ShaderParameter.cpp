#include "ShaderParameter.h"

ShaderParameter::ShaderParameter()
	: m_uiSlotID(UINT_MAX)
	, m_ParameterType(Type::Invalid)
{}

ShaderParameter::ShaderParameter(const std::string& name, UINT slotID, Shader::ShaderType shaderType, Type parameterType)
	: m_Name(name)
	, m_uiSlotID(slotID)
	, m_ShaderType(shaderType)
	, m_ParameterType(parameterType)
{}

bool ShaderParameter::IsValid() const
{
	return m_ParameterType != ShaderParameter::Type::Invalid;
}

ShaderParameter::Type ShaderParameter::GetType() const
{
	return m_ParameterType;
}

void ShaderParameter::Bind()
{
	if (std::shared_ptr<ConstantBuffer> constantBuffer = m_pConstantBuffer.lock())
	{
		constantBuffer->Bind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<Texture> texture = m_pTexture.lock())
	{
		texture->Bind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<SamplerState> samplerState = m_pSamplerState.lock())
	{
		samplerState->Bind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<StructuredBuffer> buffer = m_pStructuredBuffer.lock())
	{
		buffer->Bind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
}

void ShaderParameter::Unbind()
{
	if (std::shared_ptr<ConstantBuffer> constantBuffer = m_pConstantBuffer.lock())
	{
		constantBuffer->Unbind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<Texture> texture = m_pTexture.lock())
	{
		texture->Unbind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<SamplerState> samplerState = m_pSamplerState.lock())
	{
		samplerState->Unbind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<StructuredBuffer> buffer = m_pStructuredBuffer.lock())
	{
		buffer->Unbind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
}

void ShaderParameter::SetConstantBuffer(std::shared_ptr<ConstantBuffer> buffer)
{
	m_pConstantBuffer = buffer;
}

void ShaderParameter::SetStructuredBuffer(std::shared_ptr<StructuredBuffer> rwBuffer)
{
	m_pStructuredBuffer = rwBuffer;
}

void ShaderParameter::SetTexture(std::shared_ptr<Texture> texture)
{
	m_pTexture = texture;
}

void ShaderParameter::SetSampler(std::shared_ptr<SamplerState> sampler)
{
	m_pSamplerState = sampler;
}
