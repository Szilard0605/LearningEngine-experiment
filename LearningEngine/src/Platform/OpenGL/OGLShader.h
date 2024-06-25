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
 
		virtual void Compile(ShaderType type = ShaderType::NONE) override;
		virtual void Reload() override;
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

		virtual std::string GetFilePath() override { return m_FilePath; }

		//
		virtual void SetFloatArray(const std::string& name, int count, const float* arr) override;

	private:
		void AttachAndLink(uint32_t vertexID, uint32_t fragmentID);
		bool ReadShaderSource(const std::string path, std::string& vertexSource, std::string& fragmentSource);
		uint32_t ShaderTypeToGLType(ShaderType type);

		int m_program = 0;
		std::string m_FilePath;
		std::string m_VertexSource;
		std::string m_FragmentSource;

		uint32_t m_VertexShaderID;
		uint32_t m_FragmentShaderID;
};