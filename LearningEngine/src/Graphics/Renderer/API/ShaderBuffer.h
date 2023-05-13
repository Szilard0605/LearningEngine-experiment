#pragma once

#include <stdint.h>

class ShaderBuffer
{
public:

	enum Type
	{
		Vertex, Pixel
	};

	static ShaderBuffer* Create(const void* data, uint32_t size, uint32_t slot, Type type = Type::Vertex);

	virtual void SetData(const void* data, uint32_t size) = 0;
	virtual void Bind() = 0;
};

