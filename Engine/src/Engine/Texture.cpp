#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

std::string Texture::PATH = "";

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
		Log("Binding texture that has not been created", LOG::WARNING);
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
		Log("Creating texture that has not been deleted properly", LOG::WARNING);
	}
}

void Texture::Delete()
{
	glDeleteTextures(1, &id);
	id = 0;
}

void Texture::Update()
{
	Delete();
	Create();
}

Texture::Texture(std::shared_ptr<uint8_t[]> buffer, unsigned int width, unsigned int height)
	: width(width), height(height), id(0)
{
	this->buffer = buffer;

	Create();
	Unbind();
}

Texture::Texture(std::string filename)
	: id(0)
{
	std::string full_path = PATH + filename;

	// Load pixel data into our buffer, and set width/height of image
	int iwidth, iheight, bpp;
	uint8_t* image_data = stbi_load(full_path.c_str(), &iwidth, &iheight, &bpp, STBI_rgb_alpha);

	// Set image width/height
	width = iwidth;
	height = iheight;

	// Set buffer as a shared ptr to that image data
	buffer = std::shared_ptr<uint8_t[]>(image_data);

	// Create texture object
	Create();
	// Unbind it
	Unbind();
}

Texture::~Texture()
{
	Delete();
}
