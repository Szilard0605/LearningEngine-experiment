#pragma once

#include <stdint.h>

#include <vector>

enum class FramebufferAttachment
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


struct FramebufferSpecifications
{
	uint32_t Width = 0, Height = 0;
	std::vector<FramebufferAttachment> Attachments;
	int Samples = 1;
};


class Framebuffer
{
public:
	static Framebuffer* Create(FramebufferSpecifications specs);

	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	virtual void Resize(uint32_t width, uint32_t height) = 0;
	virtual uint32_t GetColorAttachmentID(uint32_t slot) = 0;
	virtual FramebufferSpecifications& GetSpecification() = 0;
	virtual int ReadPixel(uint32_t attachment, int x, int y) = 0;
};

