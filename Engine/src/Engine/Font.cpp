#include "Font.h"

std::string Font::PATH = "";

void Font::m_LoadChar(const FT_Face& face, char character)
{
	if (FT_Load_Char(face, character, FT_LOAD_RENDER)) {
		Log("Failed to load glyph", Utils::ERROR::FATAL);
	}
}

Font::Font(const char* font_path) {
	// Initialise FT library
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		Log("Couldn't initialise FreeType library", Utils::ERROR::FATAL);
	}

	// Initialise consolas font
	FT_Face face;
	if (FT_New_Face(ft, (PATH + font_path).c_str(), 0, &face)) {
		Log("Failed to load font", Utils::ERROR::FATAL);
	}

	FT_Set_Pixel_Sizes(face, 0, 48); // Extract a font size of 48

	GlCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // Disable byte-alignment restriction
	
	// Load 128 ASCII characters
	for (unsigned char c = 0; c < 128; c++)
	{
		m_LoadChar(face, c);

		// Generate texture
		uint32_t id;
		GlCall(glGenTextures(1, &id));
		GlCall(glBindTexture(GL_TEXTURE_2D, id));
		GlCall(glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		));

		// Set texture options
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		// Store character
		Character character = Character(
			id,
			Vector2<int>(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			Vector2<int>(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		);
		character_map[c] = character;
	}
	// Unbind texture
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	// Free FreeType memory
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Setup vertex array and vertex buffer for texture quads
	GlCall(glGenVertexArrays(1, &vertex_array_id));
	GlCall(glGenBuffers(1, &vertex_buffer_id));
	GlCall(glBindVertexArray(vertex_array_id));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id));
	GlCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW));
	GlCall(glEnableVertexAttribArray(0));
	GlCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GlCall(glBindVertexArray(0));
}

Font::Character::Character(uint32_t texture_id, Vector2<int> size, Vector2<int> bearing, uint32_t advance)
	: texture_id(texture_id), size(size), bearing(bearing), advance(advance) {}

Font::Character::Character()
	: texture_id(0), size(Vector2<int>::ZERO), bearing(Vector2<int>::ZERO), advance(0) {}

Font::Character::Character(const Character& other)
	: texture_id(other.texture_id), size(other.size), bearing(other.bearing), advance(other.advance) {}
