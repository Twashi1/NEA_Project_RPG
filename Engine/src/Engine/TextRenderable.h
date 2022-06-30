#pragma once

#include "Renderer.h"

//static void DrawText(std::string text, Vector2<float> pos, float scale, Vector3<float> color, Shader& shader, const Font& font);

struct TextRenderable {
private:
	int z; // Z level to display text on

	void m_Register(int nz); // Registers renderable on correct z level

public:
	static std::shared_ptr<Shader> shader;

	std::string text;      // Text to render
	Vector2<float> pos;    // Position to render text
	Font* font;			   // Pointer to font
	float scale;		   // Scale for text
	Vector3<float> color;  // Colour to render text as
	bool isVisible = true; // If text should be drawn

	TextRenderable();
	TextRenderable(const TextRenderable& other);
	TextRenderable(const std::string& text, const Vector2<float>& pos, Font* font, int z);
	TextRenderable(const std::string& text, const Vector2<float>& pos, Font* font, const Vector3<float>& color, int z);
	TextRenderable(const std::string& text, const Vector2<float>& pos, Font* font, const Vector3<float>& color, float scale, int z);

	~TextRenderable();
};