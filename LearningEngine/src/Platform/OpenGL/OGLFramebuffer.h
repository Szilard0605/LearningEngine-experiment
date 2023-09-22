#pragma once

#include "Graphics/Renderer/API/Framebuffer.h"

#include <vector>

class OGLFramebuffer : public Framebuffer
{
public:
	OGLFramebuffer(FramebufferSpecifications specs);
	~OGLFramebuffer();

	void Invalidate();

	virtual void Bind() override;
	virtual void Unbind() override;
	virtual void Resize(uint32_t width, uint32_t height) override;
	virtual FramebufferSpecifications& GetSpecification() override { return m_Specification; }
	virtual uint32_t GetColorAttachmentID(uint32_t slot) override;
	virtual int ReadPixel(uint32_t attachment, int x, int y) override;

	void AttachColorTexture(uint32_t& id, int samples, uint32_t internalFormat, uint32_t format, uint32_t width, uint32_t height, int index);
	void AttachDepthTexture(int samples, uint32_t format, uint32_t attachmentType, uint32_t width, uint32_t height);

private:
	uint32_t m_Framebuffer = 0;
	FramebufferSpecifications m_Specification;

	std::vector<uint32_t> m_ColorAttachments;
	uint32_t m_DepthAttachment;

	uint32_t m_RedIntegerAttachment;
	uint32_t m_TestColorAttachment;
};

