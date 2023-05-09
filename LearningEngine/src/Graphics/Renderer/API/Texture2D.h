#pragma once


/*class OGLTexture2D
{
public:
	OGLTexture2D() = default;
	~OGLTexture2D();
	OGLTexture2D(const std::string image_path);
	OGLTexture2D(uint32_t pwidth, uint32_t pheight);
	void Bind(uint32_t slot = 0);
	void SetData(void* data, uint32_t size);

	inline bool operator==(const GLTexture2D& other)
	{
		return m_texture == other.m_texture;
	}

	int width, height, nrChannels;

	inline uint32_t GetTextureID() { return m_texture; }

private:
	uint32_t m_texture;
	uint32_t m_image_per_pixel;
	int m_InternalFormat, m_DataFormat;

};*/

#include <string>

class Texture2D
{
public:
	static Texture2D* Create(const std::string path);
	static Texture2D* Create(uint32_t width, uint32_t height);

	virtual void Bind(uint32_t slot = 0) = 0;
	virtual void SetData(const void* data, uint32_t size) = 0;

	virtual uint32_t GetWidth() = 0;
	virtual uint32_t GetHeight() = 0;
};

