#include "OGLFramebuffer.h"

#include <glew.h>

#include "Log/Log.h"

#define TextureTarget(ms) (ms ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D)

static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
{
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

static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
{
	bool multisampled = samples > 1;
	if (multisampled)
	{
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
	}
	else
	{
		glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
}

static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
{
	glCreateTextures(TextureTarget(multisampled), count, outID);
}

static void BindTexture(bool multisampled, uint32_t id)
{
	glBindTexture(TextureTarget(multisampled), id);
}


// NEEDS A MASSIVE REWORK ITS FINE FOR NOW
void OGLFramebuffer::Invalidate()
{
	if (m_Framebuffer)
	{
		glDeleteFramebuffers(1, &m_Framebuffer);
	}

	glCreateFramebuffers(1, &m_Framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_TestColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_TestColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TestColorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer is incomplete! %x\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		//GL_FRAMEBUFFER_UNDEFINED
	}

	Unbind();
}

OGLFramebuffer::OGLFramebuffer(FramebufferSpecifications& specs)
	: m_Specification(specs)
{
	/*glGenFramebuffers(1, &m_Framebuffer);
	Bind();*/

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
	m_Specification.Width = width;
	m_Specification.Height = height;

	Invalidate();
}

uint32_t OGLFramebuffer::GetColorAttachmentID(uint32_t slot)
{
	/*if (m_ColorAttachments.size())
	{
		if (m_ColorAttachments[slot] == 0)
		{
			//LE_LOG_INFO(std::string("[Framebuffer]: There is no color attachment, returning 0");
			printf("[Framebuffer]: There is no color attachment at slot %d, returning 0\n", slot);
			return 0;
		}
		return m_ColorAttachments[slot];
	}
	else
	{
		LE_LOG_INFO("[Framebuffer]: There is no color attachment, returning 0\n");
	}*/

	return m_TestColorAttachment;
}
