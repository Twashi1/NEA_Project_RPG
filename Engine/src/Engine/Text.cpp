#include "Text.h"

namespace Vivium {
    ENG_Ptr(Shader) Text::m_DefaultShader = nullptr;
    ENG_Ptr(Font) Text::m_DefaultFont = nullptr;

    void Text::m_Construct()
    {
    }

    void Text::Init()
    {
        m_DefaultShader = ENG_MakePtr(Shader, "text_vertex", "text_frag");
        m_DefaultFont = ENG_MakePtr(Font, "consola.ttf");
    }

    ENG_Ptr(Font) Text::GetDefaultFont()
    {
        if (m_DefaultFont == nullptr) {
            ENG_LogWarn("Default font not initialised yet");
        }

        return m_DefaultFont;
    }

    void Text::SetPos(const Vector2<float>& pos)
    {
        this->pos = pos;
    }

    const Vector2<float>& Text::GetPos() const
    {
        return pos;
    }

    Text::Text()
        : text(""), pos(), font(nullptr), shader(nullptr), scale(0.0f)
    {
        ENG_LogWarn("Default constructor called! Values will not be properly initialised");
        m_Construct();
    }

    Text::Text(const Text& other)
        : text(other.text), pos(other.pos), font(other.font), shader(other.shader), scale(other.scale)
    {
        m_Construct();
    }

    Text::Text(const std::string& text, const Vector2<float>& pos, float scale)
        : text(text), pos(pos), font(m_DefaultFont), shader(m_DefaultShader), scale(scale)
    {
        m_Construct();
    }

    Text::Text(const std::string& text, const Vector2<float>& pos, ENG_Ptr(Font) font, float scale)
        : text(text), pos(pos), font(font), shader(m_DefaultShader), scale(scale)
    {
        m_Construct();
    }

    Text::Text(const std::string& text, const Vector2<float>& pos, ENG_Ptr(Shader) shader, float scale)
        : text(text), pos(pos), font(m_DefaultFont), shader(shader), scale(scale)
    {
        m_Construct();
    }

    Text::Text(const std::string& text, const Vector2<float>& pos, ENG_Ptr(Font) font, ENG_Ptr(Shader) shader, float scale)
        : text(text), pos(pos), font(font), shader(shader), scale(scale)
    {
        m_Construct();
    }

    Text::~Text()
    {
    }
}