#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

namespace Vivium {
	std::string Texture::PATH = "";

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Bind(uint8_t slot) const
	{
		if (slot > Renderer::MAX_TEXTURE_SLOT) { LogError("Attempted to bind texture to invalid slot: {}", (int)slot); }
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
		glActiveTexture(GL_TEXTURE0);
	}

	Texture::Texture(Ref(uint8_t[]) buffer, unsigned int width, unsigned int height)
		: width(width), height(height), id(0)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
	}

	Texture::Texture(const std::string& filename)
		: id(0)
	{
		std::string full_path = PATH + filename;

		// Load pixel data into our buffer, and set width/height of image
		int iwidth, iheight, bpp;
		uint8_t* image_data = stbi_load(full_path.c_str(), &iwidth, &iheight, &bpp, STBI_rgb_alpha);

		// Set image width/height
		width = iwidth;
		height = iheight;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

		stbi_image_free(image_data);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &id);
	}
}