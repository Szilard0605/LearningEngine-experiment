#pragma once

#include <string>
#include "Core/Base.h"

#include "Graphics/Renderer/API/Texture2D.h"
#include "Graphics/Renderer/Image.h"

class OGLTexture2D : public Texture2D
{
	public:
		OGLTexture2D() = default;
		~OGLTexture2D();
		OGLTexture2D(const std::string image_path);
		OGLTexture2D(uint32_t pwidth, uint32_t pheight);
		OGLTexture2D(Texture2DProperties& properties);
		OGLTexture2D(Image& image);

		void CreateTextureFromImage(Image& image);

		virtual void Bind(uint32_t slot = 0) override; 
		virtual void SetData(const void* data, uint32_t size) override;
		
		inline bool operator==(const OGLTexture2D& other)
		{
			return m_texture == other.m_texture;
		}
		
		virtual uint32_t GetWidth() override { return m_Properties.Width; }
		virtual uint32_t GetHeight() override { return m_Properties.Height; }
		
		virtual uint32_t GetTextureID() override { return m_texture; }


	private:

		Texture2DProperties m_Properties;
		Image m_Image;

		uint32_t m_texture;
		int m_InternalFormat, m_DataFormat;

};

