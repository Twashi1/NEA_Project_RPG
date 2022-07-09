#include "Text.h"

Shader* Text::m_DefaultShader = nullptr;
Font* Text::m_DefaultFont = nullptr;

void Text::Init()
{
    m_DefaultShader = new Shader("text_vertex", "text_frag");
    m_DefaultFont = new Font("consola.ttf");
}

Text::Text()
    : text(""), pos(0), font(nullptr), shader(nullptr), scale(0.0f)
{
    Log("Default constructor called! Values will not be properly initialised", LOG::WARNING);
}

Text::Text(const Text& other)
    : text(other.text), pos(other.pos), font(other.font), shader(other.shader), scale(other.scale)
{}

Text::Text(const std::string& text, const Vector2<float>&pos, float scale)
    : text(text), pos(pos), font(m_DefaultFont), shader(m_DefaultShader), scale(scale)
{}

Text::Text(const std::string& text, const Vector2<float>& pos, Font* font, float scale)
	: text(text), pos(pos), font(font), shader(m_DefaultShader), scale(scale)
{}

Text::Text(const std::string& text, const Vector2<float>& pos, Shader* shader, float scale)
    : text(text), pos(pos), font(m_DefaultFont), shader(shader), scale(scale)
{}

Text::Text(const std::string& text, const Vector2<float>& pos, Font* font, Shader* shader, float scale)
    : text(text), pos(pos), font(font), shader(shader), scale(scale)
{}

Text::~Text() {}