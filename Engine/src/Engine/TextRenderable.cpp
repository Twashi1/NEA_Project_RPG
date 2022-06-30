#include "TextRenderable.h"

std::shared_ptr<Shader> TextRenderable::shader = nullptr;

void TextRenderable::m_Register(int nz)
{
    auto it = Renderer::m_TextRenderables.find(nz); // Get iterator to "z" key in map

    // Key didn't exist (so we need to create a list there and add ourselves to it)
    if (it == Renderer::m_TextRenderables.end()) {
        // Create a list which contains a pointer to this instance
        Utils::List<TextRenderable*> new_list; new_list.Push(this);
        // Add that list at key "z"
        Renderer::m_TextRenderables.insert({ nz, new_list });
    }
    // Key did exist
    else {
        // At our key value, push back a pointer to this instance to the list of Renderables
        Renderer::m_TextRenderables.at(nz).Push(this);
    }
}

TextRenderable::TextRenderable()
    : text(""), pos(Vector2<float>::ZERO), font(nullptr), scale(1.0f), color(Vector3<float>::ZERO), z(0)
{
    Log("Default constructor called! (Values may not be properly initialised)", Utils::ERROR::WARNING);
}

TextRenderable::TextRenderable(const TextRenderable& other)
    : text(other.text), pos(other.pos), font(other.font), scale(other.scale), color(other.color), isVisible(other.isVisible), z(other.z)
{
    m_Register(z);
}

TextRenderable::TextRenderable(const std::string& text, const Vector2<float>& pos, Font* font, int z)
	: text(text), pos(pos), font(font), scale(1.0f), z(z), color(COLORS::WHITE)
{
	m_Register(z);
}

TextRenderable::TextRenderable(const std::string& text, const Vector2<float>& pos, Font* font, const Vector3<float>& color, int z)
    : text(text), pos(pos), font(font), scale(1.0f), z(z), color(color)
{
    m_Register(z);
}

TextRenderable::TextRenderable(const std::string& text, const Vector2<float>& pos, Font* font, const Vector3<float>& color, float scale, int z)
	: text(text), pos(pos), font(font), scale(scale), z(z), color(color)
{
	m_Register(z);
}

TextRenderable::~TextRenderable()
{
    // Remove ourselves from renderer
    Renderer::m_TextRenderables.at(z).Remove(this);
}
