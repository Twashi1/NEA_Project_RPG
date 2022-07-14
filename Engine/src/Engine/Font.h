#pragma once

#include "Utils.h"
#include "Texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H


// Following this tutorial for rendering text
// https://learnopengl.com/In-Practice/Text-Rendering

class ENGINE_API Font {
	void m_LoadChar(const FT_Face& face, char character);

public:
	static std::string PATH;

	uint32_t max_height = 0;

	struct Character {
		uint32_t texture_id;  // ID of glyph
		Vector2<int> size;	  // Size of glyph
		Vector2<int> bearing; // Offset from baseline to top left of glyph
		uint32_t advance;     // Offset to advance to next glpyh

		Character();
		Character(const Character& other);
		Character(uint32_t texture_id, Vector2<int> size, Vector2<int> bearing, uint32_t advance);
	};

	typedef std::unordered_map<char, Character> CharacterMap_t;

	CharacterMap_t character_map;
	uint32_t vertex_array_id, vertex_buffer_id;
	
	Font(const char* font_path);
};