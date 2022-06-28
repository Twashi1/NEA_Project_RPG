#include "Texture.h"

void Texture::Unbind()
{
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Bind(uint8_t slot) const
{
	if (id != 0) {
		GlCall(glActiveTexture(GL_TEXTURE0 + slot));
		GlCall(glBindTexture(GL_TEXTURE_2D, id));
	}
	else {
		Log("Binding texture that has not been created", Utilities::ERROR::WARNING);
	}
}

void Texture::Create()
{
	if (id == 0) {
		GlCall(glGenTextures(1, &id));
		GlCall(glBindTexture(GL_TEXTURE_2D, id));

		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get()));
	}
	else {
		Log("Creating texture that has not been deleted properly", Utilities::ERROR::WARNING);
	}
}

void Texture::Delete()
{
	glDeleteTextures(1, &id);
	id = 0;
}

Texture::Texture(std::shared_ptr<uint8_t[]> buffer, unsigned int width, unsigned int height)
	: width(width), height(height), id(0)
{
	this->buffer = buffer;

	Create();
	Unbind();
}

Texture::~Texture()
{
	Delete();
}
