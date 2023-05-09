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
		Shader* GetShader() { return m_Shader; }

		Texture2D* GetTexture() { return m_Texture; }

		glm::vec4 BaseColor;
	private:
		Texture2D* m_Texture;
		Shader* m_Shader;
};

