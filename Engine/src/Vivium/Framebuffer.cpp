#include "Framebuffer.h"

namespace Vivium {
	Framebuffer::Framebuffer(int width, int height)
		: width(width), height(height)
	{
		Resize(width, height);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &id);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::SetSlot(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, color_attachment);
	}

	int Framebuffer::GetWidth() const
	{
		return width;
	}

	int Framebuffer::GetHeight() const
	{
		return height;
	}

	Vector2<int> Framebuffer::GetDim() const
	{
		return Vector2<int>(width, height);
	}

	GLuint Framebuffer::GetColorAttachment() const
	{
		return color_attachment;
	}

	void Framebuffer::Resize(int width, int height)
	{
		this->width = width;
		this->height = height;

		glCreateFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		glCreateTextures(GL_TEXTURE_2D, 1, &color_attachment);
		glBindTexture(GL_TEXTURE_2D, color_attachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { LogError("Framebuffer is incomplete"); }

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
