#include "OGLShader.h"

#include <glew.h>
#include <fstream>
#include <sstream>
#include "glm.hpp"
#include <string>

#include "Log/Log.h"

OGLShader::OGLShader(const std::string& vertex, const std::string& fragment)
{
	m_program = glCreateProgram();
	int fs = Compile(GL_VERTEX_SHADER, vertex);
	int vs = Compile(GL_FRAGMENT_SHADER, fragment);

	glAttachShader(m_program, fs);
	glAttachShader(m_program, vs);

	glLinkProgram(m_program);
	glValidateProgram(m_program);

	glDeleteShader(fs);
	glDeleteShader(vs);

}


OGLShader::OGLShader(const std::string& path)
{
	m_filepath = path;

	std::ifstream stream(path);

	if (stream.is_open())
	{
		int type;

		unsigned int vs = -1;
		unsigned int fs = -1;

		m_program = glCreateProgram();

		std::string line;
		std::stringstream ss[2];

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

		vs = Compile(GL_VERTEX_SHADER, ss[0].str());
		glAttachShader(m_program, vs);

		fs = Compile(GL_FRAGMENT_SHADER, ss[1].str());
		glAttachShader(m_program, fs);

		glLinkProgram(m_program);
		glValidateProgram(m_program);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}
	else LE_CORE_ERROR(std::string("Can't open shader file: ") + path);
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

int OGLShader::Compile(int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//ERROR HANDLING
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		printf("%s\n", message);

		glDeleteShader(id);
		return 0;

	}
	return id;
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
	glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
}

void OGLShader::SetVec4f(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
}