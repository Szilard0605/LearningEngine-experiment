#include "OGLVertexBuffer.h"

#include <glew.h>

OGLVertexBuffer::OGLVertexBuffer(const void* verticies, uint32_t size)
{
	glCreateBuffers(1, &m_buffer);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
}


OGLVertexBuffer::OGLVertexBuffer(uint32_t size)
{
	glCreateBuffers(1, &m_buffer);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

	
}
OGLVertexBuffer::~OGLVertexBuffer()
{
	glDeleteBuffers(1, &m_buffer);
}

void OGLVertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
}

void OGLVertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OGLVertexBuffer::SetData(const void* data, uint32_t size)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void OGLVertexBuffer::SetLayout(BufferLayout layout)
{
	m_layout = layout;
}
