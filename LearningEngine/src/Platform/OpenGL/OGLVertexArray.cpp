#include "OGLVertexArray.h"
#include <glew.h>

OGLVertexArray::OGLVertexArray()
{
	glCreateVertexArrays(1, &m_vertexarray);
}

OGLVertexArray::~OGLVertexArray()
{
	glDeleteVertexArrays(1, &m_vertexarray);
}

void OGLVertexArray::Bind()
{
	glBindVertexArray(m_vertexarray);
}

void OGLVertexArray::Unbind()
{
	glBindVertexArray(0);
}

void OGLVertexArray::DrawIndexed(uint32_t count)
{
	Bind();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

}

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
	}

	return 0;
}


void OGLVertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
{
	this->Bind();
	vertexBuffer->Bind();

	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout)
	{
		switch (element.Type)
		{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_vertexbufferindex);
				glVertexAttribPointer(m_vertexbufferindex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_vertexbufferindex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_vertexbufferindex);
				glVertexAttribIPointer(m_vertexbufferindex, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), layout.GetStride(), (const void*)element.Offset);


				m_vertexbufferindex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_vertexbufferindex);
					glVertexAttribPointer(m_vertexbufferindex, count, ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_vertexbufferindex, 1);
					m_vertexbufferindex++;
				}
				break;
			}
		}
	}

	m_VertexBuffers.push_back(vertexBuffer);
}

/*void OGLVertexArray::SetIndexBuffer(const Ref<GLIndexBuffer>& indexBuffer)
{
	this->Bind();	
	indexBuffer->Bind();
	m_IndexBuffer = indexBuffer;
}*/
