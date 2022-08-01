#pragma once

#include "Renderer.h"
#include "Utils.h"

#include "stb_image.h"

namespace Vivium {
	class VIVIUM_API Texture {
	public:
		static std::string PATH;

		GLuint id;

		unsigned int width, height;

		static void Unbind();
		void Bind(uint8_t slot) const;

		// TODO: allow grayscale, RGB textures
		// Construct from RGBA data (4 bytes per pixel), and width/height of image
		Texture(Ref(uint8_t[]) buffer, unsigned int width, unsigned int height);
		// Construct from png
		Texture(const std::string& filename);

		~Texture();
	};
}
