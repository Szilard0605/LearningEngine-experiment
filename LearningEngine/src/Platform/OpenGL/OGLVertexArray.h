#pragma once

#include "OGLVertexBuffer.h"
#include "OGLIndexbuffer.h"

#include "Graphics/Renderer/API/VertexArray.h"
#include "Graphics/Renderer/API/BufferLayout.h"
#include "Graphics/Renderer/API/VertexBuffer.h"

#include "Core/Base.h"


class OGLVertexArray : public VertexArray
{
	public:
		OGLVertexArray();
		~OGLVertexArray();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void AddVertexBuffer(VertexBuffer* vertexBuffer) override;
		//void SetIndexBuffer(const Ref<GLIndexBuffer>& indexBuffer);

		virtual void DrawIndexed(uint32_t count) override;
	private:
		uint32_t m_vertexarray = 0;
		uint32_t m_vertexbufferindex = 0;
		std::vector<VertexBuffer*> m_VertexBuffers;
};

