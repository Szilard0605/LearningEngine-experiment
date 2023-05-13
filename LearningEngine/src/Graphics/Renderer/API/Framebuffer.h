#pragma once

#include <stdint.h>

#include <initializer_list>

class Framebuffer
{
public:
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferAttachment
	{
		FramebufferAttachment(FramebufferTextureFormat textureFormat)
			: TextureFormat(textureFormat) { }

		FramebufferTextureFormat TextureFormat;
	};

	struct FramebufferSpecifications
	{
		uint32_t Width = 0, Height = 0;
		std::initializer_list<FramebufferAttachment> Attachments;
		uint32_t Samples = 1;
	};

	static Framebuffer* Create(FramebufferSpecifications& specs);

	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	virtual void Resize(uint32_t width, uint32_t height) = 0;
	virtual uint32_t GetColorAttachmentID(uint32_t slot) = 0;
	virtual FramebufferSpecifications GetSpecification() = 0; 
};

