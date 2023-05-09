#pragma once

#include <cstdint> 

#include "Graphics/Renderer/API/IndexBuffer.h"

class OGLIndexBuffer : public IndexBuffer
{
	public:
		OGLIndexBuffer() = default;
		OGLIndexBuffer(void* indicies, uint32_t size);
		~OGLIndexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual uint32_t GetCount() override { return m_size; }
	private:
		uint32_t m_indexbuffer = 0;
		uint32_t m_size;
};

