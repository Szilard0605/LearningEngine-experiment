#pragma once

#include <string>
#include "Core/Base.h"

#include "Graphics/Renderer/API/Texture2D.h"

class OGLTexture2D : public Texture2D
{
	public:
		OGLTexture2D() = default;
		~OGLTexture2D();
		OGLTexture2D(const std::string image_path);
		OGLTexture2D(uint32_t pwidth, uint32_t pheight);
		
		virtual void Bind(uint32_t slot = 0) override; 
		virtual void SetData(const void* data, uint32_t size) override;

		inline bool operator==(const OGLTexture2D& other)
		{
			return m_texture == other.m_texture;
		}

		inline uint32_t GetTextureID() { return m_texture; }

		virtual uint32_t GetWidth() override { return m_Width; }
		virtual uint32_t GetHeight() override { return m_Height; }

	private:
		int m_Width, m_Height, m_nrChannels;
		uint32_t m_texture;
		uint32_t m_image_per_pixel;
		int m_InternalFormat, m_DataFormat;

};

