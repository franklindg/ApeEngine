#ifndef _BUFFER_H
#define _BUFFER_H


class Buffer
{
public:
	enum BufferType
	{
		Unknown = 0,
		VertexBuffer,
		IndexBuffer,
		StructuredBuffer,
		ConstantBuffer,
	};

	//virtual bool Bind(unsigned int id, Shader::ShaderType shaderType, ShaderParameters::Type parameterType) = 0;
	//virtual bool Unbind(unsigned int id, Shader::ShaderType shaderType, ShaderParameters::Type parameterType) = 0;

	virtual BufferType GetType() const = 0;
	virtual unsigned int GetElementCount() const = 0;
};

#endif