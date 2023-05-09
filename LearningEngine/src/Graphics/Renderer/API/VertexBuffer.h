#pragma once

#include <stdint.h>

#include "Graphics/Renderer/API/BufferLayout.h"

class VertexBuffer
{
	public:

		static VertexBuffer* Create(uint32_t size);
		static VertexBuffer* Create(const void* data, uint32_t size);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual void SetLayout(BufferLayout layout) = 0;
		//virtual void Resize(uint32_t size) = 0;
		virtual BufferLayout GetLayout() = 0;
};