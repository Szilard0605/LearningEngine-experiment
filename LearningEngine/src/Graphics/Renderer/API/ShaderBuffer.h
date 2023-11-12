#pragma once

#include <stdint.h>

class ShaderBuffer
{
public:

	enum ShaderType
	{
		Vertex, Fragment
	};

	static ShaderBuffer* Create(__int64 size, ShaderType type = ShaderType::Vertex);

	virtual void SetData(const void* data, __int64 size, uint32_t offset) = 0;
	virtual void Bind(uint32_t binding) = 0;
};

