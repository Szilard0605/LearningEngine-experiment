#include "OGLTexture2D.h"

#include "glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"
#include "Log/Log.h"

OGLTexture2D::OGLTexture2D(const std::string image_path)
{
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(image_path.c_str(), &m_Width, &m_Height, &m_nrChannels, 0);

	if (data)
	{

		GLenum internalFormat = 0, dataFormat = 0;

		if (m_nrChannels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (m_nrChannels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
		glTextureStorage2D(m_texture, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_texture, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	else printf("Cannot load texture image: %s\n", image_path.c_str());
}


OGLTexture2D::OGLTexture2D(uint32_t pwidth, uint32_t pheight)
	: m_Width(pwidth), m_Height(pheight)
{
	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
	glTextureStorage2D(m_texture, 1, m_InternalFormat, m_Width, m_Height);

	glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


OGLTexture2D::~OGLTexture2D()
{
	glDeleteTextures(1, &m_texture);
}

void OGLTexture2D::Bind(uint32_t slot)
{
	glBindTextureUnit(slot, m_texture);
}

void OGLTexture2D::SetData(const void* data, uint32_t size)
{
	uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
	//HZ_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
	glTextureSubImage2D(m_texture, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}