#pragma once

#include <string>
#include <vector>

#include "glm.hpp"

class Shader
{
public:
	static Shader* Create(const std::string& path);
	static Shader* Create(const std::string vertex, const std::string fragment);

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual std::string GetFilePath() = 0;

	//OpenGL
	virtual void SetBool(const std::string& name, const bool value) = 0;
	virtual void SetInt(const std::string& name, const int value) = 0;
	virtual void SetFloat(const std::string& name, const float value) = 0;
	virtual void Set3Float(const std::string& name, const float v1, const float v2, const float v3) = 0;
	virtual void Set4Float(const std::string& name, const float v1, const float v2, const float v3, const float v4) = 0;
	virtual void SetMatrix4f(const std::string& name, const glm::mat4& matrix4) = 0;
	virtual void SetVec2f(const std::string& name, const glm::vec2& value)= 0;
	virtual void SetVec3f(const std::string& name, const glm::vec3& value)= 0;
	virtual void SetVec4f(const std::string& name, const glm::vec4& value)= 0;

};