#include "OGLTexture2D.h"

#include "glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"
#include "Log/Log.h"

OGLTexture2D::OGLTexture2D(const std::string image_path)
{
	m_Image = Image(image_path);

	ImageProperties& props = m_Image.GetProperties();

	m_Properties.Width = props.Width;
	m_Properties.Height = props.Height;

	CreateTextureFromImage(m_Image);
}


OGLTexture2D::OGLTexture2D(uint32_t pwidth, uint32_t pheight)
{

	ImageProperties properties;
	properties.Width = pwidth;
	properties.Height = pheight;
	properties.ColorChannels = 4;
	m_Image = Image(properties);

	m_Properties.Width = pwidth;
	m_Properties.Height = pheight;

	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
	glTextureStorage2D(m_texture, 1, m_InternalFormat, pwidth, pheight);

	glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OGLTexture2D::OGLTexture2D(Texture2DProperties& properties)
	: OGLTexture2D(m_Properties.Width, m_Properties.Height)
{	
}

OGLTexture2D::OGLTexture2D(Image& image)
	: m_Image(image)
{
	CreateTextureFromImage(image);

	m_Properties.Width = image.GetProperties().Width;
	m_Properties.Height = image.GetProperties().Height;
}

void OGLTexture2D::CreateTextureFromImage(Image& image)
{
	if (m_Image.GetRawData())
	{
		ImageProperties& props = m_Image.GetProperties();

		GLenum internalFormat = 0, dataFormat = 0;

		if (props.ColorChannels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (props.ColorChannels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
		glTextureStorage2D(m_texture, 1, internalFormat, props.Width, props.Height);

		glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
		glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_texture, 0, 0, 0, props.Width, props.Height, dataFormat, GL_UNSIGNED_BYTE, m_Image.GetRawData());

	}
	//else printf("Cannot load texture image: %s\n", m_Image.GetFilePath().c_str());
	else LE_CORE_ERROR(std::string("Cannot load texture image: ") + m_Image.GetFilePath());
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
	glTextureSubImage2D(m_texture, 0, 0, 0, m_Properties.Width, m_Properties.Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}