#pragma once

#include "Graphics/Renderer/API/ShaderBuffer.h"

class OGLShaderBuffer : public ShaderBuffer
{
public:
	OGLShaderBuffer(__int64 size, ShaderType type = ShaderType::Vertex);
	~OGLShaderBuffer();

	virtual void SetData(const void* data, __int64 size, uint32_t offset) override;
	virtual void Bind(uint32_t binding) override;

private:
	uint32_t m_UniformBuffer;
	uint32_t m_Binding;
};

