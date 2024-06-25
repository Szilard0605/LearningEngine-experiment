#include "OGLShader.h"

#include <glew.h>
#include <fstream>
#include <sstream>
#include "glm.hpp"
#include <string>

#include "Log/Log.h"
#include <gtc/type_ptr.hpp>

OGLShader::OGLShader(const std::string& vertex, const std::string& fragment)
{
	m_VertexSource = vertex;
	m_FragmentSource = fragment;

	Compile(ShaderType::VERTEX_SHADER);
	Compile(ShaderType::FRAGMENT_SHADER);
}


OGLShader::OGLShader(const std::string& path)
{
	m_FilePath = path;
	m_program = glCreateProgram();
	if (!ReadShaderSource(path, m_VertexSource, m_FragmentSource))
	{
		LE_CORE_ERROR(std::string("Can't read shader file: ") + path);
		return;
	}

	Compile(ShaderType::VERTEX_SHADER);
	Compile(ShaderType::FRAGMENT_SHADER);

	AttachAndLink(m_VertexShaderID, m_FragmentShaderID);
}


Ref<OGLShader> OGLShader::Create(const std::string& path)
{
	return CreateRef<OGLShader>(path);
}


void OGLShader::Bind()
{
	glUseProgram(m_program);
}

void OGLShader::Unbind()
{
	glUseProgram(0);
}

void OGLShader::Compile(ShaderType type)
{
	std::string source;
	uint32_t* id = nullptr;
	switch (type)
	{
		case ShaderType::VERTEX_SHADER:
		{
			source = m_VertexSource;
			id = &m_VertexShaderID;
			break;
		}
		case ShaderType::FRAGMENT_SHADER:
		{
			source = m_FragmentSource;
			id = &m_FragmentShaderID;
			break;
		}
		default:
			return;
	}

	const char* src = source.c_str();


	*id = glCreateShader(ShaderTypeToGLType(type));
	glShaderSource(*id, 1, &src, nullptr);

	LE_CORE_INFO("Compiling %s: %s\n", Shader::ShaderTypeToString(type).c_str(), m_FilePath.c_str()); // replace with core log after merge
	glCompileShader(*id);

	int result;
	glGetShaderiv(*id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(*id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(*id, length, &length, message);
		LE_CORE_ERROR("%s\n", message);

		glDeleteShader(*id);

		return;
	}

	LE_CORE_INFO("Succesfully compiled %s: %s\n", Shader::ShaderTypeToString(type).c_str(), m_FilePath.c_str());
}

void OGLShader::Reload()
{
	glDeleteProgram(m_program);
	if (!ReadShaderSource(m_FilePath, m_VertexSource, m_FragmentSource))
	{
		LE_CORE_ERROR(std::string("Can't read shader file: ") + m_FilePath);
		return;
	}

	Compile(ShaderType::VERTEX_SHADER);
	Compile(ShaderType::FRAGMENT_SHADER);

	AttachAndLink(m_VertexShaderID, m_FragmentShaderID);
}

void OGLShader::AttachAndLink(uint32_t vertexID, uint32_t fragmentID)
{
	m_program = glCreateProgram();

	glAttachShader(m_program, vertexID);
	glAttachShader(m_program, fragmentID);
	glLinkProgram(m_program);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

bool OGLShader::ReadShaderSource(const std::string path, std::string& vertexSource, std::string& fragmentSource)
{
	int type;

	std::string line;
	std::stringstream ss[2];
	std::ifstream stream(path);

	if (!stream.is_open())
		return false;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = 0;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = 1;
			}
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	stream.close();
	vertexSource = ss[0].str();
	fragmentSource = ss[1].str();
	return true;
}

void OGLShader::SetBool(const std::string& name, const bool value)
{
	glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)value);
}

void OGLShader::SetInt(const std::string& name, const int value)
{
	glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
}

void OGLShader::SetFloat(const std::string& name, const float value)
{
	glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
}

void OGLShader::Set3Float(const std::string& name, const float v1, const float v2, const float v3)
{
	glUniform3f(glGetUniformLocation(m_program, name.c_str()), v1, v2, v3);
}

void OGLShader::Set4Float(const std::string& name, const float v1, const float v2, const float v3, const float v4)
{
	glUniform4f(glGetUniformLocation(m_program, name.c_str()), v1, v2, v3, v4);
}

void OGLShader::SetMatrix4f(const std::string& name, const glm::mat4& matrix4)
{
	glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &matrix4[0][0]);
}

void OGLShader::SetVec2f(const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
}

void OGLShader::SetVec3f(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, glm::value_ptr(value));
}

void OGLShader::SetVec4f(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
}

void OGLShader::SetFloatArray(const std::string& name, int count, const float* arr)
{
	glUniform1fv(glGetUniformLocation(m_program, name.c_str()), count, arr);
}

uint32_t OGLShader::ShaderTypeToGLType(ShaderType type)
{
	switch (type)
	{
	case ShaderType::FRAGMENT_SHADER:
		return GL_FRAGMENT_SHADER;
	case ShaderType::VERTEX_SHADER:
		return GL_VERTEX_SHADER;
	default:
		return GL_NONE;
	}
}
