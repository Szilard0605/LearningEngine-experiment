#include "Image.h"

#include "Utils/stb_image.h"

Image::Image(ImageProperties properties)
	: m_Properties(properties)
{

}

Image::Image(std::string path, bool verticalflip)
{
	stbi_set_flip_vertically_on_load(verticalflip);
	m_RawImageData = stbi_load(path.c_str(), &m_Properties.Width, &m_Properties.Height, &m_Properties.ColorChannels, 0);
}

Image::Image(std::string path, ImageFormat format, bool verticalflip)
	: m_FilePath(path)
{
	stbi_set_flip_vertically_on_load(verticalflip);
	m_RawImageData = stbi_load(path.c_str(), &m_Properties.Width, &m_Properties.Height, &m_Properties.ColorChannels, (int)format);
	m_Properties.ColorChannels = (int)format;
}

void Image::SetRawData(unsigned char* data)
{
	m_RawImageData = data;
}
