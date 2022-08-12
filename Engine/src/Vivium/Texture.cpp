#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "Font.h"

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

	Texture::Texture(Font* font)
		: width(font->buffer_width), height(font->buffer_height)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font->buffer_width, font->buffer_height, 0, GL_RED, GL_UNSIGNED_BYTE, &font->buffer->at(0));
	}

	Texture::~Texture() {
		glDeleteTextures(1, &id);
	}

	void TextureArray::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	void TextureArray::Bind(uint8_t slot) const
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, id);
	}

	TextureArray::TextureArray(int width, int height, int count, unsigned char* data)
		: width(width), height(height)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D_ARRAY, id);

		glTexImage3D(GL_TEXTURE_2D_ARRAY,
			0,
			GL_RGBA8,
			width,
			height,
			count,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			(void*)data
		);
	}

	TextureArray::~TextureArray()
	{
	}
}