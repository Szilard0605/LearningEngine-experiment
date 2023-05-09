#include "OGLIndexBuffer.h"

#include <glew.h>
#include <stdio.h>

OGLIndexBuffer::OGLIndexBuffer(void* indicies, uint32_t size) : m_size(size)
{
	glCreateBuffers(1, &m_indexbuffer);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), indicies, GL_STATIC_DRAW);

}

OGLIndexBuffer::~OGLIndexBuffer()
{
	printf("Indexbuffer destructor\n");

	glDeleteBuffers(1, &m_indexbuffer);
}

void OGLIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexbuffer);
}

void OGLIndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}