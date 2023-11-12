#include "OGLShaderBuffer.h"

#include "glew.h"
#include <vcruntime_string.h>
#include <stdio.h>

OGLShaderBuffer::OGLShaderBuffer(__int64 size, ShaderType type)
{
	glCreateBuffers(1, &m_UniformBuffer);

	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	
    glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

}

OGLShaderBuffer::~OGLShaderBuffer()
{
	glDeleteBuffers(1, &m_UniformBuffer);
}

void OGLShaderBuffer::SetData(const void* data, __int64 size, uint32_t offset)
{
	//glNamedBufferSubData(m_UniformBuffer, offset, size, data);

    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);

    /*void* ptr = glMapNamedBufferRange(m_UniformBuffer, offset, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    if (ptr)
    {
        memcpy(ptr, data, size);
        glUnmapNamedBuffer(m_UniformBuffer);
    }
    else
    {
        // Handle error
            
        printf("there is no data\n");
    }*/
}

void OGLShaderBuffer::Bind(uint32_t binding)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_UniformBuffer);
}
