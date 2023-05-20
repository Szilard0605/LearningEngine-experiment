#include "Image.h"

#include "Utils/stb_image.h"

Image::Image(ImageProperties properties)
	: m_Properties(properties)
{

}

Image::Image(std::string path)
	: m_FilePath(path)
{
	stbi_set_flip_vertically_on_load(1);
	m_RawImageData = stbi_load(path.c_str(), &m_Properties.Width, &m_Properties.Height, &m_Properties.ColorChannels, 0);
}

void Image::SetRawData(unsigned char* data)
{
	m_RawImageData = data;
}
