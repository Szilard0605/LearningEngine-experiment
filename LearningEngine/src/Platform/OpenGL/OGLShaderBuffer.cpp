#include "OGLShaderBuffer.h"

#include "glew.h"

OGLShaderBuffer::OGLShaderBuffer(uint32_t size, ShaderType type)
{
	glCreateBuffers(1, &m_UniformBuffer);

	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glNamedBufferStorage(m_UniformBuffer, size, NULL, flags);
}

OGLShaderBuffer::~OGLShaderBuffer()
{
	glDeleteBuffers(1, &m_UniformBuffer);
}

void OGLShaderBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
{
	glNamedBufferSubData(m_UniformBuffer, offset, size, data);
}

void OGLShaderBuffer::Bind(uint32_t binding)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_UniformBuffer);
}