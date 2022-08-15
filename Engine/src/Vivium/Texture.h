#pragma once

#include "Renderer.h"
#include "Logger.h"

#include "stb_image.h"

namespace Vivium {
	class Font;

	class VIVIUM_API Texture {
	private:
		GLuint m_ID;
		int m_Width, m_Height;

	public:
		static std::string PATH;

		static void Unbind();
		void Bind(uint8_t slot) const;

		int GetWidth() const;
		int GetHeight() const;
		Vector2<int> GetDim() const;

		// TODO: allow grayscale, RGB textures
		// Construct from RGBA data (4 bytes per pixel), and width/height of image
		Texture(Ref(uint8_t[]) buffer, int width, int height);
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
