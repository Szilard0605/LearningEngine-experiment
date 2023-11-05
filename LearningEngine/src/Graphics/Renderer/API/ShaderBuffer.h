#pragma once

#include <stdint.h>

class ShaderBuffer
{
public:

	enum ShaderType
	{
		Vertex, Fragment
	};

	static ShaderBuffer* Create(uint32_t size, ShaderType type = ShaderType::Vertex);

	virtual void SetData(const void* data, uint32_t size, uint32_t offset) = 0;
	virtual void Bind(uint32_t binding) = 0;
};

