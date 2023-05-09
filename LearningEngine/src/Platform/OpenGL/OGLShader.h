#pragma once

#include <string>

#include "glm.hpp"
#include "Core/Base.h"

#include "Graphics/Renderer/API/Shader.h"

class OGLShader : public Shader
{
	public:
		OGLShader() = default;
		OGLShader(const std::string& vertex, const std::string& fragment);
		OGLShader(const std::string& path);

		static Ref<OGLShader> Create(const std::string& path);
 
		int Compile(int type, const std::string& source);
		virtual void Bind() override;
		virtual void Unbind() override;
	
		virtual void SetBool(const std::string& name, const bool value) override;
		virtual void SetInt(const std::string& name, const int value) override;
		virtual void SetFloat(const std::string& name, const float value) override;
		virtual void Set3Float(const std::string& name, const float v1, const float v2, const float v3) override;
		virtual void Set4Float(const std::string& name, const float v1, const float v2, const float v3, const float v4) override;
		virtual void SetMatrix4f(const std::string& name, const glm::mat4& matrix4) override;
		virtual void SetVec2f(const std::string& name, const glm::vec2& value) override;
		virtual void SetVec3f(const std::string& name, const glm::vec3& value) override;
		virtual void SetVec4f(const std::string& name, const glm::vec4& value) override;

		virtual std::string GetFilePath() override { return m_filepath; }

	private:
		int m_program = 0;
		std::string m_filepath;
		std::string m_vertexshader;
		std::string m_fragmentsahder;
};