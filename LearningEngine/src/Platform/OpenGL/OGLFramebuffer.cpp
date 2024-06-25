#include "OGLFramebuffer.h"

#include <glew.h>

#include "Log/Log.h"

#define TextureTarget(ms) ((ms) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D)

void OGLFramebuffer::AttachColorTexture(uint32_t& id, int samples, uint32_t internalFormat, uint32_t format, uint32_t width, uint32_t height, int index)
{
	Bind();

	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	bool multisampled = samples > 1;
	if (multisampled)
	{
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
}

void OGLFramebuffer::AttachDepthTexture(int samples, uint32_t format, uint32_t attachmentType, uint32_t width, uint32_t height)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	bool multisampled = samples > 1;

	if (multisampled)
	{
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
	}
	else
	{
		glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), m_DepthAttachment, 0);
}

static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
{
	glCreateTextures(TextureTarget(multisampled), count, outID);
}

static void BindTexture(bool multisampled, uint32_t id)
{
	glBindTexture(TextureTarget(multisampled), id);
}

static void DeleteTextures(std::vector<uint32_t>& color, uint32_t depth)
{
	glDeleteTextures(color.size(), color.data());
	glDeleteTextures(1, &depth);
}

// NEEDS A MASSIVE REWORK ITS FINE FOR NOW
// IT CRASHES IF ITS REPEATED A LOT OF TIMES
void OGLFramebuffer::Invalidate()
{
	if(m_Framebuffer)
	{
		glDeleteFramebuffers(1, &m_Framebuffer);
		DeleteTextures(m_ColorAttachments, m_DepthAttachment);
		m_ColorAttachments.clear();
		m_DepthAttachment = 0;
	}
	

	glCreateFramebuffers(1, &m_Framebuffer);
	Bind();

	if (!m_Specification.Attachments.size())
		return;

	//m_Specification.Attachments = { FramebufferAttachment::RGBA8, FramebufferAttachment::RED_INTEGER, FramebufferAttachment::Depth };

	for(const auto attachment : m_Specification.Attachments)
	{
		switch (attachment)
		{
			case FramebufferAttachment::RGBA8:
			{
				m_ColorAttachments.resize(m_ColorAttachments.size() + 1);
				int id = m_ColorAttachments.size() - 1;
				AttachColorTexture(m_ColorAttachments[id], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, id);
				break;
			}

			case FramebufferAttachment::RED_INTEGER:
			{
				m_ColorAttachments.resize(m_ColorAttachments.size() + 1);
				int id = m_ColorAttachments.size() - 1;
				AttachColorTexture(m_ColorAttachments[id], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, id);
				break;
			}

			case FramebufferAttachment::DEPTH24STENCIL8:
			{
				AttachDepthTexture(m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}
		}
	}

	std::vector<GLenum> drawBuffers(m_ColorAttachments.size());
	for (size_t i = 0; i < m_ColorAttachments.size(); ++i) {
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LE_CORE_ERROR("Framebuffer is incomplete! Error: ", std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
	}

	Unbind();
}

OGLFramebuffer::OGLFramebuffer(FramebufferSpecifications specs)
	: m_Specification(specs)
{
	Invalidate();
}

OGLFramebuffer::~OGLFramebuffer()
{
	glDeleteFramebuffers(1, &m_Framebuffer);
}

void OGLFramebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void OGLFramebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OGLFramebuffer::Resize(uint32_t width, uint32_t height)
{
	FramebufferSpecifications sp = m_Specification;

	m_Specification.Width = width;
	m_Specification.Height = height;

	Invalidate();
}

uint32_t OGLFramebuffer::GetColorAttachmentID(uint32_t slot)
{
	if (!m_ColorAttachments.size())
	{
		LE_CORE_ERROR("[Framebuffer]: There are no color attachments");
		return 0;
	}
	
	if (m_ColorAttachments[slot] == 0)
	{
		LE_CORE_ERROR("[Framebuffer]: There is no color attachment at the slot %d", slot);
		return 0;
	}

	return m_ColorAttachments[slot];

}

int OGLFramebuffer::ReadPixel(uint32_t attachment, int x, int y)
{
	int pixelData = 0;
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
	uint32_t error = glGetError();
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	return pixelData;
}
