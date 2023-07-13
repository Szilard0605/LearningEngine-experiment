#pragma once
#include <string>
#include <stdint.h>

enum class ImageFormat : int
{

	GREY = 1,
	GREY_ALPHA = 2,
	RGB = 3,
	RGBA = 4
};

struct ImageProperties
{
	int Width;
	int Height;
	int ColorChannels;
	ImageFormat Format;
};

class Image
{
public:
	Image() = default;
	Image(ImageProperties properties);
	Image(std::string path, bool verticalflip = true);
	Image(std::string path, ImageFormat format, bool verticalflip = true);

	ImageProperties& GetProperties() { return m_Properties; }
	unsigned char* GetRawData() { return m_RawImageData; }
	const std::string GetFilePath() { return m_FilePath; }
	void SetRawData(unsigned char* data);


private:
	unsigned char* m_RawImageData = nullptr;
	std::string m_FilePath;
	ImageProperties m_Properties;
};

