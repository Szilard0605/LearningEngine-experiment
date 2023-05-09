#include "Material.h"

Material::Material(Shader* shader)
	: m_Shader(shader)
{

}

void Material::Bind()
{
	m_Shader->Bind();
	m_Texture->Bind();
}

void Material::Unbind()
{

}

void Material::SetTexture(Texture2D* texture)
{
	m_Texture = texture;
}
