#include "Text.h"

namespace Vivium {
    Ref(Shader) Text::m_DefaultShader = nullptr;
    Ref(Font) Text::m_DefaultFont = nullptr;

    void Text::m_Construct()
    {
    }

    void Text::m_Init()
    {
        m_DefaultShader = MakeRef(Shader, "text_vertex", "text_frag");
        m_DefaultFont = MakeRef(Font, "consola.ttf");
    }

    Ref(Font) Text::GetDefaultFont()
    {
        if (m_DefaultFont == nullptr) {
            LogWarn("Default font not initialised yet");
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
        LogWarn("Default constructor called! Values will not be properly initialised");
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

    Text::Text(const std::string& text, const Vector2<float>& pos, Ref(Font) font, float scale)
        : text(text), pos(pos), font(font), shader(m_DefaultShader), scale(scale)
    {
        m_Construct();
    }

    Text::Text(const std::string& text, const Vector2<float>& pos, Ref(Shader) shader, float scale)
        : text(text), pos(pos), font(m_DefaultFont), shader(shader), scale(scale)
    {
        m_Construct();
    }

    Text::Text(const std::string& text, const Vector2<float>& pos, Ref(Font) font, Ref(Shader) shader, float scale)
        : text(text), pos(pos), font(font), shader(shader), scale(scale)
    {
        m_Construct();
    }

    Text::~Text()
    {
    }
}