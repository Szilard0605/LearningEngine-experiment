#pragma once

#include "Graphics/Renderer/API/VertexBuffer.h"
#include "Graphics/Renderer/API/BufferLayout.h"

#include <cstdint>

class OGLVertexBuffer : public VertexBuffer
{
	public:
		OGLVertexBuffer() = default;
		OGLVertexBuffer(const void* verticies, uint32_t size);
		OGLVertexBuffer(uint32_t size);
		~OGLVertexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void SetData(const void* data, uint32_t size) override;

		virtual void SetLayout(BufferLayout layout) override;
		virtual BufferLayout GetLayout() override { return m_layout; }

	private:
		uint32_t m_buffer = 0;
		BufferLayout m_layout;
};

