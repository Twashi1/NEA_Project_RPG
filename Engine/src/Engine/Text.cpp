#include "Text.h"

Shader* Text::m_DefaultShader = nullptr;
Font* Text::m_DefaultFont = nullptr;

void Text::Init()
{
    m_DefaultShader = new Shader("text_vertex", "text_frag");
    m_DefaultFont = new Font("consola.ttf");
}

Font* Text::GetDefaultFont()
{
    if (m_DefaultFont == nullptr) {
        ENG_LogWarn("Default font not initialised yet");
    }

    return m_DefaultFont;
}

Text::Text()
    : text(""), pos(0), font(nullptr), shader(nullptr), scale(0.0f)
{
    ENG_LogWarn("Default constructor called! Values will not be properly initialised");
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
