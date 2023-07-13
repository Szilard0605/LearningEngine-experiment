#pragma once

#include "API/Shader.h"
#include "API/Texture2D.h"

#include "glm.hpp"

class Material
{
	public:
		Material() = default;
		Material(Shader* shader);

		void Bind();
		void Unbind();

		void SetTexture(Texture2D* texture);
		Texture2D* GetTexture() { return m_Texture; }
		
		Shader* GetShader() { return m_Shader; }
		
		void SetNormalMap(Texture2D* normalmap) { m_NormalMap = normalmap; }
		Texture2D* GetNormalmap() { return m_NormalMap; }
		

		glm::vec4 BaseColor;
	private:
		Texture2D* m_Texture = nullptr;
		Texture2D* m_NormalMap = nullptr;

		Shader* m_Shader;
};

