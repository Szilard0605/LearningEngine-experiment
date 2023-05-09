#pragma once

#include <stdint.h>

class ConstantBuffer
{
public:

	enum Type
	{
		Vertex, Pixel
	};

	static ConstantBuffer* Create(const void* data, uint32_t size, uint32_t slot, Type type = Type::Vertex);

	virtual void SetData(const void* data, uint32_t size) = 0;
	virtual void Bind() = 0;
};

