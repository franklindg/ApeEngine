#ifndef _SHADER_PARAMETER_H
#define _SHADER_PARAMETER_H

#include "ConstantBuffer.h"
#include "StructuredBuffer.h"
#include "SamplerState.h"
#include "Shader.h"
#include "Texture.h"


class Texture;
class ConstantBuffer;
class StructuredBuffer;
class SamplerState;

class ShaderParameter
{
public:
	enum class Type
	{
		Invalid,
		Texture,
		Sampler,
		Buffer,
		RWTexture,
		RWBuffer,
	};
	
	ShaderParameter();
	ShaderParameter(const std::string& name, UINT slotID, Shader::ShaderType shaderType, Type parameterType);

	bool IsValid() const;
	virtual Type GetType() const;

	virtual void Bind();
	virtual void Unbind();

protected:
	virtual void SetConstantBuffer(std::shared_ptr<ConstantBuffer> buffer);
	virtual void SetStructuredBuffer(std::shared_ptr<StructuredBuffer> rwBuffer);
	virtual void SetTexture(std::shared_ptr<Texture> texture);
	virtual void SetSampler(std::shared_ptr<SamplerState> sampler);
	
private:
	std::string m_Name;

	std::weak_ptr<Texture> m_pTexture;
	std::weak_ptr<SamplerState> m_pSamplerState;
	std::weak_ptr<ConstantBuffer> m_pConstantBuffer;
	std::weak_ptr<StructuredBuffer> m_pStructuredBuffer;

	UINT m_uiSlotID;
	Shader::ShaderType m_ShaderType;
	Type m_ParameterType;
};
#endif 