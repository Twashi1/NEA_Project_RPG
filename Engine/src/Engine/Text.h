#pragma once

#include "Renderer.h"
#include "Font.h"
#include "Shader.h"

struct ENGINE_API Text {
private:
	static Shader* m_DefaultShader;
	static Font* m_DefaultFont;

public:
	std::string text;      // Text to render
	Vector2<float> pos;    // Position to render text
	Font* font;			   // Pointer to font
	Shader* shader;		   // Shader for text
	float scale;		   // Scale for text

	static void Init();

	Text();
	Text(const Text& other);
	Text(const std::string& text, const Vector2<float>& pos, float scale = 1.0f);
	Text(const std::string& text, const Vector2<float>& pos, Font* font, float scale = 1.0f);
	Text(const std::string& text, const Vector2<float>& pos, Shader* shader, float scale = 1.0f);
	Text(const std::string& text, const Vector2<float>& pos, Font* font, Shader* shader, float scale = 1.0f);

	~Text();
};