#pragma once
#include <string>
#include <stdint.h>

struct ImageProperties 
{
	int Width;
	int Height;
	int ColorChannels;
};

class Image
{
public:
	Image() = default;
	Image(ImageProperties properties);
	Image(std::string path);
	
	ImageProperties& GetProperties() { return m_Properties; }
	unsigned char* GetRawData() { return m_RawImageData; }
	const std::string GetFilePath() { return m_FilePath; }
	void SetRawData(unsigned char* data);


private:
	unsigned char* m_RawImageData = nullptr;
	std::string m_FilePath;
	ImageProperties m_Properties;
};

