#pragma once

#include <string>

#include "Graphics/Renderer/Image.h"

struct Texture2DProperties
{
	uint32_t Width;
	uint32_t Height;
};

class Texture2D
{
public:
	static Texture2D* Create(const std::string path);
	static Texture2D* Create(uint32_t width, uint32_t height);
	static Texture2D* Create(Texture2DProperties& properties);
	static Texture2D* Create(Image& image);

	virtual void Bind(uint32_t slot = 0) = 0;
	virtual void SetData(const void* data, uint32_t size) = 0;

	virtual uint32_t GetWidth() = 0;
	virtual uint32_t GetHeight() = 0;

	virtual uint32_t GetTextureID() = 0;
};

