#pragma once

#include "Renderer.h"
#include "Logger.h"

#include "stb_image.h"

namespace Vivium {
	class Font;

	class VIVIUM_API Texture {
	public:
		static std::string PATH;

		GLuint id;

		unsigned int width, height; // TODO: just make ints

		static void Unbind();
		void Bind(uint8_t slot) const;

		// TODO: allow grayscale, RGB textures
		// Construct from RGBA data (4 bytes per pixel), and width/height of image
		Texture(Ref(uint8_t[]) buffer, unsigned int width, unsigned int height);
		// Construct from png
		Texture(const std::string& filename);
		// Construct from font
		Texture(Font* font);

		~Texture();
	};

	// TODO
	class VIVIUM_API TextureArray {
		GLuint id;

		int width, height;

		static void Unbind();
		void Bind(uint8_t slot) const;

		TextureArray(int width, int height, int count, unsigned char* data);
		~TextureArray();
	};
}
